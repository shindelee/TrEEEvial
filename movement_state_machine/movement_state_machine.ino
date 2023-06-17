/*
   Uno sketch to drive a stepper motor using the AccelStepper library.
   Works with a ULN-2003 unipolar stepper driver, or a bipolar, constant voltage motor driver
   such as the L298 or TB6612, or a step/direction constant current driver like the a4988.
   Time to try some experiments!
   10/21/21  --jkl  jlarson@pacifier.com
   2. Try accelerating to a speed, using run, and then switch to runSpeed to keep going.
      Can try positive and negative targets.
*/
// Include the AccelStepper Library
#include <AccelStepper.h>
#include <elapsedMillis.h>

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
#define TWO_WALLS 1
#define FRONT_WALL 2
#define LEFT_WALL 3
#define RIGHT_WALL 4
#define NO_WALL 5
#define STATE_CHANGE 6

// State variable
int state;
int previous_state;
String cur_state;
bool in_node = true; // boolean to tell whether we are in a node
//bool left_wall_info_history[3];
//bool right_wall_info_history[3];
int leftSensorReading = 0;
int rightSensorReading = 0;

elapsedMillis printTime;

void setup()
{
  Serial.begin(115200);
  leftStepper.setMaxSpeed(300.0);  // must be equal to or greater than desired speed.
  rightStepper.setMaxSpeed(300.0); // must be equal to or greater than desired speed.
}

void loop()
{
  bool wall_on_left = true;
  bool wall_on_right = true;
  if (printTime >= 500)
  {
    printTime = 0;

    // Take readings from the left and right sensors:
    leftSensorReading = analogRead(leftSensorPin);
    rightSensorReading = analogRead(rightSensorPin);
    wall_on_left = leftSensorReading > 30;
    wall_on_right = rightSensorReading > 30;

    

    if (state == TWO_WALLS)
    {
      float difference = leftSensorReading - rightSensorReading;
      float p = 2;
      float weighted_difference = difference * p;

      leftStepper.setSpeed(-(50.0 + weighted_difference));
      rightStepper.setSpeed(50.0 - weighted_difference);
    }

    else if (state == LEFT_WALL) {
      float difference = leftSensorReading - 80;
      float p = 2;
      float weighted_difference = difference * p;

      leftStepper.setSpeed(-(50 + weighted_difference));
      rightStepper.setSpeed(50 - weighted_difference);
    }

    else if (state == RIGHT_WALL) {
      float difference = 80 - rightSensorReading;
      float p = 1;
      float weighted_difference = difference * p;

      leftStepper.setSpeed(-(50 + weighted_difference));
      rightStepper.setSpeed(50 - weighted_difference);
    }

    else if (state == NO_WALL) { //proceed with caution!
      leftStepper.setSpeed(-50);
      rightStepper.setSpeed(50);
    }
    
  }

  int frontSensorReading = 0;
  frontSensorReading = analogRead(frontSensorPin);

  bool wall_in_front = frontSensorReading > 50;

  Serial.print("left wall: " + String(leftSensorReading) + "  ");
  Serial.print("right wall: " + String(rightSensorReading) + "  ");
  Serial.print("front wall: " + String(frontSensorReading) + "  ");

  if (wall_in_front)
  {
    state = FRONT_WALL;
    cur_state = "front wall";
  }
  else if (wall_on_left && wall_on_right)
  {
    state = TWO_WALLS;
    cur_state = "two wall";
  }

  else if (wall_on_left && !wall_on_right)
  {
    state = LEFT_WALL;
    cur_state = "left wall";
  }
  else if (!wall_on_left && wall_on_right)
  {
    state = RIGHT_WALL;
    cur_state = "right wall";
  }

  else if (state != previous_state) {
    state = STATE_CHANGE;
  }
  else
  {
    state = NO_WALL;
    cur_state = "no wall";
  }

  previous_state = state;

  Serial.println("state = " + cur_state);

  switch (state)
  {
  case TWO_WALLS:
    leftStepper.runSpeed();
    rightStepper.runSpeed();
    break;
  case FRONT_WALL:
    leftStepper.stop();
    rightStepper.stop();
    break;

  case LEFT_WALL:
    leftStepper.runSpeed();
    rightStepper.runSpeed();
    break;

  case RIGHT_WALL:
    leftStepper.runSpeed();
    rightStepper.runSpeed();
    break;

  case NO_WALL:
    leftStepper.runSpeed();
    rightStepper.runSpeed();
    break;

  case STATE_CHANGE:
    leftStepper.stop();
    rightStepper.stop();
    delay(5000);
    break;
    
  
  }
}
