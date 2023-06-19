if (FRONT_WALL) {
  if (leftSensorReading < sensor_threshold)
    turn_left_90();
  } 

  else if (rightSensorReading < sensor_threshold) {
    turn_right_90();
  } 

  else { //dead end
    turn_180();
  }
}
