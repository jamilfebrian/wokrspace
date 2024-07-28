#include <Servo.h>

#define button1 A1
#define button2 A2

#define servoPin1   9
#define servoPin2   10

Servo servoPintuMasuk;
Servo servoPintuKeluar;

bool statusServoMasuk;
bool statusServoKeluar;

unsigned long currenTime1 = 0;
unsigned long currenTime2 = 0;

void setup() {
  Serial.begin(9600);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  
  servoPintuMasuk.attach(servoPin1); 
  servoPintuKeluar.attach(servoPin2);
  servoPintuMasuk.write(90);
  servoPintuKeluar.write(0);
}

void loop() {
  bool pintuMasuk = digitalRead(button1);
  bool pintuKeluar = digitalRead(button2);

  Serial.print(pintuMasuk);Serial.print(" "); Serial.println(pintuKeluar);

  if(millis() - currenTime1 > 10000){
    if(pintuMasuk){
      servoPintuMasuk.write(0);
      currenTime1 = millis();       
    } else {
      servoPintuMasuk.write(90);
    }
  }

  if(millis() - currenTime2 > 10000){
    if(pintuKeluar){
      servoPintuKeluar.write(0);
      currenTime2 = millis();       
    } else {
      servoPintuKeluar.write(90);
    } 
  }
}
