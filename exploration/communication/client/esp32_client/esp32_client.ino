#include <WiFi.h>
#include <WebSocketClient.h>
//#include <ArduinoJson.h>

//#define RX_PIN 16
//#define TX_PIN 17

// Define the baud rate
//#define BAUD_RATE 115200

const char* ssid     = "iPhone";
const char* password = "12345678";
char path[] = "/";
char host[] = "172.20.10.4:5000";

// global variables
WebSocketClient webSocketClient;
WiFiClient client;

// funcitons
void initWiFi() {
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

//create web socket
  void initWebSocket(){
    if (client.connect("172.20.10.4", 5000)) {
      Serial.println("Connected");
    } 
    else {
      Serial.println("Connection failed.");
      while(1) {
        // Hang on failure
      }
    }
  }

// websocket handshake
void handshake() {
    webSocketClient.path = path;
    webSocketClient.host = host;
    if (webSocketClient.handshake(client)) {
      Serial.println("Handshake successful");
    } else {
      Serial.println("Handshake failed.");
      while(1) {
        // Hang on failure
      }  
    }
  }

void setup() {
  Serial.begin(115200);
  // Start the UART communication with the baud rate set on the FPGA. Adjust the baud rate, data format, and RX pin as needed.
//  Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
//  delay(10);
  initWiFi();
  delay(50);
  initWebSocket();
  handshake();

}


void loop() {
//  String received_data;
//  uint32_t data;
//  uint32_t hexadeci[7];
//  uint32_t numbers[14];
//  byte b1, b2, b3, b4;
//
//  
//
////  if (Serial1.available() >= 28) {  
////        for(int i = 0; i < 7; i++){
////            b1 = Serial1.read();    // read the bytes into byte variables 'b1' to 'b4'
////            b2 = Serial1.read();  
////            b3 = Serial1.read();
////            b4 = Serial1.read();
////
////            hexadeci[i] = b1 | (b2 << 8) | (b3 << 16) | (b4 << 24); // Combine the bytes into a single 32-bit integer
//////            Serial.println(hexadeci[i]);
////            // To extract bits 10 through 0, we use a bitwise AND with a mask where these bits are 1 and the others are 0.
//////            numbers[2 * i] = hexadeci[i]  & 0x7FF;
////
////            // To extract bits 27 through 16, we again use a bitwise AND, then shift the result right 16 places.
//////            numbers[2 * i + 1]  = (hexadeci[i] & 0x0FFF0000) >> 16;
////            
////            
////        }
////        data = *hexadeci;
////        Serial.println(data);
////
////  }
////  else {
////    Serial.println("UART disconnected...");
////    data = 1000000;
////    }
//
String message = "{\"lw\":\"" + String(1) + "\",\"rw\":\"" + String(3) + "\",\"fw\":\""+ String(1) + "\"}";

  if (client.connected()) {
    webSocketClient.sendData(message); //todo: see if can send bytes
    Serial.println("sent message");
//    webSocketClient.getData(received_data);
//    if (received_data.length() > 0) {
//      Serial.print("Received data: ");
//      Serial.println(received_data);
//    }
  } 
  
  else {
    Serial.println("Server disconnected.Trying to reconnect...");
    initWiFi();
    initWebSocket(); // handle disconnections
    handshake();
    delay(5000);
    while (1) {
      // Hang on disconnect.
    }
  }
  // wait to fully let the client disconnect
  delay(2000);
  
}
