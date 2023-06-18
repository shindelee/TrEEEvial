//#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "Wire.h"
#include <math.h>
#include <AccelStepper.h> // Include the AccelStepper library
#include <MPU6050_light.h>

/********** PID **********/
#include <PID_v1.h>

double kP = 7;
double kI = 0.1;
double kD = 1;
int LIMIT = 400;
int stepperSpeed;

double setpoint, input, output;   // PID variables
PID pid(&input, &output, &setpoint, kP, kI, kD, DIRECT); // PID setup

#define dirPinLeft 15
#define stepPinLeft 32

#define dirPinRight 4
#define stepPinRight 27

MPU6050 mpu(Wire);
unsigned long timer = 0;

int16_t Xacc, Zacc, gyroY, gyroRate, mu = 0.993;
float accangle, curangle, preangle = 0, gyroAngle = 0, error = 0, errorsum = 0;
unsigned long currTime, prevTime = 0, loopTime;
int anglecontroller = 0;

AccelStepper stepper1(AccelStepper::DRIVER, stepPinLeft, dirPinLeft); // Create an instance of AccelStepper for left motor
AccelStepper stepper2(AccelStepper::DRIVER, stepPinRight, dirPinRight); // Create an instance of AccelStepper for right motor

unsigned long lastAngleUpdateTime = 0;
const unsigned long ANGLE_UPDATE_INTERVAL = 500; // Update angle every 200 milliseconds (5 times slower than the loop iteration)

void setup(void) {
  Serial.begin(115200);
  Wire.begin();

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

  // Set up the stepper motors
  stepper1.setMaxSpeed(LIMIT); // Set the maximum speed in steps per second
  stepper2.setMaxSpeed(LIMIT);
}

void motorRun() {
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
  
  if((millis() - timer) > ANGLE_UPDATE_INTERVAL){
    curangle = mpu.getAngleY();
    Serial.println(curangle);
    timer = millis();
  }
  
  // PID vars
  setpoint = 0; // to set + angleV with input to move forward
  input = curangle;

  pid.Compute();

  stepperSpeed = output;

  motorRun();
  preangle = curangle;
  
}
