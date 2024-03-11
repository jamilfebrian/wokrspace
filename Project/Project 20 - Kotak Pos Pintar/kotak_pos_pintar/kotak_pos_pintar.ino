#include <WiFi.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include "CTBot.h"

#define limitSwitchPaket 36
#define limitSwitchUang  39
#define relayPintuPaket  4
#define relayPintuUang   5
#define ledWifi          2 
#define ledPaket         15
#define buzzer           0
#define trigPin          18
#define echoPin          19

#define ssid       "realme C31"
#define password   "987654321"
#define token      "5346287582:AAFyl0CBY_hr5PJDFeCJMg9X0LwKOV-RP2s"
#define IDTelegram 1599903092

char keys[4][4] = { {'1','2','3','A'},
                    {'4','5','6','B'},
                    {'7','8','9','C'},
                    {'*','0','#','D'}};
byte rowPins[4] = {13, 12, 14, 27}; // konektor baris ke pin ESP32
byte colPins[4] = {26, 25, 33, 32}; // konektor kolom ke pin ESP32

LiquidCrystal_I2C lcd(0x27, 16, 2);
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

CTBot myBot;
String kode = "";
bool Paket = false; 
bool Uang = false;
bool statusPaket = false;
static int pesan = 0;
static int randomNumber;
TBMessage msg; 

void connectWifi(){
  pinMode(ledWifi,OUTPUT);
  digitalWrite(ledWifi,LOW);
  Serial.println("Connecting to WiFi...");
      lcd.setCursor(0,0); lcd.print("Menghubungkan Ke");
      lcd.setCursor(0,1); lcd.print("Jaringan Wifi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); digitalWrite(ledWifi, LOW);
    Serial.print("."); lcd.print(".");
  }  Serial.println("Wifi Terhubung");
       lcd.setCursor(0,0); lcd.print(" Jaringan Wifi  ");
       lcd.setCursor(0,1); lcd.print("   Terhubung    ");
  digitalWrite(ledWifi,HIGH);
}

void loginTelegram(){
    Serial.println("login in telegram...");
    while (!myBot.testConnection()){
        myBot.setTelegramToken(token);
        digitalWrite(ledWifi, LOW);
      lcd.setCursor(0,0); lcd.print("Menghubungkan Ke");
      lcd.setCursor(0,1); lcd.print("Telegram....    "); 
        Serial.print(".");
        delay(100);}
  if(myBot.testConnection()){
       Serial.println("Telegram connection OK!");
       lcd.setCursor(0,0); lcd.print("  Bot Telegram  ");
       lcd.setCursor(0,1); lcd.print("   Terhubung    ");
       delay(1500);
       digitalWrite(ledWifi, HIGH);
  } else {
       Serial.println("Connection OK");
       digitalWrite(ledWifi, LOW);
  }
}

void displayLCD(int value, String kode=""){
  switch(value){
    case 1 :   
        lcd.setCursor(0,0); lcd.print("Masukkan Kode : ");
        lcd.setCursor(0,1); lcd.print(kode+"                ");
        break;
    case 2 : 
        lcd.setCursor(0,0); lcd.print("MAAF! Kode yang ");
        lcd.setCursor(0,1); lcd.print("Dimasukkan Salah");
        break;
    case 3 :
        lcd.setCursor(0,0); lcd.print("    Silahkan    ");
        lcd.setCursor(0,1); lcd.print(" Masukkan Paket ");
        break;
     case 4 :
        lcd.setCursor(0,0); lcd.print("   Mohon Tutup  ");
        lcd.setCursor(0,1); lcd.print("  Pintu Kembali ");
        break;
     case 5 :
        lcd.setCursor(0,0); lcd.print(" Silahkan Ambil ");
        lcd.setCursor(0,1); lcd.print(" Uang Di Bawah  ");
        break;
    default : 
        break;
  }
}

