#include "drive_functions.h"

void turn_right_90(){
    rightStepper.move(108);
    leftStepper.move(108);
    while(rightStepper.distanceToGo() != 0 && leftStepper.distanceToGo() != 0) {
        rightStepper.run();
        leftStepper.run();
        // Serial.print("right stepper dist to go: " + String(rightStepper.distanceToGo()));
        // Serial.println("left stepper dist to go: " + String(leftStepper.distanceToGo()));
    }
}

void turn_left_90(){
    rightStepper.move(-108);
    leftStepper.move(-108);
    while(rightStepper.distanceToGo() != 0 && leftStepper.distanceToGo() != 0) {
        rightStepper.run();
        leftStepper.run();
        // Serial.print("right stepper dist to go: " + String(rightStepper.distanceToGo()));
        // Serial.println("left stepper dist to go: " + String(leftStepper.distanceToGo()));
    }
}

void turn_clockwise_360(){
    rightStepper.move(432);
    leftStepper.move(432);
    while(rightStepper.distanceToGo() != 0 && leftStepper.distanceToGo() != 0) {
        rightStepper.run();
        leftStepper.run();
        // Serial.print("right stepper dist to go: " + String(rightStepper.distanceToGo()));
        // Serial.println("left stepper dist to go: " + String(leftStepper.distanceToGo()));
  }
}

float get_angle_turnedL(){
    long leftWheelSteps = leftStepper.currentPosition() - prevWheelStepsL; // Get the current position of the left stepper in steps
    prevWheelStepsL = leftStepper.currentPosition();
    float leftWheelRevs = leftWheelSteps / STEPS_PER_REVOLUTION; 
    float distanceTravelledL = leftWheelRevs * wheelCircumference;
    float angleTurnedRadiansL = distanceTravelledL / wheelBase;
    float angleTurnedDegreesL = angleTurnedRadiansL * (180.0 / PI);
    return angleTurnedDegreesL;
}   

float get_angle_turnedR(){
    long rightWheelSteps = rightStepper.currentPosition() - prevWheelStepsR; // Get the current position of the left stepper in steps
    prevWheelStepsR = rightStepper.currentPosition();
    float rightWheelRevs = rightWheelSteps / STEPS_PER_REVOLUTION; 
    float distanceTravelledR = rightWheelRevs * wheelCircumference;
    float angleTurnedRadiansR = distanceTravelledR / wheelBase;
    float angleTurnedDegreesR = angleTurnedRadiansR * (180.0 / PI);
    return angleTurnedDegreesR;
}