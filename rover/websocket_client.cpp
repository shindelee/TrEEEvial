#include "websocket_client.h"
#include "shared_variables.h"

//connect to WiFi
void initWiFi(const char* ssid, const char* password) {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}



// create web socket
void initWebSocket(char ip_address[], int port_number, WiFiClient* client){
    if (client->connect(ip_address, port_number)) {
      Serial.println("Connected");
    } 
    else {
      Serial.println("Connection failed.");
      while(1) {
        // Hang on failure
      }
    }
  
}



void handshake(char path[], char host[], WebSocketClient *webSocketClient, WiFiClient *client) {
    webSocketClient->path = path;
    webSocketClient->host = host;
    if (webSocketClient->handshake(*client)) {
      Serial.println("Handshake successful");
    } else {
      Serial.println("Handshake failed.");
      while(1) {
        // Hang on failure
      }  
    }
  }

String receive_data(const char* ssid, const char* password, char path[], char host[], WebSocketClient *webSocketClient, WiFiClient *client, String message_to_send, bool start) {
  String message_received;
  if (client->connected()) {
    webSocketClient->getData(message_received);
//    while (start==false) { //handle disconnection
//      initWebSocket("54.144.255.182", 5000, client);
//      handshake(path,host, webSocketClient,client);
//      delay(500);
//      webSocketClient->sendData(message_to_send);
//      Serial.println("Resending message");
//      delay(1000);
//      webSocketClient->getData(message_received);
//    }
  }
  else{
    Serial.println("Reconnecting...");
    initWiFi(ssid, password);
    delay(50);
    initWebSocket("54.144.255.182", 5000, client);
    handshake(path,host, webSocketClient, client); 
    delay(100);
    webSocketClient->getData(message_received);
    }
    return message_received;
}


String send_data(int left_wheel_revs, int right_wheel_revs, int left_wall_presence, int right_wall_presence, int front_wall_presence, int triangulation_sequence[3], int a_alpha, int a_beta) {
  String recv;
  if(client->connected()) 
  {
    message_to_send = "{\"x\":\"" + String(left_wheel_revs) + "\",\"y\":\"" + String(right_wheel_revs) + "\",\"f\":\""+ String(front_wall_presence) + "\",\"l\":\""+ String(left_wall_presence)+ "\",\"r\":\""+ String(right_wall_presence) + "\",\"alpha\":\"" + String(a_alpha) + "\",\"beta\":\"" + String(a_beta) + "\",\"seq\":\"[" + String(triangulation_sequence[0]) + "," + String(triangulation_sequence[1]) + "," + String(triangulation_sequence[2]) + "]" + "\"}";
    webSocketClient->sendData(message_to_send);

     delay(500);
  
    //get directions from server
    recv = receive_data(ssid,password,path,host, webSocketClient, client, message_to_send, start);
    start = false;
    Serial.print("Received data: ");
    Serial.println(recv);
  }
  else {
    Serial.println("Reconnecting...");
    initWebSocket("54.144.255.182", 5000, client);
    handshake(path,host, webSocketClient, client);
    delay(100);
    webSocketClient->sendData(message_to_send);
    while(!(client->connected())) {
      delay(7000); //wait for server to restart or WifI come back on
      initWiFi(ssid, password);
      initWebSocket("54.144.255.182", 5000, client);
      handshake(path,host, webSocketClient, client);
    }
 }
 
  return recv;
}




//send data is simple enough, no need a dedicated function


  
