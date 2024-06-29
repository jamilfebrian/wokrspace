#include "Adafruit_VL53L0X.h"
#include "DFRobotDFPlayerMini.h"
DFRobotDFPlayerMini myDFPlayer; 

#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31

#define SHT_LOX1 A1
#define SHT_LOX2 A2
#define led 13

Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;

unsigned long currTime=0;

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

void setID() {
  digitalWrite(SHT_LOX1, LOW);    
  digitalWrite(SHT_LOX2, LOW);
  delay(10);
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);

  if(!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while(1);
  } delay(10);
  
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  if(!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while(1);
  }
}

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  DFConnect();

  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);

  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  setID();
}

void loop() {
  lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!

  int rangeDepan = measure1.RangeMilliMeter;
  int rangeSamping = measure2.RangeMilliMeter;

  if(rangeDepan <= 50){
    digitalWrite(led,HIGH);
    if(millis() - currTime >= 2500){
      myDFPlayer.play(1); 
      currTime = millis();
    }
  } else if (rangeSamping <= 50){
    digitalWrite(led,HIGH);
    if(millis() - currTime >= 2500){
      myDFPlayer.play(2);
      currTime = millis();
    } 
  } else {
    digitalWrite(led,LOW);
  }

  delay(100);
}
