void loop() {

  unsigned long currentTime = millis();
  if (currentTime - lastLoopTime >= 20) { // Run the loop every 20 ms

    lastLoopTime = currentTime;

    //... Rest of the loop code

    // Calculate control inputs
    calculation(y);

    // Convert control inputs to step positions
    // We assume here that the control input u directly translates into a velocity
    // This will likely not be the case in a real system
    float leftMotorSpeed = u(0, 0);
    float rightMotorSpeed = u(1, 0);

    // Convert speeds to step positions
    // This requires knowing the time step of your control loop
    int leftMotorPosition = stepper1.currentPosition() + leftMotorSpeed * (currentTime - lastLoopTime) / 1000.0;
    int rightMotorPosition = stepper2.currentPosition() + rightMotorSpeed * (currentTime - lastLoopTime) / 1000.0;

    // Move motors to calculated positions
    stepper1.moveTo(leftMotorPosition);
    stepper2.moveTo(rightMotorPosition);

    // Step the motors to their new positions
    stepper1.run();
    stepper2.run();
  }
}
