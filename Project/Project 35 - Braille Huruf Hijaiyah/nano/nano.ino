#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

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

int counter = 0;
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
  DFConnect();
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
  if(counter == 0){
    counter++;
  }
 } else if(hapus){
   digitalWrite(ledRed,HIGH);
 } else {
  digitalWrite(ledGreen,LOW);
  digitalWrite(ledRed,LOW);
  counter = 0;
 }

// mengambil nilai logika huruf
 if(A){kode[0] = 1;} 
 if(B){kode[1] = 1;} 
 if(C){kode[2] = 1;} 
 if(D){kode[3] = 1;} 
 if(E){kode[4] = 1;} 
 if(F){kode[5] = 1;} 

  kodeBraile = String(kode[0]) + String(kode[1]) + String(kode[2]) + String(kode[3]) + String(kode[4]) + String(kode[5]);
  Serial.println(kodeBraile);
  
  if(hapus){
    for(int i=0; i<6; i++){
      kode[i] = 0;
    }
  }
  
  if (play==1){
    if(kodeBraile == "100000"){  //alif
        myDFPlayer.play(1);
    } else if(kodeBraile == "110000"){
        myDFPlayer.play(2);
    }
  }
}
