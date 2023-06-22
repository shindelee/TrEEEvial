#include "drive_functions.h"
#include "shared_variables.h"
#include "movement_state_machine.h"

void turn_left_90()
{
  leftStepper.setCurrentPosition(0);
  rightStepper.setCurrentPosition(0);

  //    leftStepper.move(108*2);
  //    rightStepper.move(108 *2);

  //    while(rightStepper.distanceToGo() != 0 && leftStepper.distanceToGo() != 0) {
  //        rightStepper.runSpeed();
  //        leftStepper.runSpeed();
  //    }
  float max_val = leftSensorReading;
  float left_target_position = 108 * 2;
  float right_target_position = 108 * 2;

  for (int i = 0; i < 5; i++)
  {
    if(leftSensorReading < 10)
    {
      straight_edge();
    }
    
    leftStepper.move(108 * 2 / 5); // DO NOT replace 108 *2 with target position
    rightStepper.move(108 * 2 / 5);
    leftStepper.setSpeed(20);
    rightStepper.setSpeed(20);

    while (leftStepper.distanceToGo() > 0)
    {
      leftStepper.runSpeed();
      rightStepper.runSpeed();
    }

    read_sensors();
    if (frontSensorReading < max_val)
    {
      max_val = frontSensorReading;
      left_target_position = leftStepper.currentPosition();
      right_target_position = rightStepper.currentPosition();
    }
  }
}

void straight()
{
  leftStepper.setCurrentPosition(0);
  rightStepper.setCurrentPosition(0);
  leftStepper.move(-20 * 2);
  rightStepper.move(20 * 2);
  leftStepper.setSpeed(-50 * 2);
  rightStepper.setSpeed(50 * 2);
  while (rightStepper.distanceToGo() != 0 && leftStepper.distanceToGo() != 0)
  {
    rightStepper.runSpeed();
    leftStepper.runSpeed();
  }
}

void straight_edge()
{
  leftStepper.setCurrentPosition(0);
  rightStepper.setCurrentPosition(0);
  leftStepper.move(-250);
  rightStepper.move(250);
  leftStepper.setSpeed(-50 * 2);
  rightStepper.setSpeed(50 * 2);
  while (rightStepper.distanceToGo() != 0 && leftStepper.distanceToGo() != 0)
  {
    rightStepper.runSpeed();
    leftStepper.runSpeed();
  }
}

void reverse()
{
  leftStepper.setCurrentPosition(0);
  rightStepper.setCurrentPosition(0);
  leftStepper.move(-80);
  rightStepper.move(-80);
  leftStepper.setSpeed(-50 * 2);
  rightStepper.setSpeed(-50 * 2);
  while (rightStepper.distanceToGo() != 0 && leftStepper.distanceToGo() != 0)
  {
    rightStepper.runSpeed();
    leftStepper.runSpeed();
  }
}

// void go_to_centre(){
//     leftStepper.setCurrentPosition(0);
//     rightStepper.setCurrentPosition(0);
//     leftStepper.move(-100);
//     rightStepper.move(100);
//     leftStepper.setSpeed(-50);
//     rightStepper.setSpeed(50);
//     while(rightStepper.distanceToGo() != 0 && leftStepper.distanceToGo() != 0) {
//         int last_reading = leftStepper.currentPosition;
//         if (leftStepper.currentPosition - last_reading  = 10) {
//
//           };
//           rightStepper.runSpeed();
//           leftStepper.runSpeed();
//
//     }
// }

void edge_out()
{
  leftStepper.setCurrentPosition(0);
  rightStepper.setCurrentPosition(0);
  leftStepper.move(-40 * 2);
  rightStepper.move(40 * 2);
  leftStepper.setSpeed(-50 * 2);
  rightStepper.setSpeed(50 * 2);
  while (rightStepper.distanceToGo() != 0 && leftStepper.distanceToGo() != 0)
  {
    rightStepper.runSpeed();
    leftStepper.runSpeed();
  }
}

void turn_right_90()
{
//  Serial.println("turning 90 to the right!");
  leftStepper.setCurrentPosition(0);
  rightStepper.setCurrentPosition(0);
  leftStepper.move(-108 * 2);
  rightStepper.move(-108 * 2);
  leftStepper.setSpeed(-40);
  rightStepper.setSpeed(-40);
  while (rightStepper.distanceToGo() != 0 && leftStepper.distanceToGo() != 0)
  {
    rightStepper.runSpeed();
    leftStepper.runSpeed();
  }
}

void turn_180()
{
  leftStepper.setCurrentPosition(0);
  rightStepper.setCurrentPosition(0);
  leftStepper.move(216 * 2);
  rightStepper.move(-216 * 2);
  while (rightStepper.distanceToGo() != 0 && leftStepper.distanceToGo() != 0)
  {
    rightStepper.run();
    leftStepper.run();
  }
}

void turn_clockwise_360()
{
  leftStepper.setCurrentPosition(0);
  rightStepper.setCurrentPosition(0);
  rightStepper.move(432 * 2);
  leftStepper.move(432 * 2);
  while (rightStepper.distanceToGo() != 0 && leftStepper.distanceToGo() != 0)
  {
    rightStepper.run();
    leftStepper.run();
  }
}



/*
float get_angle_turnedL(AccelStepper *leftStepper, AccelStepper *rightStepper){
    long leftWheelSteps = leftStepper->currentPosition() - prevWheelStepsL; // Get the current position of the left stepper in steps
    prevWheelStepsL = leftStepper->currentPosition();
    float leftWheelRevs = leftWheelSteps / STEPS_PER_REVOLUTION;
    float distanceTravelledL = leftWheelRevs * wheelCircumference;
    float angleTurnedRadiansL = distanceTravelledL / wheelBase;
    float angleTurnedDegreesL = angleTurnedRadiansL * (180.0 / PI);
    return angleTurnedDegreesL;
}

float get_angle_turnedR(AccelStepper *leftStepper, AccelStepper *rightStepper){
    long rightWheelSteps = rightStepper->currentPosition() - prevWheelStepsR; // Get the current position of the left stepper in steps
    prevWheelStepsR = rightStepper->currentPosition();
    float rightWheelRevs = rightWheelSteps / STEPS_PER_REVOLUTION;
    float distanceTravelledR = rightWheelRevs * wheelCircumference;
    float angleTurnedRadiansR = distanceTravelledR / wheelBase;
    float angleTurnedDegreesR = angleTurnedRadiansR * (180.0 / PI);
    return angleTurnedDegreesR;
}
*/
