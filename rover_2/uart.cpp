#include "uart.h"
#include "shared_variables.h"

uint32_t numbers[14];

struct WheelTurns {
  float l;
  float r;

  public:
  WheelTurns(int x_, int y_) {
    l = x_;
    r = y_;
  }
};

WheelTurns red(0,0);
WheelTurns blue(0,0);
WheelTurns yellow(0,0);

bool is_in_frame(int x_min, int x_max, int y_min, int y_max) {
  return x_min > 240 && x_max <400 && y_min >180 && y_max < 300;
}

float size_bb(float min_x, float max_x, float min_y, float max_y){
    float size = (max_x - min_x) * (max_y - min_y);
    return size;
}

void beacon_detection(){
    if (Serial1.available() >= 4)
    {
        byte m1 = Serial1.read();    // read the bytes into byte variables 'b1' to 'b4'
        byte m2 = Serial1.read();  
        byte m3 = Serial1.read();
        byte m4 = Serial1.read();

        uint32_t message;
        message = m1 | (m2 << 8) | (m3 << 16) | (m4 << 24); 

        if(message == 5390914)
        {
            if (Serial1.available() >= 24)
            {
                uint32_t temp;
                uint32_t hexadeci[7];
                char terminal[7][9];
                byte b1, b2, b3, b4;

                for (int i = 0; i < 6; i++)
                {
                    b1 = Serial1.read();  // read the bytes into byte variables 'b1' to 'b4'
                    b2 = Serial1.read();
                    b3 = Serial1.read();
                    b4 = Serial1.read();

                    hexadeci[i] = b1 | (b2 << 8) | (b3 << 16) | (b4 << 24);  // Combine the bytes into a single 32-bit integer
                    
                    // char buffer[9];  // Create a character buffer to hold the hexadecimal representation
                    sprintf(terminal[i], "%08X", hexadeci[i]); // Convert the long value to hexadecimal

                    // To extract bits 10 through 0, we use a bitwise AND with a mask where these bits are 1 and the others are 0.
                    numbers[2 * i] = hexadeci[i] & 0x7FF;

                    // To extract bits 27 through 16, we again use a bitwise AND, then shift the result right 16 places.
                    numbers[2 * i + 1] = (hexadeci[i] & 0x0FFF0000) >> 16;
                }

                bool red_detect = is_in_frame(numbers[0], numbers[2], numbers[1], numbers[3]);
                bool blue_detect = is_in_frame(numbers[4], numbers[6], numbers[5], numbers[7]);
                bool yellow_detect = is_in_frame(numbers[8], numbers[10], numbers[9], numbers[11]);

                int left_wheel_revs = leftStepper.currentPosition();
                int right_wheel_revs = rightStepper.currentPosition();

                if (red_detect || yellow_detect) 
                {
                    float red_size = size_bb(numbers[0], numbers[2], numbers[1], numbers[3]);
                    float yellow_size = size_bb(numbers[8], numbers[10], numbers[9], numbers[11]);
                  
                    if (yellow_size > 0.5 * red_size) 
                    {
                        yellow.l = left_wheel_revs;
                        yellow.r = right_wheel_revs;
                        Serial.println("yellow detected!!");
                        Serial.println("left wheel revs = " + String(yellow.l));
                        Serial.println("right wheel revs = " + String(yellow.r));
                    }
                    else 
                    {
                        red.l = left_wheel_revs;
                        red.r = right_wheel_revs;
                        Serial.println("red detected!!");
                        Serial.println("left wheel revs = " + String(red.l));
                        Serial.println("right wheel revs = " + String(red.r));
                    }
                  
                }

                else if (blue_detect) 
                {
                    blue.l = left_wheel_revs;
                    blue.r = right_wheel_revs;
                    Serial.println("yellow detected!!");
                    Serial.println("left wheel revs = " + String(blue.l));
                    Serial.println("right wheel revs = " + String(blue.r));
                }
                
            }
        }

        Serial.println("done turning one turn!");
   }
}
