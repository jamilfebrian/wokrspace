#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 2);

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  pinMode(10, INPUT); //LO+
  pinMode(11, INPUT); //LO-
  pinMode(A0, INPUT);

}

void loop() {
  
  if((digitalRead(10)==1) || (digitalRead(11)==1)){
    Serial.println("!");
  } else {
    mySerial.println(analogRead(A0));
  }
  delay(10);
}
