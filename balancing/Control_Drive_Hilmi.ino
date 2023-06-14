
// --- INCLUDED LIBRARIES - - -//
#include < Wire .h >

// --- DECLARING I2C ADDRESS OF MPU6050 - - -//
int gyro_address = 0x68;
int acc_calibration_value = 480;

// --- SETTING PID PARAMETERS - - -//
float pid_p_gain = 12;
float pid_i_gain = 0.5;
float pid_d_gain = 22;

// --- DECLARING GLOBAL VARIABLES - - -//
byte start;

int left_motor, throttle_left_motor, throttle_counter_left_motor, throttle_left_motor_memory;
int right_motor, throttle_right_motor, throttle_counter_right_motor, throttle_right_motor_memory;
int gyro_pitch_data_raw, gyro_yaw_data_raw, accelerometer_data_raw;

long gyro_yaw_calibration_value, gyro_pitch_calibration_value;

unsigned long loop_timer;

// Variables for the angle and PID controller
float angle_gyro, angle_acc, angle, self_balance_pid_setpoint;
float pid_error_temp, pid_i_mem, pid_setpoint, gyro_input, pid_output, pid_last_d_error;
float pid_output_left, pid_output_right;

// Variables for steering
float spd = 1;       // Stores the desired speed value
float rotation = 1;  // Stores the rotation value
float turning_speed = 30;
float max_target_speed = 200;
float desired_rotation;
float desired_speed;

void setup() {
  // Starting the serial port att 115200 kbps
  Serial.begin(115200);

  // Starting the I2C bus
  Wire.begin();

  // Setting the I2C clock speed to 400 kHz
  TWBR = 12;


  // Creating variable pulse for stepper motor control, TIMER2_COMPA_vect
  TCCR2A = 0;
  TCCR2B = 0;
  TIMSK2 |= (1 << OCIE2A);
  TCCR2B |= (1 << CS21);
  OCR2A = 39;
  TCCR2A |= (1 << WGM21);

  // Starting the MPU6050
  Wire.beginTransmission(gyro_address);
  Wire.write(0 x6B);
  Wire.write(0 x00);
  Wire.endTransmission();

  // Setting the scale of the gyro to +/ - 250 degrees per second
  Wire.beginTransmission(gyro_address);
  Wire.write(0 x1B);
  Wire.write(0 x00);
  Wire.endTransmission();

  // Setting the scale of the accelerometer to +/ - 4g.
  Wire.beginTransmission(gyro_address);
  Wire.write(0 x1C);
  Wire.write(0 x08);
  Wire.endTransmission();

  // Setting a Low Pass Filter on MPU6050 to 43 Hz
  Wire.beginTransmission(gyro_address);
  Wire.write(0 x1A);
  Wire.write(0 x03);
  Wire.endTransmission();

  // Defining outputs
  pinMode(2, OUTPUT); //dir left
  pinMode(3, OUTPUT); //PWM for left motor
  pinMode(4, OUTPUT); //dir right
  pinMode(5, OUTPUT); //PWM for right motor
  pinMode(13, OUTPUT);

  // Calibrating the MPU6050 by reading the gyro offset 500 times and calculating the mean value
  for (receive_counter = 0; receive_counter < 500; receive_counter++) {
    Wire.beginTransmission(gyro_address);
    Wire.write(0 x43);
    Wire.endTransmission();
    Wire.requestFrom(gyro_address, 4);
    gyro_yaw_calibration_value += Wire.read() < < 8 | Wire.read();
    gyro_pitch_calibration_value += Wire.read() < < 8 | Wire.read();
    delayMicroseconds(3700);
  }
  gyro_pitch_calibration_value /= 500;
  gyro_yaw_calibration_value /= 500;

  // Creating loop timer to achieve 250 Hz frequency
  loop_timer = micros() + 4000;
}

