#ifndef UART_H
#define UART_H

//include all the libraries needed here!!
#include <Arduino.h>
#include <AccelStepper.h>

void beacon_detection();
bool is_in_frame(int x_min, int x_max, int y_min, int y_max);
float size_bb(float min_x, float max_x, float min_y, float max_y);


#endif
