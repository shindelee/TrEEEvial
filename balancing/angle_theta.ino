#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;
float Xacc, Yacc, Zacc, gyroZ, gyroY, gyroX, gyroRate;
float theta, alpha;
double mu = 0.993;

//#define RAD_TO_DEG 57.2957795131 // This equals 180/PI, the conversion factor from radians to degrees
#define dt 0.01 // This is the time interval between readings (in seconds) - for a 100Hz update rate
float accangle, curangle, preangle = 0, gyroAngle = 0;
unsigned long currTime, prevTime = 0, loopTime;

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
}

void loop() {
  /* Get new sensor events with the readings */
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
  preangle = curangle; // update preangle for the next iteration

  // Adjust the angle to the range [0, 360]
  if (curangle < 0) {
    curangle = (curangle + 360.0); 
  }
  curangle = fmod(curangle, 360.0);

  Serial.println(curangle); 
  
}