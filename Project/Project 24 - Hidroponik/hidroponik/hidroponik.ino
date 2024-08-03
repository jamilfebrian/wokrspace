#define BLYNK_TEMPLATE_ID "TMPL6x7TTtErr"
#define BLYNK_TEMPLATE_NAME "Smart Hidroponik"
#define BLYNK_AUTH_TOKEN "ZSBheV-ynltvShXNzyqBRC4PhvqxiRkf"

#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>
#include "CQRobotTDS.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "BlynkSimpleEsp32.h"

#define ssid "192.168.50.10"
#define password "12345678"
#define token BLYNK_AUTH_TOKEN
#define Web_App_URL "https://script.google.com/macros/s/AKfycbxLy5fZNoZdtKWwruCF6OKgYjV6nqg5xTc028pO1vquhkDTowiet06cZ2s0S9xwdBjllA/exec?"

#define DS18B20       4  
#define TDSSensorPin 15
#define pHSensorPin  13
#define BuzzerPin    23
#define relayPin1    14
#define relayPin2    27
#define TRIGPIN      18
#define ECHOPIN      19

BlynkTimer timer;
CQRobotTDS tds(TDSSensorPin);
OneWire oneWire(DS18B20);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2);
NewPing sonar(TRIGPIN, ECHOPIN, 200); 

unsigned long timeout = 0;
String Status_Read_Sensor = "SUCCES";

float voltage_ph7 = 1.65; 
float voltage_ph4 = 2.00; 

int PANJANG = 50;
int LEBAR = 40;

static float temperatureC;
static float temperatureF;

static float tdsValue;
static int analogValue;
static float voltage;
static float pH;

static int TINGGI ;
static int volume ;

void buzzer(int index = 3, int timer = 80){
    for(int i=0; i<index; i++){
      digitalWrite(BuzzerPin, HIGH);
      delay(timer);
      digitalWrite(BuzzerPin, LOW);
      delay(timer);
  }
}

void connectWifi(){
  buzzer(1);
  WiFi.mode(WIFI_STA);
  lcd.setCursor(0,0); lcd.print(" Menghubungkan  ");
  lcd.setCursor(0,1); lcd.print(" Ke : "+ String(ssid) +"     ");
  WiFi.begin(ssid, password);
  Blynk.begin(token, ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); 
  } buzzer(3);
  lcd.setCursor(0,0); lcd.print("    Jaringan    ");
  lcd.setCursor(0,1); lcd.print(" Wifi Terhubung ");
}

void sendDataToSpreadsheet(){
    String Send_Data_URL = Web_App_URL "sts=write";
    
    Send_Data_URL += "&srs=" + Status_Read_Sensor;
    Send_Data_URL += "&V=" + String(volume);
    Send_Data_URL += "&TDS=" + String(tdsValue);
    Send_Data_URL += "&PH=" + String(pH);
    Send_Data_URL += "&T=" + String(temperatureC);

      HTTPClient http;
  
      http.begin(Send_Data_URL.c_str());
      http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

      int httpCode = http.GET(); 
      
      String payload;
      if (httpCode > 0) {
        payload = http.getString();
      }
      http.end();
}

void setup() {
  Serial.begin(115200);
  pinMode(pHSensorPin, INPUT);
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);

  digitalWrite(relayPin1,HIGH);
  digitalWrite(relayPin2,LOW);

  lcd.init(); lcd.backlight(); lcd.clear();
  sensors.begin();
  connectWifi();

  timer.setInterval(300000L, sendDataToSpreadsheet);
}

void loop() {
  timer.run();  Blynk.run(); 
  sensors.requestTemperatures(); 
   temperatureC = sensors.getTempCByIndex(0);
   temperatureF = sensors.getTempFByIndex(0);

   tdsValue = tds.update(temperatureC);
  
   analogValue = analogRead(pHSensorPin);
   voltage = analogValue * (3.3/4095);
   pH = 7 + (voltage - voltage_ph7) * ((4 - 7) / (voltage_ph4 - voltage_ph7));

   TINGGI = sonar.ping_cm();
   volume = (PANJANG * LEBAR * TINGGI)/1000;

  if (timeout<millis()){
    Blynk.virtualWrite(V0, temperatureC); 
    Blynk.virtualWrite(V1, tdsValue); 
    Blynk.virtualWrite(V2, volume); 
    Blynk.virtualWrite(V3, pH); 

    timeout = millis() + 1000;
  }
}

BLYNK_WRITE(V4){
  int value = param.asInt();  
  if (value == 1) {
   digitalWrite(relayPin1,LOW);
  } else {
   digitalWrite(relayPin1,HIGH);
  }
}
