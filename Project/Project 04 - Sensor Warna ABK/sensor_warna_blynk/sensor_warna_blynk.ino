#define BLYNK_TEMPLATE_ID "TMPL66-pvNTq2"
#define BLYNK_TEMPLATE_NAME "Sensor Warna"
#define BLYNK_AUTH_TOKEN "KciYiTxwTpELD4HIaVdjLmgokA5bakR0"

#include <WiFi.h>
#include <Wire.h> 
#include <BlynkSimpleEsp32.h>
#include "DFRobotDFPlayerMini.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define SSID "Tinkpad"
#define PASS "12345678"
#define AUTH BLYNK_AUTH_TOKEN

#define S0 18
#define S1 23
#define S2 15
#define S3 4 
#define sensorOut 19

DFRobotDFPlayerMini myDFPlayer; 

static int red = 0; //Variabel frekuensi
static int green = 0;
static int blue = 0;

bool counter = true;;
int timer = 1000;

void DFConnect(){
  Serial.println();
  Serial.println(F("Mengaktifkan DF PLAYER"));
  Serial.println(F("Initializing DFPlayer ... (Tunggu Sekitar 3~5 detik)"));
  
  myDFPlayer.begin(Serial2);
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
}

void setup() {
  pinMode(2,OUTPUT);
  pinMode(S0, OUTPUT); //Setting S0 sebagai output
  pinMode(S1, OUTPUT); //Setting S1 sebagai output
  pinMode(S2, OUTPUT); //Setting S2 sebagai output
  pinMode(S3, OUTPUT); //Setting S3 sebagai output
  pinMode(sensorOut, INPUT); //Setting Outsensor sebagai Input

  digitalWrite(S0,HIGH); // setting 20% frequency
  digitalWrite(S1,LOW);
  Serial.begin(115200); //Mulai komunikasi serial
  Serial2.begin(9600);

  DFConnect();

  lcd.init();  lcd.backlight();

  lcd.setCursor(0,0);  lcd.print("Menyambungkan ke");  
  lcd.setCursor(0,1);  lcd.print(SSID);

  digitalWrite(2, LOW);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED){
    delay(100); digitalWrite(2, LOW);
  }   digitalWrite(2, HIGH);
  Blynk.begin(AUTH, SSID, PASS, "blynk.cloud", 80);
   lcd.clear();
}

void bacaWarna(int s2,int s3){
  //Setting baca merah
  digitalWrite(s2,LOW);
  digitalWrite(s3,LOW);
  red = pulseIn(sensorOut, LOW);
  delay(100);
  
//Setting baca hijau
  digitalWrite(s2,HIGH);
  digitalWrite(s3,HIGH);
  green = pulseIn(sensorOut, LOW);
  delay(100);

//Setting baca biru
  digitalWrite(s2,LOW);
  digitalWrite(s3,HIGH);
  blue = pulseIn(sensorOut, LOW);
  delay(100);
}

void loop() {
  Blynk.run();
  bacaWarna(S2,S3);
  
  if ((red<27 && red>14)&&(green<100 && green>50)&&(blue<25 && blue>13)){
    Serial.println("Merah");
    Blynk.virtualWrite(V0, 0); 
    Blynk.virtualWrite(V1, 1); 
    Blynk.virtualWrite(V2, 0); 
    Blynk.virtualWrite(V3, 0); 
    lcd.setCursor(7,1); lcd.print("Merah");
    myDFPlayer.play(1);
    delay(timer); counter=true;
    
  } 
  else if ((red<17 && red>10)&&(green<30 && green>10)&&(blue<11 && blue>6)){
    Serial.println("Kuning");
    Blynk.virtualWrite(V0, 1); 
    Blynk.virtualWrite(V1, 0);
    Blynk.virtualWrite(V2, 1); 
    Blynk.virtualWrite(V3, 0);  
    lcd.setCursor(7,1); lcd.print("Kuning");
    myDFPlayer.play(2);
    delay(timer); counter=true;
  } 
  else if ((red<25 && red>9)&&(green<18 && green>10)&&(blue<6 && blue>2)){
    Serial.println("Biru");
    Blynk.virtualWrite(V0, 2); 
    Blynk.virtualWrite(V1, 0);
    Blynk.virtualWrite(V2, 0); 
    Blynk.virtualWrite(V3, 1);   
    lcd.setCursor(7,1); lcd.print("Biru");
    myDFPlayer.play(3);
    delay(timer); counter=true;
  } else {
    if(counter){
      Blynk.virtualWrite(V0, 3); 
      Blynk.virtualWrite(V1, 0);
      Blynk.virtualWrite(V2, 0); 
      Blynk.virtualWrite(V3, 0);   
      lcd.setCursor(7,1); lcd.print("            ");
      delay(100); counter=false;
    }
  }
  
  lcd.setCursor(0,0);  
  lcd.print("R: ");  lcd.print(red);  
  lcd.print(" G: ");  lcd.print(green);
  lcd.print("                    ");
  lcd.setCursor(0,1);
  lcd.print("B: ");  lcd.print(blue);
}
