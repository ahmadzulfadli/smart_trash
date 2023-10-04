// https://api.watsap.id/send-messageid_device":"4829","api-key":"27c80793b8a81c66c88222d4eced071de5827853","no_hp":"089623426138","pesan":"Mr. Bin is starting"

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
    // https://wa.srv15.wapanels.com/send-message?api_key=1234567890&sender=62888xxxx&number=62888xxxx&message=Hello
    String url = "https://wa.srv15.wapanels.com/send-message?api_key=" + apiKey + "&sender=" + sender + "&number=" + reciver + "&message=" + urlEncode(message);
    Serial.println(url);
    HTTPClient http;
    http.begin(url);

    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    /// Send the GET request
    int httpResponseCode = http.POST(url);

    if (httpResponseCode > 0)
    {
        Serial.printf("HTTP Response code: %d\n", httpResponseCode);
        String response = http.getString();
        Serial.println(response);
    }
    else
    {
        Serial.printf("HTTP GET request failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
    }

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