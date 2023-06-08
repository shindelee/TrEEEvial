#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <BasicLinearAlgebra.h>
#include <ESP32TimerInterrupt.h>
#include <AccelStepper.h>

using namespace BLA;

Adafruit_MPU6050 mpu;

#define dirPinLeft 12
#define stepPinLeft 14
#define stepsPerRevolution 200
#define DISTANCE_PER_STEP 0.01

#define dirPinRight 27    // right motor driver
#define stepPinRight 26   // left motor driver

const float WHEEL_CIRCUMFERENCE = 0.204;  // circumference of the wheel in meters 

volatile int encoderCount = 0;

const float MAX_TORQUE = 1.0;  // set this to the maximum torque your motor can deliver
const int MAX_PWM = 255;  // the maximum PWM value for the Arduino

#define TIMER_INTERVAL_MS 1000
#define STEPS_PER_REV 200
#define WHEEL_RADIUS 0.0325

int LIMIT = 400;
int stepperSpeed;
int16_t Xacc, Zacc, gyroY, gyroRate, mu = 0.993;
float accangle, curangle, preangle = 0, gyroAngle = 0, alpha;
unsigned long currTime, prevTime = 0, loopTime;

AccelStepper stepper1(AccelStepper::DRIVER, stepPinLeft, dirPinLeft); // Create an instance of AccelStepper for left motor
AccelStepper stepper2(AccelStepper::DRIVER, stepPinRight, dirPinRight); // Create an instance of AccelStepper for right motor

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

Matrix<2, 1> u; // Control input: Torque
Matrix<2, 1> y_xi;

// Control gain
Matrix<2, 6> K = {-5.8956, -13.2836, 0.3939, 0.3385, -14.4569, -8.6721,
                  -8.2139, -17.1178, -0.4273, -0.3720, -17.5030, -10.7338
                 };

Matrix<6, 2> L = {0.9841, 0.4393,
                  3.1689, 2.4734,
                  0.0249, 0.3256,
                  -0.0135, -0.1246,
                  -7.4352, -5.7781,
                  -15.4129, -12.4067
                 };

// System matrix
Matrix<6, 6> A_d = {1.0000, 0.0923, 0, 0, -0.0092, 0.0005,
                    0, 0.8497, 0, 0, -0.1777, 0.0059,
                    0, 0, 1.0000, 0.0925, 0, 0,
                    0, 0, 0, 0.8548, 0, 0,
                    0, 0.0161, 0, 0, 1.0309, 0.0994,
                    0, 0.3143, 0, 0, 0.6076, 0.9995
                   };

// Input matrix
Matrix<6, 2> B_d = {0.0039, 0.0039,
                    0.0752, 0.0752,
                    0.0233, -0.0233,
                    0.4538, -0.4358,
                    -0.0081, -0.0081,
                    -0.1572, -0.1572
                   };

Matrix<2, 6> C_d = {1, 0, 0, 0, 0, 0,
                    0, 0, 1, 0, 0, 0
                   };

// State vector
Matrix<6, 1> x_i = {0, 
                    0, 
                    0, 
                    0, 
                    1.5708, 
                    0}; 

// Next State Vector
Matrix<6, 1> x_i_next;
                    
// Desired output  
Matrix<2, 1> y_d = {5, 
                    0.7854};  
                    
// Current output
Matrix<2, 1> y;

void computeControlInput(Matrix<2, 1> y_d) {
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
}

// calculate the wheel circumference (2*Pi*R)
double wheelCircumference = 2 * PI * WHEEL_RADIUS;

// calculate the distance per step
double distancePerStep = wheelCircumference / STEPS_PER_REV;

// variable to hold the total number of steps
long totalSteps = 0;

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

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  pinMode(stepPinLeft, OUTPUT);
  pinMode(dirPinLeft, OUTPUT);
  pinMode(stepPinRight, OUTPUT);
  pinMode(dirPinRight, OUTPUT);

  // Set up the stepper motors
  // Set the maximum speed in steps per second
  stepper1.setMaxSpeed(LIMIT); 
  stepper2.setMaxSpeed(LIMIT);

}

void loop() {

  // Calculate 2 readings based on the readings from MPU6050.
  // 2 readings: [x, alpha]

  // Get acceleration, gyroscope, temperature from MPU
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  currTime = millis();
  loopTime = currTime - prevTime;
  prevTime = currTime;

  gyroY = g.gyro.y;
  gyroRate = map(gyroY, -32768, 32767, -250, 250);
  gyroAngle = gyroAngle + (float)gyroRate*loopTime/1000;

  Xacc = a.acceleration.x;
  Zacc = a.acceleration.z;
  accangle = atan2(Xacc,Zacc)*RAD_TO_DEG;

  curangle = -(mu * (preangle + gyroAngle) + (1 - mu) * (accangle)); // complimentary filter

  // Set the target position:
    stepper1.moveTo(5);
    stepper2.moveTo(5);
    
  // Step the motor with a constant speed as set by setMaxSpeed():
    while (stepper1.distanceToGo() != 0) {
        stepper1.run();
    }
  
    // Calculate the displacement
    float displacement = stepper1.currentPosition() * DISTANCE_PER_STEP;

  // calculate the total displacement
  double x = totalSteps * distancePerStep;
  alpha = curangle;
  
  y = {x, alpha};

  calculation(y);
  
 //////////////////////////////////////////////////////////////////////////////////////////////////
 /*
  // map the desired torque to a PWM value
  int pwm_value = (int) (u * MAX_PWM / MAX_TORQUE);

  // constrain the PWM value to the range [0, MAX_PWM] to prevent invalid inputs
  pwm_value = constrain(pwm_value, 0, MAX_PWM);

  // send the PWM signal to the motor driver
  analogWrite(MOTOR_PIN, pwm_value);
  */
  delay(100);
}

bool matricesAreEqual(BLA::Matrix<2, 1> a, BLA::Matrix<2, 1> b)
{
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 1; ++j)
        {
            if (a(i,j) != b(i,j))
            {
                return false;
            }
        }
    }
    return true;
}

void calculation(Matrix<2, 1> y) {
  while(!matricesAreEqual(y_xi, y_d)){
    // difference between output value measured by sensor and the output of model
    y_xi = C_d * x_i_next - y;
    
    // calculate u
    computeControlInput(y_d);
    
    // Calculate next states
    x_i_next = A_d * x_i + B_d * u + L * y_xi;

  }
}


