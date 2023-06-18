#include <AccelStepper.h>
//#include <elapsedMillis.h>

#define LEFT_STEP_PIN 33 // A3
#define LEFT_DIR_PIN 15  // D12

// Define the motor interface pins for the right wheel
#define RIGHT_STEP_PIN 32 // A4
#define RIGHT_DIR_PIN 4   // D11

// Define the steps per revolution for your stepper motors
#define STEPS_PER_REVOLUTION 200.0
#define WHEEL_RADIUS 0.0325

// light sensor pins
const int leftSensorPin = 27;  // A0 for now
const int frontSensorPin = 26; // A1 for now
const int rightSensorPin = 35; // A5

float wheelDiameter = 2 * WHEEL_RADIUS;
float wheelBase = 0.14;
float wheelCircumference = wheelDiameter * PI;

// keep track of current wall

AccelStepper leftStepper(AccelStepper::DRIVER, LEFT_STEP_PIN, LEFT_DIR_PIN);
AccelStepper rightStepper(AccelStepper::DRIVER, RIGHT_STEP_PIN, RIGHT_DIR_PIN);

// State definitions
enum State
{
    TWO_WALLS,
    FRONT_WALL,
    LEFT_WALL,
    RIGHT_WALL,
    NO_WALL,
    STATE_CHANGE,
};

State state;
State previous_state;

// State variable
String cur_state;
bool in_node = true; // boolean to tell whether we are in a node
int leftSensorReading;
int rightSensorReading;
int frontSensorReading;


void setup()
{
  Serial.begin(115200);
  leftStepper.setMaxSpeed(300.0);  // must be equal to or greater than desired speed.
  rightStepper.setMaxSpeed(300.0); // must be equal to or greater than desired speed.
}

void loop()
{
  leftSensorReading = analogRead(leftSensorPin);
  rightSensorReading = analogRead(rightSensorPin);
  frontSensorReading = analogRead(frontSensorPin);
  bool wall_on_left = leftSensorReading > 50;
  bool wall_on_right = rightSensorReading > 50;
  bool wall_in_front = frontSensorReading > 50;

  if (wall_in_front)
  {
    state = FRONT_WALL;
    cur_state = "front wall";
  }
  else if (wall_on_left && wall_on_right && !wall_in_front)
  {
    state = TWO_WALLS;
    cur_state = "two wall";
    float difference = leftSensorReading - rightSensorReading;
    float p = 2;
    float weighted_difference = difference * p;

    leftStepper.setSpeed(-(50.0 + weighted_difference));
    rightStepper.setSpeed(50.0 - weighted_difference);
}

  else if (wall_on_left && !wall_on_right)
  {
    state = LEFT_WALL;
    cur_state = "left wall";
    float difference = leftSensorReading - 80;
    float p = 2;
    float weighted_difference = difference * p;

    leftStepper.setSpeed(-(50 + weighted_difference));
    rightStepper.setSpeed(50 - weighted_difference);
  }
  else if (!wall_on_left && wall_on_right)
  {
    state = RIGHT_WALL;
    cur_state = "right wall";
    float difference = 80 - rightSensorReading;
      float p = 1;
      float weighted_difference = difference * p;

      leftStepper.setSpeed(-(50 + weighted_difference));
      rightStepper.setSpeed(50 - weighted_difference);
  }

  else
  {
    state = NO_WALL;
    cur_state = "no wall";
  }

//  if (state != previous_state) {
//    state = STATE_CHANGE;
//    cur_state = "state change";
//  }
  Serial.print("left sensor = " + String(leftSensorReading) + "   ");
  Serial.print("right sensor = " + String(leftSensorReading) + "   ");
  Serial.print("front sensor = " + String(frontSensorReading) + "   ");

  Serial.println("current state = " + String(state));
  previous_state = state;

  int left_previous_position = leftStepper.currentPosition();
  int right_previous_position = rightStepper.currentPosition();

  
    
    switch (state) {
      case TWO_WALLS:
        while(abs(leftStepper.currentPosition() - left_previous_position) < 20 && abs(rightStepper.currentPosition() - right_previous_position)<20) {
        leftStepper.runSpeed();
        rightStepper.runSpeed();
        }
        break;
  
      case LEFT_WALL:
        while(abs(leftStepper.currentPosition() - left_previous_position) < 20 && abs(rightStepper.currentPosition() - right_previous_position)<20) {
          Serial.println("I'm at left wall loop!");
        leftStepper.runSpeed();
        rightStepper.runSpeed();
        }
        break;
  
      case RIGHT_WALL:
        while(abs(leftStepper.currentPosition() - left_previous_position) < 20 && abs(rightStepper.currentPosition() - right_previous_position)<20) {
          Serial.println("I'm at right wall loop!");
          leftStepper.runSpeed();
          rightStepper.runSpeed();
        }
        break;
  
      case NO_WALL:
        while(abs(leftStepper.currentPosition() - left_previous_position) < 20 && abs(rightStepper.currentPosition() - right_previous_position)<20) {
          Serial.println("I'm at right wall loop!");
          leftStepper.runSpeed();
          rightStepper.runSpeed();
        }
        break;
  
       case FRONT_WALL:
          Serial.println("I'm at front wall loop!");
          leftStepper.stop();
          rightStepper.stop();
          break;  
  
       case STATE_CHANGE:
        unsigned long cur_time  = millis();
        while(millis() - cur_time < 5000) {
          Serial.println("I'm at state change loop!");
          leftStepper.stop();
          rightStepper.stop();
        }
        break;
        
    }

  }
