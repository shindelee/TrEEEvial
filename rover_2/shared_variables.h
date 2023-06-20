#ifndef SHARED_VARIABLES_H
#define SHARED_VARIABLES_H

#include <WiFi.h>
#include <WebSocketClient.h>

extern WiFiClient* client;
extern WebSocketClient* webSocketClient;

// rover specs
extern const float STEPS_PER_REVOLUTION;
extern const float WHEEL_RADIUS;
extern const float wheelDiameter;
extern const float wheelBase;
extern const float wheelCircumference;

// Define the baud rate
extern const long int BAUD_RATE;

//motor pin interface
extern const int LEFT_STEP_PIN;    // A3
extern const int LEFT_DIR_PIN;     // D12

extern const int RIGHT_STEP_PIN;   // A4
extern const int RIGHT_DIR_PIN;     // D11

extern const int RX_PIN;           // D9 
extern const int TX_PIN;           // D8

// sensor pin interface
extern const int leftSensorPin;   // A5
extern const int frontSensorPin;
extern const int rightSensorPin;  // vn

// movement state machine
extern const int TWO_WALLS;
extern const int FRONT_WALL;
extern const int LEFT_WALL;
extern const int RIGHT_WALL;
extern const int NO_WALL;
extern const int STATE_CHANGE;

//state variables
extern int state;
extern String previous_state;
extern String cur_state;
extern bool in_node; // boolean to tell whether we are in a node
extern int frontSensorReading;
extern int leftSensorReading;
extern int rightSensorReading;
extern int last_sensor_reading_left;
extern int last_sensor_reading_right;
extern bool wall_on_left;
extern bool wall_on_right;
extern bool wall_in_front;

// wall information for message to be sent to EC2
extern bool start;
extern String message_to_send;
extern String message_received;
extern int x; //x coordinate
extern int y; //y coordinate
extern int l; //left wall
extern int f; //front wall
extern int r; //right wall

// websocket and WiFi stuff
extern const char* ssid;
extern const char* password;
extern char path[];
extern char host[];

#endif
