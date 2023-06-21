#include <AccelStepper.h>

#define RX_PIN 16
#define TX_PIN 17

#define LEFT_STEP_PIN 32 // A4, does not work
#define LEFT_DIR_PIN 15  // D12

#define RIGHT_STEP_PIN 33 // A3
#define RIGHT_DIR_PIN 4   // D11

#define STEPS_PER_REVOLUTION 200.0
#define WHEEL_RADIUS 0.0325
const float wheel_diameter = 2.0 * WHEEL_RADIUS;
const float wheelBase = 0.14;
const float wheelCircumference = wheel_diameter * PI;
uint32_t message;

struct WheelTurns
{
  float l;
  float r;

public:
  WheelTurns(int x_, int y_)
  {
    l = x_;
    r = y_;
  }
};

AccelStepper leftStepper(AccelStepper::DRIVER, LEFT_STEP_PIN, LEFT_DIR_PIN);
AccelStepper rightStepper(AccelStepper::DRIVER, RIGHT_STEP_PIN, RIGHT_DIR_PIN);

WheelTurns red(0, 0);
WheelTurns blue(0, 0);
WheelTurns yellow(0, 0);

float alpha = 0.0;
float theta = 0.0;

// functions

float get_angle_turnedL(long currWheelStepsL, long prevWheelStepsL){
    long difference = currWheelStepsL - prevWheelStepsL; 
    float leftWheelRevs = difference / STEPS_PER_REVOLUTION; 
    float distanceTravelledL = leftWheelRevs * wheelCircumference;
    float angleTurnedRadiansL = distanceTravelledL / wheelBase;
    float angleTurnedDegreesL = angleTurnedRadiansL * (180.0 / PI);
    return angleTurnedDegreesL;
} 

float get_angle_turnedR(long currWheelStepsR, long prevWheelStepsR){
    long difference = currWheelStepsR - prevWheelStepsR; 
    float rightWheelRevs = difference / STEPS_PER_REVOLUTION; 
    float distanceTravelledR = rightWheelRevs * wheelCircumference;
    float angleTurnedRadiansR = distanceTravelledR / wheelBase;
    float angleTurnedDegreesR = angleTurnedRadiansR * (180.0 / PI);
    return angleTurnedDegreesR;
}

bool is_in_frame(int x_min, int y_min, int x_max, int y_max)
{
  return (x_min > 220 && x_min < 300) || (x_max > 240 && x_max < 400) || (y_min > 240 && y_min < 300) || (y_max > 280 && y_max < 380);
}

float size_bb(float min_x, float max_x, float min_y, float max_y)
{
  float size = (max_x - min_x) * (max_y - min_y);
  return size;
}

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);

  leftStepper.setAcceleration(20);
  rightStepper.setAcceleration(20);
  leftStepper.setMaxSpeed(10);
  rightStepper.setMaxSpeed(10);
}

void loop()
{
  leftStepper.setCurrentPosition(0);
  rightStepper.setCurrentPosition(0);
  uint32_t numbers[14];
  Serial.println("initialising");

  // full 360 deg turn after the for loop
  for (int i = 0; i < 46; i++)
  {
    leftStepper.setSpeed(10);
    rightStepper.setSpeed(10);
    leftStepper.move(10);
    rightStepper.move(10);

    
    if (Serial1.available() >= 4)
    {
      byte m1 = Serial1.read(); // read the bytes into byte variables 'b1' to 'b4'
      byte m2 = Serial1.read();
      byte m3 = Serial1.read();
      byte m4 = Serial1.read();

      uint32_t message;
      message = m1 | (m2 << 8) | (m3 << 16) | (m4 << 24);

      if (message == 5390914)
      {
        if (Serial1.available() >= 24)
        {
          uint32_t temp;
          uint32_t hexadeci[7];
          char terminal[7][9];
          byte b1, b2, b3, b4;

          for (int i = 0; i < 6; i++)
          {
            b1 = Serial1.read(); // read the bytes into byte variables 'b1' to 'b4'
            b2 = Serial1.read();
            b3 = Serial1.read();
            b4 = Serial1.read();

            hexadeci[i] = b1 | (b2 << 8) | (b3 << 16) | (b4 << 24); // Combine the bytes into a single 32-bit integer

            // char buffer[9];  // Create a character buffer to hold the hexadecimal representation
            sprintf(terminal[i], "%08X", hexadeci[i]); // Convert the long value to hexadecimal

            // To extract bits 10 through 0, we use a bitwise AND with a mask where these bits are 1 and the others are 0.
            numbers[2 * i] = hexadeci[i] & 0x7FF;

            // To extract bits 27 through 16, we again use a bitwise AND, then shift the result right 16 places.
            numbers[2 * i + 1] = (hexadeci[i] & 0x0FFF0000) >> 16;
          }  
        }
      }
    
      while (rightStepper.distanceToGo() != 0 && leftStepper.distanceToGo() != 0)
      {
        rightStepper.runSpeed();
        leftStepper.runSpeed();
      }
    
      for (int j = 0; j < 12; j++)
    {
       Serial.print("numbers: " + String(numbers[j]));
       Serial.print(" ");
    }

    bool red_detect = is_in_frame(numbers[0], numbers[1], numbers[2], numbers[3]);
    bool blue_detect = is_in_frame(numbers[4], numbers[5], numbers[6], numbers[7]);
    bool yellow_detect = is_in_frame(numbers[8], numbers[9], numbers[10], numbers[11]);

    int left_wheel_revs = leftStepper.currentPosition();
    int right_wheel_revs = rightStepper.currentPosition();

    if(yellow_detect && red_detect)
    {
      yellow.l = left_wheel_revs;
      yellow.r = right_wheel_revs;
      Serial.println("yellow detected!!");
      Serial.println("left wheel revs = " + String(yellow.l));
      Serial.println("right wheel revs = " + String(yellow.r));
    }
    else if(red_detect)
    {
      red.l = left_wheel_revs;
      red.r = right_wheel_revs;
      Serial.println("red detected!!");
      Serial.println("left wheel revs = " + String(red.l));
      Serial.println("right wheel revs = " + String(red.r));
    }
    else if(blue_detect)
    {
      blue.l = left_wheel_revs;
      blue.r = right_wheel_revs;
      Serial.println("blue detected!!");
      Serial.println("left wheel revs = " + String(blue.l));
      Serial.println("right wheel revs = " + String(blue.r));
    }
    
    }

    Serial.println("done turning one turn!");
    
  }
 
  Serial.println("done 360 turning!");

  /*
  alpha = 0.5 * (get_angle_turnedL(red.l, blue.l) + get_angle_turnedR(red.r, blue.r));
  theta =  0.5 * (get_angle_turnedL(yellow.l, red.l) + get_angle_turnedR(yellow.r, red.r));

  Serial.println("alpha: " + String(alpha));
  Serial.println("theta: " + String(theta));
  */
}