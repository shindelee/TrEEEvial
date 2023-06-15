#ifndef WEBSOCKET_CLIENT_H
#define WEBSOCKET_CLIENT_H

//include all the libraries needed here!!
#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketClient.h>

void initWiFi(const char* ssid, const char* password);

void initWebSocket(char ip_address[], int port_number, WiFiClient client);

void handshake(char path[], char host[], WebSocketClient webSocketClient, WiFiClient client);

String receive_data(const char* ssid, const char* password, char path[], char host[], WebSocketClient webSocketClient, WiFiClient client, String message_to_send, bool start);



#endif
