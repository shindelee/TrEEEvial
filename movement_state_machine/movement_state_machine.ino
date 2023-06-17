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

#define LEFT_STEP_PIN 33 //A3
#define LEFT_DIR_PIN 15  //D12

// Define the motor interface pins for the right wheel
#define RIGHT_STEP_PIN 32 //A4
#define RIGHT_DIR_PIN 4   //D11

// Define the steps per revolution for your stepper motors
#define STEPS_PER_REVOLUTION 200.0
#define WHEEL_RADIUS 0.0325

//light sensor pins
const int leftSensorPin = 27; //A0 for now 
const int frontSensorPin = 26; //A1 for now 
const int rightSensorPin = 35; //A5

float wheelDiameter = 2 * WHEEL_RADIUS; 
float wheelBase = 0.14;  
float wheelCircumference = wheelDiameter * PI;

//keep track of current wall


AccelStepper leftStepper(AccelStepper::DRIVER, LEFT_STEP_PIN, LEFT_DIR_PIN);
AccelStepper rightStepper(AccelStepper::DRIVER, RIGHT_STEP_PIN, RIGHT_DIR_PIN);

// State definitions
#define RSPD 01
#define RJSTR 02
#define 

// State variable
int state;
bool in_node = true; //boolean to tell whether we are in a node 
bool left_wall_info_history[];
bool right_wall_info_history[];

elapsedMillis printTime;

void setup() {
  Serial.begin(115200);
  leftStepper.setMaxSpeed(300.0);    // must be equal to or greater than desired speed.
  rightStepper.setMaxSpeed(300.0);    // must be equal to or greater than desired speed.

}

void loop() {
  if (printTime >= 1000) {
    printTime = 0;

    // Take readings from the left and right sensors:
    int leftSensorReading = 0;
    leftSensorReading = analogRead(leftSensorPin);
    int rightSensorReading = 0;
    rightSensorReading = analogRead(rightSensorPin);

    bool wall_on_left = leftSensorReading >15;
    bool wall_on_right = rightSensorReading > 15;
  

    //remember wall readings
    if(sizeof(left_wall_info_history) < 4 ) { //remember the last 4 consecutive walls 
      left_wall_info_history[sizeof(left_wall_info_history)] = wall_on_left;
    }
    else {
      left_wall_info[3] = left_wall_info[2];
      left_wall_info[2] = left_wall_info[1];
      left_wall_info[1] = left_wall_info[0];
      left_wall_info[0] = wall_on_left;
    }
  
    if(sizeof(right_wall_info_history) < 4 ) { //remember the last 4 consecutive walls 
      right_wall_info_history[sizeof(right_wall_info_history)] = wall_on_right;
    }
    else {
      right_wall_info[3] = right_wall_info[2];
      right_wall_info[2] = right_wall_info[1];
      right_wall_info[1] = right_wall_info[0];
      right_wall_info[0] = wall_on_right;
    }
  
    if (state ==RSPD) {
    float difference = leftSensorReading - rightSensorReading;
    float p = 2;
    float weighted_difference = difference * p;
    
    leftStepper.setSpeed(50.0 + weighted_difference);
    rightStepper.setSpeed(50.0 - weighted_difference);
    }
  }

  int frontSensorReading = 0;
  frontSensorReading = analogRead(frontSensorPin);

  bool wall_in_front = frontSensorReading > 50;
  
  
  if(wall_in_front){
      state = RJSTR;
    }
  else if(wall_on_left && wall_on_right){
      state = RSPD;
    }

  else if (wall_on_left && !wall_on_right) {
      
  }
  else if (!wall_on_left && wall_on_right) {
    
  }
  
  switch (state) {
    case RSPD:
      myStepperR.runSpeed();
      myStepperL.runSpeed();
      break;
    case RJSTR:
      myStepperR.stop();
      myStepperL.stop();
      break;
  }
}
