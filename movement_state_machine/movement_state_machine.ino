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

// Creates an instance - Pick the version you want to use and un-comment it. That's the only required change.
//AccelStepper myStepper(AccelStepper::FULL4WIRE, AIn1, AIn2, BIn1, BIn2);  // works for TB6612 (Bipolar, constant voltage, H-Bridge motor driver)
//AccelStepper myStepper(AccelStepper::FULL4WIRE, In1, In3, In2, In4);    // works for ULN2003 (Unipolar motor driver)
AccelStepper leftStepper(AccelStepper::DRIVER, LEFT_STEP_PIN, LEFT_DIR_PIN);
AccelStepper rightStepper(AccelStepper::DRIVER, RIGHT_STEP_PIN, RIGHT_DIR_PIN);

// State definitions
#define RSPD 01
#define RJSTR 02
// State variable
int state;

elapsedMillis printTime;

void setup() {
  Serial.begin(115200);
  // set the maximum speed and initial speed. The initial speed will be the only
  // speed used. No acceleration will happen - only runSpeed is used. Runs forever.
  myStepperR.setMaxSpeed(200.0);    // must be equal to or greater than desired speed.
  //myStepperR.setSpeed(-150.0);       // desired speed to run at
  myStepperL.setMaxSpeed(200.0);    // must be equal to or greater than desired speed.
  //myStepperL.setSpeed(150.0);
  //myStepper.setSpeed(-100.0);    // use this to run in opposite direction
}

void loop() {
  if (printTime >= 1000) {
    printTime = 0;

    // Take readings from the left and right sensors:
    int leftSensorReading = 0;
    leftSensorReading = analogRead(leftSensorPin);
    //Serial.println(leftSensorReading);
    int rightSensorReading = 0;
    rightSensorReading = analogRead(rightSensorPin);
    //Serial.println(rightSensorReading);
    float difference = leftSensorReading - rightSensorReading;
    float p = 2;
    float weighted_difference = difference * p;
    
    myStepperR.setSpeed(50.0 + weighted_difference);
    myStepperL.setSpeed(50.0 - weighted_difference);
    state = RSPD;
    float mSpeedR = myStepperR.speed();
    float mSpeedL = myStepperL.speed();
    //Serial.print(mSpeedR);
    //Serial.print("  ");
    //Serial.println(myStepperR.currentPosition());
  }

  int frontSensorReading = 0;
  frontSensorReading = analogRead(frontSensorPin);
  //Serial.println(frontSensorReading);
  if(frontSensorReading > 10){
      state = RJSTR;
    }
  else{
      state = RSPD;
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
