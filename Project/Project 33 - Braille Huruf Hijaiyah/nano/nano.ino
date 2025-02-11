#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include "DFRobotDFPlayerMini.h"

LiquidCrystal_I2C lcd(0x27,16,2); 
SoftwareSerial mySerial(3, 2); //pin RX dan TX
DFRobotDFPlayerMini myDFPlayer; 

#define button1 4
#define button2 5 
#define button3 6 
#define button4 7 
#define button5 8 
#define button6 9 

#define PLAY   10
#define DELETE 11

#define ledGreen 13
#define ledRed 12

static int kode[6];
static String kodeBraile;
const String BRAILE[28] = {
  "100000", //alif
  "110000", //Ba
  "011110", //Taa
  "100111", //Tsa
  "010110", //Jeem
  "100011", //Haa
  "101101", //Khoo
  "100110", //Daal
  "011101", //Dzal
  "111010", //Raa
  "101011", //Zaa
  "011100", //Sin
  "100101", //Syin
  "111101", //Shod
  "110101", //Dhod
  "011111", //Tho
  "111111", //Dzhoo
  "111011", //Ain
  "110001", //Ghain
  "110100", //Faa
  "111110", //Qaf
  "101000", //Kaf
  "111000", //Lam
  "101100", //Mim
  "101110", //Nun
  "110010", //Haa
  "010111", //Waw
  "010100", //Yaa
};

int set_volume = 30;  //Set volume value. From 0 to 30

void DFConnect(){
  Serial.println();
  Serial.println(F("Menghidupkan DF PLAYER"));
  Serial.println(F("Tunggu Sekitar 3~5 detik"));
  
  if (!myDFPlayer.begin(mySerial)) {  
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); 
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(set_volume); 
}

void setup() {
 pinMode(button1,INPUT);
 pinMode(button2,INPUT);
 pinMode(button3,INPUT);
 pinMode(button4,INPUT);
 pinMode(button5,INPUT);
 pinMode(button6,INPUT);
 
 pinMode(PLAY,INPUT);
 pinMode(DELETE,INPUT);
 pinMode(ledRed, OUTPUT);
 pinMode(ledGreen, OUTPUT);

  Serial.begin(9600); 
  mySerial.begin(9600);
  
  lcd.init(); lcd.backlight();
  
  lcd.setCursor(0,0);
  lcd.print("  Tugas  Akhir  ");
  lcd.setCursor(0,1);
  lcd.print("                ");

  DFConnect(); delay(1000);

  lcd.setCursor(0,0);
  lcd.print("    DELMAWATI   ");
  lcd.setCursor(0,1);
  lcd.print("    21003016    ");
}

void loop() {
 int A = digitalRead(button1);
 int B = digitalRead(button2);
 int C = digitalRead(button3);
 int D = digitalRead(button4);
 int E = digitalRead(button5);
 int F = digitalRead(button6);
 int play = digitalRead(PLAY);
 int hapus = digitalRead(DELETE);


 if( A || B || C || D || E || F || play ){
  digitalWrite(ledGreen,HIGH);
 } else if(hapus){
   digitalWrite(ledRed,HIGH);
   for(int i=0; i<6; i++){
      kode[i] = 0;
    }
 } else {
  digitalWrite(ledGreen,LOW);
  digitalWrite(ledRed,LOW);
 }

// mengambil nilai logika huruf
 if(A){kode[0] = 1; } 
 if(B){kode[1] = 1; } 
 if(C){kode[2] = 1; } 
 if(D){kode[3] = 1; } 
 if(E){kode[4] = 1; } 
 if(F){kode[5] = 1; } 

  kodeBraile = String(kode[0]) + String(kode[1]) + String(kode[2]) + String(kode[3]) + String(kode[4]) + String(kode[5]);
  Serial.println(kodeBraile);
  
  if (play==1){
    for(int i=0; i<28; i++){
      if(kodeBraile == BRAILE[i]){
         digitalWrite(ledGreen, HIGH);
         myDFPlayer.play(i+1);
         Serial.println(i+1);
         delay(2000);
      } 
    }
    digitalWrite(ledGreen, LOW);
    for(int i=0; i<6; i++){
      kode[i] = 0;
    }
  }
}
