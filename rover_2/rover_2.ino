// include header files

#include "drive_functions.h"
#include "websocket_client.h"
#include "balancing.h"
#include "uart.h"
#include "shared_variables.h"
#include "movement_state_machine.h"

// define globals here
// websocket and WiFi stuff

const char* ssid     = "Shinde";
const char* password = "12345678";
char path[] = "/";
char host[] = "52.91.70.167:5000";

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

//state variables
int state;
String previous_state;
String cur_state;
bool in_node = true; // boolean to tell whether we are in a node
int frontSensorReading = 0;
int leftSensorReading = 0;
int rightSensorReading = 0;
int last_sensor_reading_left = 0;
int last_sensor_reading_right = 0;
bool wall_on_left;
bool wall_on_right;
bool wall_in_front;
int state_history[5];
int left_sensor_history[5];
int right_sensor_history[5];
int sensor_threshold = 50;

void setup() {
  Serial.begin(115200);

  // EC2 Connection
  initWiFi(ssid, password);
  delay(50);
  initWebSocket("52.91.70.167", 5000, client);
  handshake(path,host, webSocketClient, client);

  
  // motor preamble
  leftStepper.setMaxSpeed(400);
  leftStepper.setAcceleration(10);
  rightStepper.setMaxSpeed(400);
  rightStepper.setAcceleration(10);

  // get initial sensor reading
  read_sensors_and_set_speed();
  previous_state = cur_state;

  initialise_left_sensor_history();
  initialise_right_sensor_history();
  
}


void loop() {

  int time_since_reading_left = abs(leftStepper.currentPosition() - last_sensor_reading_left);
  int time_since_reading_right = abs(rightStepper.currentPosition() - last_sensor_reading_right);
  if (state == FRONT_WALL || (time_since_reading_left >=20 || time_since_reading_right >=20))
  {
    // Take readings from the sensors and set states
    read_sensors_and_set_speed();
    
    
    bool change_state = (state_history[0] == state_history[1]);
    change_state = change_state && (state_history[1]== state_history[2]);
    change_state = change_state && (state_history[2]== state_history[3]);
    change_state = change_state && (state_history[3]!= state_history[4]);

    bool lost_wall = (abs(left_sensor_history[4] - leftSensorReading) > 40) || (abs(right_sensor_history[4] - rightSensorReading) > 40);
    if ((change_state) || state == FRONT_WALL) { //entering a node
      Serial.println("in a node!");
      leftStepper.stop();
      rightStepper.stop();

      if (state != FRONT_WALL) {
        leftStepper.move(-30);
        rightStepper.move(30);
        leftStepper.setSpeed(-20);
        rightStepper.setSpeed(20);
        while (abs(leftStepper.distanceToGo()) >0) {
          Serial.println(leftStepper.distanceToGo());
          leftStepper.runSpeed();
          rightStepper.runSpeed();
        }
      }    
      read_sensors();
      //for triangulation sequence array, 0- blue, 1-red, 2-yellow
      message_received = send_data(leftStepper.position(), rightStepper.position(), int(wall_on_left), int(wall_on_right), int(wall_in_front), [1,0,2], 0, 0)
      initialise_state_history();
      initialise_left_sensor_history();
      initialise_right_sensor_history();

      if (message_received == "left") {
        turn_left_90(); 
      }
      else if (message_received == "right") {
        turn_right_90();
      }

      else if (message_received == "u-turn") {
        turn_180();
      }
    }
  }

  Serial.print("left speed = " + String(leftStepper.speed()) + "   ");
  Serial.println("right speed = " + String(rightStepper.speed()));

  //run the stepper
  leftStepper.runSpeed();
  rightStepper.runSpeed();

  
    
}
