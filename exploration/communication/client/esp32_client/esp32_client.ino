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
int counter = 0;
int array_count[7] = {0,1,1,0,1,0,0};
int alive_count = 0;
bool start = true;
int message_counter = 0;

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
  int x = counter;
  int y = counter + 2;
  int l = array_count[counter];
  int r = array_count[counter+1];
  int f = array_count[counter+2];;

String message1 = "{\"x\":\"" + String(x) + "\",\"y\":\"" + String(y) + "\",\"f\":\""+ String(f) + "\",\"l\":\""+ String(l)+ "\",\"r\":\""+ String(r) + "\",\"seq_no\":\""+ String(seq_no)+ "\"}";
//String message2 = "{\"x\":\"" + String(3) + "\",\"y\":\"" + String(7) + "\",\"f\":\""+ String(0) + "\",\"l\":\""+ String(1)+ "\",\"r\":\""+ String(1) + "\",\"seq_no\":\""+ String(seq_no)+ "\"}";
String received_data;
bool connection = client.connected();
  if (connection) {
    webSocketClient.getData(received_data);
    while (start==false && received_data.length() <=0 ) {
      initWebSocket();
      handshake();
      delay(500);
      webSocketClient.sendData(message1);
      Serial.println("Resending message #" + String(message_counter));
      delay(1000);
      webSocketClient.getData(received_data);
    }
    
    Serial.print("Received data: ");
    Serial.println(received_data);
    delay(2000);

    webSocketClient.sendData(message1); //todo: see if can send bytes
    start = false;
    Serial.println("sent message #" + String(message_counter));
    message_counter = message_counter +1;
    delay(500);
    
    counter ++;
    if (counter ==5) {
      counter = 0;
    }

    }
 
  
  else {
    Serial.println("Server disconnected.Trying to reconnect...");
    initWiFi();
    initWebSocket(); // handle disconnections
    handshake();
    delay(1000);
    while (1) {
      // Hang on disconnect.
    }
  }
  // wait to fully let the client disconnect
  
}
