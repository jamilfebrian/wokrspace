#include <WiFi.h>
#include <Keypad.h>
#include <NewPing.h>
#include <LiquidCrystal_I2C.h>
#include "CTBot.h"

#define LIMIT_SWICTH_PAKET 34
#define LIMIT_SWICTH_UANG  39
#define RELAY_PINTU_PAKET  4
#define RELAY_PINTU_UANG   5
#define LED_PIN            2 
#define BUZZER             15
#define TRIGPIN            18
#define ECHOPIN            19

#define ssid        "Tinkpad"
#define password    "12345678"
#define token       "7111168526:AAGVUM_-6o5o0S3UVcUsgmsFsgd2g6p1Unc"
#define IDTelegram  msg.sender.id

char keys[4][4] = { {'1','2','3','A'},
                    {'4','5','6','B'},
                    {'7','8','9','C'},
                    {'*','0','#','D'}};
byte rowPins[4] = {13, 12, 14, 27}; // konektor baris ke pin ESP32
byte colPins[4] = {26, 25, 33, 32}; // konektor kolom ke pin ESP32

LiquidCrystal_I2C lcd(0x27, 16, 2);
NewPing sonar(TRIGPIN, ECHOPIN, 200); 
Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);
CTBot myBot;
TBMessage msg; 

static unsigned long prevMillis = 0, timerMillis = 500;
static int randomNumber;
static String passwordPaket = "";
static String cursorLCD = "";
static bool logic0, logic1, logic2, logic3, logic4;
static bool x1,x2,x3,x4,x5;

void buzzer(int mode = 0, int index = 3, int timer = 80){
  switch(mode){
    case 0 :
      for(int i=0; i<index; i++){
        digitalWrite(BUZZER, HIGH);
        delay(timer);
        digitalWrite(BUZZER, LOW);
        delay(timer);
      } break;
    case 1 : 
      for(int i=0; i<index; i++){
        digitalWrite(LED_PIN, HIGH);
        delay(timer);
        digitalWrite(LED_PIN, LOW);
        delay(timer);
      } break;
    case 2 :
      for(int i=0; i<index; i++){
        digitalWrite(LED_PIN, HIGH);
        digitalWrite(BUZZER, HIGH);
        delay(timer);
        digitalWrite(LED_PIN, LOW);
        digitalWrite(BUZZER, LOW);
        delay(timer);
      } break;
    default :
      break;
  }
}

void connectWifi(){
  long respond;
  long response = millis();
  Serial.println("Connecting to WiFi...");
      lcd.setCursor(0,0); lcd.print("Menghubungkan Ke");
      lcd.setCursor(0,1); lcd.print("Jaringan Wifi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); 
    Serial.print("."); lcd.print(".");
  }  Serial.println("Wifi Terhubung");
  respond = millis();
  Serial.print("Waktu Koneksi WIfi : ");  Serial.print(respond-response); Serial.println(" detik");
       lcd.setCursor(0,0); lcd.print(" Jaringan Wifi  ");
       lcd.setCursor(0,1); lcd.print("   Terhubung    ");
       buzzer(2,2);
}

void loginTelegram(){
  long respond;
  long response = millis();
    Serial.println("login in telegram...");
    while (!myBot.testConnection()){
        myBot.setTelegramToken(token);
        delay(100);}
  if(myBot.testConnection()){
    respond = millis();
       Serial.print("Telegram connection OK!"); 
       Serial.print("Waktu Koneksi Telegram : ");  
       Serial.print(respond-response); Serial.println(" detik");
  } else {
       Serial.println("Connection Not OK");
       buzzer(2,5); 
  }
}

void setup() {
  Serial.begin(115200);
  lcd.init(); lcd.backlight(); lcd.clear();
  
  pinMode(LIMIT_SWICTH_PAKET,INPUT);
  pinMode(LIMIT_SWICTH_UANG,INPUT);
  pinMode(RELAY_PINTU_PAKET,OUTPUT);
  pinMode(RELAY_PINTU_UANG,OUTPUT);
  pinMode(LED_PIN,OUTPUT);
  pinMode(BUZZER,OUTPUT);

  digitalWrite(RELAY_PINTU_PAKET,HIGH);
  digitalWrite(RELAY_PINTU_UANG,HIGH);
  digitalWrite(LED_PIN,LOW);

  buzzer(2,1);
  connectWifi();
  loginTelegram();
  logic0 = true;

  randomNumber = random(1000, 9999);
  Serial.print("Kode Anda : "); Serial.println(randomNumber);
}

