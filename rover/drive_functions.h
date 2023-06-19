#ifndef DRIVE_FUNCTIONS_H
#define DRIVE_FUNCTIONS_H

// Include all the libraries we need here!
#include <Arduino.h>
#include <AccelStepper.h>

void turn_right_90();
void turn_left_90();
void turn_clockwise_360();
float get_angle_turnedL();
float get_angle_turnedR();

#endif
