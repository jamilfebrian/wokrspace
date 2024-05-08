#define BLYNK_TEMPLATE_ID "TMPL6R6Zgjx6p"
#define BLYNK_TEMPLATE_NAME "Biodiagaster metana meter control"
#define BLYNK_AUTH_TOKEN "ufq-ukh8suzjr-a9XgfK-urLNl34Lc2I"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>
#include "Adafruit_VL53L0X.h"

#define ssid "Tinkpad"
#define password "12345678"
#define token BLYNK_AUTH_TOKEN

WidgetLCD lcdBlynk1(V9);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

#define MQ4_A0  34
#define MQ4_D0  12
#define IN1     27
#define IN2     14
#define IN3     12
#define IN4     13
#define relay1  18  //relay untuk membuka katup selenoid
#define relay2  19  //relay untuk menggerakkan motor DC

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Blynk.begin(token, ssid, password, "blynk.cloud", 80);
  
  lcd.init(); lcd.backlight(); lcd.clear();
  lcdBlynk1.clear();
  
  pinMode(MQ4_A0,INPUT);
  pinMode(MQ4_D0,INPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  } lox.startRangeContinuous();
}

void loop() {
  Blynk.run();

  int CH4 = analogRead(MQ4_A0);
  int percent = CH4*100/4095;
  int sludge = 819 - (lox.readRange()/10);

  Blynk.virtualWrite(V1, CH4); 
  Blynk.virtualWrite(V2, sludge); 
  
  lcd.setCursor(0, 0);
  lcd.print("CH4:"); lcd.print(CH4); lcd.print("     ");
  lcd.setCursor(0, 1);
  lcd.print("Slg:"); lcd.print(sludge); lcd.print("  ");
  lcdBlynk1.print(0,0,"CH4 : "+String(percent)+"%    ");
  
  if(sludge < 10){
    lcd.setCursor(9, 1);
    lcd.print("(Empty)   ");
    lcdBlynk1.print(0,1,"Sludge : (Empty)  ");
  } else if(sludge > 800){
    lcd.setCursor(9, 1);
    lcd.print("(Full)   ");
    lcdBlynk1.print(0,1,"Sludge : (Full)  ");
  } else {
    lcd.setCursor(9, 1);
    lcd.print("           ");
    lcdBlynk1.print(0,1,"Sludge : (Fill) ");
  }
}

BLYNK_WRITE(V0){
  int pinValue = param.asInt();
  if(pinValue){
    digitalWrite(relay1, HIGH);
  } else {
    digitalWrite(relay1, LOW);
  }
}
