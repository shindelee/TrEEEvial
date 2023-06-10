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

#define dirPinRight 27    // right motor driver
#define stepPinRight 26   // left motor driver

const float WHEEL_CIRCUMFERENCE = 0.204;  // circumference of the wheel in meters 
const float MAX_TORQUE = 1.0;  // set this to the maximum torque your motor can deliver

#define TIMER_INTERVAL_MS 1000
#define STEPS_PER_REV 200
#define WHEEL_RADIUS 0.0325

#define SPEED_LIMIT 400
#define ACC_LIMIT 500 // (example)

int stepperSpeed;
int16_t Xacc, Yacc, Zacc, gyroZ, gyroY, gyroX, gyroRate;
double mu = 0.993;

#define rad_to_deg 57.2957795131 // This equals 180/PI, the conversion factor from radians to degrees
#define dt 0.01 // This is the time interval between readings (in seconds) - for a 100Hz update rate
float accangle, curangle, preangle = 0, gyroAngle = 0;
unsigned long currTime, prevTime = 0, loopTime;

double initialAngularVelocity = 0.0;
double momentOfInertia = 0.01475;

AccelStepper stepper1(AccelStepper::DRIVER, stepPinLeft, dirPinLeft); // Create an instance of AccelStepper for left motor
AccelStepper stepper2(AccelStepper::DRIVER, stepPinRight, dirPinRight); // Create an instance of AccelStepper for right motor

// calculate the wheel circumference (2*Pi*R)
double wheelCircumference = 2 * PI * WHEEL_RADIUS;

// calculate the distance per step
double distancePerStep = wheelCircumference / STEPS_PER_REV;

// variable to hold the total number of steps
long totalSteps = 0;

void motorRun(float u1, float u2){

  stepper1.setAcceleration(u1);
  stepper2.setAcceleration(u2);

  Serial.println(String("u1: ") + u1);
  Serial.println(String("u2: ") + u2);
  
  stepper1.run();
  stepper2.run();
}

// Control Law Matrices

// Matrix<2, 1> u; // Control input: Torque
Matrix<2, 1> y_xi = {0,
                     0
                    };

// Control gain
Matrix<2, 6> K = {-16.8410,  -36.9115,    0.2766,    0.1289,  -54.2005,  -55.1613,
                  -16.3738,  -36.1393,   -0.2610,   -0.1134,  -53.0907,  -54.0331
                 };

Matrix<6, 2> L = {0.7419,    0.0398,
                  1.4520,    0.1303,
                  0.0018,    0.0548,
                  -0.0044,    0.0982,
                  55.3496,    4.4709,
                  53.3626,    4.2821
                 };

// System matrix
Matrix<6, 6> A_d = {1.0000,    0.0735,         0,         0,    0.0000,    0.0009,
                         0,    0.5234,         0,        0,    0.0007,    0.0155,
                         0,         0,    1.0000,    0.0841,         0,         0,
                         0,         0,         0,    0.7001,         0,         0,
                         0,    0.0190,         0,         0,    1.0048,    0.0995,
                         0,    0.3425,        0,         0,    0.0953,     0.9936
                   };

// Input matrix
Matrix<6, 2> B_d = {0.0043,    0.0043,
                    0.0775,    0.0775,
                    0.0369,   -0.0369,
                    0.6963,   -0.6963,
                   -0.0031,  -0.0031,
                   -0.0557,   -0.0557
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
                    0};  
                    
// Current output
Matrix<2, 1> y;

Matrix<2, 1> computeControlInput(Matrix<2, 1> y_d) {
  Matrix<2, 1> u = {0,
                    0
                   };
                   
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

  return u;
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

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  
  pinMode(stepPinLeft, OUTPUT);
  pinMode(dirPinLeft, OUTPUT);
  pinMode(stepPinRight, OUTPUT);
  pinMode(dirPinRight, OUTPUT);

  // Set up the stepper motors

  stepper1.setMaxSpeed(SPEED_LIMIT); 
  stepper2.setMaxSpeed(SPEED_LIMIT);

  stepper1.setAcceleration(ACC_LIMIT); 
  stepper2.setAcceleration(ACC_LIMIT);

  stepper1.moveTo(4897); // Move 10000 steps
  stepper2.moveTo(4897); // Move 10000 steps
}

void loop() {
  static double x = 0;
  float alpha = 0;
  
  double deltaTime = 0.01; // adjust as needed, this is 10 milliseconds
  delay(deltaTime * 1000); // delay in milliseconds

  // Calculate 2 readings based on the readings from MPU6050.
  // 2 readings: [x, alpha]
  /*
  // Get acceleration, gyroscope, temperature from MPU
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  currTime = millis();
  loopTime = currTime - prevTime;
  prevTime = currTime;

  gz = g.gyro.z;
  gyroRate = map(gx, -32768, 32767, -250, 250);
  gyroAngle = gyroAngle + (float)gyroRate*loopTime/1000;

  ax = a.acceleration.x;
  ay = a.acceleration.y;
  accangle = atan2(ay,ax)*RAD_TO_DEG;

  curangle = -(mu * (preangle + gyroAngle) + (1 - mu) * (accangle)); // complimentary filter
  // Serial.println(curangle);

  alpha = 0;
  
  
  stepper1.moveTo(5);
  stepper2.moveTo(5);

  if (stepper1.distanceToGo() == 0) {
    stepper1.moveTo(-stepper1.currentPosition()); // Move back to zero
  }

  Serial.println(stepper1.currentPosition());
  float revolutions = abs(stepper1.currentPosition()) / (float)stepsPerRevolution;
  Serial.println(revolutions);
  
  float displacement = wheelCircumference * revolutions;

  // calculate the total displacement
  // double x = totalSteps * distancePerStep;
  
  double x = displacement;
  // alpha = curangle;

  Serial.println(x);
  // Serial.println(alpha);
  */
  
  y = {x, alpha};
  Serial.println(String("y: ") + x + alpha);

  // calculation

  if((abs(y_xi(0, 0) - y_d(0, 0)) >= 1) || (abs(y_xi(1, 0) - y_d(1, 0)) >= 0.5)){
    Serial.println(String("difference in first ele: ") + abs(y_xi(0, 0) - y_d(0, 0)));
    // difference between output value measured by sensor and the output of model
    y_xi = C_d * x_i_next - y;
    Serial.println(String("y_xi: ") + y_xi(0,0) + y_xi(1,0));

    // calculate u
    Matrix<2, 1> u = computeControlInput(y_d);

    float output1 = u(0, 0); // first element
    float output2 = u(1, 0); // second element

    Serial.println(String("torque1: ") + u(0, 0));
    Serial.println(String("torque2: ") + u(1, 0));

    float a1 = output1 / momentOfInertia;
    float a2 = output2 / momentOfInertia;
    
    motorRun(a1, a2);

    // calculate the change in angular velocity
    double deltaAngularVelocity = a1 * deltaTime;

    // calculate the final angular velocity
    double finalAngularVelocity = initialAngularVelocity + deltaAngularVelocity;

    // calculate the total angle rotated
    double deltaTheta = finalAngularVelocity * deltaTime;

    // calculate the total distance travelled
    double distanceTravelled = WHEEL_RADIUS * deltaTheta;

    x += distanceTravelled;
    Serial.println(String("Distance Travelled: ") + x);

    // Calculate next states
    x_i_next = A_d * x_i + B_d * u + L * y_xi;
    Serial.println(String("x_i_next: ") + x_i_next(0,0) + x_i_next(1,0));

  }
 
}