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
AccelStepper rightStepper(AccelStepper::DRIVER, RIGHT_STEP_PIN, RIGHT_DIR_PIN);

void setup()
{
    Serial.begin(115200);
    Serial.println("hi");

    // Set the maximum speed and acceleration for the stepper motors
    leftStepper.setMaxSpeed(200); // Adjust as needed
    leftStepper.setSpeed(-500);
    leftStepper.setAcceleration(10); // Adjust as needed

    rightStepper.setMaxSpeed(200); // Adjust as needed
    rightStepper.setSpeed(500);
    rightStepper.setAcceleration(10); // Adjust as needed
    //  rightStepper.move(1000);
}

void loop()
{
    while (leftStepper.currentPosition() < 500)
    {
        leftStepper.runSpeed();
        rightStepper.runSpeed();
        Serial.println("left stepper dist to go: " + String(leftStepper.currentPosition()));
    }

    //  Serial.println("right stepper dist to go: " + String(rightStepper.distanceToGo()));
}