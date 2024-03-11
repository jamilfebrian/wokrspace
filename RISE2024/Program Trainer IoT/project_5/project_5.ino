#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int lamp_pin = 4;
int analogValue;
float millivolts, celsius;
byte delaySend;


#define SSID "Tinkpad"
#define PASS "12345678"

#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883 // use 8883 for SSL
#define AIO_USERNAME  "jamilfebrian68"
#define AIO_KEY       "aio_NLIj40PkQoCjQ8dg94KLNj5WRV0W"

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish suhu = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Suhu LM35");

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.print("Koneksi WiFi");
  lcd.setCursor(0, 1);
  lcd.print(SSID);
  pinMode(lamp_pin, OUTPUT);
  Serial.begin(115200);
  delay(10);
  Serial.println(F("Adafruit MQTT demo"));
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  lcd.print(" Oke...!");
  delay(3000);
  lcd.clear();
}

void loop() {
  MQTT_connect();
  analogValue = analogRead(A0);
  millivolts = (analogValue / 1024.0) * 3300;
  celsius = millivolts / 10;
  
  lcd.setCursor(0, 0);
  lcd.print("Suhu:");
  lcd.print(celsius);
  lcd.print("C ");
  delay(1000);
  
  delaySend++;
  if (delaySend < 20)return; //tunda kirim data 20detik
  delaySend = 0;
  lcd.setCursor(0, 0);
  lcd.print("kirim data suhu");
  Serial.print(F("\nSending celsius val "));
  Serial.print(celsius);
  Serial.print("...");
  if (!suhu.publish(celsius)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
  mqtt.processPackets(1000);
  lcd.clear();
}


void MQTT_connect() {
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 10 seconds...");
    mqtt.disconnect();
    delay(10000); // wait 10 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}
