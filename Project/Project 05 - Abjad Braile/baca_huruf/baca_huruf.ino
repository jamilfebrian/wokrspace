#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySerial(3, 2); //pin RX dan TX
DFRobotDFPlayerMini myDFPlayer; 

#define button1 11 
#define button2 10 
#define button3 9 
#define button4 8 
#define button5 7 
#define button6 6 
#define button7 5
#define button8 4
#define ledGreen 13
#define ledRed 12
#define buzzerPin A0

int counter = 0;
static int a=0,b=0,c=0,d=0,e=0,f=0;
int set_volume = 30;  //Set volume value. From 0 to 30

void DFConnect(){
  Serial.println();
  Serial.println(F("Menghidupkan DF PLAYER"));
  Serial.println(F("Tunggu Sekitar 3~5 detik"));
  
  if (!myDFPlayer.begin(mySerial)) {  
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); 
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(set_volume); 
}

void buzzer(int index = 1, int period = 75){
  for(int i=0; i<index; i++){
    digitalWrite(buzzerPin, HIGH);
    delay(period);
    digitalWrite(buzzerPin, LOW);
    delay(period);
  } 
}

void setup() {
 pinMode(button1,INPUT);
 pinMode(button2,INPUT);
 pinMode(button3,INPUT);
 pinMode(button4,INPUT);
 pinMode(button5,INPUT);
 pinMode(button6,INPUT);
 pinMode(button7,INPUT);
 pinMode(button8,INPUT);
 pinMode(ledRed, OUTPUT);
 pinMode(ledGreen, OUTPUT);
 pinMode(buzzerPin, OUTPUT);


  Serial.begin(9600); 
  mySerial.begin(9600);
  DFConnect();
   buzzer();
}

void loop() {
 int A = digitalRead(button1);
 int B = digitalRead(button2);
 int C = digitalRead(button3);
 int D = digitalRead(button4);
 int E = digitalRead(button5);
 int F = digitalRead(button6);
 int play = digitalRead(button7);
 int hapus = digitalRead(button8);


 if( A || B || C || D || E || F || play ){
  digitalWrite(ledGreen,HIGH);
  if(counter == 0){
    buzzer();
    counter++;
  }
 } else if(hapus){
   digitalWrite(ledRed,HIGH);
 } else {
  digitalWrite(ledGreen,LOW);
  digitalWrite(ledRed,LOW);
  counter = 0;
 }

// mengambil nilai logika huruf
 if(A){a = 1;} 
 if(B){b = 1;} 
 if(C){c = 1;} 
 if(D){d = 1;} 
 if(E){e = 1;} 
 if(F){f = 1;} 

if(hapus){
  a=0,b=0,c=0,d=0,e=0,f=0;
}

Serial.print(a);Serial.print(b);Serial.print(c);Serial.print(d);Serial.print(e);Serial.print(f);Serial.print(" ");Serial.print(play);Serial.println(hapus);

if (play==1){
  if (a==1 && b==0 && c==0 && d==0 && e==0 && f==0){          //huruf A
    myDFPlayer.play(1);
  } else if (a==1 && b==1 && c==0 && d==0 && e==0 && f==0){   //huruf B
    myDFPlayer.play(2);
  } else if (a==1 && b==0 && c==0 && d==1 && e==0 && f==0){   //huruf C
    myDFPlayer.play(3);
  } else if (a==1 && b==0 && c==0 && d==1 && e==1 && f==0){   //huruf D
    myDFPlayer.play(4);
  } else if (a==1 && b==0 && c==0 && d==0 && e==1 && f==0){   //huruf E
    myDFPlayer.play(5);
  } else if (a==1 && b==1 && c==0 && d==1 && e==0 && f==0){   //huruf F
    myDFPlayer.play(6);
  } else if (a==1 && b==1 && c==0 && d==1 && e==1 && f==0){   //huruf G
    myDFPlayer.play(7);
  } else if (a==1 && b==1 && c==0 && d==0 && e==1 && f==0){   //huruf H
    myDFPlayer.play(8);
  } else if (a==0 && b==1 && c==0 && d==1 && e==0 && f==0){   //huruf I
    myDFPlayer.play(9);
  } else if (a==0 && b==1 && c==0 && d==1 && e==1 && f==0){   //huruf J
    myDFPlayer.play(10);
  } else if (a==1 && b==0 && c==1 && d==0 && e==0 && f==0){   //huruf K
    myDFPlayer.play(11);
  } else if (a==1 && b==1 && c==1 && d==0 && e==0 && f==0){   //huruf L
    myDFPlayer.play(12);
  } else if (a==1 && b==0 && c==1 && d==1 && e==0 && f==0){   //huruf M
    myDFPlayer.play(13);
  } else if (a==1 && b==0 && c==1 && d==1 && e==1 && f==0){   //huruf N
    myDFPlayer.play(14);
  } else if (a==1 && b==0 && c==1 && d==0 && e==1 && f==0){   //huruf O
    myDFPlayer.play(15);
  } else if (a==1 && b==1 && c==1 && d==1 && e==0 && f==0){   //huruf P
    myDFPlayer.play(16);
  } else if (a==1 && b==1 && c==1 && d==1 && e==1 && f==0){   //huruf Q
    myDFPlayer.play(17);
  } else if (a==1 && b==1 && c==1 && d==0 && e==1 && f==0){   //huruf R
    myDFPlayer.play(18);
  } else if (a==0 && b==1 && c==1 && d==1 && e==0 && f==0){   //huruf S
    myDFPlayer.play(19);
  } else if (a==0 && b==1 && c==1 && d==1 && e==1 && f==0){   //huruf T
    myDFPlayer.play(20);
  } else if (a==1 && b==0 && c==1 && d==0 && e==0 && f==1){   //huruf U
    myDFPlayer.play(21);
  } else if (a==1 && b==1 && c==1 && d==0 && e==0 && f==1){   //huruf V
    myDFPlayer.play(22);
  } else if (a==0 && b==1 && c==0 && d==1 && e==1 && f==1){   //huruf W
    myDFPlayer.play(23);
  } else if (a==1 && b==0 && c==1 && d==1 && e==0 && f==1){   //huruf X
    myDFPlayer.play(24);
  } else if (a==1 && b==0 && c==1 && d==1 && e==1 && f==1){   //huruf Y
    myDFPlayer.play(25);
  } else if (a==1 && b==0 && c==1 && d==0 && e==1 && f==1){   //huruf Z
    myDFPlayer.play(26);
  }  a=0,b=0,c=0,d=0,e=0,f=0; //reset nilai logika
} 
}
