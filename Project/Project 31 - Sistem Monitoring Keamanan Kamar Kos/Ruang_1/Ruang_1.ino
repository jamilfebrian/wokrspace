#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <LiquidCrystal_I2C.h> 
#include <MFRC522.h>
#include <CTBot.h>

#define ssid          "DEBY"   
#define password      "12345678" 
#define telegramToken "7952401874:AAELtBFaYbJhSRGUMwgV8yd6xkQKLACwtIA" 
#define IDTelegram     1259810770
#define path          "http://192.168.137.1/TA/"
#define url            path "db_insert.php"

#define RST_PIN    D3     
#define SS_PIN     D8
#define RELAY      D4
#define BUTTON     A0

static String arrData[3];
static String nama, uuid, nik;

const String no_ruang = "1";
LiquidCrystal_I2C lcd(0x27,16,2); 

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200, 60000);
MFRC522 mfrc522(SS_PIN, RST_PIN); 

CTBot myBot;
TBMessage msg; 

unsigned long cTime = 0;
unsigned long bTime = 0;

bool pesanTerkirim = false;  

bool logic1 = false;
bool logic2 = false;
bool logic3 = false;

void connectWifi(){
  lcd.setCursor(0,0); lcd.print("Menghubungkan Ke");
  lcd.setCursor(0,1); lcd.print(" Wifi : "+ String(ssid));
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Menghubungkan Wifi");
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    } Serial.println("Wifi Terhubung");
}

void loginTelegram(){
       lcd.setCursor(0,0); lcd.print("Menghubungkan Ke");
       lcd.setCursor(0,1); lcd.print(" Bot Telegram   ");
    Serial.println("login in telegram...");
    while (!myBot.testConnection()){
        myBot.setTelegramToken(telegramToken);
        delay(1000);}
  if(myBot.testConnection()){
    lcd.setCursor(0,0); lcd.print("Koneksi Telegram");
    lcd.setCursor(0,1); lcd.print("   Terhubung    ");
       Serial.print("Telegram connection OK!"); 
       Serial.print("Waktu Koneksi Telegram : ");  
       delay(100);
  } else {
       Serial.println("Connection Not OK");
  }
}


