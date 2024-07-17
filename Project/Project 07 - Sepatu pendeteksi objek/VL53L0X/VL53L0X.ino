#include "Adafruit_VL53L0X.h"

#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31

#define SHT_LOX1 4
#define SHT_LOX2 5
#define led 12
#define buzzer 13

Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;

unsigned int currTime=0;

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
  pinMode(buzzer, OUTPUT);
  Serial.begin(115200);

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

  Serial.print(rangeDepan);
  Serial.print("  ");
  Serial.println(rangeSamping);


  if(rangeDepan <= 500){
    digitalWrite(led,HIGH);
    if(millis() - currTime >= 2500){
      digitalWrite(buzzer, HIGH);
      currTime = millis();
    }
  } else if (rangeSamping <= 500){
    digitalWrite(led,HIGH);
    if(millis() - currTime >= 2500){
      digitalWrite(buzzer, HIGH);
      currTime = millis();
    } 
  } else {
    digitalWrite(led,LOW);
    digitalWrite(buzzer, LOW);
  }

  delay(100);
}
