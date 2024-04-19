#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

//Sensor Pin
#define dhtPin 
#define soilPin_1 
#define soilPin_2 

//Output PIN
#define led_wifi  

void connectWifi() {
  pinMode(led_wifi, OUTPUT);
  digitalWrite(led_wifi, LOW);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); digitalWrite(led_wifi, LOW);
    Serial.println("Connecting to WiFi...");
  }  Serial.println("Wifi Terhubung");
  Blynk.begin(auth, ssid, password, "blynk.cloud", 80);
  digitalWrite(led_wifi, HIGH);
} 

void setup() {
 

}

void loop() {
 

}
