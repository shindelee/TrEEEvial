#include <Adafruit_MPU6050.h>
#include "Wire.h"
#include <math.h>
#include <AccelStepper.h>  // Include the AccelStepper library
#include <MPU6050_light.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>


/********** PID **********/
#include <PID_v1.h>

double angle_kP = 392;  //Increase response time but cause overshooting and oscillation
double angle_kI = 0.0001; // remove bias/offset error but can cause oscillation and instability
double angle_kD = 32; //Damps the oscillation but too high can cause instability

double velocity_kP = 0.001;  // PID gains for velocity control
double velocity_kI = 0.0001;
double velocity_kD = 0.3;
int LIMIT = 128000;

double CENTRE = 0;          // Desired Angle
unsigned long previousTime;  // Previous timestamp

double maxAcceleration = 128000.0;  // Maximum acceleration in steps/s^2
int targetVelocity = 0;           // Target velocity in steps/s
int currentVelocity = 0;          // Current velocity in steps/s
int stepperSpeed = 0;             // Stepper motor speed in steps/s

double angleSetpoint, angleInput, angleOutput;                                                  // Angle PID variables
PID anglePid(&angleInput, &angleOutput, &angleSetpoint, angle_kP, angle_kI, angle_kD, DIRECT);  // Angle PID setup

double velocitySetpoint, velocityInput, velocityOutput;                                                              // Velocity PID variables
PID velocityPid(&velocityInput, &velocityOutput, &velocitySetpoint, velocity_kP, velocity_kI, velocity_kD, DIRECT);  // Velocity PID setup

#define dirPinLeft 15
#define stepPinLeft 32

#define dirPinRight 4
#define stepPinRight 27

#define red 2
#define blue 12

MPU6050 mpu(Wire);
unsigned long timer = 0;

float compAngle = 0;

AccelStepper stepper1(AccelStepper::DRIVER, stepPinLeft, dirPinLeft);    // Create an instance of AccelStepper for left motor
AccelStepper stepper2(AccelStepper::DRIVER, stepPinRight, dirPinRight);  // Create an instance of AccelStepper for right motor

void setup(void) {


  Serial.begin(115200);
  pinMode(blue, OUTPUT); //setup LED
  pinMode(red, OUTPUT);
  Wire.begin();

  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while (status != 0) {}  // stop everything if could not connect to MPU6050


  Serial.println(F("Calculating offsets, do not move MPU6050"));
  digitalWrite(blue, HIGH); //switch on LED
  delay(1000);
  // mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
  mpu.calcOffsets();  // gyro and accelero
  Serial.println("Done!\n");
  digitalWrite(blue, LOW);   //switch off LED

  anglePid.SetMode(AUTOMATIC);
  anglePid.SetOutputLimits(-LIMIT, LIMIT);

  velocityPid.SetMode(AUTOMATIC);
  velocityPid.SetOutputLimits(-LIMIT, LIMIT);

  // Set up the stepper motors
  stepper1.setMaxSpeed(LIMIT);  // Set the maximum speed in steps per second
  stepper2.setMaxSpeed(LIMIT);

  stepper1.setAcceleration(maxAcceleration);  // Set the maximum acceleration in steps per second squared
  stepper2.setAcceleration(maxAcceleration);

  stepper1.setCurrentPosition(0);  // Set the initial position of the stepper motors to 0
  stepper2.setCurrentPosition(0);

  xTaskCreatePinnedToCore(
    task2,    // Function to run as a task
    "Task2",  // Name of the task
    10000,    // Stack size (in words)
    NULL,     // Task parameter
    1,        // Task priority
    NULL,     // Task handle
    1         // Core to run the task on (0 or 1)
  );
}

void motorRun() {
  // Set the direction and speed of the stepper motors
  if (angleOutput >= 0) {
    digitalWrite(dirPinLeft, LOW);
    digitalWrite(dirPinRight, HIGH);
  } else {
    digitalWrite(dirPinLeft, HIGH);
    digitalWrite(dirPinRight, LOW);
  }

  stepper1.setSpeed(-stepperSpeed);
  stepper2.setSpeed(stepperSpeed);

  stepper1.setAcceleration(maxAcceleration);
  stepper2.setAcceleration(maxAcceleration);

  // Run the stepper motors
  stepper1.runSpeed();
  stepper2.runSpeed();
}

void task2(void *pvParameters) {
  for (;;) {
    motorRun();
    vTaskDelay(1);  // Delay to allow other tasks to run
  }
}


void loop() {

  unsigned long currentTime = millis();                  // Current timestamp
  unsigned long deltaTime = currentTime - previousTime;  // Time difference since the last iteration

  //Use MPULight
  if (deltaTime >= 20) {
    mpu.update();
    compAngle = mpu.getAngleY();
  

  // Velocity PID control
  velocitySetpoint = 0;  // Set the target velocity as the output of the angle PID
  velocityInput = velocity;

  velocityPid.Compute();

  // Angle PID control
  angleSetpoint = velocityOutput;  // Desired angle for 1 PID,  for casecade use output of velocity PID
  angleInput = compAngle;

  anglePid.Compute();

  // Update the target velocity based on the output of the velocity PID
  targetVelocity = angleOutput;

  // Update the stepper motor speed based on the current velocity
  stepperSpeed = targetVelocity;

  //Dead band angle
  if (compAngle < CENTRE + 1.5 && compAngle > CENTRE - 1.5) {
    stepperSpeed = 0;
    currentVelocity = 0;
    digitalWrite(red, HIGH); //switch on LED
  }
  else{
    digitalWrite(red, LOW); //switch on LED
  }

  Serial.print("compAngle : ");
  Serial.print(compAngle);
  Serial.print("\t\t");

  Serial.print(" Stepper : ");
  Serial.println(stepperSpeed);

  previousTime = currentTime;  // Update previousTime with the current timestamp
  }
}
