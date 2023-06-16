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

// Motor Connections (unipolar motor driver)
const int In1 = 8;
const int In2 = 9;
const int In3 = 10;
const int In4 = 11;
// Motor Connections (constant voltage bipolar H-bridge motor driver)
const int AIn1 = 8;
const int AIn2 = 9;
const int BIn1 = 10;
const int BIn2 = 11;


// Left sensor. A3. (A1 for now)
const int leftSensorPin = 26;
// Front sensor. 34. (A2 for now)
const int frontSensorPin = 25;
// Right sensor. 39. (VN) (A5 for now)
const int rightSensorPin = 35;

// Sebastian's change connected DIRL to D10 (14), and STPL to A5 (35).
// Motor Connections (constant current, step/direction bipolar motor driver)
const int dirPinR = 4; //D11
const int stepPinR = 27; //A0

const int dirPinL = 15; //D12
const int stepPinL = 32; //A4

// Creates an instance - Pick the version you want to use and un-comment it. That's the only required change.
//AccelStepper myStepper(AccelStepper::FULL4WIRE, AIn1, AIn2, BIn1, BIn2);  // works for TB6612 (Bipolar, constant voltage, H-Bridge motor driver)
//AccelStepper myStepper(AccelStepper::FULL4WIRE, In1, In3, In2, In4);    // works for ULN2003 (Unipolar motor driver)
AccelStepper myStepperR(AccelStepper::DRIVER, stepPinR, dirPinR);           // works for a4988 (Bipolar, constant current, step/direction driver)
AccelStepper myStepperL(AccelStepper::DRIVER, stepPinL, dirPinL);          // works for a4988 (Bipolar, constant current, step/direction driver)

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
    
    myStepperR.setSpeed(100.0 + weighted_difference);
    myStepperL.setSpeed(100.0 - weighted_difference);
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
