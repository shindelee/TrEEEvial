// Import required libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>

//Network credentials
const char* ssid = "Sebastian's iPhone";
const char* password = "12345678";

//create async web server on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void setup(){}

void loop(){}
