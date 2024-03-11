#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_TEMPLATE_ID "TMPL6UnrWxT-C"
#define BLYNK_TEMPLATE_NAME "Sortir Barang Berdasarkan Warna"
#define BLYNK_AUTH_TOKEN "_SpuzzM9n4wwiGrgkbXxl4Gz6LnEgRkF"

char auth[] = BLYNK_AUTH_TOKEN;  // masukkan auth token dari aplikasi Blynk Anda
char ssid[] = "Galaxy A12E2CC";  // masukkan SSID WiFi Anda
char password[] = "ampk3657";  // masukkan password WiFi Anda

unsigned long previousMillis = 0;
const unsigned long interval = 800;

String Kotak[4];
int kotakMerah, kotakHijau, kotakBiru, kotakKuning;

void setup(){
  Serial.begin(9600);  
  pinMode (LED_BUILTIN, OUTPUT);
  WiFi.begin(ssid, password);
  Blynk.begin(auth, ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); digitalWrite(LED_BUILTIN, 1);
  }   digitalWrite (LED_BUILTIN, 0);
}

void loop(){
  Blynk.run();
  unsigned long currenMillis = millis();
  if(currenMillis - previousMillis >= interval){
    previousMillis = currenMillis;

    String data = "";
    while(Serial.available()>0){
      data += char(Serial.read());
    } data.trim();

    if (data != ""){
      int index = 0;
      for(int i=0; i<=data.length();i++){
        char delimiter = ' ';
        if(data[i] != delimiter){
          Kotak[index] += data[i];
        } else { 
          index++;
        }
      } 
      
      if (index == 3){
        String data0 = Kotak[0];
        String data1 = Kotak[1];
        String data2 = Kotak[2];
        String data3 = Kotak[3];
         kotakMerah = data0.toInt() ;
         kotakHijau = data1.toInt();  
         kotakBiru = data2.toInt();  
         kotakKuning = data3.toInt();  
      }
      Kotak[0] = ""; Kotak[2] = "";
      Kotak[1] = ""; Kotak[3] = "";
    }
    Serial.println("YA");
  }
  Blynk.virtualWrite(V0, kotakMerah); 
  Blynk.virtualWrite(V1, kotakHijau); 
  Blynk.virtualWrite(V2, kotakBiru); 
  Blynk.virtualWrite(V3, kotakKuning); 
}
