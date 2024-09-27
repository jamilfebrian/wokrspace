#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define ssid      "Tinkpad"   
#define password  "12345678" 
#define url       "http://192.168.130.31/myPhpProject/project_1/db_insert.php"

#define RST_PIN    D3     
#define SS_PIN     D8
#define RELAY      D4

#define jumlah_kartu   2
#define USER     "03 E4 E4 26"
#define MAMIKOS  "C3 83 70 14"

const String no_ruang = "2";
String IDCard[jumlah_kartu] = {USER, MAMIKOS};
MFRC522 mfrc522(SS_PIN, RST_PIN); 

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000); 

String date[7] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jum\\'at", "Sabtu"};

bool kunci = false;
String hari;

void connectWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Menghubungkan Wifi");
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    } Serial.println("Wifi Terhubung");
}

void sendDataBase(String nama, String UID, String Status){
  timeClient.update();
  int dayOfWeek = timeClient.getDay();
  String hari = date[dayOfWeek];
  
  String postData = "nama=" + nama + "&UID=" + UID + "&ruang=" + no_ruang + "&status=" + Status + "&hari=" + hari;

  WiFiClient client;
  HTTPClient http;
  http.begin(client, url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(postData);
  String payload = http.getString();
  Serial.println(httpCode);  Serial.println(payload);
}

void readRFID(){
  SPI.begin();  mfrc522.PCD_Init(); 
  String content = "";
  if (!mfrc522.PICC_IsNewCardPresent()) {
     return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  for (byte i = 0; i < mfrc522.uid.size; i++){
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }  content.toUpperCase(); //Serial.println(content);
    for(byte i=0; i<jumlah_kartu; i++){
      if(content.substring(1) == IDCard[i]){
         String nama = (content.substring(1) == IDCard[0]) ? "Joni Simatupang" : "IBUK KOS";
         digitalWrite(RELAY, LOW); 
         sendDataBase(nama, content, "Masuk");
         delay(1000);
         return;
      } else {
         continue;
      }
    }   

    digitalWrite(RELAY, HIGH); Serial.println("UID Tidak Terdaftar");
    kunci = false;
}

void setup() {
  Serial.begin(115200);
  while (!Serial); 
  SPI.begin();  mfrc522.PCD_Init(); 
  delay(4); 
  mfrc522.PCD_DumpVersionToSerial(); 

  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, HIGH);

  connectWifi();
   timeClient.begin();
}

void loop() {
  if(WiFi.status() != WL_CONNECTED) {
     connectWifi();
  }
  
   readRFID();
}
