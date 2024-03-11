#include <DFRobotDFPlayerMini.h>

DFRobotDFPlayerMini myDFPlayer; 
int touch[5][6]= {{57,56,55,54,52,50},
                  {48,46,44,42,40,38},
                  {36,34,32,30,28,26},
                  {24,22,13,12,11,10},
                  {9,8,7,6,5,4}}; 
int value[5][6];

void DFConnect(){
  Serial.println();
  Serial.println(F("Menghidupkan DF PLAYER"));
  Serial.println(F("Tunggu Sekitar 3~5 detik"));
  
  myDFPlayer.begin(Serial1);
    
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(30); 
}
 
void setup() {
 for(int i=0; i<5; i++){
  for(int j=0; j<6; j++){
    pinMode(touch[i][j], INPUT);
  }
 }
  Serial.begin(9600); 
  Serial1.begin(9600);
  DFConnect();
}

void loop() {
  for(int i=0; i<5; i++){
    for(int j=0; j<6; j++){
     value[i][j] = digitalRead(touch[i][j]);
     if(value[i][j]){
      myDFPlayer.play((i*6)+(j+1));
      Serial.println(touch[i][j]);
      delay(1000);
    }
   }
 }
}
