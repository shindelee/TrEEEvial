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

#define dirPinRight 27    // right motor driver
#define stepPinRight 26   // left motor driver

const float MAX_TORQUE = 1.0;  // set this to the maximum torque your motor can deliver

#define TIMER_INTERVAL_MS 1000
#define STEPS_PER_REV 200
#define WHEEL_RADIUS 0.0325

#define SPEED_LIMIT 400
#define ACC_LIMIT 500 // (example)
#define TORQUE_LIMIT 15

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
  
  // steps/second^2
  // u1 = the linear acceleration in m/s²
  
  // Convert linear acceleration to angular acceleration
  float au1 = u1 / WHEEL_RADIUS;

  // Convert angular acceleration to steps
  float sa1 = au1 * STEPS_PER_REV / (2 * PI);

  // Convert linear acceleration to angular acceleration
  float au2 = u2 / WHEEL_RADIUS;

  // Convert angular acceleration to steps
  float sa2 = au2 * STEPS_PER_REV / (2 * PI);
  
  stepper1.setAcceleration(sa1);
  stepper2.setAcceleration(sa2);

  Serial.println(String("sa1: ") + sa1);
  Serial.println(String("sa2: ") + sa2);
  
  stepper1.run();
  stepper2.run();
}

// Control Law Matrices

// Matrix<2, 1> u; // Control input: Torque
Matrix<2, 1> y_xi = {0,
                     0
                    };

// Control gain
Matrix<2, 6> K = {-17.1934,  -37.6384,    0.2753,    0.1277,  -57.7937,  -59.4161,
                  -16.8092,  -37.0033,   -0.2623,   -0.1147,  -56.8375,  -58.4360
                 };

Matrix<6, 2> L = {0.7427,    0.0363,
                  1.4515,    0.1190,
                  0.0016,    0.0547,
                 -0.0040,    0.0984,
                 55.4731,    4.0965,
                 52.7943,    3.8684
                 };

// System matrix
Matrix<6, 6> A_d = {1.0000,    0.0736,         0,         0,    0.0000,    0.0009,
                         0,    0.5237,         0,         0,    0.0008,    0.0155,
                         0,         0,    1.0000,    0.0841,         0,         0,
                         0,         0,         0,    0.7001,         0,         0,
                         0,    0.0180,         0,         0,    1.0047,    0.0996,
                         0,    0.3236,         0,         0,    0.0931,    0.9941
                   };

// Input matrix
Matrix<6, 2> B_d = {0.0043,    0.0043,
                    0.0774,    0.0774,
                    0.0369,   -0.0369,
                    0.6963,   -0.6963,
                   -0.0029,   -0.0029,
                   -0.0526,   -0.0526
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
Matrix<6, 1> x1 = {0, 
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

Matrix<2, 1> computeControlInput(Matrix<2, 1> y_d, Matrix<6, 1> x) {
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

  Matrix<6, 6> temp1 = I - (A_d - (B_d * K)); // Compute (I - (A_d - B_d * K))
  //Serial.println(String("temp1: ") + temp1(2,2) + temp1(3,3)); // correct

  // Invert the matrix
  BLA::Matrix<6,6> temp1_inv;
  Invert(temp1, temp1_inv);
    
  Matrix<2, 2> temp2 = C_d * temp1_inv * B_d; // Compute (C_d / (I - (A_d - B_d * K)) * B_d)
  //Serial.println(String("temp2: ") + temp2(0,0) + temp2(0,1) +temp2(1,0) + temp2(1,1));
    
  BLA::Matrix<2,2> temp2_inv;
  Invert(temp2, temp2_inv);
  u = -K * x + temp2_inv * y_d; // Compute -K * x_i + ((C_d / (I - (A_d - B_d * K)) * B_d) \ y_d)

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

  // stepper1.setAcceleration(ACC_LIMIT); 
  // stepper2.setAcceleration(ACC_LIMIT);

  //stepper1.moveTo(4897); // 5m
  //stepper2.moveTo(4897); // 5m
}

void loop() {
  static double x = 0;
  float alpha = 0;
  
  //double deltaTime = 0.01; // adjust as needed, this is 10 milliseconds
  //delay(deltaTime * 1000); // delay in milliseconds

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
  
  // state observer
  y = {x, alpha};
  Serial.println(String("y: ") + x + alpha);

  // calculation
  Serial.println(String("difference in first ele: ") + abs(y_xi(0, 0) - y_d(0, 0)));
  //if((abs(y_xi(0, 0) - y_d(0, 0)) >= 2) || (abs(y_xi(1, 0) - y_d(1, 0)) >= 0.5)){

    Serial.println(String("x_i: ") + x_i(0,0) + x_i(1,0));
    
    // difference between output value measured by sensor and the output of model
    y_xi = C_d * x_i - y;
    Serial.println(String("y_xi: ") + y_xi(0,0) + y_xi(1,0));

    // calculate u
    Matrix<2, 1> u = computeControlInput(y_d, x_i);

    // input torque constraints 
    if(abs(u(0, 0)) > TORQUE_LIMIT){
      if(u(0, 0) > 0){
        u(0, 0) = TORQUE_LIMIT;
      }
        u(0, 0) = -TORQUE_LIMIT;
    }

    if(abs(u(1, 0)) > TORQUE_LIMIT){
      if(u(1, 0) > 0){
        u(1, 0) = TORQUE_LIMIT;
      }
        u(1, 0) = -TORQUE_LIMIT;
    }
    
    float output1 = u(0, 0); // first element
    float output2 = u(1, 0); // second element

    Serial.println(String("torque1: ") + u(0, 0));
    Serial.println(String("torque2: ") + u(1, 0));

    // convert to linear acceleration
    float a1 = (output1 / momentOfInertia) * WHEEL_RADIUS;
    float a2 = (output2 / momentOfInertia) * WHEEL_RADIUS;
  
    
    long initialPosition = stepper1.currentPosition(); // get the initial position of the motor
    Serial.println(String("initialPosition: ") + initialPosition);
    
    motorRun(a1, a2);

    long finalPosition = stepper1.currentPosition(); // get the final position of the motor
    Serial.println(String("finalPosition: ") + finalPosition);
    
    long angularDisplacement = finalPosition - initialPosition;
    double linearDisplacement = angularDisplacement * distancePerStep;

    x += linearDisplacement;
    

    /*
    // calculate the change in angular velocity
    double deltaAngularVelocity = a1 * deltaTime;

    // calculate the final angular velocity
    double finalAngularVelocity = initialAngularVelocity + deltaAngularVelocity;

    // calculate the total angle rotated
    double deltaTheta = finalAngularVelocity * deltaTime;

    // calculate the total distance travelled
    double distanceTravelled = WHEEL_RADIUS * deltaTheta;

    x += distanceTravelled;
    */
    Serial.println(String("Distance Travelled: ") + x);

    // Calculate next states
    x_i_next = A_d * x_i + B_d * u + L * y_xi;
    // Matrix <6, 1> x_next = A_d * x_i + B_d * u; 
    // Serial.println(String("x1: ") + x1(0,0) + x1(1,0));
    
    Serial.println(String("x_i_next: ") + x_i_next(0,0) + x_i_next(1,0));

    x_i = x_i_next;

  //}
 
}