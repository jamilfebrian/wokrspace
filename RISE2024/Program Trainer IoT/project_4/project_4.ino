#include <DHT.h>
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "Tinkpad";
const char* password = "12345678";
unsigned long myChannelNumber = 2436100;
const char* myWriteAPIKey = "X7W8PFCDBUSTCQKU";

LiquidCrystal_I2C lcd(0x27, 16, 2);
WiFiClient client;

#define DHTPIN D3
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
byte delaySend;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.print("Koneksi WiFi");
  lcd.setCursor(0, 1);
  lcd.print(ssid);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(" ");
    Serial.println(" ");
    Serial.print("Attempting to connect to SSID: ");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nConnected.");
    lcd.print(" Oke...!");
  }
  delay(2000);
  lcd.clear();
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("DHT22 tidak terbaca... !");
    lcd.setCursor(0, 0);
    lcd.print(" DHT22 ");
    lcd.setCursor(0, 1);
    lcd.print(" tidak terbaca");
    return;
  }
  lcd.setCursor(0, 0);
  lcd.print("Suhu:");
  lcd.print(t);
  lcd.print("C ");
  lcd.setCursor(0, 1);
  lcd.print("Humi:");
  lcd.print(h);
  lcd.print("% ");
  delay(1000);
  delaySend++;
  if (delaySend < 20)return;
  lcd.clear();
  lcd.print(" Update DHT22");
  lcd.setCursor(0, 1);
  lcd.print("ke Thingspeak...");
  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
    lcd.clear();
    lcd.print("Successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
    lcd.clear();
    lcd.print("ERROR.....");
  }
  delaySend = 0;
  delay(3000);
}
