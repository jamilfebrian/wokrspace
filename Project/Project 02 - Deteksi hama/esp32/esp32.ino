#include "CTBot.h" 
#include "DFRobotDFPlayerMini.h"
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
DFRobotDFPlayerMini myDFPlayer; 
CTBot myBot;


#define ssid ""
#define password ""
#define token "5574999227:AAG3-h30YWAknn9-9GVLnnDlsNy0JCLKqwY"
#define id   2002845113

#define PIR1  15
#define PIR2  2
#define PIR3  4
#define PIR4  5

#define echo1  18
#define trig1  19
#define echo2  33
#define trig2  25
#define echo3  35
#define trig3  32

#define IN1   13
#define IN2   12
#define IN3   14
#define IN4   27
#define buzzer 26

int JARAK = 4;
int vol = 30;   // 0 SAMPAI 30
int kecepatanStepper=1;
int sensorValue1, sensorValue2, sensorValue3, sensorValue4; 
int counter = 0;

long distance(int TRIG, int ECHO){
  digitalWrite(TRIG, LOW);  delayMicroseconds(8);
  digitalWrite(TRIG, HIGH); delayMicroseconds(8);
  digitalWrite(TRIG, LOW);  delayMicroseconds(8);

  long durasi = pulseIn(ECHO, HIGH); 
  long jarak = (durasi / 2) / 29.1;
  return jarak;
}

void connectWifi(){
    Serial.println("Connecting To Wifi");
    lcd.setCursor(0,0);
    lcd.print("   Connecting   ");
    lcd.setCursor(0,1);
    lcd.print("     To Wifi    ");
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
    lcd.setCursor(0,0);
    lcd.print("      WiFi      ");
    lcd.setCursor(0,1);
    lcd.print("   Connected    ");
    delay(1000);
}

void loginTelegram(){
    lcd.setCursor(0,0);
    lcd.print(" Menghubungkan  ");
    lcd.setCursor(0,1);
    lcd.print("  Telegram Bot  ");
    Serial.println("logging in...");
    while (!myBot.testConnection())    {
        myBot.setTelegramToken(token);
        Serial.print(".");
        delay(100);}
        
  if(myBot.testConnection()){
       Serial.println("Telegram connection OK!");
       lcd.setCursor(0,0);
       lcd.print("  Telegram Bot  ");
       lcd.setCursor(0,1);
       lcd.print("   Terhubung    ");
  } else {
       Serial.println("Connection NOK");
       lcd.setCursor(0,0);
       lcd.print("  Telegram Bot  ");
       lcd.setCursor(0,1);
       lcd.print(" Gagal Terhubung");
  }delay(1000);
}

