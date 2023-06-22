#ifndef SMART_TRASH_FUNGSI_H
#define SMART_TRASH_FUNGSI_H
#include <config.h>

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

#endif // SMART_TRASH_FUNGSI_H