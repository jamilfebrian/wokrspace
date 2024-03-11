#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include <NeoSWSerial.h>
#include "Adafruit_TCS34725.h"
#include <Servo.h>

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_1X);
Servo servodorong, servogate1, servogate2, servogate3;
LiquidCrystal_I2C lcd(0x27,16,2);
NeoSWSerial mySerial(8, 12);

const int servoDorong = 6;
const int servoGate1 = 10;
const int servoGate2 = 9;
const int servoGate3 = 11;
const int kotak1 = 2;
const int kotak2 = 3;
const int kotak3 = 4;
const int kotak4 = 5;
const int kotakseleksi = 7;

int barang = 0;
int dataKotak1 = 0;
int dataKotak2 = 0;
int dataKotak3 = 0;
int dataKotak4 = 0;
int lcdtampilan = 1;

unsigned long waktuLCD = 0;
unsigned long waktuIntervalLCD = 1500;
unsigned long waktuDataSekarang = 0;
unsigned long waktuKirimData = 500;
unsigned long waktuSensorSekarang = 0;
unsigned long waktuSensor = 700;

const int jumlahBaca = 10;

int bacaR[jumlahBaca];
int indexBacaR = 0;
int totalR = 0;
int averageR = 0;
int bacaG[jumlahBaca];
int indexBacaG = 0;
int totalG = 0;
int averageG = 0;
int bacaB[jumlahBaca];
int indexBacaB = 0;
int totalB = 0;
int averageB = 0;
int bacaC[jumlahBaca];
int indexBacaC = 0;
int totalC = 0;
int averageC = 0;


void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    lcd.setCursor(0,0);
    lcd.print("Sensor tidak");
    lcd.setCursor(1,1);
    lcd.print("ditemukan");
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }

  for (int thisReading = 0; thisReading < jumlahBaca; thisReading++) {
    bacaR[thisReading] = 0;
    bacaG[thisReading] = 0;
    bacaB[thisReading] = 0;
    bacaC[thisReading] = 0;
  }

  mySerial.begin(9600);
  pinMode(kotakseleksi, INPUT);
  pinMode(kotak1, INPUT);
  pinMode(kotak2, INPUT);
  pinMode(kotak3, INPUT);
  pinMode(kotak4, INPUT);
  servodorong.attach(servoDorong);
  servogate1.attach(servoGate1);
  servogate2.attach(servoGate2);
  servogate3.attach(servoGate3);
  servodorong.write(0);
  servogate1.write(70);
  servogate2.write(70);
  servogate3.write(70);
  lcd.setCursor(0,0);
  lcd.print(" Annes Vardila");
  lcd.setCursor(4,1);
  lcd.print("18130003");
  delay(2000);  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Penyortir Warna");
  lcd.setCursor(4,1);
  lcd.print("Barang");
  delay(2000); 
  lcd.clear();
}

