#include "config.h"

int kolom = 20;
int baris = 4;

int alaram1_jam = 6;
int alaram1_menit = 30;

int alaram2_jam = 20;
int alaram2_menit = 30;

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
    if (!rtc.isrunning())
    {
        Serial.println("RTC is NOT running!");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // update rtc dari waktu komputer
    }
    // MQ135
    // Set math model to calculate the PPM concentration and the value of constants
    mq135.setRegressionMethod(1); //_PPM =  a*ratio^b
    mq135.setA(102.2);
    mq135.setB(-2.473); // Configurate the ecuation values to get NH4 concentration

    /*
      Exponential regression:
    GAS      | a      | b
    CO       | 605.18 | -3.937
    Alcohol  | 77.255 | -3.18
    CO2      | 110.47 | -2.862
    Tolueno  | 44.947 | -3.445
    NH4      | 102.2  | -2.473
    Acetona  | 34.668 | -3.369
    */

    /*****************************  MQ Init ********************************************/
    // Remarks: Configure the pin of arduino as input.
    /************************************************************************************/
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
    /*****************************  MQ CAlibration ********************************************/
    // mq135.serialDebug(true);

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

    // mq135
    mq135.update();
    float ppmnh4 = mq135.readSensor();

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
    if (humd > 85)
    {
        status_humd = 1;
    }
    else
    {
        status_humd = 0;
    }

    // kadar gas metana dalam tong sampah
    if (ppmnh4 > 20)
    {
        status_ppm = 1;
    }
    else
    {
        status_ppm = 0;
    }

    // SHOE CAPASITY TO LCD 20x4
    lcd.setCursor(0, 2);
    lcd.print("Kapasitas: ");
    lcd.print(capasity);
    lcd.print("%");

    // SHOW HUMIDITY TO LCD 20x4
    lcd.setCursor(0, 1);
    lcd.print("Kelembaban: ");
    lcd.print(humd);
    lcd.print("%");

    // SHOW PPM TO LCD 20x4
    lcd.setCursor(0, 3);
    lcd.print("Gas Amonia: ");
    lcd.print(ppmnh4);
    Serial.print("PPM:");
    Serial.println(ppmnh4);

    // SHOW STATUS TEMPERATURE, HUMADITI, CAPASITY AND PPM TO LCD 20x4

    if (status_sampah == 1)
    {
        TulisanBergerak(0, "Sampah Penuh Berpotensi Penyakit", 500, kolom);

        digitalWrite(led, HIGH);

        if (now.hour() == alaram1_jam and now.minute() == alaram1_menit or now.hour() == alaram2_jam and now.minute() == alaram2_menit)
        {
            // maka bunyikan buzzer
            buzzer_berbunyi();
        }
    }
    else if (status_humd == 1)
    {
        TulisanBergerak(0, "Sampah Basah Berpotensi Penyakit", 500, kolom);

        digitalWrite(led, HIGH);

        if (now.hour() == alaram1_jam and now.minute() == alaram1_menit or now.hour() == alaram2_jam and now.minute() == alaram2_menit)
        {
            // maka bunyikan buzzer
            buzzer_berbunyi();
        }
    }
    else if (status_ppm == 1)
    {
        TulisanBergerak(0, "Sampah Busuk Berpotensi Penyakit", 500, kolom);

        digitalWrite(led, HIGH);

        if (now.hour() == alaram1_jam and now.minute() == alaram1_menit or now.hour() == alaram2_jam and now.minute() == alaram2_menit)
        {
            // maka bunyikan buzzer
            buzzer_berbunyi();
        }
    }
    else
    {
        // SHOW TITLE TO LCD
        lcd.setCursor(0, 0);
        lcd.print("SMART TRASH");

        // SHOW TIME TO LCD
        lcd.setCursor(14, 0);
        lcd.print(now.hour(), DEC);
        lcd.print(':');
        lcd.print(now.minute(), DEC);
        digitalWrite(led, LOW);
    }

    delay(3000);
    lcd.clear();
}