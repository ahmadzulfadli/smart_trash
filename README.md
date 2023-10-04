# Smart Trash

## Smart Trash menggunakan ESP32

## Deskripsi

Sistem ini dirancang untuk memonitoring tingkat ketinggian sampah, kadar gas amonia dan kelembapan udara dalam tong sampah. Sistem ini dirancang menggunakan ESP32, LCD 20X4, sensor Ultrasonic, sensor MQ-135, dan sensor DHT22.

## Library yang Dibutuhkan

- DHT.h
- MQUnifiedsensor.h
- LiquidCrystal_I2C.h

## Fitur

- Memantau ketinggian sampah
- Memantau kadar gas metana dalam tong sampah
- Memantau kelembapan udara dalam tong sampah
- Memberikan peringatan jika sampah sudah penuh atau berpotensi menyebabkan penyakit

## Requirements

- ESP32
- LCD 20X4
- Sensor Ultrasonic HC-SR04
- Sensor MQ-135
- Sensor DHT22
- LED dan Buzzer
- Breadboard
- Jumper Wires
- Power supply

## Cara Kerja

- Sensor Ultrasonic digunakan untuk memantau tingkat ketinggian sampah pada tong sampah
- Sensor MQ-135 digunakan untuk memantau kadar Gas Amonia dalam tong sampah
- Sensor DHT22 digunakan untuk memantau kelembapan udara dalam tong sampah
- RTC Module digunakan untuk memantau waktu dan tanggal
- Data yang dikumpulkan oleh sensor akan ditampilkan pada LCD 20X4
- Jika Gas Amonia tinggi atau kelembapan sampah sudah mencapai nilai threshold atau tingkat ketinggian sampah sudah mencapai batas maksimal, maka LED dan Buzzer akan menyala sebagai peringatan
