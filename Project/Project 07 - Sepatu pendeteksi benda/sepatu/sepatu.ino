#include "DFRobotDFPlayerMini.h"
DFRobotDFPlayerMini myDFPlayer; 

#define echo1  5
#define trig1  4
#define echo2  3
#define trig2  2
#define led  13

int deteksi_jarak = 20;
unsigned long currTime=0;

long distance(int TRIG, int ECHO){
  digitalWrite(TRIG, LOW);  delayMicroseconds(8);
  digitalWrite(TRIG, HIGH); delayMicroseconds(8);
  digitalWrite(TRIG, LOW);  delayMicroseconds(8);
  long durasi = pulseIn(ECHO, HIGH); 
  long jarak = (durasi / 2) / 29.1;
  return jarak;
}

void DFConnect(){
  if (!myDFPlayer.begin(Serial)) {  
    digitalWrite(led,LOW);
    while(true){
      delay(0); 
    }
  }
  digitalWrite(led,HIGH);
  myDFPlayer.volume(30);
  delay(1000);
  digitalWrite(led,LOW);
}

void setup() {
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT); 
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT); 
  Serial.begin(9600);
  DFConnect();
}

void loop() {
  long depan = distance(trig1,echo1);
  long samping = distance(trig2,echo2);

  if(depan<=deteksi_jarak && depan!=0){
    digitalWrite(led,HIGH);
    if(millis() - currTime >= 2500){
      myDFPlayer.play(1); 
      currTime = millis();
    }
  } else if (samping<=deteksi_jarak && samping!=0){
    digitalWrite(led,HIGH);
    if(millis() - currTime >= 2500){
      myDFPlayer.play(2);
      currTime = millis();
    } 
  } else {
    digitalWrite(led,LOW);
  }
}
