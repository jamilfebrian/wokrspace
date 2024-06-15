#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>

#define BLYNK_TEMPLATE_ID "TMPL6wQ1NJBwh"
#define BLYNK_TEMPLATE_NAME "Electrical Panel Failure Early Warning System"
#define BLYNK_AUTH_TOKEN "_qH8fLfatOETy8vfQ7Tfr55svTJPUYto"

char auth[] = BLYNK_AUTH_TOKEN;  
char ssid[] = "Thom"; 
char password[] = "87654321";
SoftwareSerial myserial(14,12);
BlynkTimer timer;

int ToggleValue;
String data="";
static String dataValue[4];
static float suhu, gas;
static int humi, api;
unsigned long cTime=0;

WidgetLED LED_ON(V5);
WidgetLED LED_OFF(V6);

String dataPesan(){
  String pesan="";
  while(myserial.available()){
      pesan += char(myserial.read());
    } pesan.trim();
    return pesan;
}

void Data(){
  if(millis()-cTime>500){
    cTime=millis();
    data = dataPesan();
    Serial.println(data);
    
    if(data != ""){
      int index = 0;
      for(int i=0; i<=data.length();i++){
        if(data[i] != ' '){
          dataValue[index] += data[i];
        } else { 
          index++;
        }
      }
      if (index == 3){
      suhu = dataValue[0].toInt();
      humi = dataValue[1].toInt();
      gas = dataValue[2].toInt();
      api = dataValue[3].toInt();
     }
      for(int i=0;i<4;i++){
        dataValue[i] = "";
      }
    }
  }
}

void bacaSensor(){
  if(gas>=250){
    myserial.println("off");
    Blynk.logEvent("gas_terdeteksi","BAHAYA! Terdeteksi Asap!"); 
    LED_OFF.on(); LED_ON.off();
  } else if(api==10){
    Blynk.logEvent("api_terdeteksi","BAHAYA! Percikan Api Terdeteksi!"); 
    myserial.println("off");
    LED_OFF.on(); LED_ON.off();
  } else if(suhu>=60){
    Blynk.logEvent("suhu_tinggi","BAHAYA! Suhu Terlalu Tinggi"); 
    myserial.println("off");
    LED_OFF.on(); LED_ON.off();
  }
}
void setup(){
  Serial.begin(115200);
  myserial.begin(115200);
  pinMode(2,OUTPUT);
  digitalWrite(2, HIGH);
  WiFi.begin(ssid, password);
  Blynk.begin(auth, ssid, password, "blynk.cloud", 80);

  timer.setInterval(100L, Data);
  timer.setInterval(100L, bacaSensor);
  LED_OFF.off();
  LED_ON.on();
  
  while (WiFi.status() != WL_CONNECTED){
    delay(100); digitalWrite(2, HIGH);
  }   digitalWrite(2, LOW);
}

void loop(){
  Blynk.run();
  Data();
  bacaSensor();
    
  Blynk.virtualWrite(V3, suhu); 
  Blynk.virtualWrite(V4, humi); 
  Blynk.virtualWrite(V2, gas); 
  Blynk.virtualWrite(V7, api); 
}

BLYNK_WRITE(V1){
  int value = param.asInt();  
  if (value == 1) {
   myserial.println("1");
  } else {
   myserial.println("0");
  }
}
