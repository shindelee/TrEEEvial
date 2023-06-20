#include <WiFi.h>
#include <WebSocketsClient.h>

WebSocketsClient webSocket;

const char* ssid = "your_SSID";
const char* password =  "your_PASSWORD";
const char* websocket_server = "server_IP_or_hostname";

unsigned long messageTimestamp;
unsigned long rttSum = 0;
unsigned int messageCount = 0;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  webSocket.begin(websocket_server, 80, "/"); // 80 is the port. Adjust accordingly.
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();

  if(messageCount < 500) {
    messageTimestamp = micros();
    String message = String(messageTimestamp);
    webSocket.sendTXT(message);
    delay(100);
  }
}

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  switch(type) {
    case WStype_TEXT: {
      unsigned long echoTimestamp = strtoul((const char *)payload, NULL, 10);
      unsigned long rtt = micros() - echoTimestamp;

      rttSum += rtt;
      messageCount++;
      float rttAvg = rttSum / (float)messageCount;
      Serial.println("Average RTT: " + String(rttAvg, 2));
    }
    break;
  }
}
