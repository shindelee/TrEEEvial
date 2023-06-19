#include "drive_functions.h"

void turn_right_90(){
    leftStepper.setCurrentPosition(0);
    rightStepper.setCurrentPosition(0);
    leftStepper.move(108);
    rightStepper.move(108);
    while(rightStepper.distanceToGo() != 0 && leftStepper.distanceToGo() != 0) {
        rightStepper.run();
        leftStepper.run();
  }
}

void turn_left_90(){
    leftStepper.setCurrentPosition(0);
    rightStepper.setCurrentPosition(0);
    leftStepper.move(-108);
    rightStepper.move(-108);
    while(rightStepper.distanceToGo() != 0 && leftStepper.distanceToGo() != 0) {
        rightStepper.run();
        leftStepper.run();
  }
}

void turn_clockwise_360(){
    leftStepper.setCurrentPosition(0);
    rightStepper.setCurrentPosition(0);
    rightStepper.move(432);
    leftStepper.move(432);
    while(rightStepper.distanceToGo() != 0 && leftStepper.distanceToGo() != 0) {
        rightStepper.run();
        leftStepper.run();
  }
}

void set_speed(int left_sensor_reading, int right_sensor_reading){
    float difference = leftSensorReading - rightSensorReading;
    float p = 1;
    float weighted_difference = difference * p;

    leftStepper.setSpeed(-(50.0 + weighted_difference));
    rightStepper.setSpeed(50.0 - weighted_difference);
}

void set_wall_states() {
    if (wall_in_front)
    {
      state = FRONT_WALL;
      cur_state = "front wall";
      leftStepper.setSpeed(0);
      rightStepper.setSpeed(0);
    }
    else if (wall_on_left && wall_on_right && !wall_in_front)
    {
      state = TWO_WALLS;
      cur_state = "two wall";
      set_speed(leftSensorReading,rightSensorReading);
    }
  
    else if (wall_on_left && !wall_on_right)
    {
      state = LEFT_WALL;
      cur_state = "left wall";
      set_speed(leftSensorReading,50);
    }
    else if (!wall_on_left && wall_on_right)
    {
      state = RIGHT_WALL;
      cur_state = "right wall";
      set_speed(50,rightSensorReading);
    }
    else
    {
      state = NO_WALL;
      cur_state = "no wall";
      leftStepper.setSpeed(-50);
      rightStepper.setSpeed(50);
    }
}

void read_sensors_and_set_speed(){
    leftSensorReading = analogRead(leftSensorPin);
    rightSensorReading = analogRead(rightSensorPin);
    frontSensorReading = analogRead(frontSensorPin);
    wall_in_front = frontSensorReading > 50;
    wall_on_left = leftSensorReading > 50;
    wall_on_right = rightSensorReading > 50;

    set_wall_states();
        
    Serial.print("left wall: " + String(leftSensorReading) + "  ");
    Serial.print("right wall: " + String(rightSensorReading) + "  ");
    Serial.print("front wall: " + String(frontSensorReading) + "  ");
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