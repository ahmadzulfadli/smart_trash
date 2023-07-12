#ifndef SMART_TRASH_FUNGSI_H
#define SMART_TRASH_FUNGSI_H
#include <config.h>

void baca_sensor(float &capasity, float &humd, float &ppmnh4)
{
    // DHT22
    humd = dht.readHumidity();

    // mq135
    mq135.update();
    ppmnh4 = mq135.readSensor();

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

    capasity = map(distance, 35, 2, 0, 100);

    // tampilkan data ke serial monitor
    Serial.print("Kapasitas: ");
    Serial.print(capasity);
    Serial.println("%");

    Serial.print("Kelembaban: ");
    Serial.print(humd);
    Serial.println("%");

    Serial.print("Gas Amonia: ");
    Serial.print(ppmnh4);
    Serial.println(" ppm");

    // dispaly
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
    lcd.print(" ppm");
}

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

// kirim data ke web server
/* void kirim_data(float capasity, float humd, float ppmnh4)
{
    //--------------------------------------------
    // Send data to server
    WiFiClient client;
    if (!client.connect(host, port))
    {
        Serial.println("Connection failed");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Connection failed");
        delay(1000);
        return;
    }

    // pengiriman nilai sensor ke web server
    // windows
    String apiUrl = "http://localhost/monitoring_trash/crud/kirim_data.php?";
    // linux
    // String apiUrl = "http://monitoring_daya/crud/kirim_data.php?";

    apiUrl += "mode=save";
    apiUrl += "&capasity=" + String(capasity);
    apiUrl += "&humd=" + String(humd);
    apiUrl += "&ppmnh4=" + String(ppmnh4);

    // Set header Request
    client.print(String("GET ") + apiUrl + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

    // Pastikan tidak berlarut-larut
    unsigned long timeout = millis();
    while (client.available() == 0)
    {
        if (millis() - timeout > 3000)
        {
            Serial.println(">>> Client Timeout !");
            Serial.println(">>> Operation failed !");
            client.stop();
            return;
        }
    }

    // Baca hasil balasan dari PHP
    while (client.available())
    {
        String line = client.readStringUntil('\r');
        Serial.println(line);
    }

    //--------------------------------------------
} */

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

void alaram(int jam, int menit, int detik)
{
    // Menyala buzzer selama 30 detik setiap 1 jam
    if (menit == 0 && detik == 0)
    {
        if (jam % 1 == 0)
        {
            buzzer_berbunyi();
            delay(30000); // Buzzer aktif selama 30 detik
        }
    }
}

void kirim_data_thingspeak(float capasity, float humd, float ppmnh4)
{
    // kirim data ke thingspeak
    ThingSpeak.setField(1, capasity);
    ThingSpeak.setField(2, humd);
    ThingSpeak.setField(3, ppmnh4);

    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (x == 200)
    {
        Serial.println("Channel update successful.");
    }
    else
    {
        Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
}
#endif // SMART_TRASH_FUNGSI_H