void loop() {
  TBMessage msg; 
  char key = myKeypad.getKey();
  int range = sonar.ping_cm();
  int status_paket = digitalRead(LIMIT_SWICTH_PAKET);
  int status_uang = digitalRead(LIMIT_SWICTH_UANG);

  Serial.print("Range : "); Serial.print(range); Serial.println(" cm");

  if((millis() - prevMillis) > timerMillis){
    if(cursorLCD == "_"){
      cursorLCD = "";
    } else {
      cursorLCD = "_";
    } 
    prevMillis = millis();
  }


  if(logic0){
    lcd.setCursor(0,0); lcd.print("Masukkan Kode : ");
    lcd.setCursor(0,1); lcd.print(passwordPaket + cursorLCD + "                ");

     if (key != NO_KEY) {
      if(key != '#'){
        buzzer(0,1,40);
      }
      passwordPaket += key;
      if(key == '*'){
          passwordPaket.remove(passwordPaket.length()-2);
      } else if(key == '#'){
          passwordPaket.remove(passwordPaket.length()-1);
          if(passwordPaket == String(randomNumber)){
              lcd.setCursor(0,0); lcd.print("   Kode Benar   ");
              lcd.setCursor(0,1); lcd.print("                ");
              buzzer(0); digitalWrite(RELAY_PINTU_PAKET, LOW);
              logic0 = false; logic1 = true;
          } else {
              lcd.setCursor(0,0); lcd.print("MAAF! Kode yang ");
              lcd.setCursor(0,1); lcd.print("Dimasukkan Salah");
              buzzer(0,1,1000);
              delay(1000);
          }
          passwordPaket = "";
      }
    }
  }
  

  if(logic1){
    if(range < 30 && range != 0){
      digitalWrite(LED_PIN, HIGH);
      if(x1){
        lcd.setCursor(0,0); lcd.print("  Paket  Telah  ");
        lcd.setCursor(0,1); lcd.print("   Dimasukkan   ");
        delay(1000); x1 = false;
      } else {
        lcd.setCursor(0,0); lcd.print(" Silahkan Tutup ");
        lcd.setCursor(0,1); lcd.print("  Pintu Kembali ");
        logic2 = true; 
      }
    } else {
      lcd.setCursor(0,0); lcd.print("    Silahkan    ");
      lcd.setCursor(0,1); lcd.print(" Masukkan Paket ");
      digitalWrite(LED_PIN, LOW);
      x1 = true;  logic2 = false; 
    }
  }
  
  if(logic2){
    if(!status_paket){
      logic1 = false;
      digitalWrite(RELAY_PINTU_UANG, LOW);
      digitalWrite(RELAY_PINTU_PAKET, HIGH);
      buzzer(2,2,200);
      digitalWrite(LED_PIN, HIGH);
        lcd.setCursor(0,0); lcd.print(" Silahkan Ambil ");
        lcd.setCursor(0,1); lcd.print("  Uang Di Atas  ");
      myBot.sendMessage(IDTelegram, "Kotak paket anda dibuka oleh kurir");
        delay(3000);  logic3 = true; logic2 = false;
    }
  }

  if(logic3){
    if(!status_uang){
        lcd.setCursor(0,0); lcd.print("  Terima Kasih  ");
        lcd.setCursor(0,1); lcd.print("  Pak Kurir! :) ");
 
        buzzer(0);
        digitalWrite(LED_PIN, LOW);
        randomNumber = random(1000, 9999);
        digitalWrite(RELAY_PINTU_UANG, HIGH);
    
        myBot.sendMessage(IDTelegram, "Paket pesanan anda telah sampai,\nSegera cek kelengkapan pesanan anda.");
        
        myBot.sendMessage(IDTelegram, "Kode Paket Diperbarui\nKode Paket Anda : " +String(randomNumber));
        delay(3000); logic3 = false; logic0 = true;
    } else {
        lcd.setCursor(0,0); lcd.print("  Mohon Pintu   ");
        lcd.setCursor(0,1); lcd.print("Ditutup Kembali ");
    }
  }

long respond;
long response = millis();
  if (myBot.getNewMessage(msg)){ 
    if (msg.text.equalsIgnoreCase("/start") || msg.text.equalsIgnoreCase("/menu") || msg.text.equalsIgnoreCase("/help")) {
      myBot.sendMessage(IDTelegram, "Menu :\n\n/code -> Mengecek kode paket.\n/changeCode -> Update kode paket\n/openPaket -> Membuka kotak paket\n/openBrankas ->Membuka kotak brankas\n/status -> Mengecek status isi kotak paket");
      respond = millis();   Serial.print("Waktu response Telegram : ");  Serial.print(respond-response); Serial.println(" detik");
    } else if (msg.text.equalsIgnoreCase("/changeCode")){
      randomNumber = random(1000, 9999);
      myBot.sendMessage(IDTelegram, "Kode Paket Diperbarui\nKode Paket Anda : " +String(randomNumber));
    } else if (msg.text.equalsIgnoreCase("/code")){
      myBot.sendMessage(IDTelegram, "Kode Paket Anda : " +String(randomNumber));
    } else if (msg.text.equalsIgnoreCase("/openPaket")){
      buzzer(2,2,200);
      digitalWrite(RELAY_PINTU_PAKET,LOW); x2=true;
      myBot.sendMessage(IDTelegram, "Kotak paket berhasil dibuka");
    } else if (msg.text.equalsIgnoreCase("/openBrankas")){
      buzzer(2,2,200);
      digitalWrite(RELAY_PINTU_UANG,LOW); x3=true;
      digitalWrite(LED_PIN,HIGH);
      myBot.sendMessage(IDTelegram, "Kotak brankas berhasil dibuka");
    } else if (msg.text.equalsIgnoreCase("/status")){
      if(range < 30){
        myBot.sendMessage(IDTelegram, "Paket pesanan anda telah sampai,\nSegera cek kelengkapan pesanan anda.");
      } else {
        myBot.sendMessage(IDTelegram, "Paket pesanan anda belum sampai,\nMohon menunggu hingga paket pesanan anda sampai. ");
      }
    } else if (msg.text.equalsIgnoreCase("p") || msg.text.equalsIgnoreCase("P")){
      lcd.init(); lcd.backlight(); lcd.clear();
    } else {
      myBot.sendMessage(IDTelegram, "Perintah yang anda masukkan tidak valid!\ntekan /menu untuk melihat daftar perintah");
    }
  }
    
  if(x2){
    if(!status_paket){
      digitalWrite(RELAY_PINTU_PAKET,HIGH);
      x2=false;
    }
  }
  if(x3){
    if(!status_uang){
      digitalWrite(RELAY_PINTU_UANG,HIGH); 
      digitalWrite(LED_PIN,LOW);
      x3=false;
    }
  }
}
