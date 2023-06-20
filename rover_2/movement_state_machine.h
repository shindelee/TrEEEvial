#ifndef DRIVE_FUNCTIONS_H
#define DRIVE_FUNCTIONS_H

#include <AccelStepper.h> 
#include <Math.h>

void update_state_history();
void update_left_sensor_history();
void update_right_sensor_history();
void initialise_state_history();
void initialise_left_sensor_history();
void initialise_right_sensor_history();
void set_speed(int left_sensor_reading, int right_sensor_reading, int required_speed);
void set_wall_states(AccelStepper *leftStepper, AccelStepper *rightStepper);
void leave_node(AccelStepper *leftStepper, AccelStepper *rightStepper);
void read_sensors();
void read_sensors_and_set_speed();
void turn_right_90(AccelStepper *leftStepper, AccelStepper *rightStepper);


#endif
