#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiUdp.h> 
#include <NTPClient.h>
#include "RTClib.h"

#define sensorSuhu        12 //D6
#define waterFlowSensor   14 //D5
#define soilSensor        A0 //ADC0
#define waterPump         16 //D0
#define led_wifi          15 //D8
#define led_pump          10 //SD3
#define button            2  //D4

#define auth      "z04QM-56J8bJZjvA48sNhBp5SLdWr8ZE"
#define ssid      "realme C31"
#define password  "123456789"

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 20, 4);
DHT dht(sensorSuhu, DHT11);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
BlynkTimer timer;

static float flowRate;           // Variabel untuk menghitung laju aliran air (liter per menit)
unsigned int flowMilliLitres; // Variabel untuk menghitung aliran dalam mililiter
unsigned long totalMilliLitres; // Total aliran dalam mililiter
unsigned long oldTime;    // Waktu terakhir saat pulsa terdeteksi

String hari[7] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jum'at", "Sabtu"};

static int kelembapan_tanah;
static int kelembapan_udara;
static float suhu;
static String statusPompa = "";
static String currentTime = "";
volatile bool relay1State = LOW; 
volatile bool pushButton1State = HIGH;
volatile int pulseCount;

void connectWifi() {
  pinMode(led_wifi, OUTPUT);
  digitalWrite(led_wifi, LOW);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); digitalWrite(led_wifi, LOW);
    Serial.println("Connecting to WiFi...");
  }  Serial.println("Wifi Terhubung");
  Blynk.begin(auth, ssid, password, "blynk.cloud", 80);
  digitalWrite(led_wifi, HIGH);
} 

void displayLCD(int angka) {
  switch (angka) {
    case 1 :
      lcd.setCursor(0, 1);
      lcd.print("    Tugas Akhir     ");
      lcd.setCursor(0, 2);
      lcd.print(" Rani Elsa Fajriyah ");
      break;
    case 2 :
      lcd.setCursor(0, 0);
      lcd.print("   SISTEM KONTROL   ");
      lcd.setCursor(0, 1);
      lcd.print("   DAN MONITORING   ");
      lcd.setCursor(0, 2);
      lcd.print("  PENYIRAMAN CABAI  ");
      lcd.setCursor(0, 3);
      lcd.print("OTOMATIS BRBASIS IOT");
      break;
    case 3 :
      lcd.setCursor(0, 0);
//      lcd.print("Waktu : " + currentTime + " WIB " );
      lcd.setCursor(0, 1);  lcd.print("Temp:"); lcd.print(suhu); lcd.print(" ");
      lcd.print(" Humy:"); lcd.print(kelembapan_udara); lcd.print("% ");
      lcd.setCursor(0, 2);
      lcd.print("Soil:"); lcd.print(kelembapan_tanah); lcd.print("%                ");
      lcd.setCursor(0, 3);
      lcd.print("WaterPump : " + statusPompa + "          ");
      break;
  }
}

ICACHE_RAM_ATTR void pulseCounter() {
  pulseCount++;
}

void waterflow(){
  unsigned long currentTime = millis();
  if (currentTime - oldTime > 1000) { 
    flowRate = (1000.0 / (currentTime - oldTime)) * pulseCount;  
    oldTime = currentTime;
    flowMilliLitres = (flowRate / 60) * 1000;  
    totalMilliLitres += flowMilliLitres;
    pulseCount = 0;
  }
}

void setup() {
  pinMode(sensorSuhu, INPUT);
  pinMode(soilSensor, INPUT);
  pinMode(waterPump, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  pinMode(waterFlowSensor, INPUT_PULLUP);
  digitalWrite(waterPump, LOW);
  
  lcd.init(); lcd.backlight();
  dht.begin();
  Serial.begin(9600);
//  connectWifi();

  timeClient.begin();
  timeClient.setTimeOffset(25200);

//  rtc.begin();
//  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

//  timer.setInterval(100L, checkPhysicalButton);
//  attachInterrupt(digitalPinToInterrupt(waterFlowSensor), pulseCounter, FALLING);
}

void loop() {
//  Blynk.run(); timer.run();
//  DateTime now = rtc.now();
  waterflow();
 
//  timeClient.update();
//  currentTime = timeClient.getFormattedTime();
//  byte currentDay = timeClient.getDay();
//  byte tanggal = now.day();
//  byte bulan = now.month();
//  int tahun = now.year();
//  float debitAir = flowRate;
    displayLCD(3);

  kelembapan_tanah = analogRead(soilSensor);
  kelembapan_tanah = map(kelembapan_tanah, 0, 1024, 0, 100);
  kelembapan_udara = dht.readHumidity();
  suhu = dht.readTemperature();

//  String date = String(tanggal)+"/"+String(bulan)+"/"+String(tahun);

//  Blynk.virtualWrite(V0, hari[currentDay]);
//  Blynk.virtualWrite(V1, date);
//  Blynk.virtualWrite(V2, currentTime);
//  Blynk.virtualWrite(V4, suhu);
//  Blynk.virtualWrite(V5, kelembapan_udara);
//  Blynk.virtualWrite(V6, kelembapan_tanah);
//  Blynk.virtualWrite(V7, debitAir);

  if(kelembapan_tanah>=80 && relay1State == HIGH){  // jika basah dan air hidup maka air mati
     relay1State != relay1State; 
     Blynk.virtualWrite(V3, relay1State);
     digitalWrite(waterPump, relay1State);
     statusPompa = "OFF";
  } else if(kelembapan_tanah<80 && relay1State == LOW){ //jika kering dan air tidak hidup maka air hidup otomatis
     relay1State != relay1State;
     Blynk.virtualWrite(V3, relay1State);
     digitalWrite(waterPump, relay1State);
     statusPompa = "ON";
  }
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V3);
}

BLYNK_WRITE(V3) {
  relay1State = param.asInt();
  digitalWrite(waterPump, relay1State);
}

void checkPhysicalButton(){
  if (digitalRead(button) == LOW) {
    if (pushButton1State != LOW) {
      relay1State = !relay1State;
      digitalWrite(waterPump, relay1State);
      Blynk.virtualWrite(V3, relay1State);
    }
    pushButton1State = LOW;
  } else {
    pushButton1State = HIGH;
  }
}
