#include <Arduino.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define RX_PIN 16
#define TX_PIN 17

const char* hex_char_to_bin(char c);
const String  bin_to_colour (String c);
String hex_str_to_bin_str(const String& hex);
int bin_str_to_dec_num(const String& binaryString);
float calculateDistance(position c1, position c2);
position centre_coord(float min_x, float max_x, float min_y, float max_y);
float size_bb(float min_x, float max_x, float min_y, float max_y);
bool middle(position centrePos);
bool detect(position centre_red, position centre_yellow, position centre_blue);
position top_left(int min_x, int max_y);
position bottom_right(int max_x, int min_y);
float angle(float a, float b, float c);
position current_pos(position beacon2, position beacon3, float angle1, float angle2);
position current_pos_check(position beacon1, position beacon3, float angle1, float angle2);
