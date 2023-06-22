#ifndef DRIVE_FUNCTIONS_H
#define DRIVE_FUNCTIONS_H

// Include all the libraries we need here!
#include <Arduino.h>
#include <AccelStepper.h>
#include "shared_variables.h"

void turn_left_90();
void straight();
void turn_180();
void turn_clockwise_360();
void set_speed(int left_sensor_reading, int right_sensor_reading);
void turn_right_90();
void edge_out();
// void set_wall_states();

// float get_angle_turnedL();
// float get_angle_turnedR();

#endif
