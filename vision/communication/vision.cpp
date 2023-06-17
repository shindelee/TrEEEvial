// #include "vision.h"

// float pixel_to_cm = 0.1390625;

// x, y - coordinates
struct position {
  float x;
  float y;
};

// Hexadecimal (char) -> Binary (string)
const char* hex_to_bin (char c){

    switch(toupper(c))
    {
        case '0': return "000";
        case '1': return "001";
        case '2': return "010";
        case '3': return "011";
        case '4': return "100";
        case '5': return "101";
        case '6': return "110";
        case '7': return "111";
        default: return "000";
    }
}

// Binary (string) -> Colour (string)
const String bin_to_colour (String b){
   
    String colour;

    if (b == "010" || b == "011"){
        colour = "red";
    }
    else if (b == "100" || b == "101"){
        colour = "blue";
    }
    else if (b == "110" || b == "111"){
        colour = "yellow";
    }
    else {
        colour = "NULL";
    }
    
    return colour;
}

// Hexadecimal (string) -> Binary (string)
String hex_str_to_bin_str (const String& hex){
   
    String bin;

    for(unsigned i = 0; i != hex.length(); ++i){
       bin += hex_to_bin(hex[i]);
    }

    return bin;
}

// Binary (string) -> Decimal (number)
int bin_str_to_dec_num (const String& bin){

    int value = 0; 
	int count = 0; 
	for(int i = bin.length() - 1; i >= 0; i--) { 
 
      if(bin [i] == '1') { 
        value += pow(2, count); 
    	} 

        count++; 
	} 
	return value;
}

// Euclidean Distance (between 2 known coordinates)
float calculateDistance(position c1, position c2) {
  float distance = sqrt(pow((c1.x - c2.x), 2) + pow((c1.y - c2.y), 2));
  return distance;
}

// Need to re-read this section
// Size to Scale factor (beacon as standard)
float size_to_sf (float left_x, float right_x, float actual_size){
    float sf = 0;
    float size = right_x - left_x;

    // actual size = 4 cm 
    sf = actual_size / size;

    return sf;
}

// Centre of the box (Coordinates)
position centre_coord(float min_x, float max_x, float min_y, float max_y){
    
    // Create an instance of position
    position centrePos;

    centrePos.x = (min_x + max_x) / 2;
    centrePos.y = (min_y + max_y) / 2;

    return centrePos;
}

// Size of the boundary box
float size_bb(float min_x, float max_x, float min_y, float max_y){
    float size = (max_x - min_x) * (max_y - min_y);
    return size;
}

// Check if a specific beacon has reached the middle section of the frame
// Need to double-check the boundaries of the middle section!
bool middle(position centrePos){
    int left_boundary = 240;
    int right_boundary = 400;

    if(centrePos.x > left_boundary && centrePos.x < right_boundary){
        return 1;
    }
    return 0;
}

// Check if there is any beacon in the middle section of the frame
bool detect(position centre_red, position centre_yellow, position centre_blue){

    if(middle(centre_red) || middle(centre_yellow) || middle(centre_blue)){
        return 1;
    }
    else{
        return 0;
    }
}

position top_left(float min_x, float max_y){
    position coord;
    coord.x = min_x;
    coord.y = max_y;

    return coord;
}

position bottom_right(float max_x, float min_y){
    position coord;
    coord.x = max_x;
    coord.y = min_y;

    return coord;
}

/*
// differentiate between the 3 beacons - should be in the main loop

// size_yellow = size_bb(yellow_x_min, yellow_x_max, yellow_y_min, yellow_y_max);
// size_red = size_bb(red_x_min, red_x_max, red_y_min, red_y_max);

if (middle(red) || middle(yellow)){
    if (size_yellow > 0.7 * (size_red)){
        return "yellow";
    }
    else{
        return "red";
    }
}

if(middle(blue)){
    return "blue";
}
*/
