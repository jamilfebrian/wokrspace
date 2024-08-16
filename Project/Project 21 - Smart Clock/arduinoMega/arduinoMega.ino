#include "DFRobotDFPlayerMini.h"
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

DFRobotDFPlayerMini myDFPlayer; 
LiquidCrystal_I2C lcd(0x27,16,2); 
Servo myServo; 

const int BUZZER = 23;
const int adjustPin = 37;
const int LED[12] = {2,3,4,5,6,7,8,9,10,11,12,13};
const int stepper[4] = {31, 29, 27, 25};

#define stepperSpeed 40
#define stepperStep 42


void buzzer(int x=2, int timer=100){
  for(int i=0; i<x; i++){
    digitalWrite(BUZZER,HIGH); delay(timer);
    digitalWrite(BUZZER,LOW); delay(timer);
  }
}

void setup() {
  pinMode(BUZZER, OUTPUT);
  pinMode(adjustPin, INPUT_PULLUP);
  buzzer();

  Serial.begin(9600);
  Serial1.begin(9600);
  
  lcd.init(); lcd.clear(); lcd.backlight();
  lcd.setCursor(0,0); lcd.print("  TUGAS AKHIR   ");
  lcd.setCursor(0,1); lcd.print("                ");
  delay(1000);

  for(int i=0; i<12; i++){
    pinMode(LED[i], OUTPUT);
  }
  for(int i=0; i<4; i++){
    pinMode(stepper[i], OUTPUT);
  }

  DFPlayerConnect(); myDFPlayer.play(12);
  lcd.setCursor(0,0); lcd.print("   IYUT SEPMI   ");
  lcd.setCursor(0,1); lcd.print("     GUSTIA     ");
}

void loop() {
  for(int i=0; i<12; i++){
      clockWise(stepperStep);
      digitalWrite(LED[i], HIGH);
      if(!digitalRead(adjustPin)){
         break;
      }
      myDFPlayer.play(i+1);
      Serial.println(i+1);
  }
  for(int i=0; i<12; i++){
     digitalWrite(LED[i], LOW);
  }
}

/*Konfigurasi DFPlayer */
void DFPlayerConnect(){
  myDFPlayer.begin(Serial1);
  myDFPlayer.setTimeOut(500);
  myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
  myDFPlayer.volume(30);
}

//fungsi untuk mengatur arah putar stepper searah jarum jam
void counterClockWise(int step){
  for(int h=0; h<=step; h++){
    for(int i=0; i<4; i++){
      for(int j=0; j<4; j++){
        if(!digitalRead(adjustPin)){
          return;
        }
        if(i==j){
          digitalWrite(stepper[j], HIGH);
        } else {
          digitalWrite(stepper[j], LOW);
        }
      }
      delay(stepperSpeed);
    }
  }
}

//fungsi untuk mengatur arah putar stepper berlawanan arah jarum jam
void clockWise(int step){
  for(int h=0; h<=step; h++){
    for(int i=3; i>=0; i--){
      for(int j=3; j>=0; j--){
        if(!digitalRead(adjustPin)){
          return;
        }
        if(i==j){
          digitalWrite(stepper[j], HIGH);
        } else {
          digitalWrite(stepper[j], LOW);
        }
      }
     delay(stepperSpeed);
    }
  }
}
