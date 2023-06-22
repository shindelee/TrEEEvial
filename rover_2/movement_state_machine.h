#ifndef MOVEMENT_STATE_MACHINE_H
#define MOVEMENT_STATE_MACHINE_H

#include <AccelStepper.h> 
#include <Math.h>

void update_state_history();
void update_left_sensor_history();
void update_right_sensor_history();
void initialise_state_history();
void initialise_left_sensor_history();
void initialise_right_sensor_history();
void set_speed(int left_sensor_reading, int right_sensor_reading, int required_speed);
void set_wall_states();
void leave_node();
void read_sensors();
void read_sensors_and_set_speed();
float read_ultrasound(int trigPin, int echoPin);



#endif
