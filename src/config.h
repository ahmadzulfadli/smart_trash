#ifndef SMART_TRASH_CONFIG_H
#define SMART_TRASH_CONFIG_H

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <Wire.h>

#include "RTClib.h"
#include <DHT.h>
#include <MQUnifiedsensor.h>

// DHT 22
#define DHTPIN 10
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// RTC
RTC_DS1307 rtc;
char namaHari[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};

// MQ4
#define Board ("Arduino UNO")
#define Pin (A3)
#define Type ("MQ-4")
#define Voltage_Resolution (5)
#define ADC_Bit_Resolution (10)
#define RatioMQ4CleanAir (8) // RS / R0 = 60 ppm

MQUnifiedsensor MQ4(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);

// ULTRASONIC
#define echoPin 8
#define trigPin 9
long duration, distance;

// LED DAN BUZZER
#define led 6
#define buzz 7

#endif