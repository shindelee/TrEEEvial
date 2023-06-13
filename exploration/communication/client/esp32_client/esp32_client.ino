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
int seq_no;

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
      seq_no = 0;
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
  Serial.println("input x :");
  int x = parseInt();
  Serial.println("input y:");
  int y = parseInt();

String message1 = "{\"x\":\"" + String(x) + "\",\"y\":\"" + String(y) + "\",\"f\":\""+ String(0) + "\",\"l\":\""+ String(1)+ "\",\"r\":\""+ String(1) + "\",\"seq_no\":\""+ String(seq_no)+ "\"}";
//String message2 = "{\"x\":\"" + String(3) + "\",\"y\":\"" + String(7) + "\",\"f\":\""+ String(0) + "\",\"l\":\""+ String(1)+ "\",\"r\":\""+ String(1) + "\",\"seq_no\":\""+ String(seq_no)+ "\"}";
String received_data;
  if (client.connected()) {
    delay(1000);
    webSocketClient.sendData(message1); //todo: see if can send bytes
    Serial.println("sent message 1");
    webSocketClient.getData(received_data);
    while (received_data.length() <= 0) {
      delay(5000);
      Serial.println("sent a duplicate message 1");
      webSocketClient.sendData(message1);
      webSocketClient.getData(received_data);
    }
    Serial.print("Received data: ");
    Serial.println(received_data);

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
  delay(5000);
  
}
