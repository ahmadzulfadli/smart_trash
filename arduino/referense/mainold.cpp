#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_I2CDevice.h>
#include <SPI.h>
#include <Wire.h>

// ULTRASONIC
#define echoPin 8
#define trigPin 9
long duration, distance;

void setup()
{

  Serial.begin(9600);

  // ULTRASONIC
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // LCD
}

void loop()
{
  // ULTRASONIC
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);

  // perhitungan untuk dijadikan jarak
  distance = duration / 58.2;

  // LOGIKA SISTEM================================
  // ketinggian sampah

  if (distance > 40)
  {
    distance = 40;
  }
  else
  {
    distance = distance;
  }
  Serial.print("jarak : ");
  Serial.print(distance);
  Serial.println("");

  unsigned int capasity = map(distance, 40, 2, 0, 100);

  Serial.print("capasity : ");
  Serial.print(capasity);
  Serial.println("");

  if (distance > 95)
  {
    Serial.println("Sampah Penuh");
  }

  //==============================================

  // MENAMPILKAN HASIL KE SERIAL MONITOR

  // MENAMPILKAN HASIL KE LCD
  delay(1000);
}