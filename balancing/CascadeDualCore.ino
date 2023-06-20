#include <Adafruit_MPU6050.h>
#include "Wire.h"
#include <math.h>
#include <AccelStepper.h>  // Include the AccelStepper library
#include <MPU6050_light.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

/********** PID **********/
#include <PID_v1.h>

double angle_kP = 60;  // PID gains for angle control
double angle_kI = 0;
double angle_kD = 30;

double velocity_kP = 0.001;  // PID gains for velocity control
double velocity_kI = 0.001;
double velocity_kD = 0.01;

int LIMIT = 400;

float accY;                  // Accelerometer reading for Y-axis
float velocity = 0.0;        // Current velocity
unsigned long previousTime;  // Previous timestamp
const float dampingFactor = 0.95;  // Damping factor to limit velocity accumulation

double CENTRE = -1.2; // centre of gravity 

double maxAcceleration = 130.0;  // Maximum acceleration in steps/s^2
int targetVelocity = 0;          // Target velocity in steps/s
int currentVelocity = 0;         // Current velocity in steps/s
int stepperSpeed = 0;            // Stepper motor speed in steps/s

double angleSetpoint, angleInput, angleOutput;                                                  // Angle PID variables
PID anglePid(&angleInput, &angleOutput, &angleSetpoint, angle_kP, angle_kI, angle_kD, DIRECT);  // Angle PID setup

double velocitySetpoint, velocityInput, velocityOutput;                                                              // Velocity PID variables
PID velocityPid(&velocityInput, &velocityOutput, &velocitySetpoint, velocity_kP, velocity_kI, velocity_kD, DIRECT);  // Velocity PID setup

#define dirPinLeft 15
#define stepPinLeft 32

#define dirPinRight 4
#define stepPinRight 27

#define PI 3.141


Adafruit_MPU6050 mpu;
float accAngle = 0;     // Angle calculated from accelerometer
float gyroRate = 0;     // Angular rate from gyroscope
float compAngle = 0;    // Complementary angle
const float alpha = 0.98;  // Complementary filter coefficient

AccelStepper stepper1(AccelStepper::DRIVER, stepPinLeft, dirPinLeft);    // Create an instance of AccelStepper for left motor
AccelStepper stepper2(AccelStepper::DRIVER, stepPinRight, dirPinRight);  // Create an instance of AccelStepper for right motor

unsigned long lastAngleUpdateTime = 0;
const unsigned long ANGLE_UPDATE_INTERVAL = 500;  // Update angle every 500 milliseconds

void setup(void) {

  Serial.begin(115200);
  Wire.begin();
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  previousTime = millis();  // Initialize previousTime with the current timestamp
  

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

  if (stepperSpeed > LIMIT){
    stepperSpeed = LIMIT;
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

  unsigned long currentTime = millis();  // Current timestamp
  unsigned long deltaTime = currentTime - previousTime;  // Time difference since the last iteration

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Accelerometer-based angle calculation
  accAngle = atan2(-a.acceleration.x, a.acceleration.z) * RAD_TO_DEG;

  // Gyroscope-based angle calculation
  gyroRate = g.gyro.y / 131.0;  // Adjust the division factor based on the gyroscope sensitivity

  // Complementary filter
  compAngle = alpha * (compAngle + gyroRate * 0.01) + (1 - alpha) * accAngle;

  Serial.print("Complementary Angle: ");
  Serial.print(compAngle);
  Serial.print("\t\t");

  if (deltaTime >= 10) {  // Sample the accelerometer every 10 milliseconds (adjust as needed)
    previousTime = currentTime;  // Update previousTime with the current timestamp

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    accY = a.acceleration.y;

    // Apply damping factor to limit velocity accumulation
    velocity = dampingFactor * (velocity + (accY * deltaTime) / 1000.0);  // Convert deltaTime to seconds

    // Print the current velocity
    
  }

  Serial.print("Velocity: ");
  Serial.print(velocity);
  Serial.print("\t\t");

  // Velocity PID control
  velocitySetpoint = 0;  // Set the target velocity as the output of the angle PID
  velocityInput = velocity;

  double vErr = velocitySetpoint - velocityInput;
  Serial.print("Velocity error:");
  Serial.print(vErr);
  Serial.print("\t\t");

  velocityPid.Compute();

  // Angle PID control
  angleSetpoint = velocityOutput;  // Desired angle (set as per your requirements)
  angleInput = compAngle;

  // Serial.print("Angle error: ");
  // double aErr = angleSetpoint - angleInput;
  // Serial.print(aErr);
  // Serial.print("\t\t");

  anglePid.Compute();

  // Update the target velocity based on the output of the velocity PID
  targetVelocity = angleOutput;

  // Gradually adjust the current velocity towards the target velocity
  // if (currentVelocity < targetVelocity) {
  //   currentVelocity += maxAcceleration;
  //   if (currentVelocity > targetVelocity) {
  //     currentVelocity = targetVelocity;
  //   }
  // } else if (currentVelocity > targetVelocity) {
  //   currentVelocity -= maxAcceleration;
  //   if (currentVelocity < targetVelocity) {
  //     currentVelocity = targetVelocity;
  //   }
  // }

  
  // Update the stepper motor speed based on the current velocity
  stepperSpeed = targetVelocity;

  //Dead band angle
  if (compAngle < CENTRE + 1.5 && compAngle > CENTRE - 1.5) {
    stepperSpeed = 0;
    currentVelocity = 0;
  }

  Serial.print(" Stepper : ");
  Serial.println(stepperSpeed);

  //prevAngle = compAngle;

   //delay(2); //delay on angle calculation

   
}
