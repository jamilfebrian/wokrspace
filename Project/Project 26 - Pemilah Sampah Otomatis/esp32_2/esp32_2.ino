#define BLYNK_TEMPLATE_ID "TMPL64FoMgu2L"
#define BLYNK_TEMPLATE_NAME "ronii"
#define BLYNK_AUTH_TOKEN "3NIv-gKgqTZerucHfPi-TCx8JgWhDlHY"

#include <NewPing.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#define TRIGGER_PIN_1  32  
#define ECHO_PIN_1     33  
#define TRIGGER_PIN_2  25  
#define ECHO_PIN_2     26  
#define TRIGGER_PIN_3  19  
#define ECHO_PIN_3     18 
#define TRIGGER_PIN_4  27  
#define ECHO_PIN_4     14 

 
#define MAX_DISTANCE   200

#define ssid      "Tinkpad"
#define password  "12345678"
#define token BLYNK_AUTH_TOKEN

NewPing sonar1(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE);
NewPing sonar3(TRIGGER_PIN_3, ECHO_PIN_3, MAX_DISTANCE);
NewPing sonar4(TRIGGER_PIN_4, ECHO_PIN_4, MAX_DISTANCE);

const int tinggiSampah = 45;

void connectWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Blynk.begin(token, ssid, password, "blynk.cloud", 80);
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

int volumeSampah(int tinggi){
  int volume = tinggi * 100 / tinggiSampah;
  return volume;
}

void setup() {
  Serial.begin(115200);
  connectWifi();

}

void loop() {
  Blynk.run(); 
   
  int sampah_organik_basah  = tinggiSampah - sonar1.ping_cm();
  int sampah_organik_kering = tinggiSampah - sonar2.ping_cm();
  int sampah_logam = tinggiSampah - sonar3.ping_cm();
  int sampah_plastik = tinggiSampah - sonar4.ping_cm();

  sampah_organik_basah = volumeSampah(sampah_organik_basah);
  sampah_organik_kering = volumeSampah(sampah_organik_kering);
  sampah_logam = volumeSampah(sampah_logam);
  sampah_plastik = volumeSampah(sampah_plastik);

  Serial.print(sampah_organik_basah); Serial.print(" "); 
  Serial.print(sampah_organik_kering); Serial.print(" "); 
  Serial.print(sampah_logam); Serial.print(" "); 
  Serial.print(sampah_plastik); Serial.println(" "); 

  if(sampah_organik_basah > 90){
    Blynk.logEvent("sampah_organik_basah","Sampah Organik Basah Telah Penuh"); 
  } else if (sampah_organik_basah > 80){
    Blynk.logEvent("sampah_organik_basah","Sampah Organik Basah Telah mencapai 80%"); 
  } else if(sampah_organik_basah > 70){
    Blynk.logEvent("sampah_organik_basah","Sampah Organik Basah Telah mencapai 70%"); 
  }
  if(sampah_organik_kering > 90){
    Blynk.logEvent("sampah_organik_kering","Sampah Organik Basah Telah Penuh"); 
  } else if (sampah_organik_kering > 80){
    Blynk.logEvent("sampah_organik_kering","Sampah Organik Basah Telah mencapai 80%"); 
  } else if(sampah_organik_kering > 70){
    Blynk.logEvent("sampah_organik_kering","Sampah Organik Basah Telah mencapai 70%"); 
  }
  if(sampah_logam > 90){
    Blynk.logEvent("sampah_logam","Sampah Logam Telah Penuh"); 
  } else if (sampah_logam > 80){
    Blynk.logEvent("sampah_logam","Sampah Logam Telah mencapai 80%"); 
  } else if(sampah_logam > 70){
    Blynk.logEvent("sampah_logam","Sampah Logam Telah mencapai 70%"); 
  }
  if(sampah_plastik > 90){
    Blynk.logEvent("sampah_plastik","Sampah Logam Telah Penuh"); 
  } else if (sampah_plastik > 80){
    Blynk.logEvent("sampah_plastik","Sampah Logam Telah mencapai 80%"); 
  } else if(sampah_plastik > 70){
    Blynk.logEvent("sampah_plastik","Sampah Logam Telah mencapai 70%"); 
  }
  
  
  Blynk.virtualWrite(V0, sampah_organik_basah); 
  Blynk.virtualWrite(V1, sampah_organik_kering); 
  Blynk.virtualWrite(V2, sampah_logam); 
  Blynk.virtualWrite(V3, sampah_plastik); 
  delay(500);

}
