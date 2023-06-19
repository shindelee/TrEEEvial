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
    while (start==false && message_received.length() <=0 && message_to_send != "") { //handle disconnection
      initWebSocket("52.91.70.167", 5000, client);
      handshake(path,host, webSocketClient,client);
      delay(500);
      webSocketClient->sendData(message_to_send);
      Serial.println("Resending message");
      delay(1000);
      webSocketClient->getData(message_received);
    }
  }
  else{
    Serial.println("Reconnecting...");
    initWiFi(ssid, password);
    delay(50);
    initWebSocket("52.91.70.167", 5000, client);
    handshake(path,host, webSocketClient, client); 
    delay(100);
    webSocketClient->getData(message_received);
    }
    return message_received;
}





//send data is simple enough, no need a dedicated function


  
