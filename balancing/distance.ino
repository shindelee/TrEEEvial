#include <AccelStepper.h>

// Define the motor interface pins for the left wheel
#define LEFT_STEP_PIN 32
#define LEFT_DIR_PIN 15

// Define the motor interface pins for the right wheel
#define RIGHT_STEP_PIN 33
#define RIGHT_DIR_PIN 4

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
  leftStepper.setCurrentPosition(0);
  leftStepper.setCurrentPosition(0);
  
  leftStepper.setSpeed(-100); // Set constant speed
  rightStepper.setSpeed(100); // Set constant speed
  
  // Let's run the motor for 10 seconds
   unsigned long startTime = millis();
   while(millis() - startTime < 3000) {
    leftStepper.runSpeed(); // This will run the stepper at the set speed
    rightStepper.runSpeed(); 
  }

  // Calculate the distance travelled
  long stepsMoved = min(leftStepper.currentPosition(), rightStepper.currentPosition());
  Serial.println(leftStepper.currentPosition());
  Serial.println(rightStepper.currentPosition());
  
  float wheelRevolutions = (float)stepsMoved / STEPS_PER_REVOLUTION;
  float distanceTravelled = wheelRevolutions * wheelCircumference;
  
  Serial.println(distanceTravelled);
  delay(5000);
}
