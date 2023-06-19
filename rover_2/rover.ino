//include header files

#include "drive_functions.h"
#include "websocket_client.h"
#include "balancing.h"
#include "uart.h"
#include "shared_variables.h"

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
int x = 0; //x coordinate
int y = 0; //y coordinate
int l = 0; //left wall
int f = 0; //front wall
int r = 0; //right wall

// Define the baud rate
#define BAUD_RATE 115200

//motor pin interface
#define LEFT_STEP_PIN 32    // A4
#define LEFT_DIR_PIN 15     // D12

#define RIGHT_STEP_PIN 33   // A3
#define RIGHT_DIR_PIN 4     // D11

#define RX_PIN 16           // D9 
#define TX_PIN 17           // D8

// sensor pin interface
const int leftSensorPin = 35;   // A5
const int frontSensorPin = 34;
const int rightSensorPin = 39;  // vn

// rover specs
const float STEPS_PER_REVOLUTION = 200.0;
const float WHEEL_RADIUS = 0.0325;
const float wheel_diameter = 2.0 * WHEEL_RADIUS;
const float wheelBase = 0.14;
const float wheelCircumference = wheel_diameter * PI;

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
  read_sensors_and_set_speed(&leftStepper, &rightStepper);
  previous_state = cur_state;
  
}


void loop() {
  
  //get directions from server
  message_received = receive_data(ssid,password,path,host, webSocketClient, client, message_to_send, start);
  start = false;
  Serial.print("Received data: ");
  Serial.println(message_received);

  //send node information to server
  if(client->connected()) 
  {
    message_to_send = "{\"x\":\"" + String(x) + "\",\"y\":\"" + String(y) + "\",\"f\":\""+ String(f) + "\",\"l\":\""+ String(l)+ "\",\"r\":\""+ String(r) + "\"}";
    webSocketClient->sendData(message_to_send);
  }
  else {
    Serial.println("Reconnecting...");
    initWebSocket("52.91.70.167", 5000, client);
    handshake(path,host, webSocketClient, client);
    delay(100);
    webSocketClient->sendData(message_to_send);
 }
 
 delay(500);
    
}
