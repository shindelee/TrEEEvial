#ifndef DRIVE_FUNCTIONS_H
#define DRIVE_FUNCTIONS_H

// Include all the libraries we need here!
#include <Arduino.h>
#include <AccelStepper.h>
#include "shared_variables.h"

void turn_right_90(AccelStepper *leftStepper, AccelStepper *rightStepper);
void turn_left_90(AccelStepper *leftStepper, AccelStepper *rightStepper);
void turn_clockwise_360(AccelStepper *leftStepper, AccelStepper *rightStepper);
void set_speed(AccelStepper *leftStepper, AccelStepper *rightStepper);
void set_wall_states(AccelStepper *leftStepper, AccelStepper *rightStepper);
void read_sensors_and_set_speed(AccelStepper *leftStepper, AccelStepper *rightStepper);


float get_angle_turnedL(AccelStepper *leftStepper, AccelStepper *rightStepper);
float get_angle_turnedR(AccelStepper *leftStepper, AccelStepper *rightStepper);

#endif
