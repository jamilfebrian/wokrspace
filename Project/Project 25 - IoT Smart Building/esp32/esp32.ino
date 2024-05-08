#include "WiFi.h"
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

#define LAMPU_1   15
#define LAMPU_2   2
#define LAMPU_3   4
#define LAMPU_4   13
#define LAMPU_5   12
#define LAMPU_6   14

#define BUTTON_1  36
#define BUTTON_2  39
#define BUTTON_3  34
#define BUTTON_4  35
#define BUTTON_5  32
#define BUTTON_6  33

#define RXSensorPin 16
#define TXSensorPin 17

#define MAGNET_SENSOR 27
#define BUZZER 26

/*MISO=19  MOSI=23  SCK=18  SDA=5  RST=25  VCC  GND*/
#define RST_PIN       25       
#define SS_PIN        5   

#define ssid      "Tinkpad"   //--> Your wifi name
#define password  "12345678" //--> Your wifi password
#define Web_App_URL  "REPLACE_WITH_YOUR_WEB_APP_URL"

void pin_configurasi(){
  pinMode(LAMPU_1, OUTPUT);
  pinMode(LAMPU_2, OUTPUT);
  pinMode(LAMPU_3, OUTPUT);
  pinMode(LAMPU_4, OUTPUT);
  pinMode(LAMPU_5, OUTPUT);
  pinMode(LAMPU_6, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT_PULLUP);
  pinMode(BUTTON_4, INPUT_PULLUP);
  pinMode(BUTTON_5, INPUT_PULLUP);
  pinMode(BUTTON_6, INPUT_PULLUP);
}

void connectWifi(){
  Serial.println("WIFI mode : STA");
  WiFi.mode(WIFI_STA);
  Serial.println("------------");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("Wifi Connected");
    Serial.println(WiFi.SSID());
    Serial.println(WiFi.RSSI());
    Serial.println(WiFi.macAddress());
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.gatewayIP());
    Serial.println(WiFi.dnsIP());
}

void setup(){
  Serial.begin(115200);
  pin_configurasi();
  connectWifi();
  
}
void loop(){

  String Send_Data_URL = Web_App_URL + "?sts=write";
    Send_Data_URL += "&srs=" + Status_Read_Sensor;
    Send_Data_URL += "&temp=" + String(Temp);
    Send_Data_URL += "&humd=" + String(Humd);
    Send_Data_URL += "&swtc1=" + Switch_1_State;
    Send_Data_URL += "&swtc2=" + Switch_2_State;

    Serial.println();
    Serial.println("-------------");
    Serial.println("Send data to Google Spreadsheet...");
    Serial.print("URL : ");
    Serial.println(Send_Data_URL);

      HTTPClient http;
  
      http.begin(Send_Data_URL.c_str());
      http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  
      int httpCode = http.GET(); 
      Serial.print("HTTP Status Code : ");
      Serial.println(httpCode);
  
      String payload;
      if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload : " + payload);    
      }
      
      http.end();

    Serial.println("-------------");
  }
  delay(10000);
}
