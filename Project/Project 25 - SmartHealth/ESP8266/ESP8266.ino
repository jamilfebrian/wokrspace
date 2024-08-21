#define BLYNK_TEMPLATE_ID "TMPL61IxapWdA"
#define BLYNK_TEMPLATE_NAME "Project Testing"
#define BLYNK_AUTH_TOKEN "ICfMQqpE867-F7YvRUOehw5CPgQO8pGz"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

PulseOximeter pox;
uint32_t tsLastReport = 0;

#define ssid      "192.168.50.10"   
#define password  "12345678" 
#define auth  BLYNK_AUTH_TOKEN 

void connectWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Blynk.begin(auth, ssid, password, "blynk.cloud", 80);
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

void setup() {
  Serial.begin(115200);
  connectWifi();
  if (!pox.begin()) {
      Serial.println("Gagal menginisialisasi sensor MAX30100. Periksa koneksi!");
      while (true);
    } else {
      Serial.println("Sensor MAX30100 berhasil diinisialisasi.");
    }
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  pox.update(); Blynk.run(); 

  float heartRate = pox.getHeartRate();
  int SpO2 = pox.getSpO2();

  // Setiap 1 detik, tampilkan data suhu, SpO2, dan detak jantung
  if (millis() - tsLastReport > 1000) {
    tsLastReport = millis();

    Serial.print("Detak Jantung: ");
    Serial.print(heartRate);
    Serial.print(" bpm / SpO2: ");
    Serial.print(SpO2);
    Serial.println(" %");

    Blynk.virtualWrite(V2, heartRate);
  }

}

void onBeatDetected(){
  
}
