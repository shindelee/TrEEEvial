#include <AccelStepper.h>
#include <elapsedMillis.h>

// Motor Connections (unipolar motor driver)
const int In1 = 8;
const int In2 = 9;
const int In3 = 10;
const int In4 = 11;
// Motor Connections (constant voltage bipolar H-bridge motor driver)
const int AIn1 = 8;
const int AIn2 = 9;
const int BIn1 = 10;
const int BIn2 = 11;

// Front sensor.
const int lightSensorPin = 25;

// Motor Connections (constant current, step/direction bipolar motor driver)
const int dirPinR = 27;
const int stepPinR = 26;

const int dirPinL = 12;
const int stepPinL = 14;

AccelStepper myStepperR(AccelStepper::DRIVER, stepPinR, dirPinR);         
AccelStepper myStepperL(AccelStepper::DRIVER, stepPinL, dirPinL);

//rover properties
#define WHEEL_DIAMETER 0.067 // in meters
#define WHEELS_DISTANCE 0.22 // in meters
#define STEPS_PER_REVOLUTION 200 // steps per revolution for your motor

//calculate rotation angle
float calculateAngle(int steps) {
  float wheelCircumference = PI * WHEEL_DIAMETER;
  float totalCircumference = 2 * PI * WHEELS_DISTANCE;
  float revolutions = (float)steps / STEPS_PER_REVOLUTION;
  float distanceTraveled = revolutions * wheelCircumference;
  float angle = 360 * (distanceTraveled / totalCircumference);
  return angle;
}

int turn90 {
  int arc_length = WHEELS_DISTANCE * PI / 2; //90degrees
  int distance_per_step = STEPS_PER_REVOLUTION / (PI * WHEEL_DIAMETER);
  return arc_length/distance_per_step;
}


void setup() {
  myStepperL.moveTo(0);
  myStepperR.moveTo(0);
  
}

void loop() {
  //left turn
  myStepperL.moveTo(turn90);
  myStepperR.moveTo(0);

  delay(5000);

  myStepperL.moveTo(0);
  myStepperR.moveTO(turn90);
  
}
