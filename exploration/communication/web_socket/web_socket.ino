#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>

//macros
const int lightSensorPin = 32;

//network credentials
const char* ssid = "iPhone";
const char* password = "12345678";

//Async Web Server object
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

//send info
void notifyClients() {
  ws.textAll("wall ahead!!");
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    Serial.println((char*)data);
//    if (strcmp((char*)data, "turn left!") == 0) {
//      Serial.println((char*)data);
//    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

//String processor(const String& var){
//  return String(var=="STATE" && sensor_1.state ? "path":"wall")
//}


//Board-side 
//struct Phototransistor{
//  uint8_t  pin;
////  bool     lastReading;
////  uint32_t lastDebounceTime;
//  bool state; //true if wall present
//
//  void read() {
//    bool reading = analogRead(pin);
//
//  if (reading > 100) { //wall
//    state = true;
//    }
//
//   else state = false;
//  }
//  };

//global variable
//Phototransistor sensor_1 = {SENSOR1_PIN, false};


void initWifi(){
  //Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  //ESP32's local IP
  Serial.println(WiFi.localIP());
  }


void setup(){
  Serial.begin(115200);
  initWifi();
  initWebSocket();
  pinMode(lightSensorPin, INPUT);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", "initialise!");
  });

  server.begin();
}


void loop() {
  ws.cleanupClients();
//  digitalWrite(ledPin, ledState);

  int lightSensorReading = 0;
  lightSensorReading = analogRead(lightSensorPin);
//  Serial.println(lightSensorPin);
//  Serial.println(lightSensorReading);
//  delay(1000);
  if (lightSensorReading > 100) {
//      Serial.println("sending message to client...");
      notifyClients();
      delay(5000);
    }

//  delay(5000);
//  notifyClients();

  
  
}
