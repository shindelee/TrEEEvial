#include <AccelStepper.h>

// Define the motor interface pins for the left wheel
#define LEFT_STEP_PIN 14
#define LEFT_DIR_PIN 12

// Define the motor interface pins for the right wheel
#define RIGHT_STEP_PIN 26
#define RIGHT_DIR_PIN 27

// Define the steps per revolution for your stepper motors
#define STEPS_PER_REVOLUTION 200.0
#define WHEEL_RADIUS 0.0325

float wheelDiameter = 2 * WHEEL_RADIUS; 
float wheelBase = 0.14;  
float wheelCircumference = wheelDiameter * PI;


// Create instances of the AccelStepper class for the left and right wheels
AccelStepper leftStepper(AccelStepper::DRIVER, LEFT_STEP_PIN, LEFT_DIR_PIN);
AccelStepper rightStepper(AccelStepper::DRIVER, RIGHT_STEP_PIN, RIGHT_DIR_PIN);

void setup() {
  Serial.begin(115200);
  Serial.println("hi");
  
  // Set the maximum speed and acceleration for the stepper motors
  leftStepper.setMaxSpeed(200);  // Adjust as needed
  leftStepper.setAcceleration(10);  // Adjust as needed
  
  
  rightStepper.setMaxSpeed(200);  // Adjust as needed
  rightStepper.setAcceleration(10);  // Adjust as needed
  //rightStepper.move(1000);
}

void loop() {
  static long prevWheelSteps = 0;
  Serial.println(prevWheelSteps);
  
  // 90 degree - right

  /*
  leftStepper.move(-216); // -90.26
  while(leftStepper.distanceToGo() != 0) {
    leftStepper.run();
    Serial.println("left stepper dist to go: " + String(leftStepper.distanceToGo()));
  }
  */
  
  // 90 degree - left

  /*
  rightStepper.move(216);
  while(rightStepper.distanceToGo() != 0) {
    rightStepper.run();
    Serial.print("right stepper dist to go: " + String(rightStepper.distanceToGo()));
  }
  */

  // in - place 90
  /*
  rightStepper.move(108);
  leftStepper.move(108);
  while(rightStepper.distanceToGo() != 0 && leftStepper.distanceToGo() != 0) {
    rightStepper.run();
    leftStepper.run();
    Serial.print("right stepper dist to go: " + String(rightStepper.distanceToGo()));
    Serial.println("left stepper dist to go: " + String(leftStepper.distanceToGo()));
  }
  */
  /*
  // in - place 360 
  
  rightStepper.move(432);
  leftStepper.move(432);
  while(rightStepper.distanceToGo() != 0 && leftStepper.distanceToGo() != 0) {
    rightStepper.run();
    leftStepper.run();
    Serial.print("right stepper dist to go: " + String(rightStepper.distanceToGo()));
    Serial.println("left stepper dist to go: " + String(leftStepper.distanceToGo()));
  }
  */
  
  // use wheel revolution to calculate angle turned (heading angle)
  // left
  long leftWheelSteps = leftStepper.currentPosition() - prevWheelStepsL; // Get the current position of the left stepper in steps
  Serial.println(leftStepper.currentPosition());
  Serial.println(leftWheelSteps);
  prevWheelStepsL = leftStepper.currentPosition();
  float leftWheelRevs = leftWheelSteps / STEPS_PER_REVOLUTION; 
  Serial.println(leftWheelRevs);
  float distanceTravelledL = leftWheelRevs * wheelCircumference;
  Serial.println(distanceTravelledL);
  float angleTurnedRadiansL = distanceTravelled / wheelBase;
  Serial.println(angleTurnedRadiansL);
  float angleTurnedDegreesL = angleTurnedRadians * (180.0 / PI);

  Serial.println(angleTurnedDegrees); 
  /*
  // right
  long rightWheelSteps = rightStepper.currentPosition() - prevWheelStepsR; // Get the current position of the left stepper in steps
  Serial.println(rightStepper.currentPosition());
  Serial.println(rightWheelSteps);
  prevWheelStepsR = rightStepper.currentPosition();
  float rightWheelRevs = rightWheelSteps / STEPS_PER_REVOLUTION; 
  Serial.println(rightWheelRevs);
  float distanceTravelledR = rightWheelRevs * wheelCircumference;
  Serial.println(distanceTravelledR);
  float angleTurnedRadiansR = distanceTravelledR / wheelBase;
  Serial.println(angleTurnedRadiansR);
  float angleTurnedDegreesR = angleTurnedRadiansR * (180.0 / PI);

  Serial.println(angleTurnedDegrees); 
  */
  delay(3000); // 3 second
}

/*
int getWheelRevolutions(int wheel) {
  myStepper.step(1); // Move the motor by one step
  leftSteps++ ; // Increment the number of steps
  int leftWheelRevs = leftSteps / STEPS_PER_REVOLUTION;
  return leftWheelRevs;
}
*/