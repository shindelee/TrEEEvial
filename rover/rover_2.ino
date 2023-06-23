
// include header files

#include "drive_functions.h"
#include "websocket_client.h"
#include "balancing.h"
#include "uart.h"
#include "shared_variables.h"
#include "movement_state_machine.h"

// define globals here
// websocket and WiFi stuff

const char *ssid = "iPhone";
const char *password = "12345678";
char path[] = "/";
char host[] = "54.144.255.182:5000";

WebSocketClient *webSocketClient = new WebSocketClient();

// Use WiFiClient class to create TCP connections
WiFiClient *client = new WiFiClient();

/*
WebSocketClient webSocketClient;

// Use WiFiClient class to create TCP connections
WiFiClient client;
*/

// wall information for message to be sent to EC2
bool start = true;
String message_to_send = "";
String message_received = "";
int x = 0; // x coordinate
int y = 0; // y coordinate
int l = 0; // left wall
int f = 0; // front wall
int r = 0; // right wall

// Define the baud rate
const long int BAUD_RATE = 115200;

// motor pin interface
const int LEFT_STEP_PIN = 33; // A3
const int LEFT_DIR_PIN = 15;  // D12

const int RIGHT_STEP_PIN = 32; // A4
const int RIGHT_DIR_PIN = 4;   // D11

const int RX_PIN = 16; // D9
const int TX_PIN = 17; // D8

// sensor pin interface
const int leftSensorPin = 34;  // A5 for now
const int frontSensorPin = 39; // A1 for now
const int rightSensorPin = 35; // A5

// rover specs
const float STEPS_PER_REVOLUTION = 200.0;
const float WHEEL_RADIUS = 0.0325;
const float wheelDiameter = 2.0 * WHEEL_RADIUS;
const float wheelBase = 0.14;
const float wheelCircumference = wheelDiameter * PI;

int last_sent_message;

// motors
AccelStepper leftStepper(AccelStepper::DRIVER, LEFT_STEP_PIN, LEFT_DIR_PIN);
AccelStepper rightStepper(AccelStepper::DRIVER, RIGHT_STEP_PIN, RIGHT_DIR_PIN);

// movement state machine

const int TWO_WALLS = 1;
const int FRONT_WALL = 2;
const int LEFT_WALL = 3;
const int RIGHT_WALL = 4;
const int NO_WALL = 5;
const int STATE_CHANGE = 6;

const int FOLLOW_LEFT = 1;
const int FOLLOW_RIGHT = 2;

// ultrasound pins
const int trigPinLeft = 13; // sda
const int echoPinLeft = 12; // scl

const int trigPinRight = 27;
const int echoPinRight = 26;

const int trigPinFront = 2;
const int echoPinFront = 14;

// state variables
int state;
String previous_state;
String cur_state;
bool in_node = true; // boolean to tell whether we are in a node
float frontSensorReading = 0;
float leftSensorReading = 0;
float rightSensorReading = 0;
float last_sensor_reading_left = 0;
float last_sensor_reading_right = 0;
bool wall_on_left;
bool wall_on_right;
bool wall_in_front;
int state_history[5];
// int left_sensor_history[5];
// int right_sensor_history[5];
int sensor_setpoint = 15;
int sensor_upper_bound = 15; // 12;
int sensor_front = 10; // 10;
int wall_follow_state = 0;
unsigned long int timer1 = 0;
unsigned long int timer2 = 300000;

// test stuff
// int test[10] = {204,204,93,93,12,-12,460,-460,274,626};
// int test2[10] = {1,0,1,1,1,0,1,1,0,0};

void setup()
{
  Serial.begin(115200);

//  EC2 Connection
  initWiFi(ssid, password);
  delay(50);
  initWebSocket("54.144.255.182", 5000, client);
  handshake(path,host, webSocketClient, client);
  last_sent_message = millis();

  // motor preamble
  leftStepper.setMaxSpeed(400);
  leftStepper.setAcceleration(10);
  rightStepper.setMaxSpeed(400);
  rightStepper.setAcceleration(10);

  // get initial sensor reading
  read_sensors;
  set_speed(leftSensorReading, 10, 100);
  previous_state = cur_state;

  // ultrasound sensor setup
  pinMode(trigPinLeft, OUTPUT);
  pinMode(echoPinLeft, INPUT);

  pinMode(trigPinRight, OUTPUT);
  pinMode(echoPinRight, INPUT);

  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);

  //  initialise_left_sensor_history();
  //  initialise_right_sensor_history();

  // state machine
  // change this code !
  wall_follow_state = FOLLOW_LEFT;
}

