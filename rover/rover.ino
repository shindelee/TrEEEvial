//include header files
#include "drive_functions.h"
#include "websocket_client.h"
#include "balancing.h"


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


//sensor pin interface





void setup() {
  Serial.begin(115200);

  //EC2 Connection
  initWiFi(ssid, password);
  delay(50);
  initWebSocket("172.20.10.4", 5000, client);
  handshake(path,host, webSocketClient,client);

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
