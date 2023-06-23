#include "movement_state_machine.h"
#include "shared_variables.h"

// functions
void update_state_history()
{
  state_history[4] = state_history[3];
  state_history[3] = state_history[2];
  state_history[2] = state_history[1];
  state_history[1] = state_history[0];
  state_history[0] = state;
}

void initialise_state_history()
{
  state_history[0] = state;
  state_history[1] = state;
  state_history[2] = state;
  state_history[3] = state;
  state_history[4] = state;
}

void set_speed(int left_sensor_reading, int right_sensor_reading, int required_speed)
{
  // Serial.print("left sensor" + String(left_sensor_reading));
  // Serial.print("   right sensor" + String(right_sensor_reading));
  int difference = left_sensor_reading - right_sensor_reading;
  // Serial.print("difference = " + String(difference) + "   ");
  int weighted_difference = static_cast<int>(difference) * 3;
  // Serial.print("weighted difference = " + String(weighted_difference) + "   ");

  leftStepper.setSpeed(required_speed - weighted_difference);
  rightStepper.setSpeed(required_speed + weighted_difference);
}

/*
void set_wall_states()
{
  if (wall_in_front)
  {
    state = FRONT_WALL;
    cur_state = "front wall";

    leftStepper.stop();
    rightStepper.stop();
    leftStepper.setSpeed(0);
    rightStepper.setSpeed(0);
  }
  else if (wall_on_left && wall_on_right && !wall_in_front)
  {
    state = TWO_WALLS;
    cur_state = "two wall";
    set_speed(leftSensorReading, rightSensorReading, 70 * 2); // sensor needs to be at 70 on track!
  }

  else if (wall_on_left && !wall_on_right)
  {
    state = LEFT_WALL;
    cur_state = "left wall";
    set_speed(leftSensorReading, sensor_setpoint, 70 * 2);
  }
  else if (!wall_on_left && wall_on_right)
  {
    state = RIGHT_WALL;
    cur_state = "right wall";
    set_speed(sensor_setpoint, rightSensorReading, 70 * 2);
  }
  else
  {
    state = NO_WALL;
    cur_state = "no wall";
    leftStepper.setSpeed(-50);
    rightStepper.setSpeed(50);
  }
}
*/
float read_ultrasound(int trigPin, int echoPin)
{
  // left ultrasound
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  float duration_left = static_cast<float>(pulseIn(echoPin, HIGH));
  return float(duration_left * 0.034 / 2);
}

void read_sensors()
{
  //  leftSensorReading = analogRead(leftSensorPin);
  //  rightSensorReading = analogRead(rightSensorPin); //offset since right sensor less sensitive
  //  frontSensorReading = analogRead(frontSensorPin);

  frontSensorReading = read_ultrasound(trigPinFront, echoPinFront); // D13, D10
  rightSensorReading = read_ultrasound(trigPinRight, echoPinRight); // A0, A1
  leftSensorReading = read_ultrasound(trigPinLeft, echoPinLeft);

  wall_in_front = frontSensorReading < sensor_front;
  wall_on_left = leftSensorReading < sensor_upper_bound;
  wall_on_right = rightSensorReading < sensor_upper_bound;
}

//void read_sensors_and_set_speed()
//{
//  read_sensors();
//  set_wall_states();
//  update_state_history();
//  //    update_left_sensor_history();
//  //    update_right_sensor_history();
//
//  Serial.print("left: " + String(leftSensorReading) + "  ");
//  Serial.print("front: " + String(frontSensorReading) + "  ");
//  Serial.print("right: " + String(rightSensorReading) + "  ");
//
//  Serial.print("lw : " + String(wall_on_left) + "   ");
//  Serial.print("fw : " + String(wall_in_front) + "   ");
//  Serial.print("rw : " + String(wall_on_right) + "   ");
//}
