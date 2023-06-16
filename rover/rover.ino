//include header files
#include "drive_functions.h"
#include "websocket_client.h"
#include "balancing.h"
#include "uart.h"


//define globals here

//websocket and WiFi stuff
const char* ssid     = "iPhone";
const char* password = "12345678";
char path[] = "/";
char host[] = "172.20.10.4:5000";
WebSocketClient webSocketClient;
WiFiClient client;


//wall information for message to be sent to EC2
bool start = true;
String message_to_send = "";
String message_received = "";
int x = 0; //x coordinate
int y = 0; //y coordinate
int l = 0; //left wall
int f = 0; //front wall
int r = 0; //right wall


//motor pin interface
#define LEFT_STEP_PIN 32 //A4
#define LEFT_DIR_PIN 15 //D12

#define RIGHT_STEP_PIN 33 //A3
#define RIGHT_DIR_PIN 4 //D11

//sensor pin interface
const int leftSensorPin = 35; //a5
const int frontSensorPin = 34;
const int rightSensorPin = 39; //vn

//rover specs
#define STEPS_PER_REVOLUTION 200.0
#define WHEEL_RADIUS 0.0325
const float wheel_diameter = 2.0 * WHEEL_RADIUS;
const float wheel_base = 0.14;
const float wheel_circumference = wheel_diameter * PI;

//motors
AccelStepper leftStepper(AccelStepper::DRIVER, LEFT_STEP_PIN, LEFT_DIR_PIN);
AccelStepper rightStepper(AccelStepper::DRIVER, RIGHT_STEP_PIN, RIGHT_DIR_PIN);


//movement functions
void turn_right_90(){
  leftStepper.setCurrentPosition(0);
  rightStepper.setCurrentPosition(0);
  leftStepper.move(108);
  rightStepper.move(108);
  while(rightStepper.distanceToGo() != 0 && leftStepper.distanceToGo() != 0) {
    rightStepper.run();
    leftStepper.run();
  }
}

void turn_left_90(){
  leftStepper.setCurrentPosition(0);
  rightStepper.setCurrentPosition(0);
  leftStepper.move(-108);
  rightStepper.move(-108);
  while(rightStepper.distanceToGo() != 0 && leftStepper.distanceToGo() != 0) {
    rightStepper.run();
    leftStepper.run();
  }
}

void turn360(){
  leftStepper.setCurrentPosition(0);
  rightStepper.setCurrentPosition(0);
  rightStepper.move(432);
  leftStepper.move(432);
  while(rightStepper.distanceToGo() != 0 && leftStepper.distanceToGo() != 0) {
    rightStepper.run();
    leftStepper.run();
  }
}


float angleTurned(int initial_left_wheel_pos, int initial_right_wheel_pos,int final_left_wheel_pos,int final_right_wheel_pos,){
  //all inputs should be in steps!!
  float dist_travelled_r = (final_right_wheel_pos - initial_right_wheel_pos) * wheel_circumference /STEPS_PER_REVOLUTION;
  float dist_travelled_l = (final_left_wheel_pos - initial_left_wheel_pos) * wheel_circumference /STEPS_PER_REVOLUTION;
  
}



void setup() {
  Serial.begin(115200);

  //EC2 Connection
  initWiFi(ssid, password);
  delay(50);
  initWebSocket("172.20.10.4", 5000, client);
  handshake(path,host, webSocketClient,client);

  //motor preamble
  leftStepper.setMaxSpeed(200);
  leftStepper.setAcceleration(10);
  rightStepper.setMaxSpeed(200);
  rightStepper.setAcceleration(10);

  //Movement

}

void loop() {
  //get directions from server
  message_received = receive_data(ssid,password,path,host, webSocketClient, client, message_to_send, start);
  start = false;
  Serial.print("Received data: ");
  Serial.println(message_received);
  

  

  



  //send node information to server
  if(client.connected()) {
  message_to_send = "{\"x\":\"" + String(x) + "\",\"y\":\"" + String(y) + "\",\"f\":\""+ String(f) + "\",\"l\":\""+ String(l)+ "\",\"r\":\""+ String(r) + "\"}";
  webSocketClient.sendData(message_to_send);
  }
  else {
    Serial.println("Reconnecting...");
    initWebSocket("172.20.10.4", 5000, client);
    handshake(path,host, webSocketClient,client);
    delay(100);
    webSocketClient.sendData(message_to_send);
    }
    delay(500);
}
