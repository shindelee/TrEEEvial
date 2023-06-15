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
  //leftStepper.setSpeed(180);
  leftStepper.setAcceleration(10);  // Adjust as needed
  
  
  rightStepper.setMaxSpeed(200);  // Adjust as needed
  rightStepper.setAcceleration(10);  // Adjust as needed
  //rightStepper.move(1000);
}

void loop() {
  static long prevWheelSteps = 0;
  Serial.println(prevWheelSteps);
  
  // 90 degree - right
  
  leftStepper.move(-240);
  while(leftStepper.distanceToGo() != 0) {
    leftStepper.run();
    Serial.println("left stepper dist to go: " + String(leftStepper.distanceToGo()));
  }
  
  
  // 90 degree - left
  /*
  rightStepper.move(235);
  while(rightStepper.distanceToGo() != 0) {
    rightStepper.run();
    Serial.print("right stepper dist to go: " + String(rightStepper.distanceToGo()));
  }
  delay(5000);
  */

  // use wheel revolution to calculate angle turned (heading angle)

  //int leftWheelRevs = getWheelRevolutions(leftStepper); 
  long leftWheelSteps = leftStepper.currentPosition() - prevWheelSteps; // Get the current position of the left stepper in steps
  Serial.println(leftStepper.currentPosition());
  Serial.println(leftWheelSteps);
  prevWheelSteps = leftStepper.currentPosition();
  float leftWheelRevs = leftWheelSteps / STEPS_PER_REVOLUTION; 
  Serial.println(leftWheelRevs);
  float distanceTravelled = leftWheelRevs * wheelCircumference;
  Serial.println(distanceTravelled);
  float angleTurnedRadians = distanceTravelled / wheelBase;
  Serial.println(angleTurnedRadians);
  float angleTurnedDegrees = angleTurnedRadians * (180.0 / PI);

  Serial.println(angleTurnedDegrees);
  delay(1000); // 1 second

  // offsets

  
}
