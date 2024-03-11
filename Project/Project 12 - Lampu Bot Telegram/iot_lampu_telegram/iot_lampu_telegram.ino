#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

SoftwareSerial myserial(2,3);
LiquidCrystal_I2C lcd(0x27, 16, 2);
int led[5] =  {9,10,11,12,13};
bool statusLampu[5] = {0,0,0,0,0};
String lampu_on = "", lampu_off = "";
static unsigned long currTime = 0;

void displayLCD(String statusOn,String StatusOff){
      
      if(millis() - currTime <= 2000){
          lcd.setCursor(0,0);
          lcd.print("Lampu yg Menyala");
          lcd.setCursor(0,1);
          lcd.print(statusOn);
          lcd.print("                ");
      } else if(millis() - currTime > 2000 && millis() - currTime <= 4000){
          lcd.setCursor(0,0);
          lcd.print("Lampu yg Mati   ");
          lcd.setCursor(0,1);
          lcd.print(StatusOff);
          lcd.print("                ");
      } else {
        currTime = millis();
      }
}

void setup() {
  Serial.begin(9600);
  myserial.begin(9600);
  lcd.begin(); lcd.backlight();
  
  for(int i=0; i<5; i++){
    pinMode(led[i], OUTPUT);
  }
}

void loop() {
 String pesan = "", data = "";
    while(myserial.available()>0){
      data += char(myserial.read());
    } data.trim();

    if (data != ""){
      for(int i=0; i<=data.length();i++){
          pesan += data[i];
        }
      }
 
 if(pesan == "1on"){
  digitalWrite(led[0], HIGH); 
  statusLampu[0] = 1;
 } else if(pesan == "2on"){
  digitalWrite(led[1],HIGH);
  statusLampu[1] = 1;
 } else if(pesan == "3on"){
  digitalWrite(led[2],HIGH);
  statusLampu[2] = 1;
 } else if(pesan == "4on"){
  digitalWrite(led[3],HIGH);
  statusLampu[3] = 1;
 } else if(pesan == "5on"){
  digitalWrite(led[4],HIGH); 
  statusLampu[4] = 1;
 } 
 
 else if(pesan == "1off"){
  digitalWrite(led[0],LOW); 
  statusLampu[0] = 0;
 } else if(pesan == "2off"){
  digitalWrite(led[1],LOW); 
  statusLampu[1] = 0;
 } else if(pesan == "3off"){
  digitalWrite(led[2],LOW); 
  statusLampu[2] = 0;
 } else if(pesan == "4off"){
  digitalWrite(led[3],LOW); 
  statusLampu[3] = 0;
 } else if(pesan == "5off"){
  digitalWrite(led[4],LOW); 
  statusLampu[4] = 0;
 } 
 
 else if(pesan == "allon"){
  for(int i=0; i<5; i++){
    digitalWrite(led[i],HIGH);
    statusLampu[i] = 1;
  }
 } else if(pesan == "alloff"){
  for(int i=0; i<5; i++){
    digitalWrite(led[i],LOW);
    statusLampu[i] = 0;
  }
 }

 for(int j=0; j<5; j++){
  if(statusLampu[j]){
    lampu_on += String(j+1) + "  ";
  } if(!statusLampu[j]){
    lampu_off += String(j+1) + "  ";
  }
 } displayLCD(lampu_on, lampu_off);
 lampu_on=""; lampu_off="";
Serial.println(pesan);
delay(200);
}
