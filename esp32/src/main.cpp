#include "config.h"
#include "fungsi.h"

void setup()
{
    Serial.begin(115200);

    // LCD 20x4
    Wire.begin(I2C_SDA, I2C_SCL); // Needed for CTP and LTR329

    lcd.init();
    lcd.backlight();

    // WIFI
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");

        lcd.setCursor(0, 0);
        lcd.print("Mencari Jaringan ...");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");

    // Init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    // DHT
    dht.begin();

    // MQ135
    // Set math model to calculate the PPM concentration and the value of constants
    mq135.setRegressionMethod(1); //_PPM =  a*ratio^b
    mq135.setA(102.2);
    mq135.setB(-2.473); // Configurate the ecuation values to get NH4 concentration

    Serial.print("Calibrating please wait.");
    float calcR0 = 0;
    for (int i = 1; i <= 10; i++)
    {
        mq135.update(); // Update data, the arduino will be read the voltage on the analog pin
        calcR0 += mq135.calibrate(RatioMQ135CleanAir);
        Serial.print(".");
    }
    mq135.setR0(calcR0 / 10);
    Serial.println("  done!.");

    if (isinf(calcR0))
    {
        lcd.setCursor(0, 0);
        lcd.print("Koneksi MQ4 ...");
        Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your "
                       "wiring and supply");
        while (1)
            ;
    }
    if (calcR0 == 0)
    {
        lcd.setCursor(0, 0);
        lcd.print("Koneksi MQ4 ...");
        Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please "
                       "check your wiring and supply");
        while (1)
            ;
    }
    /*****************************  MQ CAlibration ********************************************/
    mq135.serialDebug(true);

    // ULTRASONIC
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // LED AND BUZZER
    pinMode(led, OUTPUT);
    pinMode(buzz, OUTPUT);

    // SEND MESSAGE
    sendMessage("Mr. Bin is ready to use");

    lcd.clear();
}

void loop()
{
    // millis
    if ((millis() - lastTime) > timerDelay)
    {
        // GET TIME
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo))
        {
            Serial.println("Failed to obtain time");
            return;
        }

        int hour = timeinfo.tm_hour;
        int minute = timeinfo.tm_min;
        int second = timeinfo.tm_sec;

        // baca sensor
        float capasity = 0;
        float humd = 0;
        float ppmnh4 = 0;

        baca_sensor(capasity, humd, ppmnh4);

        // konversi variabel ke string
        String str_capasity = String(capasity);
        String str_humd = String(humd);
        String str_ppmnh4 = String(ppmnh4);
        String pesan = "Hallo, ini Mr. Bin\n";
        pesan += "Kapasitas: " + str_capasity + "%\nKelembaban: " + str_humd + "%\nGas Amonia: " + str_ppmnh4 + " ppm\n";

        if (capasity > 90) // kapasitas dalam tong sampah
        {
            // SEND MESSAGE TO WA
            if (jum_pesan1 < 1)
            {
                pesan += "Sampah Penuh, Segera Buang Sampah";
                sendMessage(pesan);
            }
            jum_pesan1++;

            digitalWrite(led, HIGH);
            alaram(hour, minute, second);
            TulisanBergerak(0, "Sampah Penuh, Segera Buang Sampah", 500, kolom);
        }
        else if (capasity > 70) // kapasitas dalam tong sampah
        {
            // SEND MESSAGE TO WA
            if (jum_pesan2 < 1)
            {
                pesan += "Sampah Hampir Penuh, Segera Buang Sampah";
                sendMessage(pesan);
            }
            jum_pesan2++;

            digitalWrite(led, HIGH);

            TulisanBergerak(0, "Sampah Hampir Penuh, Segera Buang Sampah", 500, kolom);
        }
        else if (humd > 85) // kelembapan udara dalam tong sampah
        {
            // SEND MESSAGE TO WA
            if (jum_pesan3 < 1)
            {
                pesan += "Sampah Basah, Segera Buang Sampah";
                sendMessage(pesan);
            }
            jum_pesan3++;

            digitalWrite(led, HIGH);

            alaram(hour, minute, second);
            TulisanBergerak(0, "Sampah Basah, Segera Buang Sampah", 500, kolom);
        }
        else if (ppmnh4 > 10) // kadar gas metana dalam tong sampah
        {
            // SEND MESSAGE TO WA
            if (jum_pesan4 < 1)
            {
                pesan += "Sampah Busuk, Segera Buang Sampah";
                sendMessage(pesan);
            }
            jum_pesan4++;

            digitalWrite(led, HIGH);

            alaram(hour, minute, second);
            TulisanBergerak(0, "Sampah Busuk, Segera Buang Sampah", 500, kolom);
        }
        else // kapasitas dalam tong sampah normal
        {
            // SHOW TITLE TO LCD
            lcd.setCursor(0, 0);
            lcd.print("MR. BIN");

            // SHOW TIME TO SERIAL MONITOR
            Serial.print("Jam: ");
            Serial.print(hour);
            Serial.print(":");
            Serial.print(minute);
            Serial.print(":");
            Serial.println(second);

            // SHOW TIME TO LCD
            lcd.setCursor(12, 0);
            lcd.print(hour);
            lcd.print(":");
            lcd.print(minute);
            lcd.print(":");
            lcd.print(second);

            digitalWrite(led, LOW);
            jum_pesan1 = 0;
            jum_pesan2 = 0;
            jum_pesan3 = 0;
            jum_pesan4 = 0;
        }

        // kirim data ke webserver
        kirim_data(capasity, humd, ppmnh4);

        lastTime = millis();
    }
}