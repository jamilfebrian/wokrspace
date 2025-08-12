#define BLYNK_TEMPLATE_ID "TMPL61IxapWdA"
#define BLYNK_TEMPLATE_NAME "ESP32"
#define BLYNK_AUTH_TOKEN "ICfMQqpE867-F7YvRUOehw5CPgQO8pGz"
#define Web_App_URL   "https://script.google.com/macros/s/AKfycbyHoMOOxLJLqL0TGfHlv_w7ezHphhurnto_Z5ebDQMRCbdCyID9e4_4lw6WASO8qAkXhw/exec"

#include <DHT.h>
#include "WiFi.h"
#include <HTTPClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

#define dht22 4
#define pompaRelay 15

#define ssid      "PAK CIK"   
#define password  "pakboy0509" 
#define auth  BLYNK_AUTH_TOKEN

Adafruit_INA219 ina219;
DHT dht(dht22, DHT22);

unsigned long previousMillis = 0;
const unsigned long interval = 60000; 

float humidity, temperature;
float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float power_mW = 0;

bool statusPompa;
static String Status_Read_Sensor = ""; 

void connectWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Blynk.begin(auth, ssid, password, "blynk.cloud", 80);
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    } Blynk.run(); 
    Serial.println("Wifi Terhubung");
}

void sendDataToSpreadsheet(){
    String Send_Data_URL = String(Web_App_URL) + "?sts=writeSensor";
    
    Send_Data_URL += "&srs=" + Status_Read_Sensor;
    Send_Data_URL += "&T=" + String(temperature);
    Send_Data_URL += "&I=" + String(current_mA);
    Send_Data_URL += "&V=" + String(loadvoltage);
    Send_Data_URL += "&P=" + String(power_mW);

      HTTPClient http;
  
      http.begin(Send_Data_URL.c_str());
      http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

      int httpCode = http.GET(); 
      
      String payload;
      if (httpCode > 0) {
        payload = http.getString();
      }
//      Serial.println(httpCode);
      http.end();
}

void setup(){
  Serial.begin(115200);
  pinMode(dht22, INPUT);
  pinMode(pompaRelay, OUTPUT);
  
  digitalWrite(pompaRelay, HIGH);
  dht.begin();

  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  } ina219.setCalibration_32V_1A();

  connectWifi();
 
}

void loop(){
  Blynk.run(); 
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  delay(1000);

  Blynk.virtualWrite(V0, temperature); 
  Blynk.virtualWrite(V2, current_mA); 
  Blynk.virtualWrite(V1, loadvoltage); 
  Blynk.virtualWrite(V3, power_mW); 

  Serial.print("humidity : "); Serial.print(humidity);  Serial.println(" %"); 
  Serial.print("temperature : "); Serial.print(temperature); Serial.println(" °C"); 
  Serial.print("voltageValue : "); Serial.print(loadvoltage); Serial.println(" V"); 
  Serial.print("currentValue : "); Serial.print(current_mA); Serial.println(" mA"); 
  Serial.print("powerValue : "); Serial.print(power_mW); Serial.println(" mW"); 

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    sendDataToSpreadsheet();
  }

  if(temperature > 40){
    statusPompa = true;
    Status_Read_Sensor = "ON";
  } else if(temperature <= 35){
    statusPompa = false;
    Status_Read_Sensor = "OFF";
  }

  if(statusPompa){
    digitalWrite(pompaRelay, LOW);
    Blynk.virtualWrite(V4, LOW);
  } else {
    digitalWrite(pompaRelay, HIGH);
    Blynk.virtualWrite(V4, HIGH);
  }
}
