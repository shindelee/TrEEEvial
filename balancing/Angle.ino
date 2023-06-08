#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>

MPU6050 mpu;

int16_t gyroX, gyroY, gyroRate;
float gyroAngle=0;
float alpha=0;
unsigned long currTime, prevTime=0, loopTime;

void setup() {  
  mpu.initialize();
  Serial.begin(9600);
}

void loop() {
  currTime = millis();
  loopTime = currTime - prevTime;
  prevTime = currTime;
  
  gyroX = mpu.getRotationX();
  gyroRate = map(gyroX, -32768, 32767, -250, 250);
  gyroAngle = gyroAngle + (float)gyroRate*loopTime/1000;

  gyroY = mpu.getRotationY();
  gyroRate = map(gyroY, -32768, 32767, -250, 250);
  alpha = alpha + (float)gyroRate*loopTime/1000;
  
  Serial.println(gyroAngle);
  Serial.println(alpha);
}