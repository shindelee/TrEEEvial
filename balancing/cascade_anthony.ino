#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>
#include <Arduino.h>
#include <AccelStepper.h>

int SpeedMax = 300;
int stepperSpeed;
int Stepper_demand = 0;

#define LEFT_DIR_PIN 15
#define LEFT_STEP_PIN 32
#define RIGHT_DIR_PIN 4
#define RIGHT_STEP_PIN 27
#define PI 3.1415926535897932384626433832795

Adafruit_MPU6050 mpu;
int16_t X_accelo, Z_accelo, Y_gyro, gyroRate, mu = 0.993;
float acceloAngle, CurAngle, prevAngle = 0, gyroAngle = 0, estimated_speed_ms = 0, step_angle = 1.8;
int estimated_speed_ss = 0, speed_error = 0, angle_error = 0, corrected_Stepper_demand = 0;
float angle_ref = 0;
unsigned long CurrTime = 0, PrevTime = 0, dt = 0;

AccelStepper leftStepper(AccelStepper::DRIVER, LEFT_STEP_PIN, LEFT_DIR_PIN);
AccelStepper rightStepper(AccelStepper::DRIVER, RIGHT_STEP_PIN, RIGHT_DIR_PIN);

float saturation(float parameter, float low_lim, float up_lim) {
  if (parameter > up_lim) {
    parameter = up_lim;
  } else if (parameter < low_lim) {
    parameter = low_lim;
  }
  return parameter;
}

float sKp = 1, sKi = 0, sKd = 0, sUp_lim = 90, sLow_lim = -90;
float aKp = 1, aKi = 0, aKd = 0, aUp_lim = SpeedMax, aLow_lim = -SpeedMax;

class PID {
public:
  PID(float kp, float ki, float kd, float up_lim, float low_lim);
  float controller(float reference, float parameter);

private:
  float kp, ki, kd, reference, parameter, error, prevError, derivative, integral, prevInput, up_lim, low_lim;
  unsigned long prevTime;
};

PID::PID(float kp, float ki, float kd, float up_lim, float low_lim) {
  this->kp = kp;
  this->ki = ki;
  this->kd = kd;
  reference = 0;
  parameter = 0;
  error = 0;
  prevError = 0;
  derivative = 0;
  integral = 0;
  prevInput = 0;
  this->up_lim = up_lim;
  this->low_lim = low_lim;
  prevTime = millis();
}

float PID::controller(float reference, float parameter) {
  unsigned long curTime = millis();
  float dt = (curTime - prevTime) / 1000.0;
  error = reference - parameter;
  derivative = (dt > 0) ? (error - prevError) / dt : 0;
  integral += error * dt;
  integral = saturation(integral, low_lim, up_lim);
  float control_signal = (kp * error) + (kd * derivative) + (ki * integral);
  prevTime = curTime;
  prevError = error;
  return control_signal;
}

PID* SpeedPID = new PID(sKp, sKi, sKd, sUp_lim, sLow_lim);
PID* AnglePID = new PID(aKp, aKi, aKd, aUp_lim, aLow_lim);

void setup(void) {
  Serial.begin(115200);
  leftStepper.setMaxSpeed(SpeedMax);
  rightStepper.setMaxSpeed(SpeedMax);

  while (!Serial) {
    delay(10);
  }

  Serial.println("Adafruit MPU6050 test!");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
    case MPU6050_RANGE_2_G:
      Serial.println("+-2G");
      break;
    case MPU6050_RANGE_4_G:
      Serial.println("+-4G");
      break;
    case MPU6050_RANGE_8_G:
      Serial.println("+-8G");
      break;
    case MPU6050_RANGE_16_G:
      Serial.println("+-16G");
      break;
  }

  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
    case MPU6050_RANGE_250_DEG:
      Serial.println("+- 250 deg/s");
      break;
    case MPU6050_RANGE_500_DEG:
      Serial.println("+- 500 deg/s");
      break;
    case MPU6050_RANGE_1000_DEG:
      Serial.println("+- 1000 deg/s");
      break;
    case MPU6050_RANGE_2000_DEG:
      Serial.println("+- 2000 deg/s");
      break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
    case MPU6050_BAND_260_HZ:
      Serial.println("260 Hz");
      break;
    case MPU6050_BAND_184_HZ:
      Serial.println("184 Hz");
      break;
    case MPU6050_BAND_94_HZ:
      Serial.println("94 Hz");
      break;
    case MPU6050_BAND_44_HZ:
      Serial.println("44 Hz");
      break;
    case MPU6050_BAND_21_HZ:
      Serial.println("21 Hz");
      break;
    case MPU6050_BAND_10_HZ:
      Serial.println("10 Hz");
      break;
    case MPU6050_BAND_5_HZ:
      Serial.println("5 Hz");
      break;
  }

  Serial.println("");
}

void motorRun() {
  leftStepper.setSpeed(-stepperSpeed);
  rightStepper.setSpeed(stepperSpeed);
  leftStepper.runSpeed();
  rightStepper.runSpeed();
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  CurrTime = millis();
  dt = CurrTime - PrevTime;
  PrevTime = CurrTime;

  Y_gyro = g.gyro.y;
  gyroRate = map(Y_gyro, -32768, 32767, -250, 250);
  gyroAngle = gyroAngle + (float)gyroRate * dt / 1000.0;

  X_accelo = a.acceleration.x;
  Z_accelo = a.acceleration.z;
  acceloAngle = atan2(X_accelo, Z_accelo) * RAD_TO_DEG;
  CurAngle = (mu * (prevAngle + gyroAngle) + (1 - mu) * (acceloAngle));
  Serial.print("Measured angle: ");
  Serial.print(CurAngle);
  Serial.print("\t");

  estimated_speed_ms += X_accelo * dt;
  estimated_speed_ms /= 1000;

  Serial.print("Estimated speed (meters per second): ");
  Serial.print(estimated_speed_ms);
  Serial.print("\t");
  
  if (abs(CurAngle) <= 4.5) {
    estimated_speed_ms = 0;
  }

  estimated_speed_ss = (estimated_speed_ms / 0.065 * PI) / (step_angle / (180.0 * PI));
  Serial.print("Estimated speed (steps per second): ");
  Serial.print(estimated_speed_ss);
  Serial.print("\t");

  prevAngle = CurAngle;

  angle_ref = SpeedPID->controller(Stepper_demand, estimated_speed_ss);
  corrected_Stepper_demand = AnglePID->controller(angle_ref, CurAngle);

  stepperSpeed = map(corrected_Stepper_demand, -SpeedMax, SpeedMax, -SpeedMax, SpeedMax);
  motorRun();
}
