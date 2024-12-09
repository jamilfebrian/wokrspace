#include <LiquidCrystal_I2C.h>

float revolutions=0;
int rpm=0; // max value 32,767 16 bit
long  startTime=0;
long  elapsedTime;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void interruptFunction(){  
  revolutions++;
}

float kecepatan(int _rpm){
  float D = 0.045;
  float phi = 3.141509;
  float RPM = float(_rpm);

  float speedValue = (RPM*D*phi)/60;

  return speedValue;
}

void setup() {
  Serial.begin(9600);

  lcd.init(); lcd.backlight(); lcd.clear();
}

void loop() {
  revolutions=0; rpm=0;
  startTime=millis();         
  attachInterrupt(digitalPinToInterrupt(2),interruptFunction,RISING);
  delay(1000);
  detachInterrupt(2);
  
  elapsedTime=millis()-startTime;

  if(revolutions>0) {
    rpm=(max(1, revolutions) * 60000) / elapsedTime;
  }

  float speedValue = round(kecepatan(rpm)*100)/100;
  speedValue = speedValue * 3.6;

lcd.setCursor(0,0); lcd.print("RPM : "); lcd.print(rpm); lcd.print(" rpm          ");
lcd.setCursor(0,1); lcd.print("v   : "); lcd.print(speedValue); lcd.print(" km/H          ");
  delay(100);
}
