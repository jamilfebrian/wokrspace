#define BLYNK_TEMPLATE_ID "TMPL61IxapWdA"
#define BLYNK_TEMPLATE_NAME "Project Testing"
#define BLYNK_AUTH_TOKEN "ICfMQqpE867-F7YvRUOehw5CPgQO8pGz"
#define Web_App_URL "https://script.google.com/macros/s/AKfycbyuAy8QFRNACNyBHZ7tnfkPQfobccYdL1I0pL7yrJNIHIHdBgjP18baDOEHlBiRdZ8zCA/exec?"  

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <MAX30100_PulseOximeter.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#define REPORTING_PERIOD_MS 1000  // Waktu pembaruan data dalam milidetik

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
void onBeatDetected() {
    Serial.println("Detak jantung terdeteksi!");
}
void sendDataToSpreadsheet(float HR){
    const String Status_Read_Sensor = "SUCCESS";
    String Send_Data_URL = Web_App_URL "sts=writeSensor";
    
    Send_Data_URL += "&srs=" + Status_Read_Sensor;
    Send_Data_URL += "&HR=" + String(HR);

      HTTPClient http;
      WiFiClientSecure client;
      client.setInsecure();
      
      http.begin(client, Send_Data_URL.c_str());
      http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

      int httpCode = http.GET(); 
      Serial.print("ISI : ");
      Serial.println(httpCode);
      
      String payload;
      if (httpCode > 0) {
        payload = http.getString();
        
      }
      http.end();
}
void setup() {
    Serial.begin(115200);
    connectWifi();
    Serial.print("Inisialisasi Pulse Oximeter...");

    if (!pox.begin()) {
        Serial.println(" Gagal!");
        for(;;);
    } else {
        Serial.println(" Sukses!");
    }

    // Mengatur level sensor ke mode pengukuran dan menambahkan callback
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
    // Perbarui data dari sensor
    pox.update(); Blynk.run();
    float heartrate = pox.getHeartRate();

    // Cek waktu pelaporan
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("SpO2: ");
        Serial.print(pox.getSpO2());
        Serial.print("%  |  Detak Jantung: ");
        Serial.print(heartrate);
        Serial.println(" bpm");
        Blynk.virtualWrite(V2, heartrate);
        tsLastReport = millis();
      if(heartrate >=20 ){
        sendDataToSpreadsheet(heartrate);
      }
    }
}
