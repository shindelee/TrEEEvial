/*
#include <AccelStepper.h>
//#include <elapsedMillis.h>
#include <Math.h>

#define LEFT_STEP_PIN 33 // A3
#define LEFT_DIR_PIN 15  // D12

// Define the motor interface pins for the right wheel
#define RIGHT_STEP_PIN 32 // A4
#define RIGHT_DIR_PIN 4   // D11

// Define the steps per revolution for your stepper motors
#define STEPS_PER_REVOLUTION 200.0
#define WHEEL_RADIUS 0.0325

// light sensor pins
const int leftSensorPin = 34;  // A0 for now
const int frontSensorPin = 39; // A1 for now
const int rightSensorPin = 35; // A5
*/
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
int state_history[5];
int left_sensor_history[5];
int right_sensor_history[5];
int sensor_threshold = 50;


//functions
void update_state_history() {
//    state_history[9] =state_history[8];
//    state_history[8] =state_history[7];
//    state_history[7] =state_history[6];
//    state_history[6] =state_history[5];
//    state_history[5] =state_history[4];
    state_history[4] =state_history[3];
    state_history[3] =state_history[2];
    state_history[2] =state_history[1];
    state_history[1] =state_history[0];
    state_history[0] =state;
}

void update_left_sensor_history() {
  left_sensor_history[4] = left_sensor_history[3];
  left_sensor_history[3] = left_sensor_history[2];
  left_sensor_history[2] = left_sensor_history[1];
  left_sensor_history[1] = left_sensor_history[0];
  left_sensor_history[0] = leftSensorReading;
}

void update_right_sensor_history() {
  right_sensor_history[4] = right_sensor_history[3];
  right_sensor_history[3] = right_sensor_history[2];
  right_sensor_history[2] = right_sensor_history[1];
  right_sensor_history[1] = right_sensor_history[0];
  right_sensor_history[0] = rightSensorReading;
}

void initialise_state_history() {
  state_history[0] =state;
  state_history[1] =state;
  state_history[2] =state;
  state_history[3] =state;
  state_history[4] =state;
}

void initialise_left_sensor_history() {
  left_sensor_history[4] = leftSensorReading;
  left_sensor_history[3] = leftSensorReading;
  left_sensor_history[2] = leftSensorReading;
  left_sensor_history[1] = leftSensorReading;
  left_sensor_history[0] = leftSensorReading;
}

void initialise_right_sensor_history() {
  right_sensor_history[4] = rightSensorReading;
  right_sensor_history[3] = rightSensorReading;
  right_sensor_history[2] = rightSensorReading;
  right_sensor_history[1] = rightSensorReading;
  right_sensor_history[0] = rightSensorReading;
}


void set_speed(int left_sensor_reading, int right_sensor_reading, int required_speed) {
  int difference = left_sensor_reading - right_sensor_reading;
  Serial.print ("difference = " + String(difference) + "   ");
  int weighted_difference = static_cast<int>(difference * 0.4);
  Serial.print ("weighted difference = " + String(weighted_difference) + "   ");

  leftStepper.setSpeed(-(required_speed + weighted_difference));
  rightStepper.setSpeed(required_speed - weighted_difference);
}


void set_wall_states() {
  if (wall_in_front)
    {
      state = FRONT_WALL;
      cur_state = "front wall";

        leftStepper.setSpeed(0);
        rightStepper.setSpeed(0);
      
    }
    else if (wall_on_left && wall_on_right && !wall_in_front)
    {
      state = TWO_WALLS;
      cur_state = "two wall";
        set_speed(leftSensorReading,rightSensorReading, 70); //sensor needs to be at 70 on track!
      
    }
  
    else if (wall_on_left && !wall_on_right)
    {
      state = LEFT_WALL;
      cur_state = "left wall";
      set_speed(leftSensorReading, sensor_threshold + 40, sensor_threshold + 40);
      
    }
    else if (!wall_on_left && wall_on_right)
    {
      state = RIGHT_WALL;
      cur_state = "right wall";
        set_speed(sensor_threshold + 40,rightSensorReading, sensor_threshold + 40);
    }
    else
    {
      state = NO_WALL;
      cur_state = "no wall";
        leftStepper.setSpeed(-50);
        rightStepper.setSpeed(50);
      
    }
}


