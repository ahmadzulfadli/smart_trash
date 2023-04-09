#include "config.h"

void setup()
{

    Serial.begin(9600);

    // DHT
    dht.begin();

    // RTC
    if (!rtc.begin())
    {
        Serial.println("RTC TIDAK TERBACA");
        while (1)
            ;
    }

    // MQ4
    MQ4.setRegressionMethod(1); //_PPM =  a*ratio^b
    MQ4.setA(1012.7);
    MQ4.setB(-2.786);
    MQ4.init();
    Serial.print("Calibrating please wait.");
    float calcR0 = 0;
    for (int i = 1; i <= 10; i++)
    {
        MQ4.update(); // Update data, the arduino will read the voltage from the analog pin
        calcR0 += MQ4.calibrate(RatioMQ4CleanAir);
        Serial.print(".");
    }
    MQ4.setR0(calcR0 / 10);
    Serial.println("  done!.");

    if (isinf(calcR0))
    {
        Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply");
        while (1)
            ;
    }
    if (calcR0 == 0)
    {
        Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply");
        while (1)
            ;
    }
    MQ4.serialDebug(false);

    // RTC
    if (!rtc.isrunning())
    {
        Serial.println("RTC is NOT running!");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // update rtc dari waktu komputer
    }

    // ULTRASONIC
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // LED DAN BUZZER
    pinMode(led, OUTPUT);
    pinMode(buzz, OUTPUT);

    // LCD
}

void loop()
{
    // RTC
    DateTime now = rtc.now();

    // DHT22
    float temp = dht.readTemperature();
    float humd = dht.readHumidity();

    // MQ4
    MQ4.update();
    float ppmCH4 = MQ4.readSensor();

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

    unsigned int capasity = map(distance, 40, 2, 0, 100);

    if (capasity > 95)
    {
        Serial.println("Sampah Penuh Berpotensi Penyakit");
    }

    // kelembapan udara dalam tong sampah
    if (humd > 85)
    {
        Serial.println("Sampah Lembab Berpotensi Penyakit");
    }

    // kadar gas metana dalam tong sampah
    if (ppmCH4 > 500)
    {
        Serial.println("Sampah Busuk Berpotensi Penyakit");
    }

    //==============================================

    // MENAMPILKAN HASIL KE SERIAL MONITOR
    Serial.print("Waktu: ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.println("");

    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println("");

    Serial.print("Humidity: ");
    Serial.print(humd);
    Serial.println("");

    Serial.print("capasity : ");
    Serial.print(capasity);
    Serial.println("");

    Serial.print("Gas Metana: ");
    Serial.print(ppmCH4);
    Serial.println("");
    Serial.println("");

    // MENAMPILKAN HASIL KE LCD

    delay(1000);
}