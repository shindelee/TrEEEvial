#include <Adafruit_MPU6050.h>
#include <Wire.h>
#include <PID_v2.h>
#include <AccelStepper.h>
#include <math.h>
#include <Arduino.h>

Adafruit_MPU6050 mpu;
int16_t X_accelo, Z_accelo, Y_gyro, gyroRate, mu = 0.993;
float acceloAngle = 0, CurrAngle =0 , prevAngle = 0, gyroAngle = 0, StepAngle = 1.8;
unsigned long CurrTime = 0, PrevTime = 0, dt = 0;
const unsigned long loopTime = 10; // Desired loop time in milliseconds
int Est_Speed = 0, Est_Speed_Step;

// Tune PID through trial and error
double kp_angle = 1.0;
double ki_angle = 0.0;
double kd_angle = 0.0;
double kp_speed = 1.0;
double ki_speed = 0.0;
double kd_speed = 0.0;

int SpeedMax = 400; // Maximum motor speed
int StepperDemand = 0;
int StepperSpeed;

#define LEFT_DIR_PIN 15 // -> 15 -> D12
#define LEFT_STEP_PIN 32 // -> 32 -> A4
#define RIGHT_DIR_PIN 4 // -> 4 -> D11
#define RIGHT_STEP_PIN 27 // -> 27 -> A0
#define PI 3.1415926535897932384626433832795

// Define PID objects
PID anglePID(&angle_input, &angle_output, &angle_setpoint, kp_angle, ki_angle, kd_angle, DIRECT);
PID speedPID(&speed_input, &speed_output, &speed_setpoint, kp_speed, ki_speed, kd_speed, DIRECT);

// Define stepper motor object
AccelStepper leftStepper(AccelStepper::DRIVER, LEFT_STEP_PIN, LEFT_DIR_PIN);
AccelStepper rightStepper(AccelStepper::DRIVER, RIGHT_STEP_PIN, RIGHT_DIR_PIN);

double angle_input = 0.0;
double speed_input = 0.0;
double angle_setpoint = 0.0;
double speed_setpoint = 0.0;
double angle_output = 0.0;
double speed_output = 0.0;

// Variables for synchronization
volatile bool syncFlag = false; // Flag to synchronize the cores

// Function to be executed on the second core
void IRAM_ATTR motorControlTask(void *parameter)
{
  for (;;)
  {
    if (syncFlag)
    {
      // Motor control code goes here
      leftStepper.setSpeed(-StepperSpeed);
      rightStepper.setSpeed(StepperSpeed);
      leftStepper.runSpeed();
      rightStepper.runSpeed();

      syncFlag = false; // Reset the flag after executing the motor control code
    }
    vTaskDelay(1); // Small delay to yield the core
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Initialize MPU6050
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
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  leftStepper.setMaxSpeed(SpeedMax);
  rightStepper.setMaxSpeed(SpeedMax);

  // Set output limits for PID controllers
  anglePID.SetOutputLimits(-SpeedMax, SpeedMax);
  speedPID.SetOutputLimits(-75, 75); // the rover prevents MPU6050 from reaching +-90

  // Attach the motor control task to the second core
  xTaskCreatePinnedToCore(
      motorControlTask,  // Function to be executed
      "motorTask",       // Task name
      10000,             // Stack size
      NULL,              // Task parameters
      1,                 // Priority
      NULL,              // Task handle
      1);                // Core number (1 for the second core)
}

void loop()
{
  CurrTime = millis();
  dt = CurrTime -PrevTime;

  if (dt >= loopTime)
  {
   PrevTime = CurrTime;

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    Y_gyro = g.gyro.y;
    gyroRate = map(Y_gyro, -32768, 32767, -250, 250);
    gyroAngle = gyroAngle + (float)gyroRate * dt / 1000.0;

    X_accelo = a.acceleration.x;
    Z_accelo = a.acceleration.z;
    acceloAngle = atan2(X_accelo, Z_accelo) * RAD_TO_DEG;
    CurrAngle = (mu * (prevAngle + gyroAngle) + (1 - mu) * (acceloAngle)); // comp filter

    Est_Speed += abs(X_accelo) * dt;
    Est_Speed /= 1000;
    Est_Speed = (X_accelo > 0) ? (Est_Speed*1):(Est_Speed*(-1));
    if (abs(CurrAngle) <= 3.5) { // int reset
      Est_Speed = 0;
    }
    Est_Speed_Step = (Est_Speed / 0.065 * PI) / (StepAngle / (180.0 * PI));

    speed_input = Est_Speed_Step;
    speed_setpoint = StepperDemand;
    speedPID.Compute();
    angle_input = CurrAngle;
    angle_setpoint = speed_output;
    anglePID.Compute();
    StepperSpeed = angle_output;

    // Set the synchronization flag to execute the motor control code
    syncFlag = true;
  }
}

