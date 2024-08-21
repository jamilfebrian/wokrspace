#include <ESP32Servo.h>
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

#define servoPin1  26
#define servoPin2  27
#define servoPin3  18
#define servoPin4  19
#define IR_Penutup 35

Servo myServo1, myServo2, myServo3, myServo4;
DFRobotDFPlayerMini myDFPlayer;

unsigned long cTime = 0;

void setup() {
  Serial2.begin(9600, SERIAL_8N1, /*rx =*/16, /*tx =*/17);
  Serial.begin(9600); Serial.println("Memulai");

  pinMode(IR_Penutup, INPUT);
  
  myServo1.attach(servoPin1); myServo1.write(90);
  myServo2.attach(servoPin2); myServo2.write(90);
  myServo3.attach(servoPin3); myServo3.write(90);
  myServo4.attach(servoPin4); myServo4.write(90);
  delay(1000);

  if (!myDFPlayer.begin(Serial2, /*isACK = */true, /*doReset = */true)) { 
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  } Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(30); 
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);  
}

void loop() {

  bool IR_Penutup_Value = digitalRead(IR_Penutup);
  Serial.println(IR_Penutup_Value);

  if(!IR_Penutup_Value){
    if(millis() - cTime > 2000){
      myDFPlayer.play(1);
      myServo1.write(0);
      myServo2.write(0);
      myServo3.write(0);
      myServo4.write(0);
      cTime = millis();
    }
  } else {
    myServo1.write(90);
    myServo2.write(90);
    myServo3.write(90);
    myServo4.write(90);
  }
}
