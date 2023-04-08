#Trash Monitoring System

##Sistem Monitoring Sampah menggunakan Arduino Uno
##Deskripsi

Sistem ini dirancang untuk memonitoring tingkat ketinggian sampah, kadar gas metana, dan kelembapan udara dalam tong sampah. Sistem ini dibangun menggunakan Arduino Uno, sensor Ultrasonic, sensor MQ-4, dan sensor DHT22.
##Fitur

    Memantau ketinggian sampah
    Memantau kadar gas metana dalam tong sampah
    Memantau kelembapan udara dalam tong sampah
    Memberikan peringatan jika sampah sudah penuh atau berpotensi menyebabkan penyakit

##Requirements

    Arduino Uno
    Sensor Ultrasonic HC-SR04
    Sensor MQ-4
    Sensor DHT22
    LED dan Buzzer
    RTC Module
    Breadboard
    Jumper Wires
    Power supply

##Cara Kerja

    Sensor Ultrasonic digunakan untuk memantau tingkat ketinggian sampah pada tong sampah
    Sensor MQ-4 digunakan untuk memantau kadar gas metana dalam tong sampah
    Sensor DHT22 digunakan untuk memantau kelembapan udara dalam tong sampah
    RTC Module digunakan untuk memantau waktu dan tanggal
    Data yang dikumpulkan oleh sensor akan ditampilkan pada Serial Monitor
    Jika tingkat ketinggian sampah sudah mencapai batas maksimal, LED dan Buzzer akan menyala sebagai peringatan

##Cara Instalasi

    Hubungkan Arduino Uno dengan komputer
    Buka file program (.ino) menggunakan Arduino IDE
    Upload program ke Arduino Uno
    Sistem siap digunakan