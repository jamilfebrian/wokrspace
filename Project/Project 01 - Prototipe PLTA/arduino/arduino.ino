int A=8; int B=9; int C=10; int D=11;
int x=0,y=0;

#include <NewPing.h>
#define VCC 4
#define GND 7
#define TRIGGER_PIN  5  
#define ECHO_PIN     6 
#define MAX_DISTANCE 300 

int button=0;
int kecepatanStepper=2;
int on=0;
NewPing jarak(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void Putaran1(){ 
  digitalWrite(A,1);
  digitalWrite(B,0);
  digitalWrite(C,0);
  digitalWrite(D,0);
}
void Putaran2(){
  digitalWrite(A,0);
  digitalWrite(B,1);
  digitalWrite(C,0);
  digitalWrite(D,0);
}
void Putaran3(){
  digitalWrite(A,0);
  digitalWrite(B,0);
  digitalWrite(C,1);
  digitalWrite(D,0);
}
void Putaran4(){
  digitalWrite(A,0);
  digitalWrite(B,0);
  digitalWrite(C,0);
  digitalWrite(D,1);
}
void Clockwise(){
  Putaran1(); delay(kecepatanStepper);
  Putaran2(); delay(kecepatanStepper);
  Putaran3(); delay(kecepatanStepper);
  Putaran4(); delay(kecepatanStepper);
}
void antiClockwise(){
  Putaran4(); delay(kecepatanStepper);
  Putaran3(); delay(kecepatanStepper);
  Putaran2(); delay(kecepatanStepper);
  Putaran1(); delay(kecepatanStepper);
}

int tinggiAir(){
  int ultrasonik = map(jarak.ping_cm(),0,20,20,0);
  return ultrasonik;
}

int teganganMotor(){
  int sensorValue = analogRead(A0);
  int x = map(sensorValue, 0,1024,0,750); 
  return x;
}

String kirimData(){
  int V = teganganMotor(); delay(1);
  int H = tinggiAir(); delay(10);
  String dataKirim = String(V)+"#"+String(H);
  return dataKirim;
}

void setup(){
  pinMode(A,OUTPUT); pinMode(B,OUTPUT);
  pinMode(C,OUTPUT); pinMode(D,OUTPUT);
  pinMode(VCC,OUTPUT); pinMode(GND,OUTPUT); 
  pinMode(3,OUTPUT); pinMode(2,INPUT_PULLUP);
  digitalWrite(VCC,HIGH); digitalWrite(GND,LOW); digitalWrite(3,LOW);
  Serial.begin(9600);
  pinMode(A0, INPUT);

  for(int STEP=0; STEP<150; STEP++){
  antiClockwise();
  }
}

void loop() {
  String dataKirim = kirimData();
  button = digitalRead(2);
  if(button==0){
    on=1;
  }
  int V = teganganMotor();
  String minta = "";
  while(Serial.available()>0){
    minta += char(Serial.read());
  } minta.trim();
  if (minta == "YA"){
    Serial.println(dataKirim);
  }   minta = "";
  
  if(on==1){

    
    if(V<250 && x<4){
      for(int STEP=0; STEP<30; STEP++){
        Clockwise();
      } delay(2000);
        x++; y=0;
    } else if(V<250 && x>4){
      //diam
    } else if(V>250 && y==0){
      for(int STEP=0; STEP<150; STEP++){
        antiClockwise();
      } x=0; y=1;
    }
  }
}
