#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"
SoftwareSerial mySerial(2, 3); //pin RX dan TX
DFRobotDFPlayerMini myDFPlayer; 

void DFConnect(){
  Serial.println();
  Serial.println(F("Ma iduik an DF PLAYER"));
  Serial.println(F("Initializing DFPlayer ... (Tunggu Sekitar 3~5 detik)"));
  
  if (!myDFPlayer.begin(Serial2)) {  
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); 
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
}
void setup() {
 Serial.begin(115200); //Mulai komunikasi serial
  Serial2.begin(9600);
  DFConnect();
  myDFPlayer.play(1);
  delay(2000);
  Serial.println(F("DFPlayer Mini play."));
}

void loop() {
  for(int i=1;i<=4; i++){
    myDFPlayer.play(i);
    delay(2000);
  }
}
