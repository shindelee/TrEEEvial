// include header files

#include "drive_functions.h"
#include "websocket_client.h"
#include "balancing.h"
#include "uart.h"
#include "shared_variables.h"
#include "movement_state_machine.h"

// define globals here
// websocket and WiFi stuff

const char* ssid     = "iPhone";
const char* password = "12345678";
char path[] = "/";
char host[] = "13.51.172.210:5000";

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

//motor pin interface
const int LEFT_STEP_PIN = 33;    // A3
const int LEFT_DIR_PIN = 15;     // D12

const int RIGHT_STEP_PIN = 32;   // A4
const int RIGHT_DIR_PIN = 4;     // D11

const int RX_PIN = 16;          // D9 
const int TX_PIN = 17;           // D8

// sensor pin interface
const int leftSensorPin = 34;   // A5 for now
const int frontSensorPin = 39;  // A1 for now 
const int rightSensorPin = 35;  // A5

// rover specs
const float STEPS_PER_REVOLUTION = 200.0;
const float WHEEL_RADIUS = 0.0325;
const float wheelDiameter = 2.0 * WHEEL_RADIUS;
const float wheelBase = 0.14;
const float wheelCircumference = wheelDiameter * PI;

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

//ultrasound pins
const int trigPinLeft = 13; //sda
const int echoPinLeft = 12; //scl

const int trigPinRight = 27; 
const int echoPinRight = 26;

const int trigPinFront = 2;
const int echoPinFront = 14;


//state variables
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
//int left_sensor_history[5];
//int right_sensor_history[5];
int sensor_setpoint = 8;
int sensor_upper_bound = 12;


//test stuff
//int test[10] = {204,204,93,93,12,-12,460,-460,274,626};
//int test2[10] = {1,0,1,1,1,0,1,1,0,0};

void setup() {
  Serial.begin(115200);

  // EC2 Connection
//  initWiFi(ssid, password);
//  delay(50);
//  initWebSocket("13.51.172.210", 5000, client);
//  handshake(path,host, webSocketClient, client);

  
  // motor preamble
  leftStepper.setMaxSpeed(400);
  leftStepper.setAcceleration(10);
  rightStepper.setMaxSpeed(400);
  rightStepper.setAcceleration(10);

  // get initial sensor reading
  read_sensors_and_set_speed();
  previous_state = cur_state;

  //ultrasound sensor setup
  pinMode(trigPinLeft, OUTPUT); 
  pinMode(echoPinLeft, INPUT); 

  pinMode(trigPinRight, OUTPUT); 
  pinMode(echoPinRight, INPUT); 

  pinMode(trigPinFront, OUTPUT); 
  pinMode(echoPinFront, INPUT); 

//  initialise_left_sensor_history();
//  initialise_right_sensor_history();
  
}


void loop() {

  int time_since_reading_left = abs(leftStepper.currentPosition() - last_sensor_reading_left);
  int time_since_reading_right = abs(rightStepper.currentPosition() - last_sensor_reading_right);
  if ((time_since_reading_left >=50 || time_since_reading_right >= 50))
  { 
    bool change_state = (state_history[0] == state_history[1]);
    change_state = change_state && (state_history[1]== state_history[2]);
    change_state = change_state && (state_history[2]== state_history[3]);
    change_state = change_state && (state_history[3]!= state_history[4]);

    if (state == FRONT_WALL) {
      leftStepper.stop();
      rightStepper.stop();
      leftStepper.setSpeed(0);
      rightStepper.setSpeed(0);
      if (wall_on_left && wall_on_right) {
        turn_180();
        edge_out();
      } 
      else if (!wall_on_left && wall_on_right) {
        turn_left_90();
        edge_out();
      }

      else if (wall_on_left && !wall_on_right) {
        turn_right_90();
        edge_out();
      }
      else {
        turn_left_90();
        edge_out();
      }
    }

//    if ((change_state)) { //entering a node
//      // Take readings from the sensors and set states
//      Serial.println("in a node!");
//      leftStepper.stop();
//      rightStepper.stop();
//      leftStepper.setSpeed(0);
//      rightStepper.setSpeed(0);
//
////      if (state != FRONT_WALL) {
////        leftStepper.move(-30);
////        rightStepper.move(30);
////        leftStepper.setSpeed(-20);
////        rightStepper.setSpeed(20);
////        while (abs(leftStepper.distanceToGo()) >0) {
////          Serial.println(leftStepper.distanceToGo());
////          leftStepper.runSpeed();
////          rightStepper.runSpeed();
////        }
////      }    
//      
//      read_sensors_and_set_speed();
//      //for triangulation sequence array, 0- blue, 1-red, 2-yellow
//      int left_wall = (int)wall_on_left;
//      int right_wall =  (int) wall_on_right;
//      int front_wall = (int) wall_in_front;
//      int left_pos = (int)leftStepper.currentPosition();
//      int right_pos = (int)rightStepper.currentPosition();
//      int tri_sequence[3] = {1,0,2};
////      message_received = send_data(left_pos, right_pos, left_wall, right_wall, front_wall, tri_sequence, 0, 0);
//      leftStepper.setCurrentPosition(0);
//      rightStepper.setCurrentPosition(0);
//      // message_received = send_data(left_pos, right_pos, 1, 1, 0, tri_sequence, 0, 0);
//      initialise_state_history();
//
//      int combined = (int(wall_on_left) << 2) | (int(wall_in_front) << 1) | int(wall_on_right);
//     Serial.print("combined = " + String(combined) + "   ");
//
//      switch (combined) {
//        case 0: 
//          Serial.print("case 0   ");
//          turn_left_90();
//          break;
//        case 1:
//          Serial.print("case 1   ");
//          turn_left_90();
//          break;
//          
//        case 2:
//          Serial.print("case 2   ");
//          turn_left_90();
//          break;
//        case 3:
//          Serial.print("case 3   ");
//          turn_left_90();
//          break;
//        case 4: 
//          Serial.print("case 4   ");
//          straight();
//          break;
//        case 5:
//          Serial.print("case 5   ");
//          straight();
//          break;
//        case 6:
//        Serial.print("case 6   ");
//          turn_right_90();
//          break;
//        case 7: 
//          turn_180();
//          break;
//        default:
//          straight();
//          break;
//      }
  

    }
    read_sensors_and_set_speed();
//}

  Serial.print("left speed = " + String(leftStepper.speed()) + "   ");
  Serial.println("right speed = " + String(rightStepper.speed()));

  //run the stepper
  leftStepper.runSpeed();
  rightStepper.runSpeed();

  
    
}
