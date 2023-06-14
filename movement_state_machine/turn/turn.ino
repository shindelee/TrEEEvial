#include <AccelStepper.h>

// Define the motor interface pins for the left wheel
#define LEFT_STEP_PIN 32
#define LEFT_DIR_PIN 15

// Define the motor interface pins for the right wheel
#define RIGHT_STEP_PIN 27
#define RIGHT_DIR_PIN 4

// Define the steps per revolution for your stepper motors
#define STEPS_PER_REVOLUTION 200

// Create instances of the AccelStepper class for the left and right wheels
AccelStepper leftStepper(AccelStepper::DRIVER, LEFT_STEP_PIN, LEFT_DIR_PIN);
//AccelStepper rightStepper(AccelStepper::DRIVER, RIGHT_STEP_PIN, RIGHT_DIR_PIN);

void setup() {
  Serial.begin(115200);
  Serial.println("hi");
  
  // Set the maximum speed and acceleration for the stepper motors
  leftStepper.setMaxSpeed(200);  // Adjust as needed
  leftStepper.setAcceleration(10);  // Adjust as needed
  
  
//  rightStepper.setMaxSpeed(200);  // Adjust as needed
//  rightStepper.setAcceleration(10);  // Adjust as needed
//  rightStepper.move(1000);
}

void loop() {
  leftStepper.move(-175);
  while(leftStepper.distanceToGo() != 0) {
    leftStepper.run();
    Serial.print("left stepper dist to go: " + String(leftStepper.distanceToGo()));
  }
  delay(5000);
  
//  Serial.println("right stepper dist to go: " + String(rightStepper.distanceToGo()));
}
