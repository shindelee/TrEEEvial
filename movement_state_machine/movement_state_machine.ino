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
#define TWO_WALLS 2
#define FRONT_WALL 1
#define LEFT_WALL 3
#define RIGHT_WALL 4
#define NO_WALL 5
#define STATE_CHANGE 6

// State variables
int state;
String previous_state;
String cur_state;
bool in_node = true; // boolean to tell whether we are in a node
int frontSensorReading = 0;
int leftSensorReading = 0;
int rightSensorReading = 0;
int last_sensor_reading_left = 0;
int last_sensor_reading_right = 0;
bool wall_on_left;
bool wall_on_right;
bool wall_in_front;

//functions
void set_speed(int left_sensor_reading, int right_sensor_reading) {
  float difference = leftSensorReading - rightSensorReading;
  float p = 1;
  float weighted_difference = difference * p;

  leftStepper.setSpeed(-(50.0 + weighted_difference));
  rightStepper.setSpeed(50.0 - weighted_difference);
}

void set_wall_states() {
  if (wall_in_front)
    {
      state = FRONT_WALL;
      cur_state = "front wall";
    }
    else if (wall_on_left && wall_on_right && !wall_in_front)
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
    else
    {
      state = NO_WALL;
      cur_state = "no wall";
    }
}

void set_motor_speeds() {

if (state == TWO_WALLS){
      set_speed(leftSensorReading,rightSensorReading);
    }

    else if (state == LEFT_WALL) {
      set_speed(leftSensorReading,50);
    }

    else if (state == RIGHT_WALL) {
      set_speed(50,rightSensorReading);
    }

    else if (state == NO_WALL) { //proceed with caution!
      leftStepper.setSpeed(-50);
      rightStepper.setSpeed(50);
    }
}

void read_sensors_and_set_speed() {
    leftSensorReading = analogRead(leftSensorPin);
    rightSensorReading = analogRead(rightSensorPin);
    frontSensorReading = analogRead(frontSensorPin);
    wall_in_front = frontSensorReading > 50;
    wall_on_left = leftSensorReading > 50;
    wall_on_right = rightSensorReading > 50;

    set_wall_states();
    set_motor_speeds();
    
    Serial.print("left wall: " + String(wall_on_left) + "  ");
    Serial.print("right wall: " + String(rightSensorReading) + "  ");
    Serial.print("front wall: " + String(wall_in_front) + "  ");
}


void setup()
{
  Serial.begin(115200);
  leftStepper.setMaxSpeed(300.0);  // must be equal to or greater than desired speed.
  rightStepper.setMaxSpeed(300.0); // must be equal to or greater than desired speed.
  read_sensors_and_set_speed();
  previous_state = cur_state;
}


void loop()
{
  int time_since_last_reading_left = abs(leftStepper.currentPosition() - last_sensor_reading_left);
  int time_since_last_reading_right = abs(rightStepper.currentPosition() - last_sensor_reading_right);
  Serial.println ("time since last reading left = " + String(time_since_last_reading_left));
  Serial.println ("time since last reading right = " + String(time_since_last_reading_right));
  if (time_since_last_reading_left >=20 && time_since_last_reading_right >=20) {
    // Take readings from the sensors and set states
    read_sensors_and_set_speed();
    if (cur_state != previous_state) {
      leftStepper.stop();
      rightStepper.stop();
      delay(5000);
    }
    previous_state = cur_state;
  
  }

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
  }
}
