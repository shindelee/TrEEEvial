#ifndef WEBSOCKET_CLIENT_H
#define WEBSOCKET_CLIENT_H

//include all the libraries needed here!!
#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketClient.h>
#include "shared_variables.h"

void initWiFi(const char* ssid, const char* password);
void initWebSocket(char ip_address[], int port_number, WiFiClient *client);

void initWebSocket(char ip_address[], int port_number, WiFiClient* client);

void handshake(char path[], char host[], WebSocketClient *webSocketClient, WiFiClient* client);

String receive_data(const char* ssid, const char* password, char path[], char host[], WebSocketClient* webSocketClient, WiFiClient* client, String message_to_send, bool start);

String send_data(int left_wheel_revs, int right_wheel_revs, int left_wall_presence, int right_wall_presence, int front_wall_presence, int triangulation_sequence[3], int a_alpha, int a_beta);

#endif
