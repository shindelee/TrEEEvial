#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <BasicLinearAlgebra.h>
#include <ESP32TimerInterrupt.h>

using namespace BLA;

Adafruit_MPU6050 mpu;
// Instantiate an ESP32TimerInterrupt object with the name "timer"
ESP32Timer ITimer0(0);

#define dirPinLeft 12
#define stepPinLeft 14
#define stepsPerRevolution 200

#define dirPinRight 27
#define stepPinRight 26

#define TIMER_INTERVAL_MS 1000

// The Matrix<6, 1> called x_i is initialized with zeros and never updated anywhere in your code. 
// It should be updated with new state variables in every control loop.


void setMotorSpeeds(float leftMotorSpeed, float rightMotorSpeed) {

  if (leftMotorSpeed >= 0) {
    analogWrite(stepPinLeft, leftMotorSpeed);
    digitalWrite(dirPinLeft, HIGH);
  }
  else {
    analogWrite(stepPinLeft, 255 + leftMotorSpeed);
    digitalWrite(dirPinLeft, LOW);
  }
  if (rightMotorSpeed >= 0) {
    analogWrite(stepPinRight, rightMotorSpeed);
    digitalWrite(dirPinRight, LOW);
  }
  else {
    analogWrite(stepPinRight, 255 + rightMotorSpeed);
    digitalWrite(dirPinRight, HIGH);
  }
}


// Control Law Matrices

Matrix<2, 1> u; // Control input

// Control gain
Matrix<2, 6> K = {0, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0
                 };

// System matrix
Matrix<6, 6> A_d = {0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0
                   };

// Input matrix
Matrix<6, 2> B_d = {0, 0,
                    0, 0,
                    0, 0,
                    0, 0,
                    0, 0,
                    0, 0
                   };

Matrix<2, 6> C_d = {0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0
                   };

// State vector
Matrix<6, 1> x_i = {0, 
                    0, 
                    0, 
                    0, 
                    0, 
                    0}; 
                    
// Desired output  
Matrix<2, 1> y_d = {0, 
                   0};  // Desired output

void computeControlInput() {
  Matrix<6, 6> I; // Identity matrix
  for(int i=0; i<6; i++) {
    for(int j=0; j<6; j++) {
      if(i==j)
        I(i,j) = 1;  // Elements on the diagonal are 1
      else
        I(i,j) = 0;  // All other elements are 0
  }
}

  Matrix<6, 6> temp1 = I - (A_d - B_d * K); // Compute (I - (A_d - B_d * K))

  // Check if the inversion is successful
  bool invertStatus = Invert(temp1);

  if(invertStatus)
  {
    Matrix<2, 2> temp2 = C_d * (~temp1) * B_d; // Compute (C_d / (I - (A_d - B_d * K)) * B_d)
    u = -K * x_i + (~temp2) * y_d; // Compute -K * x_i + ((C_d / (I - (A_d - B_d * K)) * B_d) \ y_d)
  }
  else
  {
    Serial.println("Matrix inversion failed");
    // Handle the case when the inversion fails
  }
  
  // Matrix<6, 2> temp2 = Invert(temp1) * B_d; // Compute (C_d / (I - (A_d - B_d * K)) * B_d)

  // u = -K * x_i + (~temp2) * y_d;          // Compute -K * x_i + ((C_d / (I - (A_d - B_d * K)) * B_d) \ y_d)
}

void setup() {
  Serial.begin(115200);

  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);

  // Initialize timer0
  if (ITimer0.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, TimerHandler, nullptr)) {
    Serial.print(F("Starting  ITimer0 OK, millis() = "));
    Serial.println(millis());
  } else
    Serial.println(F("Can't set ITimer0. Select another freq. or timer"));

}

void loop() {

  // Get new sensor events with the readings
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Calculate these based on the readings from MPU6050.

  // displacement
  float X = 0;

  // heading angle of robot with respect to horizontal, measure positive counterclockwise
  float alpha = atan2(a.acceleration.y, a.acceleration.z);

  // angle with respect to vertical, measure positive counterclockwise
  float theta = 0;

  // Store state variables in an array
  float xi[] = {X, alpha, theta};

  computeControlInput();

  // Set the motor speed based on the control signal.
  // Depending on your setup, you might need to convert the control signal to appropriate units for the motor speed.
  
  float leftSpeed = 0;
  float rightSpeed = 0;
  // Set the motor speeds.
  setMotorSpeeds(leftSpeed, rightSpeed);

  delay(100);
}

void TimerHandler(void *param)
{
    // code to be executed every 1 second
}