void loop()
{
  // 5 minutes
  if ((millis() - timer1) > 500000){
    Serial.println("entering more than 5 min");
  
    if((millis() - timer2) > 40000){
      Serial.println("entering 40 seconds");
      if((wall_follow_state) == FOLLOW_LEFT){
        wall_follow_state = FOLLOW_RIGHT;
      }
      else{
        wall_follow_state = FOLLOW_LEFT;
      }
    }

    timer2 = millis();
    
    int time_since_reading_left = abs(leftStepper.currentPosition() - last_sensor_reading_left);
    int time_since_reading_right = abs(rightStepper.currentPosition() - last_sensor_reading_right);

    if(millis() - last_sent_message > 1000){
      int left = int(wall_on_left);
      int right = int(wall_on_right);
      int front = int(wall_in_front);
      int hi[3] = {2,0,1};
      message_received = send_data(leftStepper.currentPosition(), leftStepper.currentPosition(), left, right, front, hi , 0, 0);
      leftStepper.setCurrentPosition(0);
        rightStepper.setCurrentPosition(0);
      last_sent_message = millis();
    }

    if ((time_since_reading_left >= 30 || time_since_reading_right >= 30))
    {
      read_sensors();

      if (wall_follow_state == FOLLOW_LEFT){
        if (wall_in_front) {
        leftStepper.stop();
        rightStepper.stop();

        reverse();
        Serial.print("TurnRight90");
        turn_right();
      } 

      if (leftSensorReading < 4) {
        leftStepper.stop();
        rightStepper.stop();
        turn_right_20();
        adjust();
      }
        set_speed(leftSensorReading, 10, 150);
      }
      else if (wall_follow_state == FOLLOW_RIGHT){
        if (wall_in_front) {
        leftStepper.stop();
        rightStepper.stop();

        reverse();
        Serial.print("TurnRight90");
        turn_left();
      } 

      if (leftSensorReading < 4) {
        leftStepper.stop();
        rightStepper.stop();
        turn_left_20();
        adjust();
      }
        set_speed(10 , rightSensorReading, 150);
      }
   
      if (wall_in_front && wall_on_left && wall_on_right){
        leftStepper.stop();
        rightStepper.stop();

        reverse();
        turn_180();
      } 

      Serial.print("left speed = " + String(leftStepper.speed()) + "   ");
      Serial.println("right speed = " + String(rightStepper.speed()));

      // run the stepper
      leftStepper.runSpeed();
      rightStepper.runSpeed();

    }
  }
  
  // before 5 minutes (follow the LEFT wall)
  else
  {
    int time_since_reading_left = abs(leftStepper.currentPosition() - last_sensor_reading_left);
    int time_since_reading_right = abs(rightStepper.currentPosition() - last_sensor_reading_right);

    Serial.println("frontSensorReading: " + String(frontSensorReading) + "    " 
    + "leftSensorReading: " + String(leftSensorReading) + "     " + "rightSensorReading: " + String(rightSensorReading));

    if ((time_since_reading_left >= 20 || time_since_reading_right >= 20))
    {
      read_sensors();

      if(millis() - last_sent_message > 1000) {
        int left = int(wall_on_left);
        int right = int(wall_on_right);
        int front = int(wall_in_front);
        int hi[3] = {2,0,1};
        message_received = send_data(leftStepper.currentPosition(), rightStepper.currentPosition(), left, right, front, hi , 0, 0);
        leftStepper.setCurrentPosition(0);
        rightStepper.setCurrentPosition(0);
        last_sent_message = millis();
      }
      
      if (wall_in_front) {
        leftStepper.stop();
        rightStepper.stop();

        reverse();
        Serial.print("TurnRight90");
        turn_right();
      } 

      if (leftSensorReading < 4) {
        leftStepper.stop();
        rightStepper.stop();
        turn_right_20();
        adjust();
      }
      
      set_speed(leftSensorReading, 10, 150); // following left all the time

      
      
      // set_speed(10 , rightSensorReading, 150); // following right all the time
      
    }

    
  }

    leftStepper.runSpeed();
    rightStepper.runSpeed();
    Serial.print("left speed = " + String(leftStepper.speed()) + "   ");
    Serial.println("right speed = " + String(rightStepper.speed()) + "   ");
}
