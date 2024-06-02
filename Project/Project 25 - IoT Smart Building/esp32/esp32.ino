#define BLYNK_TEMPLATE_ID "TMPL6KSutaXFk"
#define BLYNK_TEMPLATE_NAME "My Project Testing"
#define BLYNK_AUTH_TOKEN "SV052QVxnitEnDe6j6KfsGARLijLfgZf"

#include "WiFi.h"
#include <HTTPClient.h>
#include <BlynkSimpleEsp32.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

#include <PZEM004Tv30.h>
#if !defined(PZEM_RX_PIN) && !defined(PZEM_TX_PIN)
    #define PZEM_RX_PIN 16
    #define PZEM_TX_PIN 17
#endif

#if !defined(PZEM_SERIAL)
    #define PZEM_SERIAL Serial2
#endif

#if defined(ESP32)
    PZEM004Tv30 pzem(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN);
#elif defined(ESP8266)
    PZEM004Tv30 pzem(Serial1);
#else
    PZEM004Tv30 pzem(PZEM_SERIAL);
#endif

#define LAMPU_1   13  //pin Lampu Relay
#define LAMPU_2   12
#define LAMPU_3   14
#define LAMPU_4   15
#define LAMPU_5   2
#define SELENOID  4

//#define BUTTON_1  36  //pin Button
#define BUTTON_2  39
#define BUTTON_3  34
#define BUTTON_4  35
#define BUTTON_5  32
#define BUTTON_6  33 //button kunci pintu by satpam

#define RXSensorPin 16 //Pin Sensor Arus AC
#define TXSensorPin 17
 
#define MAGNET_SENSOR 27 // pin Sensor Magnet
#define BUZZER 26 // pin Buzzer

/*MISO=19  MOSI=23  SCK=18  SDA=5  RST=25  VCC  GND*/
#define RST_PIN       25  // pin RFID
#define SS_PIN        5   

/*Konfigurasi Jaringan Wifi*/
#define ssid      "Tinkpad"   
#define password  "12345678" 
#define auth  BLYNK_AUTH_TOKEN
#define Web_App_URL  "https://script.google.com/macros/s/AKfycbxK7EyNtw-oLWUZ8L1xxgZskmSGiYjOljkBKm0cSE8oGt7FUfJzW6380JjYWgh5b8OP/exec"

BlynkTimer timer;
LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 mfrc522(SS_PIN, RST_PIN);

unsigned long Ctimer = 0;
unsigned long rfidtimer = 0;
unsigned long doorlockTimer = 0;


static float voltage, current, power, frequency; //variabel untuk menampung sensor tegangan AC
static String Status_Read_Sensor = ""; //variabel untuk menampung status http request ke spreadsheet
static bool relayState[6] = {1,1,1,1,1,1}; // variabel menampung status lampu

#define jumlah_kartu 1
String IDCard[jumlah_kartu] = {"B7 BF A5 6B"};

void Blynk_initialize(){
  Blynk.virtualWrite(V0, HIGH); 
  Blynk.virtualWrite(V1, HIGH); 
  Blynk.virtualWrite(V2, HIGH); 
  Blynk.virtualWrite(V3, HIGH); 
  Blynk.virtualWrite(V4, HIGH); 
  Blynk.virtualWrite(V5, HIGH); 
}

