#ifndef SMART_TRASH_CONFIG_H
#define SMART_TRASH_CONFIG_H

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <Wire.h>

#include "RTClib.h"
#include <DHT.h>
#include "MQ-Sensor-SOLDERED.h"

#include <LiquidCrystal_I2C.h>

// DHT 22
#define DHTPIN 10
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// RTC
RTC_DS1307 rtc;
char namaHari[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};

// MQ4
#define RatioMQ135CleanAir 3.6 // RS / R0 = 3.6 ppm
#define SENSOR_ANALOG_PIN A3
MQ2 mq135(SENSOR_ANALOG_PIN);

// ULTRASONIC
#define echoPin 8
#define trigPin 9
long duration, distance;

// LED AND BUZZER
#define led 6
#define buzz 7

// LCD 20x4
LiquidCrystal_I2C lcd(0x27, 20, 4);

#endif