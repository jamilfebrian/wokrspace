#include <SoftwareSerial.h>
SoftwareSerial myserial(4,5);

#define strobePin  A1
#define dataPin   A2
#define clockPin  A3

bool logic1 = true;

byte segChar[]={
  0b00000000, // 0
  0b00001000, // 1
  0b00011000, // 2
  0b00111000, // 3
  0b01111000, // 4
  0b11111000, // 5
  0b11111111, // 6
  0b11111111, // 7
  0b11111111, // 8
  0b11111111, // 9
  0b11111111, // 10
  0b11111111, // 11 off
  0b11111111, // 12
};


unsigned long cTime = 0;

String dataReceive(){
  String data = "";
    while(myserial.available()>0){
      data += char(myserial.read());
    } data.trim(); 
  return data;
} 

void resetDisplay(){
    digitalWrite(strobePin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, segChar[0]); 
      shiftOut(dataPin, clockPin, MSBFIRST, segChar[0]); 
      shiftOut(dataPin, clockPin, MSBFIRST, segChar[0]); 
      shiftOut(dataPin, clockPin, MSBFIRST, segChar[0]); 
      shiftOut(dataPin, clockPin, MSBFIRST, segChar[0]); 
      shiftOut(dataPin, clockPin, MSBFIRST, segChar[0]); 
      shiftOut(dataPin, clockPin, MSBFIRST, segChar[0]); 
      shiftOut(dataPin, clockPin, MSBFIRST, segChar[0]); 
      shiftOut(dataPin, clockPin, MSBFIRST, segChar[0]); 
      shiftOut(dataPin, clockPin, MSBFIRST, segChar[0]); 
      shiftOut(dataPin, clockPin, MSBFIRST, segChar[0]); 
      shiftOut(dataPin, clockPin, MSBFIRST, segChar[0]); 
    digitalWrite(strobePin, HIGH);
}


void setup() {
   myserial.begin(9600);
   Serial.begin(115200);
   pinMode(strobePin, OUTPUT);
   pinMode(dataPin, OUTPUT);
   pinMode(clockPin, OUTPUT);
   pinMode(13, OUTPUT);

   digitalWrite(13, HIGH);
   
}

void loop() {

  String myData = dataReceive();
  delay(300);


  if(myData != "" ){
    
    if(myData == "Hres"){
      resetDisplay(); return;
    } else if(myData == "Gres"){
      resetDisplay(); return;
    }
    Serial.println(myData) ;
    
    digitalWrite(strobePin, LOW);
    for(int i=12; i>0; i--){
      shiftOut(dataPin, clockPin, MSBFIRST, segChar[(int)myData[i] - 48]); 
    }
    digitalWrite(strobePin, HIGH);
  } 

  if(logic1){
    resetDisplay();
    logic1 = false;
  }
  
}
