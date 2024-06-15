#include "DFRobotDFPlayerMini.h"
#define jumlah_button 21

byte button[jumlah_button] = {2,3,4,5,6,7,8,9,10,11,12,53,51,49,47,45,43,41,39,37,35};
bool array_value[jumlah_button] = {};

DFRobotDFPlayerMini myDFPlayer; 

void DFConnect(){
  if (!myDFPlayer.begin(Serial1)) {  
    Serial.println(F("Unable to begin:"));
    while(true){
      delay(0); 
    }
  }  Serial.println(F("DFPlayer Mini online."));
     myDFPlayer.volume(30); 
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  for(int i=0; i<jumlah_button; i++){
    pinMode(button[i], INPUT_PULLUP);
  }  DFConnect();
}

void loop() {
  for(int i=0; i<jumlah_button; i++){
    array_value[i] = digitalRead(button[i]);
    if(!array_value[i] ){
      myDFPlayer.play(i+1);
      Serial.println(button[i]);
      delay(2000);
    }
  }
}
