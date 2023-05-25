#include "config.h"

int kolom = 20;
int baris = 4;

void TulisanBergerak(int row, String message, int delayTime, int kolom)
{
    for (int i = 0; i < kolom; i++)
    {
        message = " " + message;
    }
    message = message + " ";
    for (int pos = 0; pos < message.length(); pos++)
    {
        lcd.setCursor(0, row);
        lcd.print(message.substring(pos, pos + kolom));
        delay(delayTime);
    }
}

void buzzer_berbunyi()
{
    digitalWrite(buzz, HIGH);
    delay(1000);
    digitalWrite(buzz, LOW);
    delay(1000);
}

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
    // RTC
    if (!rtc.isrunning())
    {
        Serial.println("RTC is NOT running!");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // update rtc dari waktu komputer
    }
    // MQ4
    // Set math model to calculate the PPM concentration and the value of constants
    mq4.setRegressionMethod(1); //_PPM =  a*ratio^b
    mq4.setA(1012.7);
    mq4.setB(-2.786); // Configurate the ecuation values to get CH4 concentration
    /*
      Exponential regression:
    Gas    | a      | b
    LPG    | 3811.9 | -3.113
    CH4    | 1012.7 | -2.786
    CO     | 200000000000000 | -19.05
    Alcohol| 60000000000 | -14.01
    smoke  | 30000000 | -8.308
    */

    /*****************************  MQ Init ********************************************/
    // Remarks: Configure the pin of arduino as input.
    /************************************************************************************/
    mq4.begin();
    Serial.print("Calibrating please wait.");
    float calcR0 = 0;
    for (int i = 1; i <= 10; i++)
    {
        mq4.update(); // Update data, the arduino will be read the voltage on the analog pin
        calcR0 += mq4.calibrate(RatioMQ4CleanAir);
        Serial.print(".");
    }
    mq4.setR0(calcR0 / 10);
    Serial.println("  done!.");

    if (isinf(calcR0))
    {
        Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your "
                       "wiring and supply");
        while (1)
            ;
    }
    if (calcR0 == 0)
    {
        Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please "
                       "check your wiring and supply");
        while (1)
            ;
    }

    mq4.serialDebug(true);

    // ULTRASONIC
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // LED AND BUZZER
    pinMode(led, OUTPUT);
    pinMode(buzz, OUTPUT);

    // LCD 20x4
    lcd.init();
    lcd.backlight();
}

void loop()
{
    // RTC
    DateTime now = rtc.now();

    // DHT22
    float temp = dht.readTemperature();
    float humd = dht.readHumidity();

    // MQ4
    mq4.update();
    float ppmCH4 = mq4.readSensor();

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
    if (distance > 35)
    {
        distance = 35;
    }
    else
    {
        distance = distance;
    }

    Serial.print("Jarak: ");
    Serial.print(distance);
    Serial.println(" cm");

    unsigned int capasity = map(distance, 35, 2, 0, 100);

    // STATUS SMART TRASH
    int status_sampah, status_humd, status_ppm;

    if (capasity > 95)
    {
        status_sampah = 1;
    }
    else
    {
        status_sampah = 0;
    }

    // kelembapan udara dalam tong sampah
    if (humd > 90)
    {
        status_humd = 1;
    }
    else
    {
        status_humd = 0;
    }

    // kadar gas metana dalam tong sampah
    if (ppmCH4 > 500)
    {
        status_ppm = 1;
    }
    else
    {
        status_ppm = 0;
    }

    // SHOW TITLE TO LCD
    lcd.setCursor(0, 0);
    lcd.print("SMART TRASH");

    // SHOW TIME TO LCD
    lcd.setCursor(14, 0);
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    lcd.print(now.minute(), DEC);

    // SHOW TEMPERATURE TO LCD
    lcd.setCursor(0, 1);
    lcd.print("T:");
    lcd.print(temp);
    lcd.print("C");

    // SHOW HUMIDITY TO LCD 20x4
    lcd.setCursor(10, 1);
    lcd.print("H:");
    lcd.print(humd);
    lcd.print("%");

    // SHOE CAPASITY TO LCD 20x4
    lcd.setCursor(0, 2);
    lcd.print("C:");
    lcd.print(capasity);
    lcd.print("%");

    // SHOW PPM TO LCD 20x4
    lcd.setCursor(10, 2);
    lcd.print("PPM:");
    lcd.print(ppmCH4);
    Serial.print("PPM:");
    Serial.println(ppmCH4);

    // SHOW STATUS TEMPERATURE, HUMADITI, CAPASITY AND PPM TO LCD 20x4

    int alaram1_jam = 6;
    int alaram1_menit = 30;

    int alaram2_jam = 20;
    int alaram2_menit = 30;

    if (status_sampah == 1)
    {
        TulisanBergerak(3, "Sampah Penuh Berpotensi Penyakit", 500, kolom);

        digitalWrite(led, HIGH);

        if (now.hour() == alaram1_jam and now.minute() == alaram1_menit or now.hour() == alaram2_jam and now.minute() == alaram2_menit)
        {
            // maka bunyikan buzzer
            buzzer_berbunyi();
        }
    }
    else if (status_humd == 1)
    {
        TulisanBergerak(3, "Sampah Basah Berpotensi Penyakit", 500, kolom);

        digitalWrite(led, HIGH);

        if (now.hour() == alaram1_jam and now.minute() == alaram1_menit or now.hour() == alaram2_jam and now.minute() == alaram2_menit)
        {
            // maka bunyikan buzzer
            buzzer_berbunyi();
        }
    }
    else if (status_ppm == 1)
    {
        TulisanBergerak(3, "Sampah Busuk Berpotensi Penyakit", 500, kolom);

        digitalWrite(led, HIGH);

        if (now.hour() == alaram1_jam and now.minute() == alaram1_menit or now.hour() == alaram2_jam and now.minute() == alaram2_menit)
        {
            // maka bunyikan buzzer
            buzzer_berbunyi();
        }
    }
    else
    {
        digitalWrite(led, LOW);
        TulisanBergerak(3, "C : Belum Penuh H : Normal PPM : Normal", 500, kolom);
    }

    delay(1000);
    lcd.clear();
}