void leave_node(){
    Serial.println("turning 90 to the right!");
  leftStepper.setCurrentPosition(0);
  rightStepper.setCurrentPosition(0);
  leftStepper.move(-200);
  rightStepper.move(200);
  leftStepper.setSpeed(-50);
  rightStepper.setSpeed(50);
  while(rightStepper.distanceToGo() != 0 && leftStepper.distanceToGo() != 0) {
    rightStepper.runSpeed();
    leftStepper.runSpeed();
  }
    read_sensors_and_set_speed();

    
   
}

void read_sensors() {
  leftSensorReading = analogRead(leftSensorPin);
  rightSensorReading = analogRead(rightSensorPin); //offset since right sensor less sensitive
  frontSensorReading = analogRead(frontSensorPin);
  wall_in_front = frontSensorReading > sensor_threshold;
  wall_on_left = leftSensorReading > sensor_threshold;
  wall_on_right = rightSensorReading > sensor_threshold;
}

void read_sensors_and_set_speed() {
    read_sensors();
    set_wall_states();
    update_state_history();
    update_left_sensor_history();
    update_right_sensor_history();
        
    Serial.print("left: " + String(leftSensorReading) + "  ");
    Serial.print("right: " + String(rightSensorReading) + "  ");
    Serial.print("front: " + String(frontSensorReading) + "  ");
}


void turn_right_90(){
  Serial.println("turning 90 to the right!");
  leftStepper.setCurrentPosition(0);
  rightStepper.setCurrentPosition(0);
  leftStepper.move(108);
  rightStepper.move(108);
  leftStepper.setSpeed(50);
  rightStepper.setSpeed(50);
  while(rightStepper.distanceToGo() != 0 && leftStepper.distanceToGo() != 0) {
    rightStepper.runSpeed();
    leftStepper.runSpeed();
  }
}


void setup()
{
  Serial.begin(115200);
  leftStepper.setMaxSpeed(300.0);  // must be equal to or greater than desired speed.
  rightStepper.setMaxSpeed(300.0); // must be equal to or greater than desired speed.
  read_sensors_and_set_speed();
  initialise_state_history();

  initialise_left_sensor_history();
  initialise_right_sensor_history();
  
    
}


void loop()
{
  int time_since_reading_left = abs(leftStepper.currentPosition() - last_sensor_reading_left);
  int time_since_reading_right = abs(rightStepper.currentPosition() - last_sensor_reading_right);
  if (state == FRONT_WALL || (time_since_reading_left >=20 || time_since_reading_right >=20))
  {
    // Take readings from the sensors and set states
    read_sensors_and_set_speed();
    
    
    bool change_state = (state_history[0] == state_history[1]);
    change_state = change_state && (state_history[1]== state_history[2]);
    change_state = change_state && (state_history[2]== state_history[3]);
    change_state = change_state && (state_history[3]!= state_history[4]);

    bool lost_wall = (abs(left_sensor_history[4] - leftSensorReading) > 40) || (abs(right_sensor_history[4] - rightSensorReading) > 40);
    if ((change_state) || state == FRONT_WALL) { //entering a node
      Serial.println("in a node!");
      leftStepper.stop();
      rightStepper.stop();

      if (state != FRONT_WALL) {
        leftStepper.move(-30);
        rightStepper.move(30);
        leftStepper.setSpeed(-20);
        rightStepper.setSpeed(20);
        while (abs(leftStepper.distanceToGo()) >0) {
          Serial.println(leftStepper.distanceToGo());
          leftStepper.runSpeed();
          rightStepper.runSpeed();
        }
      }
      else {
        delay(2000);
        turn_right_90();
        delay(1000);
        leave_node();
      }
      
      read_sensors_and_set_speed();
      delay(2000);
      initialise_state_history();
      initialise_left_sensor_history();
      initialise_right_sensor_history();

  
    }
  
  }

  Serial.print("left speed = " + String(leftStepper.speed()) + "   ");
  Serial.println("right speed = " + String(rightStepper.speed()));

  //run the stepper
  leftStepper.runSpeed();
  rightStepper.runSpeed();

}
