#include <SoftwareSerial.h>
SoftwareSerial myserial(8, 9);

#define NUM_OF_DISPLAY 14
#define BuzzerPin       2

#define strobePin  A1
#define dataPin   A2
#define clockPin  A3
char shiftOutBuffer[NUM_OF_DISPLAY]={0};

int homeScore = 0;  int a = 0;
int Quarter = 0;  
int guestScore = 0; int b = 0;

int menit2 = 0; int menit1 = 0;
int detik2 = 0; int detik1 = 0; 

int guestFoul = 0; int homeFoul = 0;

int timerOut1 = 0; int timerOut2 = 0;

unsigned long currentTime = 0;
unsigned long buzzerTime = 0;
static bool buz = false;

bool buzzerStatus = LOW;
bool timeoutStart = false;
bool timerStart = false;

byte segChar[]={
  0b11111100, // 0
  0b01100000, // 1
  0b11011010, // 2
  0b11110010, // 3
  0b01100110, // 4
  0b10110110, // 5
  0b10111110, // 6
  0b11100000, // 7
  0b11111110, // 8
  0b11110110, // 9
  0b11111101, // 10
  0b00000000, // 11 off
  0b11111111, // 12
};


void update_display(){
   String data = "";
    while(myserial.available()>0){
      data += char(myserial.read());
    } data.trim(); 
  Serial.println(data);

if(data == "Reset"){
  homeScore = 0;   a = 0;
  Quarter = 0;  
  guestScore = 0;  b = 0;
  menit2 = 0;  menit1 = 0;
  detik2 = 0;  detik1 = 0; 
  guestFoul = 0;  homeFoul = 0;
  timerOut1 = 0;  timerOut2 = 0;
  buzzerStatus = LOW;
  timeoutStart = false;
  timerStart = false;
}
  
  digitalWrite(strobePin, LOW);//===================================================

if(data == "GuestScore+"){
  homeScore++;
  if(homeScore >= 10){
    homeScore = 0;
    a++;
  }
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[homeScore]);
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[a]);
} else if(data == "GuestScore-"){
  homeScore--;
  if(homeScore < 0){
    homeScore = 9;
    a--;
  }
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[homeScore]);
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[a]);
} else {
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[homeScore]);
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[a]);
}

if(data == "Quarter"){
  Quarter++;
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[Quarter]);
} else {
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[Quarter]);
}

if(data == "HomeScore+"){
  guestScore++;
  if(guestScore >= 10){
    guestScore = 0;
    b++;
  }
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[guestScore]);
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[b]);
}else if(data == "HomeScore-"){
  guestScore--;
  if(guestScore < 0){
    guestScore = 9;
    b--;
  }
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[guestScore]);
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[b]);
} else {
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[guestScore]);
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[b]);
}

if(data == "GuestFoul+"){
  guestFoul = (guestFoul<5) ? guestFoul+1 : guestFoul;
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[guestFoul]);
} else if (data == "GuestFoul-"){
  guestFoul = (guestFoul>0) ? guestFoul-1 : guestFoul;
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[guestFoul]);
} else {
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[guestFoul]);
}

if(data == "ResetTout"){
  timeoutStart = false; 
  timerOut1 = 0;
  timerOut2 = 0;
}

if(data == "HomeTout+"){
  timerOut2 = (timerOut2<6) ? timerOut2+1 : timerOut2;
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[timerOut1]);
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[timerOut2]);
} else if(data == "HomeTout-"){
  timerOut2 = (timerOut2>0) ? timerOut2-1 : timerOut2;
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[timerOut1]);
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[timerOut2]);
} else {
  if(data == "GuestTout-" || data == "GuestTout+" ){
    timeoutStart = true; 
    if(timerOut2<=0 && timerOut1<=0){
       timeoutStart = false;
    }
  }
  if(timeoutStart){
    timerOut1--;
    timerOut2 = (timerOut1<0) ? timerOut2-1 : timerOut2;
    timerOut1 = (timerOut1<0) ? 9 : timerOut1;
    shiftOut(dataPin, clockPin, MSBFIRST, segChar[timerOut1]);
    shiftOut(dataPin, clockPin, MSBFIRST, segChar[timerOut2]);
    if(timerOut2<=0 && timerOut1<=0){
       digitalWrite(BuzzerPin,HIGH);
       delay(1000);
       digitalWrite(BuzzerPin,LOW);
       timeoutStart = false;
    }
  } else {
    shiftOut(dataPin, clockPin, MSBFIRST, segChar[timerOut1]);
    shiftOut(dataPin, clockPin, MSBFIRST, segChar[timerOut2]);
  }
}
  
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[12]);

if(data == "HomeFoul+"){
  homeFoul = (homeFoul<5) ? homeFoul+1 : homeFoul;
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[homeFoul]);
} else if (data == "HomeFoul-"){
  homeFoul = (homeFoul>0) ? homeFoul-1 : homeFoul;
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[homeFoul]);
} else {
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[homeFoul]);
}

if(data == "Start"){
  timerStart = (data == "Start") ? !timerStart : timerStart;
  timerStart = (menit2<=0 && menit1<=0 && detik2<=0 && detik1<=0) ? false : timerStart;
}
if(timerStart){
  detik1--; buz = true;
  if(detik1 < 0){
    detik1 = 9;  detik2--;
  } if(detik2 < 0){
    detik2 = 5;  menit1--;
  } if(menit1 < 0){
    menit1 = 9;  menit2--;
  } if(menit2<=0 && menit1<=0 && detik2<=0 && detik1<=0){
    timerStart = false; 
  } 
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[detik1]);
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[detik2]);
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[menit1]);
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[menit2]);
} else {
  if(data == "Timer+"){
    menit1 = (menit1<=9 && menit2<6) ? menit1+1 : menit1; 
    menit2 = (menit1>9 && menit2<6) ? menit2+1 : menit2;
    menit1 = (menit1>9) ? 0 : menit1;
    
  } else if(data == "Timer-"){
    menit1 = (menit1>=0 && menit2>=0 && !(menit1==0 && menit2==0) ) ? menit1-1 : menit1; 
    menit2 = (menit1<0 && menit2>0) ? menit2-1 : menit2;
    menit1 = (menit1<0) ? 9 : menit1;
  } else if(data == "Set20m"){
    menit2+=2;
    if(menit2 >= 6){
      menit2 = 6; menit1 = 0;
    }
  }
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[detik1]);
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[detik2]);
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[menit1]);
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[menit2]);
}
  digitalWrite(strobePin, HIGH);  //===================================================

if(data == "BELL"){
  digitalWrite(BuzzerPin,HIGH);
  delay(1000);
  digitalWrite(BuzzerPin,LOW);
}
  
}

void setup() {
   myserial.begin(9600);
   Serial.begin(115200);
  pinMode(strobePin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(BuzzerPin, OUTPUT);  
}

void loop() {
  if(millis() - currentTime > 1000){
    update_display();
    currentTime = millis();
  }

  if(menit2==0 && menit1==0 && detik2==0 && detik1<=10 && detik1!=0){
    if(millis() - buzzerTime > 500){
      digitalWrite(BuzzerPin,buzzerStatus);
      buzzerStatus =! buzzerStatus;
      buzzerTime = millis();
    }
  }
  if (menit2==0 && menit1==0 && detik2==0 && detik1==0){
    if(buz == 1){
      digitalWrite(BuzzerPin,HIGH);
      delay(1000);
      digitalWrite(BuzzerPin,LOW);
      buz = false;
    }
  }
}
