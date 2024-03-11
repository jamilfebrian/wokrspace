#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

#define S0 8 //S0 di pin D8
#define S1 9 //S1 di pin D9
#define S2 10 //S2 di pin D10
#define S3 11 //S3 di pin D11
#define sensorOut 12 //Out di pin D12

SoftwareSerial mySerial(2, 3); //pin RX dan TX
DFRobotDFPlayerMini myDFPlayer; 

static int red = 0; //Variabel frekuensi
static int green = 0;
static int blue = 0;

int timer = 1000;

void DFConnect(){
  Serial.println();
  Serial.println(F("Mengaktifkan DF PLAYER"));
  Serial.println(F("Initializing DFPlayer ... (Tunggu Sekitar 3~5 detik)"));
  
  if (!myDFPlayer.begin(mySerial)) {  
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); 
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(15);  //Set volume value. From 0 to 30
}

void setup() {
  pinMode(S0, OUTPUT); //Setting S0 sebagai output
  pinMode(S1, OUTPUT); //Setting S1 sebagai output
  pinMode(S2, OUTPUT); //Setting S2 sebagai output
  pinMode(S3, OUTPUT); //Setting S3 sebagai output
  pinMode(sensorOut, INPUT); //Setting Outsensor sebagai Input

  digitalWrite(S0,HIGH); // setting 20% frequency
  digitalWrite(S1,LOW);
  Serial.begin(9600); //Mulai komunikasi serial
  mySerial.begin(9600);
  DFConnect();
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
  bacaWarna(S2,S3);
  
  if ((red<423 && red>245)&&(green<500 && green>338)&&(blue<530 && blue>317)){
    Serial.println("Hitam");
    myDFPlayer.play(1);
    delay(timer);
  } 
  else if ((red<350 && red>258)&&(green<461 && green>363)&&(blue<466 && blue>336)){
    Serial.println("Coklat");
    myDFPlayer.play(10);
    delay(timer);
  } 
  else if ((red<142 && red>123)&&(green<343 && green>319)&&(blue<312 && blue>282)){
    Serial.println("Merah");
    myDFPlayer.play(9);
    delay(timer);
  } 
  else if ((red<127 && red>80)&&(green<300 && green>182)&&(blue<310 && blue>169)){
    Serial.println("Jingga");
    myDFPlayer.play(8);
    delay(timer);
  } 
  else if ((red<90 && red>46)&&(green<133 && green>68)&&(blue<217 && blue>115)){
    Serial.println("Kuning");
    myDFPlayer.play(7);
    delay(timer);
  } 
  else if ((red<232 && red>143)&&(green<198 && green>130)&&(blue<284 && blue>166)){
    Serial.println("Hijau");
    myDFPlayer.play(6);
    delay(timer);
  } 
  else if ((red<335 && red>250)&&(green<293 && green>217)&&(blue<162 && blue>126)){
    Serial.println("Biru");
    myDFPlayer.play(5);
    delay(timer);
  } 
  else if ((red<174 && red>73)&&(green<383 && green>193)&&(blue<245 && blue>107)){
    Serial.println("Ungu");
    myDFPlayer.play(4);
    delay(timer);
  } 
  else if ((red<233 && red>215)&&(green<301 && green>280)&&(blue<275 && blue>265)){
    Serial.println("Abu-Abu");
    myDFPlayer.play(3);
    delay(timer);
  } 
  else if ((red<107 && red>62)&&(green<125 && green>69)&&(blue<98 && blue>58)){
    Serial.println("Putih");
    myDFPlayer.play(2);
    delay(timer);
  } 
}
