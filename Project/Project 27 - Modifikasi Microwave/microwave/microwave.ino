#include <OneWire.h>
#include <DallasTemperature.h>
#include <NewPing.h>
#include <LiquidCrystal_I2C.h>

#define BuzzerPin 13
#define DS18B20    3

#define TRIGPIN_1     6
#define ECHOPIN_1     7
#define TRIGPIN_2     8
#define ECHOPIN_2     9

float revolutions=0;
int rpm=0; // max value 32,767 16 bit
long  startTime=0;
long  elapsedTime;

#define PANJANG 12
#define LEBAR   12

byte derajat[8] = {
    0b00111,
    0b00101,
    0b00111,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000
  };

OneWire oneWire(DS18B20);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 20, 4);
NewPing sonar1(TRIGPIN_1, ECHOPIN_1, 200); 
NewPing sonar2(TRIGPIN_2, ECHOPIN_2, 200); 

void buzzer(int index = 3, int timer = 80){
   for(int i=0; i<index; i++){
      digitalWrite(BuzzerPin, HIGH);
      delay(timer);
      digitalWrite(BuzzerPin, LOW);
      delay(timer);
  }
}

void interruptFunction(){  
  revolutions++;
}

void setup() {
  Serial.begin(9600);
  pinMode(BuzzerPin, OUTPUT);

  lcd.init(); lcd.backlight(); lcd.clear();
  sensors.begin();

  lcd.createChar(0, derajat); 
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

    sensors.requestTemperatures(); 
   float temperatureC = sensors.getTempCByIndex(0);
   float temperatureF = sensors.getTempFByIndex(0);

   int range_1 = 25 - sonar1.ping_cm();
   int range_2 = 25 - sonar2.ping_cm();

   float volume_1 = float(range_1 * PANJANG * LEBAR)/1000;
   float volume_2 = float(range_2 * PANJANG * LEBAR)/1000;

   Serial.print("Suhu "); Serial.print(temperatureC); Serial.print(" ");  Serial.println(temperatureF);
   Serial.print("range1 "); Serial.print(range_1); Serial.print(" ");  Serial.println(range_2);

  lcd.setCursor(0,0); lcd.print("Suhu  : "); lcd.print(temperatureC); lcd.write(byte(0)); lcd.print("C    ");
  lcd.setCursor(0,1); lcd.print("Vol 1 : "); lcd.print(volume_1); lcd.print(" Liter      ");
  lcd.setCursor(0,2); lcd.print("Vol 2 : "); lcd.print(volume_2); lcd.print(" Liter      ");
  lcd.setCursor(0,3); lcd.print("RPM   : "); lcd.print(rpm); lcd.print(" rpm          ");
  delay(100);
}
