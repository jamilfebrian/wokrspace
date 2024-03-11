#include "CTBot.h"
#include <ESP8266WiFi.h>

#define ssid      "Wiwindaaulia"
#define password  "12345678"
#define token     "6199106634:AAHpzc0ntzvZQpeLynqJps2eAjXNvoeMqOI"
#define id        1256033605

#define mq2         A0
#define led_merah   D5
#define led_kuning  D6
#define led_hijau   D7
#define led_biru    D8
#define buzzer      D2

CTBot myBot;
static unsigned long presentTime = 0;
static unsigned long curTime = 0;
static short int arrKondosi[5] = {0,0,0,0,0};

void nyalaLampu(int lampu){
  int arrLamp[5] = {D8,D7,D6,D5,D2};
  for(int i=0; i<5;i++){
    if(arrLamp[i]==lampu){
      digitalWrite(lampu,HIGH);
      arrKondosi[i]=1;
      continue;
    } 
    digitalWrite(arrLamp[i],LOW); 
    arrKondosi[i]=0;
  }
}

void blinkled(int lampu){
  if(millis() - curTime <= 250){
    digitalWrite(lampu,HIGH);
  } else if ((millis() - curTime) <= 500 && (millis() - curTime) >= 250){
    digitalWrite(lampu,LOW);
  } else {
    curTime = millis();
  }
}

void connectWifi(){
    Serial.println("Connecting To Wifi");
    pinMode(D4, OUTPUT); 
    digitalWrite(D4,HIGH);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED){
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
    digitalWrite(D4,LOW);
    delay(100);
}

void loginTelegram(){
  digitalWrite(D4,HIGH);
    Serial.println("logging in...");
    while (!myBot.testConnection()){
        myBot.setTelegramToken(token);
        Serial.print(".");
        delay(100);}
    Serial.println("Telegram connection OK!");
    digitalWrite(D4,LOW);
}

void serial(float timeRespond){
  float Respond = timeRespond/1000;
  Serial.print(Respond); 
  Serial.println(" detik");
}

void setup() {
 pinMode(mq2, INPUT);
 pinMode(led_merah, OUTPUT);
 pinMode(led_kuning, OUTPUT);
 pinMode(led_hijau, OUTPUT);
 pinMode(led_biru, OUTPUT);
 pinMode(buzzer, OUTPUT);
 
 Serial.begin(9600);
 connectWifi();
 loginTelegram();
}

void loop(){
  float timeRespond;
  int gas = analogRead(mq2);
  presentTime = millis();

  if(gas>=0 && gas<=50){
    if(arrKondosi[0]==0){
      myBot.sendMessage(id,"Kondisi udara Sangat Baik! \n"+String(gas)+"ppm");
      timeRespond = millis() - presentTime;
      serial(timeRespond);
    } nyalaLampu(led_biru); 
  } else if (gas>=51 && gas<=100){
    if(arrKondosi[1]==0){
      myBot.sendMessage(id,"Kondisi udara Baik! \n"+String(gas)+"ppm");
      timeRespond = millis() - presentTime;
      serial(timeRespond);
    } nyalaLampu(led_hijau);
  } else if (gas>=101 && gas<=200){
    if(arrKondosi[2]==0){
      myBot.sendMessage(id,"Kondisi udara Tidak Sehat! \n"+String(gas)+"ppm");
      timeRespond = millis() - presentTime;
      serial(timeRespond);
    } nyalaLampu(led_kuning);
  } else if (gas>=201 && gas<=300){
    if(arrKondosi[3]==0){
      myBot.sendMessage(id,"Kondisi udara Sangat Tidak Sehat! \n"+String(gas)+"ppm");
      timeRespond = millis() - presentTime;
      serial(timeRespond);
    } nyalaLampu(led_merah); 
  } else if (gas>=301){
    if (arrKondosi[4]==0){
      myBot.sendMessage(id,"Kondisi udara Sangat Bahaya! \n"+String(gas)+"ppm");
      timeRespond = millis() - presentTime;
      serial(timeRespond);
    }
    blinkled(led_merah); 
    blinkled(buzzer); 
  }
}