// --- MAIN LOOP - - -//
void loop() {


  // Calculate forvard speed from serial reading
  if (spd > 1) {
    desired_speed = max_target_speed * (spd - 1);
  }

  // Calculate backward speed from serial reading
  if (spd < 1) {
    desired_speed = max_target_speed * (1 - spd);
  }

  // Calculate clockwise rotation speed from serial reading
  if (rotation > 1) {
    desired_rotation = turning_speed * (rotation - 1);
  }

  // Calculate counter clockwise rotation speed from serial reading
  if (rotation < 1) {
    desired_rotation = turning_speed * (1 - rotation);
  }

  // --- READ DATA FROM ACCELEROMETER - - -//
  Wire.beginTransmission(gyro_address);
  Wire.write(0 x3F);
  Wire.endTransmission();
  Wire.requestFrom(gyro_address, 2);
  accelerometer_data_raw = Wire.read() < < 8 | Wire.read();
  accelerometer_data_raw += acc_calibration_value;
  if (accelerometer_data_raw > 8200) accelerometer_data_raw = 8200;
  if (accelerometer_data_raw < -8200) accelerometer_data_raw = -8200;

  // Calculate angle from the accelerometer data
  angle_acc = asin((float)accelerometer_data_raw / 8200.0) * 57.296;

  // Set the angle of the gyro to the angle of the accelerometer if the robot is vertical
  if (start == 0 && angle_acc > -0.5 && angle_acc < 0.5) {
    angle_gyro = angle_acc;
    start = 1;
  }

  /// - - - READING DATA FROM THE GYRO - - -///
  Wire.beginTransmission(gyro_address);
  Wire.write(0 x43);
  Wire.endTransmission();
  Wire.requestFrom(gyro_address, 4);
  gyro_yaw_data_raw = Wire.read() < < 8 | Wire.read();
  gyro_pitch_data_raw = Wire.read() < < 8 | Wire.read();

  // Compensate the angle data with the calibration value
  gyro_pitch_data_raw -= gyro_pitch_calibration_value;
  angle_gyro += gyro_pitch_data_raw * 0.000031;

  gyro_yaw_data_raw -= gyro_yaw_calibration_value;

  // Corecting gyro drift with complementary filter
  angle_gyro = angle_gyro * 0.9996 + angle_acc * 0.0004;

  // Print the angle of the gyro for experiments
  Serial.println(angle_gyro);

  // --- PID CONTROLLER - - -//
  // Calculating the angular error
  pid_error_temp = angle_gyro - self_balance_pid_setpoint - pid_setpoint;
  if (pid_output > 10 || pid_output < -10) pid_error_temp += pid_output * 0.015;

  // Calculating the value on the I- part and add it to i_mem
  pid_i_mem += pid_i_gain * pid_error_temp;

  // Limit the maximum I- part value
  if (pid_i_mem > 400) pid_i_mem = 400;
  else if (pid_i_mem < -400) pid_i_mem = -400;

  // Calculating PID controller output
  pid_output = pid_p_gain * pid_error_temp + pid_i_mem + pid_d_gain * (pid_error_temp - pid_last_d_error);
  if (pid_output > 400) pid_output = 400;
  else if (pid_output < -400) pid_output = -400;

  // Storing the error for the next loop
  pid_last_d_error = pid_error_temp;

  // Creating a dead band for small PID outputs
  if (pid_output < 5 && pid_output > -5) pid_output = 0;

  // --- CONTROLLER OUTPUTS - - -//

  // Copying the PID output to the right and left motors
  pid_output_left = pid_output;
  pid_output_right = pid_output;

  // If a right rotation command is given , increase speed on the left motor and decrease speed on the right motor
  if (rotation < 1) {
    pid_output_left += desired_rotation;
    pid_output_right -= desired_rotation;
  }

  // If a left rotation command is given , increase speed on the right motor and decrease speed on the left motor
  if (rotation > 1) {
    pid_output_left -= desired_rotation;
    pid_output_right += desired_rotation;
  }

  // If a forward command is given , increase the PID angle set point
  if (spd > 1) {
    if (pid_setpoint > -2.5) pid_setpoint -= 0.1;
    if (pid_output > desired_speed * -1) pid_setpoint -= 0.005;
  }

  // If a backward command is given , decrease the PID angle set point
  if (spd < 1) {
    if (pid_setpoint < 2.5) pid_setpoint += 0.1;
    if (pid_output < desired_speed) pid_setpoint += 0.005;
  }

  // If no steering command is given , set the PID angle set point tozero
  if (spd == 1 && rotation == 1) {
    if (pid_setpoint > 0.5) pid_setpoint -= 0.05;
    else if (pid_setpoint < -0.5) pid_setpoint += 0.05;
    else pid_setpoint = 0;
  }

  // Change the PID angle setpoint to compensate for off centeredweight
  if (pid_setpoint == 0) {
    if (pid_output < 0) self_balance_pid_setpoint += 0.0115;
    if (pid_output > 0) self_balance_pid_setpoint -= 0.0115;
  }


  // --- CALCULATING THE STEPPER MOTOR PULSE - - -//
  // Linearize the stepper motors non - linear behavior
  if (pid_output_left > 0) pid_output_left = 405 - (1 / (pid_output_left + 9)) * 5500;
  else if (pid_output_left < 0) pid_output_left = -405 - (1 / (pid_output_left - 9)) * 5500;

  if (pid_output_right > 0) pid_output_right = 405 - (1 / (pid_output_right + 9)) * 5500;
  else if (pid_output_right < 0) pid_output_right = -405 - (1 / (pid_output_right - 9)) * 5500;

  // Calculate the pulse time for the stepper motors
  if (pid_output_left > 0) left_motor = 400 - pid_output_left;
  else if (pid_output_left < 0) left_motor = -400 - pid_output_left;
  else left_motor = 0;

  if (pid_output_right > 0) right_motor = 400 - pid_output_right;

  else if (pid_output_right < 0) right_motor = -400 - pid_output_right;
  else right_motor = 0;

  // Copy the pulse time to the throttle variables so the interruptsubroutine can use them
  throttle_left_motor = left_motor;
  throttle_right_motor = right_motor;


  // Delay loop if the time is under 40 ms (250 Hz)
  while (loop_timer > micros())
    ;
  loop_timer += 4000;
}

// --- INTERRUPT ROUTINE FOR TIMER2_COMPA_vect - - -//
ISR(TIMER2_COMPA_vect) {
  // Left motor pulse calculations
  throttle_counter_left_motor++;
  if (throttle_counter_left_motor > throttle_left_motor_memory) {
    throttle_counter_left_motor = 0;
    throttle_left_motor_memory = throttle_left_motor;
    if (throttle_left_motor_memory < 0) {
      PORTD &= 0 b11110111;
      throttle_left_motor_memory *= -1;
    } else PORTD |= 0 b00001000;
  } else if (throttle_counter_left_motor == 1) PORTD |= 0 b00000100;
  else if (throttle_counter_left_motor == 2) PORTD &= 0 b11111011;

  // right motor pulse calculations
  throttle_counter_right_motor++;
  if (throttle_counter_right_motor > throttle_right_motor_memory) {
    throttle_counter_right_motor = 0;
    throttle_right_motor_memory = throttle_right_motor;
    if (throttle_right_motor_memory < 0) {
      PORTD |= 0 b00100000;
      throttle_right_motor_memory *= -1;
    } else PORTD &= 0 b11011111;
  } else if (throttle_counter_right_motor == 1) PORTD |= 0 b00010000;
  else if (throttle_counter_right_motor == 2) PORTD &= 0 b11101111;
}



