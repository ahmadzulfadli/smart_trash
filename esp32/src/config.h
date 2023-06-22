#ifndef SMART_TRASH_CONFIG_H
#define SMART_TRASH_CONFIG_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>

#include <DHT.h>
#include "MQ-Sensor-SOLDERED.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <UrlEncode.h>
#include <time.h>

// NETWORK
const char *ssid = "Raden Mas Wifi";
const char *password = "bebaspakai";

// NTP_SET_TIME
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 6 * 3600; // Offset WIB: GMT+7
const int daylightOffset_sec = 3600;

// WHATSAPP
String phoneNumber = "6289623426138";
String apiKey = "5981469";

int jum_pesan1, jum_pesan2, jum_pesan3, jum_pesan4;

// DHT 22
#define DHTPIN 23 // D23
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// MQ4
#define RatioMQ135CleanAir 3.6 // RS / R0 = 3.6 ppm
#define SENSOR_ANALOG_PIN 34   // D34
MQ2 mq135(SENSOR_ANALOG_PIN);

// ULTRASONIC
#define echoPin 18 // D18
#define trigPin 5  // D5
long duration, distance;

// LED AND BUZZER
#define led 2   // D2
#define buzz 15 // D15

// LCD 20x4
#define I2C_Freq 100000
#define I2C_SDA 21
#define I2C_SCL 22
LiquidCrystal_I2C lcd(0x27, 20, 4); // SDA, SCL PIN 21, 22

int kolom = 20;
int baris = 4;

// ALARM
int alaram1_jam = 6;
int alaram1_menit = 30;

int alaram2_jam = 20;
int alaram2_menit = 30;

#endif // SMART_TRASH_CONFIG_H