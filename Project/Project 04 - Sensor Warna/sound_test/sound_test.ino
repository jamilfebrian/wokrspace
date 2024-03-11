#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"
SoftwareSerial mySerial(2, 3); //pin RX dan TX
DFRobotDFPlayerMini myDFPlayer; 

void DFConnect(){
  Serial.println();
  Serial.println(F("Ma iduik an DF PLAYER"));
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
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
}
void setup() {
 Serial.begin(9600); //Mulai komunikasi serial
  mySerial.begin(9600);
  DFConnect();
  myDFPlayer.play(7);
}

void loop() {
 

}
