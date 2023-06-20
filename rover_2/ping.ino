#include <WiFi.h>
#include <WebSocketsClient.h>

WebSocketsClient webSocket;
unsigned long pingTime;

const char* ssid     = "Shinde";
const char* password = "12345678";
char host[] = "52.91.70.167:5000";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println(WiFi.localIP());

  webSocket.begin(host, 5000, "/"); // replace with your EC2 public address and port
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
}

void loop() {
  webSocket.loop();
  
  if (millis() - pingTime > 5000) { // Send ping every 5 seconds
    pingTime = millis();
    String pingMsg = String(pingTime);
    webSocket.sendTXT(pingMsg);
  }
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.println("[WSc] Disconnected!");
      break;
    case WStype_CONNECTED:
      Serial.println("[WSc] Connected!");
      break;
    case WStype_TEXT: {
      unsigned long pongTime = millis();
      unsigned long pingTime = atol((const char *)payload);
      unsigned long rtt = pongTime - pingTime;
      Serial.print("Round Trip Time: ");
      Serial.println(rtt);
      break;
    }
  }
}
