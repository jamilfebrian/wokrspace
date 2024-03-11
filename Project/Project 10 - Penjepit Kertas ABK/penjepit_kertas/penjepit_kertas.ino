#include <Servo.h>
Servo myservo1, myservo2, myservo3;

#define servo1 9
#define servo2 10
#define servo3 11
#define button1 8
#define button2 7
#define ledR 12
#define ledG 13
#define ledB 6

int tombol1=0, tombol2=0;

void setup(){
  myservo1.attach(servo1);
  myservo2.attach(servo2);
  myservo3.attach(servo3);
  myservo1.write(0);
  myservo2.write(0);
  myservo3.write(0);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);

}

void loop(){
  int status1 = digitalRead(button1);
  int status2 = digitalRead(button2);
  
  if(status1){
    tombol1 += 1;
    if(tombol1>=2){tombol1=0;}
    delay(500);
  } if(status2){
    tombol2 += 1;
    if(tombol2>=2){tombol2=0;}
    delay(500);
  }

  if(tombol1==1){
      myservo1.write(90);
      myservo2.write(90);
      digitalWrite(ledG,HIGH);
  } else if (tombol1==0){
      myservo1.write(0);
      myservo2.write(0);
      digitalWrite(ledG,LOW);
  }

  if(tombol2==1){
      myservo3.write(90);
      digitalWrite(ledB,HIGH);
  } else if (tombol2==0){
      myservo3.write(0);
      digitalWrite(ledB,LOW);
  }
}
