#include <OneWire.h>
#include <DallasTemperature.h> 
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"

#define sensor 2
#define relay 3
#define button1 4 
#define button2 5
#define button3 6

OneWire oneWire(sensor);
DallasTemperature sensors(&oneWire); 
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27,16,2); 

byte Simbol_derajat = B11011111;
unsigned long currTime=0;
String waktuOn, waktuOff;
int aktif=0, timerOn=0, timerOff=0;
int counter=0; 
int suhu=0;

void displayLCD(int angka,int menit, String waktu){
  switch(angka){
    case 1 :
      lcd.setCursor(0,0);
      lcd.print("  Pengaduk On   ");
      lcd.setCursor(0,1);
      lcd.print(" Waktu : ");
      lcd.print(waktu);
      lcd.print("         ");
      break;
    case 2 :
      lcd.setCursor(0,0);
      lcd.print("  Pengaduk Off  ");
      lcd.setCursor(0,1);
      lcd.print(" Waktu : ");
      lcd.print(waktu);
      lcd.print("         ");
      break;
    case 3 :
      lcd.setCursor(0,0);
      lcd.print("Atur Lama On :  ");
      lcd.setCursor(0,1);
      lcd.print(menit);
      lcd.print("  Menit         ");
      break;
    case 4 :
      lcd.setCursor(0,0);
      lcd.print("Atur Lama Off : ");
      lcd.setCursor(0,1);
      lcd.print(menit);
      lcd.print("  Menit         ");
      break;
    case 5 :
      lcd.setCursor(0,0);
      lcd.print("Suhu Larutan :  ");
      lcd.setCursor(0,1);
      lcd.print(menit);
      lcd.write(Simbol_derajat);
      lcd.print("C");
      lcd.print("                ");
      break;
  }
}

void setup() {
  Serial.begin(9600);
  sensors.begin();
  lcd.begin(); lcd.backlight();
  
  pinMode(relay, OUTPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);

  lcd.setCursor(0,0);
  lcd.print("Magnetic Stirrer");
  lcd.setCursor(0,1);
  lcd.print("  Kelompok III  ");
  delay(2000);

  if (!rtc.begin()) {
    Serial.println("Could not find RTC! Check circuit.");
    while (1); }

    rtc.adjust(DateTime(__DATE__, __TIME__));

}

void loop() {
  DateTime now = rtc.now();
  static DateTime futureOn, futureOff;
  int detik = now.second();
  int menit = now.minute();
  int jam = now.hour();

  int value1 = digitalRead(button1);
  int value2 = digitalRead(button2);
  int value3 = digitalRead(button3);

  if(millis()-currTime>3000){
    sensors.requestTemperatures(); //baca sensor
    suhu = sensors.getTempCByIndex(0);
    currTime = millis();
  }
  
  if(!value1){
    aktif++;
    if(aktif>3){aktif=0;}
  }
  
    if(aktif==1){                   //lama on
      if(!value2){
        timerOn++; if(timerOn>=60){timerOn=60;}
      } if(!value3){
        timerOn--; if(timerOn<=0){timerOn=0;}
      } 
        futureOn = now + TimeSpan(0,0,timerOn,0);
        waktuOn = String((futureOn - now).hours())+":"+String((futureOn - now).minutes())+":"+String((futureOn - now).seconds());
        displayLCD(3, timerOn, waktuOn);
    }

    if(aktif==2){                   //lama off
      if(!value2){
        timerOff++; if(timerOff>=60){timerOff=60;}
      } if(!value3){
        timerOff--; if(timerOff<=0){timerOff=0;}
      } 
        futureOff = now + TimeSpan(0,0,timerOff,0);
        waktuOff = String((futureOff - now).hours())+":"+String((futureOff - now).minutes())+":"+String((futureOff - now).seconds());
        displayLCD(4, timerOff, waktuOff);
    }

  if(aktif==3){
    waktuOn = String((futureOn - now).hours())+":"+String((futureOn - now).minutes())+":"+String((futureOn - now).seconds());
    waktuOff = String((futureOff - now).hours())+":"+String((futureOff - now).minutes())+":"+String((futureOff - now).seconds());
    
    if(!value2){
      displayLCD(5, suhu, waktuOn);
    }
    else if((futureOn - now).minutes()>=0 && (futureOn - now).seconds()>=0 ){
      displayLCD(1, timerOn, waktuOn);
      futureOff = now + TimeSpan(0,0,timerOff,0);
      digitalWrite(relay,HIGH);
    }
    else if((futureOn - now).minutes()<=0 && (futureOn - now).seconds()<=0 && (futureOff - now).minutes()>=0 && (futureOff - now).seconds()>=0){
       displayLCD(2, timerOff, waktuOff);
       digitalWrite(relay,LOW);
      }

    else if((futureOff - now).minutes()<=0 && (futureOff - now).seconds()<=0){
       futureOn = now + TimeSpan(0,0,timerOn,0);
      }
    }
delay(100);
}
