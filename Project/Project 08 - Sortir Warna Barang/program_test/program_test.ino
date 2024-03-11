#include "Adafruit_TCS34725.h"
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_1X);

unsigned long waktuSensorSekarang = 0;
unsigned long waktuSensor = 500;

const int jumlahBaca = 10;
int bacaR[jumlahBaca], bacaG[jumlahBaca], bacaB[jumlahBaca], bacaC[jumlahBaca];
int indexBacaR=0, indexBacaG=0, indexBacaB=0, indexBacaC=0;
int totalR=0, totalG=0, totalB=0, totalC=0;
int dataR,dataG,dataB,dataC;

struct dataWarna{
  int red;
  int green;
  int blue;
  int cyan;
};

dataWarna detecksiWarna(){
  static dataWarna average;
  uint16_t r, g, b, c;
  
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
      average.red = totalR / (jumlahBaca - 1);
      indexBacaR = 0;
    }
    if (indexBacaG >= jumlahBaca) {
      average.green = totalG / (jumlahBaca - 1);
      indexBacaG = 0;
    }
    if (indexBacaB >= jumlahBaca) {
      average.blue = totalB / (jumlahBaca - 1);
      indexBacaB = 0;
    }
    if (indexBacaC >= jumlahBaca) {
      average.cyan = totalC / (jumlahBaca - 1);
      indexBacaC = 0;
    }
  return average;
}

void setup(){
  Serial.begin(115200);
  if (tcs.begin()){
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
}

void loop(){
  dataWarna dataR = detecksiWarna();
  Serial.println(String(dataR.red)+"#"+String(dataR.green)+"#"+String(dataR.blue)+"#"+String(dataR.cyan));
  delay(500);
}