void sendDataBase(String UID, String aksi){
  String postData = "UID=" + UID + "&ruang=" + no_ruang + "&aksi=" + aksi;

  WiFiClient client;
  HTTPClient http;
  http.begin(client, url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(postData);
  String payload = http.getString();
  Serial.println(httpCode);  Serial.println(payload);
}

String getDateBase(String UID){
  String postData = "UID=" + UID + "&ruang=" + no_ruang;
  String URL = String(path)+"getDate.php";

  WiFiClient client;
  HTTPClient http;
  http.begin(client, URL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(postData);
  String payload = http.getString(); 
  return payload;
}

String getUID(String UID){
  String postData = "UID=" + UID + "&ruang=" + no_ruang;
  String URL = String(path)+"getUID.php";

  WiFiClient client;
  HTTPClient http;
  http.begin(client, URL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(postData);
  String payload = http.getString(); 
  return payload;
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
  }  content.toUpperCase();
  
  String UID = content.substring(1);
  String data_user = getUID(UID);

    if (data_user != ""){
      int index = 0;
      for(int i=0; i<=data_user.length(); i++){
        char delimiter = '|';
        if(data_user[i] != delimiter){
          arrData[index] += data_user[i];
        } else { 
          index++;
        }
      }
    }

     nama = arrData[0];
     uuid = arrData[1];
     nik = arrData[2];
  
    if(UID == uuid){
       cTime = millis();
       displayLCD(3);
       digitalWrite(RELAY, LOW); 
       myBot.sendMessage(IDTelegram, nama + " Dengan NIK "+ nik +" Membuka Pintu Ruangan " + no_ruang);
       sendDataBase(UID, "Masuk"); 
       cTime = millis();
       delay(1000);
       return;
    }

    digitalWrite(RELAY, HIGH); Serial.println("UID Tidak Terdaftar");
    cTime = millis();  displayLCD(2);
    sendDataBase(UID, "Masuk"); delay(1000);
}

void displayLCD(int index){
  if(millis() - cTime < 5000){
    switch(index){
      case 1:
        lcd.setCursor(0,0); lcd.print("  Silahkan Tap  ");
        lcd.setCursor(0,1); lcd.print("   Kartu Anda   ");
        logic1 = true;
        break;
      case 2:
        lcd.setCursor(0,0); lcd.print(" ID Anda Tidak  ");
        lcd.setCursor(0,1); lcd.print("     Cocok      ");
        myBot.sendMessage(IDTelegram, "Pengguna Tidak Terdaftar Berusaha Membuka Pintu Ruangan " + no_ruang);
        logic1 = true;
        break;
      case 3:
        lcd.setCursor(0,0); lcd.print("    Silahkan    ");
        lcd.setCursor(0,1); lcd.print("     Masuk      ");
        logic1 = true;
        break;
    }
  } else {
    if(logic1){
    lcd.init(); lcd.backlight(); lcd.clear();
    lcd.setCursor(0,0); lcd.print("  Silahkan Tap  ");
    lcd.setCursor(0,1); lcd.print("   Kartu Anda   ");
      digitalWrite(RELAY, HIGH);
      logic1 = false;
    }
    
  }
}

void button(){
  int value = analogRead(BUTTON);
  if(value >= 40){
    bTime = millis();
  }
  
  if(millis() - bTime < 3000){
    lcd.setCursor(0,0); lcd.print("   Pntu Telah   ");
    lcd.setCursor(0,1); lcd.print("     dibuka     ");
    digitalWrite(RELAY, LOW); 
    logic2 = true;
    if(logic3){
      sendDataBase(uuid, "Keluar"); 
      logic3 = false;
    }
  } else {
    if(logic2){
      lcd.init(); lcd.backlight(); lcd.clear();
      lcd.setCursor(0,0); lcd.print("  Silahkan Tap  ");
      lcd.setCursor(0,1); lcd.print("   Kartu Anda   ");
      digitalWrite(RELAY, HIGH);
      logic2 = false; logic3 = true;
    }
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial); 
  
  SPI.begin();  mfrc522.PCD_Init(); 
  delay(4); logic3 = true;
  mfrc522.PCD_DumpVersionToSerial(); 

  lcd.init(); lcd.backlight(); lcd.clear();

  pinMode(BUTTON,INPUT);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, HIGH);

  connectWifi();
  loginTelegram();
  
  lcd.setCursor(0,0); lcd.print("  Silahkan Tap  ");
  lcd.setCursor(0,1); lcd.print("   Kartu Anda   ");

  timeClient.begin();
}

void loop() {
  if(WiFi.status() != WL_CONNECTED) {
     connectWifi();
  }

  timeClient.update();
  
//  String formattedTime = timeClient.getFormattedTime();
//  Serial.println("Waktu saat ini: " + formattedTime);
  
  int jam = timeClient.getHours();
  int menit = timeClient.getMinutes();
  int detik = timeClient.getSeconds();

  String statusSewa = getDateBase(uuid);

  if (jam == 8 && menit >= 0 && detik >= 0 && !pesanTerkirim) {  //Atur jam pengingat waktu sewa berakhir
    if(statusSewa == "1"){
      myBot.sendMessage(IDTelegram, "Waktu Sewa " + String(nama) + "\nPada Ruangan " + String(no_ruang)+" Berakhir 1 Hari Lagi");
      pesanTerkirim = true;  
    } else if(statusSewa == "0"){
      myBot.sendMessage(IDTelegram, "Waktu Sewa " + String(nama) + "\nPada Ruangan " + String(no_ruang)+" Telah Berakhir");
      pesanTerkirim = true;  
    }
  }

  if (jam == 0 && menit == 0 && detik == 0) {
    pesanTerkirim = false;
  }
  
   readRFID();
   displayLCD(5);
   button();
}