void loop() {
  uint16_t r, g, b, c, colorTemp, lux;

  // tcs.getRawData(&r, &g, &b, &c);

  Serial.print(digitalRead(kotakseleksi));
  Serial.print(" ");
  Serial.print(digitalRead(kotak1));
  Serial.print(" ");
  Serial.print(digitalRead(kotak2));
  Serial.print(" ");
  Serial.print(digitalRead(kotak3));
  Serial.print(" ");
  Serial.println(digitalRead(kotak4));
  if((millis()-waktuSensorSekarang)>waktuSensor){
    c = tcs.read16(TCS34725_CDATAL);
    r = tcs.read16(TCS34725_RDATAL);
    g = tcs.read16(TCS34725_GDATAL);
    b = tcs.read16(TCS34725_BDATAL);    
    totalR = totalR - bacaR[indexBacaR];
    totalG = totalG - bacaG[indexBacaG];
    totalB = totalB - bacaB[indexBacaB];
    totalC = totalC - bacaC[indexBacaC];
    bacaR[indexBacaR] = r;
    bacaG[indexBacaG] = g;
    bacaB[indexBacaB] = b;
    bacaC[indexBacaC] = c;
    totalR = totalR + bacaR[indexBacaR];
    totalG = totalG + bacaG[indexBacaG];
    totalB = totalB + bacaB[indexBacaB];
    totalC = totalC + bacaC[indexBacaC];
    indexBacaR = indexBacaR + 1;
    indexBacaG = indexBacaG + 1;
    indexBacaB = indexBacaB + 1;
    indexBacaC = indexBacaC + 1;  
    if (indexBacaR >= jumlahBaca) {
      averageR = totalR / (jumlahBaca - 1);
      indexBacaR = 0;
    }
    if (indexBacaG >= jumlahBaca) {
      averageG = totalG / (jumlahBaca - 1);
      indexBacaG = 0;
    }
    if (indexBacaB >= jumlahBaca) {
      averageB = totalB / (jumlahBaca - 1);
      indexBacaB = 0;
    }
    if (indexBacaC >= jumlahBaca) {
      averageC = totalC / (jumlahBaca - 1);
      indexBacaC = 0;
    }
    // averageR = totalR / jumlahBaca;
    // averageG = totalG / jumlahBaca;
    // averageB = totalB / jumlahBaca;
    // averageC = totalC / jumlahBaca;
    waktuSensor = millis();
  }
  

  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(averageR, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(averageG, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(averageB, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(averageC, DEC); Serial.print(" ");
  Serial.println(" ");
  if(digitalRead(kotakseleksi)==1){
    if(averageR>=470 && averageR<=650 && averageG>=190 && averageG<=270 && averageB>=200 && averageB<=260 && averageC>=870 && averageC<=930 ){  
      Serial.println("Merah");
      if(barang == 0){
        servodorong.write(40);
        servogate1.write(0);
        barang = 1;
      }
    }
    if(averageR>=245 && averageR<=270 && averageG>=225 && averageG<=240 && averageB>=225 && averageB<=250 && averageC>=520 && averageC<=560){
      Serial.println("Biru");
      if(barang == 0){
        servodorong.write(40);
        servogate3.write(5);
        barang = 3;
      }
    }
    if(averageR>=700 && averageR<=860 && averageG>=310 && averageG<=535 && averageB>=150 && averageB<=350 && averageC>=950 && averageC<=1580){
      Serial.println("Kuning");
      if(barang == 0){
        servodorong.write(40);
        servogate2.write(10);
        barang = 2;
      }
    }
    if(averageR>=220 && averageR<=310  && averageG>=120 && averageG<=310 && averageB>=205 && averageB<=240 && averageC>=500 && averageC<=640){
      Serial.println("Hijau");
      if(barang == 0){
        servodorong.write(40);
        barang = 4;
      }
    }
  }
  if(digitalRead(kotak1)==1 && barang == 1){
    servodorong.write(0);
    servogate1.write(70);
    barang = 0;
    dataKotak1++;
  }
  if(digitalRead(kotak2)==1 && barang == 2){
    servodorong.write(0);
    servogate2.write(70);
    barang = 0;
    dataKotak2++;
  }
  if(digitalRead(kotak3)==1 && barang == 3){
    servodorong.write(0);
    servogate3.write(70);
    barang = 0;
    dataKotak3++;
  }
  if(digitalRead(kotak4)==1 && barang == 4){
    servodorong.write(0);
    barang = 0;
    dataKotak4++;
  }
  tampilLCD(r,g,b,c);
  if((millis()-waktuDataSekarang)>waktuKirimData){
    mySerial.print(dataKotak1);
    mySerial.print("A");
    mySerial.print(dataKotak2);
    mySerial.print("B");
    mySerial.print(dataKotak3);
    mySerial.print("C");
    mySerial.print(dataKotak4);
    mySerial.print("D");
    mySerial.print("\n");
    waktuDataSekarang = millis();
  }
  if((millis()-waktuLCD)>waktuIntervalLCD){
    if(lcdtampilan>=3){
      lcd.clear();
      lcdtampilan = 1;
    }else{
      lcd.clear();
      lcdtampilan +=1;      
    }
    waktuLCD = millis();
  }
}

void tampilLCD(uint16_t warnaR,uint16_t warnaG,uint16_t warnaB, uint16_t warnaC){
  if(lcdtampilan == 1){
    lcd.setCursor(0,0);
    lcd.print("Kotak 1 = ");
    lcd.print(dataKotak1);
    lcd.setCursor(0,1);
    lcd.print("Kotak 2 = ");
    lcd.print(dataKotak2);
  }else if(lcdtampilan == 2){
    lcd.setCursor(0,0);
    lcd.print("Kotak 3 = ");
    lcd.print(dataKotak3);
    lcd.setCursor(0,1);
    lcd.print("Kotak 4 = ");
    lcd.print(dataKotak4);
  }else{
    lcd.setCursor(0,0);
    lcd.print("r = ");
    lcd.print(warnaR);
    lcd.setCursor(8,0);
    lcd.print("g = ");
    lcd.print(warnaG);
    lcd.setCursor(0,1);
    lcd.print("b = ");
    lcd.print(warnaB);
    lcd.setCursor(8,1);
    lcd.print("c = ");
    lcd.print(warnaC);
  }
}
