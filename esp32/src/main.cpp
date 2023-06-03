#include "config.h"

void sendMessage(String message)
{

    // Data to send with HTTP POST
    String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);
    HTTPClient http;
    http.begin(url);

    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Send HTTP POST request
    int httpResponseCode = http.POST(url);
    if (httpResponseCode == 200)
    {
        Serial.print("Message sent successfully");
    }
    else
    {
        Serial.println("Error sending the message");
        Serial.print("HTTP response code: ");
        Serial.println(httpResponseCode);
    }

    // Free resources
    http.end();
}

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
    Serial.begin(115200);

    // LCD 20x4
    // I2C_0.begin(I2C_SDA, I2C_SCL, I2C_Freq);
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
        status_sampah = 0;
    }
    else
    {
        status_sampah = 0;
    }

    // kelembapan udara dalam tong sampah
    if (humd > 85)
    {
        status_humd = 0;
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
        TulisanBergerak(0, "Sampah Penuh, Segera Buang Sampah", 500, kolom);

        digitalWrite(led, HIGH);

        if (hour == alaram1_jam and minute == alaram1_menit or hour == alaram2_jam and minute == alaram2_menit)
        {
            // maka bunyikan buzzer
            buzzer_berbunyi();
        }
    }
    else if (status_humd == 1)
    {
        TulisanBergerak(0, "Sampah Basah, Segera Buang Sampah", 500, kolom);

        digitalWrite(led, HIGH);

        if (hour == alaram1_jam and minute == alaram1_menit or hour == alaram2_jam and minute == alaram2_menit)
        {
            // maka bunyikan buzzer
            buzzer_berbunyi();
        }
    }
    else if (status_ppm == 1)
    {
        TulisanBergerak(0, "Sampah Busuk, Segera Buang Sampah", 500, kolom);

        digitalWrite(led, HIGH);

        if (hour == alaram1_jam and minute == alaram1_menit or hour == alaram2_jam and minute == alaram2_menit)
        {
            // maka bunyikan buzzer
            buzzer_berbunyi();
        }
    }
    else
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
    }

    delay(3000);
    lcd.clear();
}