//Template Blynk Configuration 
#define BLYNK_TEMPLATE_ID "TMPL6pEDfLN-R"
#define BLYNK_TEMPLATE_NAME "Smart Garden"
#define BLYNK_AUTH_TOKEN "-7FM5sOqLIMTPpwgdWsoEyvq-fqMQfx1"

//menambahkan library yang digunakan
#include "DHT.h"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

//konfigurasi pin esp32 yang digunakan
#define soilPin   32
#define dht11     14
#define MQ_sensor 33
#define BUZZER    27
#define waterpump 26

//konfigurasi ssid, password, dan token menghubungkan ke Blynk
#define ssid     "im"
#define password "122345678"
#define token BLYNK_AUTH_TOKEN

DHT dht(dht11, DHT11);

//fungsi menghasilkan suara pada buzzer
void buzzer(int index = 3, int timer = 70){
    for(int i=0; i<index; i++){
      digitalWrite(BUZZER, HIGH); delay(timer);
      digitalWrite(BUZZER, LOW); delay(timer);
  }
}

//fungsi untuk menghubungkan koneksi wifi
void connectWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Blynk.begin(token, ssid, password, "blynk.cloud", 80);
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

void setup() {
 // mengatur mode pin GPIO ESP32
  pinMode(BUZZER, OUTPUT);
  pinMode(soilPin, INPUT);
  pinMode(MQ_sensor, INPUT);
  pinMode(waterpump, OUTPUT);
  digitalWrite(waterpump,HIGH);  // inisialisasi kondisi waterpump mati
  
  buzzer(1);  // memanggil fungsi buzzer

   Serial.begin(115200); // Memulai serial monitor pada baud rate 115200
   dht.begin();
   
  connectWifi(); // memanggil fungsi koneksi wifi
  Blynk.virtualWrite(V4, LOW);  // inisialisasi button waterpump pada Blynk
  buzzer(3);  // memanggil fungsi buzzer

}

void loop() {

  Blynk.run();
  
  if (!Blynk.connected()) {
    Serial.println("Blynk disconnected, trying to reconnect...");
    Blynk.connect();
  }
  
  float humidity = dht.readHumidity();  //membaca data kelembapan udara pada DHT11
  float temperature = dht.readTemperature(); //membaca data suhu udara pada DHT11
  int gass = analogRead(MQ_sensor);   //membaca nilai data sensor gass
  int soil = analogRead(soilPin);  soil = soil*100/4095; //membaca nilai data sensor kelembapan tanah
  soil = map(soil, 0, 100, 100, 0); //mengubah rentang nilai soil 1-100 ke 100-0


  Serial.print("Soil:"); Serial.print(soil); Serial.print(" ");
  Serial.print("Temp:"); Serial.print(temperature); Serial.print(" ");
  Serial.print("Gass:"); Serial.print(gass);Serial.println(""); 
  
  Blynk.virtualWrite(V0, gass);  //mengirimkan data gas ke Blynk
  Blynk.virtualWrite(V1, soil); //mengirimkan data kelembapan tanah ke Blynk
  Blynk.virtualWrite(V2, temperature); //mengirimkan data suhu udara DHT11 ke Blynk
  Blynk.virtualWrite(V3, humidity); //mengirimkan data kelembapan udara DHT11 ke Blynk

//mengirimkan Pesan notifikasi ke Blynk
  if(temperature >= 37 ){
    Blynk.logEvent("temperature","Suhu udara melebihi 37°C"); 
  }
  if(gass >= 2000 ){
    Blynk.logEvent("gas","Gas Berbahaya Terdeteksi, Buzzer Menyala"); 
    buzzer(2);
  }
  if(soil <= 40 ){
    Blynk.logEvent("soil","Kelembapan tanah kurang dari 50%, Segera nyalakan waterpump untuk melakukan penyiraman"); 
  }
}

//menghidupkan dan mematikan waterpump DC menggunakan button Blynk
BLYNK_WRITE(V4){
  int pinValue = param.asInt();
  if(pinValue){
    digitalWrite(waterpump, LOW);
  } else {
    digitalWrite(waterpump, HIGH);
  }
}