void DFConnect(){
  Serial.println();
  Serial.println(F("Ma iduik an DF PLAYER"));
  Serial.println(F("Initializing DFPlayer ... (Tunggu Sekitar 3~5 detik)"));
  lcd.setCursor(0,0);
  lcd.print(" Connect to DF  ");
  
  if (!myDFPlayer.begin(Serial2)) {  
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    lcd.setCursor(0,0);
    lcd.print("Periksa Kembali ");
    lcd.setCursor(0,1);
    lcd.print("Koneksi DFPlayer");
    while(true){
      delay(0); 
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  lcd.setCursor(0,0);
  lcd.print("DFPlayer Online ");
  lcd.setCursor(0,1);
  lcd.print("                ");
  myDFPlayer.volume(vol);  //Set volume value. From 0 to 30
  delay(1200);
}


void Putaran1(){ 
  digitalWrite(IN1,1);
  digitalWrite(IN2,0);
  digitalWrite(IN3,0);
  digitalWrite(IN4,0);
}
void Putaran2(){
  digitalWrite(IN1,0);
  digitalWrite(IN2,1);
  digitalWrite(IN3,0);
  digitalWrite(IN4,0);
}
void Putaran3(){
  digitalWrite(IN1,0);
  digitalWrite(IN2,0);
  digitalWrite(IN3,1);
  digitalWrite(IN4,0);
}
void Putaran4(){
  digitalWrite(IN1,0);
  digitalWrite(IN2,0);
  digitalWrite(IN3,0);
  digitalWrite(IN4,1);
}
void Clockwise(){
  Putaran1(); delay(kecepatanStepper);
  Putaran2(); delay(kecepatanStepper);
  Putaran3(); delay(kecepatanStepper);
  Putaran4(); delay(kecepatanStepper);
}
void antiClockwise(){
  Putaran4(); delay(kecepatanStepper);
  Putaran3(); delay(kecepatanStepper);
  Putaran2(); delay(kecepatanStepper);
  Putaran1(); delay(kecepatanStepper);
}

void setup(){
  pinMode(PIR1,INPUT); 
  pinMode(PIR2,INPUT); 
  pinMode(PIR3,INPUT);
  pinMode(PIR4,INPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT); 
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT); 
  pinMode(trig3, OUTPUT);
  pinMode(echo3, INPUT); 
  pinMode(buzzer, OUTPUT);
  
  Serial2.begin(9600);
  Serial.begin(9600);
  
  lcd.init();
  lcd.clear();
  lcd.backlight();
  
  connectWifi();
  loginTelegram();
  DFConnect();
}
 
void loop(){

  long jarak1 = distance(trig1,echo1);
  long jarak2 = distance(trig2,echo2);
  long jarak3 = distance(trig3,echo3);
  sensorValue1=digitalRead(PIR1);
  sensorValue2=digitalRead(PIR2);
  sensorValue3=digitalRead(PIR3);
  sensorValue4=digitalRead(PIR4);
  
  if(sensorValue1==1){
    
    Serial.println("Sensor 1 terdeteksi");
    lcd.setCursor(0,0);
    lcd.print("    Sensor 1    ");
    lcd.setCursor(0,1);
    lcd.print("   Terdeteksi   ");
    myBot.sendMessage(id,"Terdeteksi hama burung pada sensor PIR 1");
    counter++;
    if(counter>=1 && counter<4){
      myDFPlayer.play(1);
    }else if(counter==4){
      for(int i=0; i<=10; i++){
        for(int j=0; j<=50; j++){
          antiClockwise();
        } for(int j=0; j<=50; j++){
          Clockwise();
        }
    }  counter=0; 
    }
  } else if (sensorValue2==1){
    Serial.println("Sensor 2 terdeteksi");
    lcd.setCursor(0,0);
    lcd.print("    Sensor 2    ");
    lcd.setCursor(0,1);
    lcd.print("   Terdeteksi   ");
    myBot.sendMessage(id,"Terdeteksi hama burung pada sensor PIR 2");
    counter++;
    if(counter>=1 && counter<4){
      myDFPlayer.play(1);
    }else if(counter==4){
      for(int i=0; i<=10; i++){
        for(int j=0; j<=50; j++){
          antiClockwise();
        } for(int j=0; j<=50; j++){
          Clockwise();
        }
    }  counter=0;
  }} else if (sensorValue3==1){
    Serial.println("Sensor 3 terdeteksi");
    lcd.setCursor(0,0);
    lcd.print("    Sensor 3    ");
    lcd.setCursor(0,1);
    lcd.print("   Terdeteksi   ");
    myBot.sendMessage(id,"Terdeteksi hama burung pada sensor PIR 3");
    counter++;
    if(counter>=1 && counter<4){
      myDFPlayer.play(1);
    }else if(counter==4){ b
      for(int i=0; i<=10; i++){
        for(int j=0; j<=50; j++){
          antiClockwise();
        } for(int j=0; j<=50; j++){
          Clockwise();
        }
    } counter=0;
  }} else if (sensorValue4==1){
    Serial.println("Sensor 4 terdeteksi");
    lcd.setCursor(0,0);
    lcd.print("    Sensor 4    ");
    lcd.setCursor(0,1);
    lcd.print("   Terdeteksi   ");
    myBot.sendMessage(id,"Terdeteksi hama burung pada sensor PIR 4");
    counter++;
    if(counter>=1 && counter<4){
      myDFPlayer.play(1);
    }else if(counter==4){
      for(int i=0; i<=10; i++){
        for(int j=0; j<=50; j++){
          antiClockwise();
        } for(int j=0; j<=50; j++){
          Clockwise();
        }
      
    } counter=0;
  }
  }else if ((jarak1<=JARAK && jarak1!=0)||(jarak2<=JARAK && jarak2!=0)||(jarak3<=JARAK && jarak3!=0)){
    Serial.println("Sensor ultrasonik terdeteksi");
    lcd.setCursor(0,0);
    lcd.print("  Sensor HC-04  ");
    lcd.setCursor(0,1);
    lcd.print("   Terdeteksi   ");
    myBot.sendMessage(id,"Terdeteksi hama Tikus pada sensor HCSR-04");
    myDFPlayer.play(2);
    delay(2000); 

  } else {
    lcd.clear();
    digitalWrite(buzzer, LOW);
  }
}
