
#include "Arduino.h"
#include "Lampu.h"
 
Lampu::Lampu(int lampu1, int lampu2, int lampu3, int lampu4, int lampu5, int lampu6){
    led1=lampu1;
    led2=lampu2;
    led3=lampu3;
    led4=lampu4;
    led5=lampu5;
    led6=lampu6;
}

void Lampu::pinSetup(){
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(led4, OUTPUT);
    pinMode(led5, OUTPUT);
    pinMode(led6, OUTPUT);
}

void Lampu::Sequence(){
    digitalWrite(led1, HIGH);
    delay(100);
    digitalWrite(led2, HIGH);
    delay(100);
    digitalWrite(led3, HIGH);
    delay(100);
    digitalWrite(led4, HIGH);
    delay(100);
    digitalWrite(led5, HIGH);
    delay(100);
    digitalWrite(led6, HIGH);
    delay(100);

    digitalWrite(led1, LOW);
    delay(100);
    digitalWrite(led2, LOW);
    delay(100);
    digitalWrite(led3, LOW);
    delay(100);
    digitalWrite(led4, LOW);
    delay(100);
    digitalWrite(led5, LOW);
    delay(100);
    digitalWrite(led6, LOW);
    delay(100);

}