void pin_config(){
  pinMode(LAMPU_1, OUTPUT);
  pinMode(LAMPU_2, OUTPUT);
  pinMode(LAMPU_3, OUTPUT);
  pinMode(LAMPU_4, OUTPUT);
  pinMode(LAMPU_5, OUTPUT);
  
  pinMode(SELENOID, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(MAGNET_SENSOR, INPUT_PULLUP);

//  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_3, INPUT);
  pinMode(BUTTON_4, INPUT);
  pinMode(BUTTON_5, INPUT);
  pinMode(BUTTON_6, INPUT);

  digitalWrite(LAMPU_1, HIGH);
  digitalWrite(LAMPU_2, HIGH); 
  digitalWrite(LAMPU_3, HIGH);
  digitalWrite(LAMPU_4, HIGH);
  digitalWrite(LAMPU_5, HIGH);
  digitalWrite(SELENOID, HIGH);
  digitalWrite(BUZZER, LOW);
}

void connectWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Blynk.begin(auth, ssid, password, "blynk.cloud", 80);
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

void sendDataToSpreadsheet(){
    String Send_Data_URL = Web_App_URL "?sts=writeSensor";
    
    Send_Data_URL += "&srs=" + Status_Read_Sensor;
    Send_Data_URL += "&V=" + String(voltage);
    Send_Data_URL += "&I=" + String(current);
    Send_Data_URL += "&P=" + String(power);
    Send_Data_URL += "&f=" + String(frequency);

      HTTPClient http;
  
      http.begin(Send_Data_URL.c_str());
      http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

      int httpCode = http.GET(); 
      
      String payload;
      if (httpCode > 0) {
        payload = http.getString();
      }
      http.end();
}

void button(){

   String button1 = "";
    while(Serial.available()>0){
      button1 += char(Serial.read());
    } button1.trim(); 

  
  if(millis() - Ctimer > 1000){
    if(button1 == "1"){
      relayState[0] = !relayState[0];
      digitalWrite(LAMPU_1, relayState[0]);
      Blynk.virtualWrite(V0, relayState[0]); 
      Ctimer = millis();
    }
    if(digitalRead(BUTTON_2)){
      relayState[1] = !relayState[1];
      digitalWrite(LAMPU_2, relayState[1]);
      Blynk.virtualWrite(V1, relayState[1]);
      Ctimer = millis();
    }
    if(digitalRead(BUTTON_3)){
      relayState[2] = !relayState[2];
      digitalWrite(LAMPU_3, relayState[2]);
      Blynk.virtualWrite(V2, relayState[2]); 
      Ctimer = millis();
    }
    if(digitalRead(BUTTON_4)){
      relayState[3] = !relayState[3];
      digitalWrite(LAMPU_4, relayState[3]);
      Blynk.virtualWrite(V3, relayState[3]); 
      Ctimer = millis();
    }
    if(digitalRead(BUTTON_5)){
      relayState[4] = !relayState[4];
      digitalWrite(LAMPU_5, relayState[4]);
      Blynk.virtualWrite(V4, relayState[4]);
      Ctimer = millis();
    }
    if(digitalRead(BUTTON_6)){
      relayState[5] = !relayState[5];
      digitalWrite(SELENOID, relayState[5]);
      Blynk.virtualWrite(V5, relayState[5]); 
      doorlockTimer = millis();
      Ctimer = millis();
    }
  }
}

void buzzer(int index = 3, int period = 75){
  for(int i=0; i<index; i++){
    digitalWrite(BUZZER, HIGH);
    delay(period);
    digitalWrite(BUZZER, LOW);
    delay(period);
  } 
}

void readRFID(){
  if(millis() - rfidtimer > 2000){
      lcd.setCursor(0,0); lcd.print("Please Take Your");
      lcd.setCursor(0,1); lcd.print("    ID Card     ");
    }

   SPI.begin();  mfrc522.PCD_Init();
   
  String content = "";
  if (!mfrc522.PICC_IsNewCardPresent()) {
     return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  doorlockTimer = millis();
  
  for (byte i = 0; i < mfrc522.uid.size; i++){
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }  content.toUpperCase();
  
    for(byte i=0; i<jumlah_kartu; i++){
      if(content.substring(1) == IDCard[i]){
         lcd.setCursor(0,0); lcd.print("    Valid  ID   ");
         lcd.setCursor(0,1); lcd.print("    Door Open   ");
         buzzer();
         relayState[5] = LOW;
         digitalWrite(SELENOID, LOW);
         Blynk.virtualWrite(V5, LOW);
         rfidtimer = millis();
      } else {
         lcd.setCursor(0,0); lcd.print("ID Not Found!!  ");
         lcd.setCursor(0,1); lcd.print("Please Try Again");
         buzzer(1,800);
         rfidtimer = millis();
      }
    }   
}


void setup(){
  Serial.begin(115200);
  SPI.begin();  mfrc522.PCD_Init();
  pin_config();
  
  buzzer(1,100);
  lcd.init(); lcd.backlight(); lcd.clear();
  lcd.setCursor(0,0); lcd.print("Menghubungkan   ");
  lcd.setCursor(0,1); lcd.print("Ke : " + String(ssid));
  
  connectWifi();
  Blynk.run(); 

  lcd.setCursor(0,0); lcd.print("    Jaringan    ");
  lcd.setCursor(0,1); lcd.print(" Wifi Terhubung ");
  
  buzzer(3,100);
  Blynk_initialize();
  
  timer.setInterval(300000L, sendDataToSpreadsheet);
}

void loop(){
   Blynk.run(); timer.run();
   readRFID();

    button();
    
     voltage = pzem.voltage(); 
     current = pzem.current();
     power = pzem.power();
     frequency = pzem.frequency();

    if(isnan(voltage) || isnan(current) || isnan(power) || isnan(frequency)){
         Status_Read_Sensor = "FAILED";
         voltage = 0;
         current = 0;
         power = 0;
         frequency = 0;
    } else {
         Status_Read_Sensor = "SUCCES";
    }
    
  Blynk.virtualWrite(V6, frequency); 
  Blynk.virtualWrite(V7, current); 
  Blynk.virtualWrite(V8, voltage); 
  Blynk.virtualWrite(V9, power); 

  if(digitalRead(MAGNET_SENSOR) && relayState[5] ){
      digitalWrite(BUZZER,HIGH);
  } else if(!digitalRead(MAGNET_SENSOR) && !relayState[5] ){
    if (millis()-doorlockTimer > 3000){
      digitalWrite(SELENOID,HIGH);
      relayState[5] = HIGH;
      Blynk.virtualWrite(V5, HIGH);
    } digitalWrite(BUZZER,LOW);
    
  } else if(digitalRead(MAGNET_SENSOR) && !relayState[5] ){
      digitalWrite(BUZZER,LOW);
      doorlockTimer = millis();
  } else {
      digitalWrite(BUZZER,LOW);
  }
}

BLYNK_WRITE(V0){
  int value = param.asInt();  
  if (value == 1) {
   digitalWrite(LAMPU_1,HIGH);
   relayState[0] = HIGH;
  } else {
   digitalWrite(LAMPU_1,LOW);
   relayState[0] = LOW;
  }
}

BLYNK_WRITE(V1){
  int value = param.asInt();  
  if (value == 1) {
   digitalWrite(LAMPU_2,HIGH);
   relayState[1] = HIGH;
  } else {
   digitalWrite(LAMPU_2,LOW);
   relayState[1] = LOW;
  }
}

BLYNK_WRITE(V2){
  int value = param.asInt();  
  if (value == 1) {
   digitalWrite(LAMPU_3,HIGH);
   relayState[2] = HIGH;
  } else {
   digitalWrite(LAMPU_3,LOW);
   relayState[2] = LOW;
  }
}

BLYNK_WRITE(V3){
  int value = param.asInt();  
  if (value == 1) {
   digitalWrite(LAMPU_4,HIGH);
   relayState[3] = HIGH;
  } else {
   digitalWrite(LAMPU_4,LOW);
   relayState[3] = LOW;
  }
}

BLYNK_WRITE(V4){
  int value = param.asInt();  
  if (value == 1) {
   digitalWrite(LAMPU_5,HIGH);
   relayState[4] = HIGH;
  } else {
   digitalWrite(LAMPU_5,LOW);
   relayState[4] = LOW;
  }
}

BLYNK_WRITE(V5){
  int value = param.asInt();  
  if (value == 1) {
   digitalWrite(SELENOID,HIGH);
   relayState[5] = HIGH;
  } else {
   digitalWrite(SELENOID,LOW);
   relayState[5] = LOW;
   doorlockTimer = millis();
  }
}