long distance(int TRIG, int ECHO){
  digitalWrite(TRIG, LOW);  delayMicroseconds(8);
  digitalWrite(TRIG, HIGH); delayMicroseconds(8);
  digitalWrite(TRIG, LOW);  delayMicroseconds(8);
  long durasi = pulseIn(ECHO, HIGH); 
  long jarak = (durasi / 2) / 29.1;
  return jarak;
}

void setup() {
  lcd.init(); lcd.backlight();
  Serial.begin(115200);
  pinMode(limitSwitchPaket, INPUT);
  pinMode(limitSwitchUang, INPUT);
  pinMode(relayPintuPaket, OUTPUT);
  pinMode(relayPintuUang, OUTPUT);
  pinMode(ledWifi, OUTPUT);
  pinMode(ledPaket, OUTPUT);
  connectWifi();
  loginTelegram();
  displayLCD(1);

  randomNumber = random(1000, 9999);
  Serial.println(randomNumber);
}

void loop() {
 
  boolean pintuPaket = digitalRead(limitSwitchPaket);
  boolean pintuUang = digitalRead(limitSwitchUang);
  long jarak = distance(trigPin,echoPin);
  char key = keypad.getKey();

  if (myBot.getNewMessage(msg)){
    if (msg.text.equalsIgnoreCase("/newCode")) {
      randomNumber = random(1000, 9999);
      myBot.sendMessage(IDTelegram, "Kode paket Telah Diperbaharui.\nKode paket " + String(randomNumber));
    } else if(msg.text.equalsIgnoreCase("/getCode")){
      myBot.sendMessage(IDTelegram, "Kode paket " + String(randomNumber));
    } else {
      myBot.sendMessage(IDTelegram, "Keyword yang anda masukkan Salah. Silahkan Pilih Menu dibawah \n\n/getCode => Untuk mendapatkan kode paket.\n/newCode => Untuk Memperbarui Kode Paket.");
    }
  }
    
    if(key){
      kode += key;
      if(key=='*'){
        kode.remove(kode.length()-2);
      } else if (key=='#'){
        kode.remove(kode.length()-1);
        if(kode==String(randomNumber)){
            lcd.setCursor(0,0); lcd.print("   Kode Benar   ");
            lcd.setCursor(0,1); lcd.print("                ");
          digitalWrite(relayPintuPaket ,HIGH);
          Paket = true;
          if(pesan==0){
            myBot.sendMessage(IDTelegram, "Pintu Paket dibuka oleh kurir");
            pesan=1;
          }
        } else {
          displayLCD(2);
          delay(2000);
        }
        kode = "";
      }
      displayLCD(1, kode);
    }

    if(pintuPaket){
      Paket = false;
    }

    if(!Paket){
      digitalWrite(relayPintuPaket ,LOW);
    }
    
//    if(Paket && jarak>30){
//      displayLCD(3); 
//    } else if(Paket && jarak<30){
//      displayLCD(4); 
//    } else if(!Paket && jarak>30){
//      digitalWrite(relayPintuPaket, LOW);
//    } else if(!Paket && jarak<30){
//      digitalWrite(relayPintuPaket, LOW);
//      if(pesan==1){
//        myBot.sendMessage(IDTelegram, "Paket Telah Sampai, Cek kotak paket!");
//        pesan=2;
//      } digitalWrite(relayPintuUang, HIGH);
//        Uang = true;
//    }
//
//    if(!pintuUang && Uang){
//      displayLCD(5);
//      if(pesan==2){
//        myBot.sendMessage(IDTelegram, "Uang COD telah diambil kurir");
//        pesan=3;
//      }
//    } else if(pintuUang && Uang){
//      
//    }
//
//    
//    
//    if(pintuPaket){
//      Paket=false;
//      displayLCD(1, kode);
//    } 
//
//    if(jarak<30){
//      digitalWrite(ledPaket, HIGH);        
//    } else {
//      digitalWrite(ledPaket, LOW);
//    }

}
