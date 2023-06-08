const float MASS = 1.0; // Set this to the mass of your robot

float leftVelocity = 0, rightVelocity = 0;
unsigned long lastTime = 0;

void loop() {
  unsigned long currentTime = micros();
  unsigned long deltaTime = currentTime - lastTime;
  lastTime = currentTime;

  // Your other code here...

  // Uncomment this to start the control loop when you're ready
  // timer = timerBegin(0, 80, true);
  // timerAttachInterrupt(timer, &onTimer, true);
  // timerAlarmWrite(timer, CONTROL_LOOP_PERIOD_US, true);
  // timerAlarmEnable(timer);

  // Calculate the acceleration from the force (F=ma)
  float leftAcceleration = u(0) / MASS;
  float rightAcceleration = u(1) / MASS;

  // Integrate the acceleration over time to get velocity (v = at)
  leftVelocity += leftAcceleration * (deltaTime / 1000000.0); // Convert microseconds to seconds
  rightVelocity += rightAcceleration * (deltaTime / 1000000.0); // Convert microseconds to seconds

  // Set the stepper motor speed to the calculated velocity
  stepper1.setSpeed(leftVelocity);
  stepper2.setSpeed(rightVelocity);
}
