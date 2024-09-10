#include <Wire.h>
#include <RtcDS1302.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>

#define BuzzerPin   13
#define DHTPIN       3
#define Relay_Lampu 12
#define Relay_Kipas 11

DHT dht(DHTPIN, DHT22);
LiquidCrystal_I2C lcd(0x27,16,2);
ThreeWire myWire(4,5,2); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

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

void buzzer(int index = 3, int timer = 80){
    for(int i=0; i<index; i++){
      digitalWrite(BuzzerPin, HIGH);
      delay(timer);
      digitalWrite(BuzzerPin, LOW);
      delay(timer);
  }
}

void setup(){
  Serial.begin(57600);
  Serial.println(F("DHTxx test!"));
  pinMode(BuzzerPin, OUTPUT);
  pinMode(Relay_Lampu, OUTPUT);
  pinMode(Relay_Kipas, OUTPUT);
  digitalWrite(Relay_Lampu, HIGH);
  digitalWrite(Relay_Kipas, HIGH);

  lcd.init(); lcd.backlight(); lcd.clear();
  dht.begin(); lcd.createChar(0, derajat); 

   Rtc.Begin(); 
   RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
}

void loop(){
  int humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  RtcDateTime now = Rtc.GetDateTime();

  lcd.setCursor(0,0);
  lcd.print("Time "); lcd.print(now.Hour()); lcd.print(":");
                      lcd.print(now.Minute()); lcd.print(":");
                      lcd.print(now.Second()); lcd.print("    ");
  lcd.setCursor(0,1);
  lcd.print("T:"); lcd.print(temperature); lcd.write(byte(0)); lcd.print("C");
  lcd.print(" H:"); lcd.print(humidity); lcd.print(" %  ");

  Serial.print("humidity: "); Serial.println(humidity);
  Serial.print("Temp    : "); Serial.println(temperature);
  Serial.println("");

  Serial.print(now.Hour()); Serial.print(":");
  Serial.print(now.Minute()); Serial.print(":");
  Serial.print(now.Second()); Serial.println("");

  if(temperature >= 39){
    digitalWrite(Relay_Lampu, HIGH);
  } else if(temperature <= 37){
    digitalWrite(Relay_Lampu, LOW);
  }

  if(humidity >= 70 || temperature <= 37){
    digitalWrite(Relay_Kipas, HIGH);
  } else if(humidity <= 64 ){
    digitalWrite(Relay_Kipas, LOW);
  }

  delay(1000);
}
