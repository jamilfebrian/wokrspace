#include <ESP32Servo.h>
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include "DHT.h"
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define servoPin1  26
#define servoPin2  27
#define servoPin3  18
#define servoPin4  19
#define IR_Penutup 35
#define relay      23

#define induktif_1   13 //pemisah organik dan non organik
#define proximity_1  25
#define kapasitif_1  33

#define proximity_2  34 //pemisah organik basah dam kering
#define dht22Pin     14

#define proximity_3  21 //pemisah logam dan non logam
#define induktif_3   32

const char* ssid     = "Tinkpad";
const char* password = "12345678";
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200, 60000); // 25200 detik = 7 jam (WIB), 60000 ms pembaruan

Servo myServo1, myServo2, myServo3, myServo4;
DFRobotDFPlayerMini myDFPlayer;
DHT dht(dht22Pin, DHT22);

unsigned long cTime1 = 0;

bool audio = true;
bool step1 = false;
bool step2 = false;
bool step3 = false;

void connectWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

void setup() {
  Serial2.begin(9600, SERIAL_8N1, /*rx =*/16, /*tx =*/17);
  Serial.begin(115200); Serial.println("Memulai");
  dht.begin(); connectWifi();  timeClient.begin();

  pinMode(IR_Penutup, INPUT);
  pinMode(relay, OUTPUT);

  pinMode(induktif_1, INPUT);
  pinMode(proximity_1, INPUT);
  pinMode(kapasitif_1, INPUT);

  pinMode(proximity_2, INPUT);

  pinMode(induktif_3, INPUT);
  pinMode(proximity_3, INPUT);

  digitalWrite(relay, HIGH);
  
  myServo1.attach(servoPin1); myServo1.write(90);
  myServo2.attach(servoPin2); myServo2.write(90);
  myServo3.attach(servoPin3); myServo3.write(90);
  myServo4.attach(servoPin4); myServo4.write(90);
  delay(1000);

  if (!myDFPlayer.begin(Serial2, /*isACK = */true, /*doReset = */true)) { 
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  } Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(30); 
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);  
}

void loop() {
  timeClient.update();
  String waktu = timeClient.getFormattedTime();
  String jam, menit, detik;
  int index1 = waktu.indexOf(':');
  jam = waktu.substring(0, index1);
  int index2 = waktu.indexOf(':', index1 + 1);
  menit = waktu.substring(index1 + 1, index2);
  detik = waktu.substring(index2 + 1);
  int time_hour = jam.toInt();
  int time_minute = menit.toInt();
  int time_second = detik.toInt();

  if(time_hour>=18 && time_hour<=22){
    digitalWrite(relay, LOW);
  } else {
    digitalWrite(relay, HIGH);
  }

  bool IR_Penutup_Value = digitalRead(IR_Penutup);

  bool induktif_1_value = digitalRead(induktif_1);
  bool proximity_1_value = digitalRead(proximity_1);
  bool kapasitif_1_value = digitalRead(kapasitif_1);

  bool proximity_2_value = digitalRead(proximity_2);
  float humidity = dht.readHumidity(); 
  float temperature = dht.readTemperature();

  bool induktif_3_value = digitalRead(induktif_3);
  bool proximity_3_value = digitalRead(proximity_3);
  
//  Serial.println(realTime);
//  Serial.print("Proxy1:"); Serial.println(proximity_1_value);
//  Serial.print("Proxy2:"); Serial.println(proximity_2_value);
//  Serial.print("Proxy3:"); Serial.println(proximity_3_value);
//  Serial.print("induk1:"); Serial.println(induktif_1_value);
//  Serial.print("induk3:"); Serial.println(induktif_3_value);
//  Serial.print("kapas1:"); Serial.println(kapasitif_1_value);
//  Serial.print("humy  :"); Serial.println(humidity);
//  Serial.print("temp  :"); Serial.println(temperature);
//  delay(1000);

  if(!IR_Penutup_Value){
    myServo1.write(0);
    if(audio){
       audio = false;
       step1 = true;
       myDFPlayer.play(1); delay(2000);
       myDFPlayer.play(2);
    }
    cTime1 = millis();
  } else {
    if(millis() - cTime1 > 4000){
       myServo1.write(80);
       audio = true;
    }
    if(audio && step1 && !proximity_1_value && kapasitif_1_value){ // sampah non organik
      myServo2.write(180); myDFPlayer.play(3);
      step1 = false; step2 = true;
      delay(1000);  myServo2.write(90);
    } else if(audio && step1 && !proximity_1_value && !kapasitif_1_value){ //sampah organik
      myServo2.write(0); myDFPlayer.play(3);
      step1 = false; step3 = true;
      delay(1000);  myServo2.write(90);
    } else{
      myServo2.write(90);
    }
  }

  induktif_3_value = digitalRead(induktif_3);
  if(step2 && induktif_3_value){
      delay(2000);
      myServo4.write(0); 
      step2 = false;
      delay(1000);
  } else if(step2 && !induktif_3_value ){
      delay(2000);
      myServo4.write(180); 
      step2 = false;
      delay(1000);
  } else {
      myServo4.write(90); 
  }

   humidity = dht.readHumidity(); 
  if(step3 && humidity>85 ){
      delay(2000);
      myServo3.write(0); 
      step3 = false; 
      delay(1000);
  } else if(step3 && humidity<85 ){
      delay(2000);
      myServo3.write(180); 
      step3 = false; 
      delay(1000);
  } else {
      myServo3.write(90); 
  }
}
