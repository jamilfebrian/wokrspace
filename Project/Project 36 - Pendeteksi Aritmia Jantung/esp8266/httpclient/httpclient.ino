#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Konfigurasi WiFi
const char* SSID = "Tinkpad";
const char* PASSWORD = "12345678";

// Token Ubidots dan variabel
const char* UBIDOTS_TOKEN = "BBUS-NeisqfaJ6GNSCd8nm7UNIN07gnV0oT";
const char* DEVICE_LABEL = "esp8266";
const char* VARIABLE_LABEL = "suhu";

const char* UBIDOTS_URL = "http://industrial.api.ubidots.com/api/v1.6/devices/";

WiFiClient client;

void setup() {
    Serial.begin(115200);
    WiFi.begin(SSID, PASSWORD);
    Serial.print("Menghubungkan ke WiFi");
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nWiFi Terhubung!");
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String url = String(UBIDOTS_URL) + DEVICE_LABEL;
        
        // Membaca nilai sensor dari pin A0
        int sensorValue = random(10,50);
        Serial.print("Nilai Sensor: ");
        Serial.println(sensorValue);
        
        // Membentuk JSON untuk dikirim
        String payload = "{\"" + String(VARIABLE_LABEL) + "\": {\"value\": " + String(sensorValue) + "}}";
        Serial.println(payload);
        
        http.begin(client, url);
        http.addHeader("Content-Type", "application/json");
        http.addHeader("X-Auth-Token", UBIDOTS_TOKEN);
        
        int httpResponseCode = http.POST(payload);
        if (httpResponseCode > 0) {
            Serial.println("Data terkirim ke Ubidots!");
        } else {
            Serial.print("Error mengirim data: ");
            Serial.println(httpResponseCode);
        }
        http.end();
    }
    delay(5000); // Kirim data setiap 5 detik
}
