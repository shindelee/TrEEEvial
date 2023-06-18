// #include <Adafruit_MPU6050.h>
#include <MPU6050_light.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>
#include <AccelStepper.h> // Include the AccelStepper library

/********** PID **********/
#include <PID_v1.h>

double kP = 6;
double kI = 0;
double kD = 0.5;
int LIMIT = 400;
int stepperSpeed;

double setpoint, input, output;   // PID variables
PID pid(&input, &output, &setpoint, kP, kI, kD, DIRECT); // PID setup

#define dirPinLeft 12
#define stepPinLeft 14

#define dirPinRight 27
#define stepPinRight 26

// Adafruit_MPU6050 mpu;
MPU6050 mpu(Wire);
int16_t Xacc, Zacc,gyroY, gyroRate, mu = 0.993;
float accangle, curangle, preangle = 0, gyroAngle = 0, error = 0, errorsum = 0;
unsigned long currTime, prevTime = 0, loopTime;
int anglecontroller = 0;
unsigned long timer = 0;

AccelStepper stepper1(AccelStepper::DRIVER, stepPinLeft, dirPinLeft); // Create an instance of AccelStepper for left motor
AccelStepper stepper2(AccelStepper::DRIVER, stepPinRight, dirPinRight); // Create an instance of AccelStepper for right motor

void setup(void) {
  Serial.begin(115200);

  /*
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
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
  */
  
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  // mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("Done!\n");
  
  pid.SetMode(AUTOMATIC);
  pid.SetOutputLimits(-LIMIT, LIMIT);
  //pid.SetSampleTime(10);

  // Set up the stepper motors
  stepper1.setMaxSpeed(LIMIT); // Set the maximum speed in steps per second
  stepper2.setMaxSpeed(LIMIT);
  
}

void motorRun(){
  stepper1.setSpeed(-stepperSpeed);
  stepper2.setSpeed(stepperSpeed);

  // Run the stepper motors
  stepper1.runSpeed();
  stepper2.runSpeed();

  Serial.print(output);
  Serial.println("");
}

void loop() {
  mpu.update();
  /*
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  currTime = millis();
  loopTime = currTime - prevTime;
  prevTime = currTime;

  gyroY = g.gyro.y;
  gyroRate = map(gyroY, -32768, 32767, -250, 250);
  gyroAngle = gyroAngle + (float)gyroRate*loopTime/1000;

  Xacc = a.acceleration.x;
  Zacc = a.acceleration.z;
  accangle = atan2(Xacc,Zacc)*RAD_TO_DEG;

  curangle = -(mu * (preangle + gyroAngle) + (1 - mu) * (accangle)); // complimentary filter
  Serial.print(curangle);
  Serial.print(" ");

  // PID vars
  setpoint = 0; //to set + angleV with input to move forward
  input = curangle;
  */

  input = mpu.getAngleY();

  pid.Compute(); 

  stepperSpeed = map(output, -LIMIT,LIMIT, -LIMIT, LIMIT); 

  motorRun();

  preangle = curangle;

  

}
