#include <WiFi.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 
#include "DHT.h"
#include "RTClib.h"
#include "CTBot.h" 

#define ssid      "OPPO A17"
#define password  ""
#define token     "6234639140:AAEAVHJCiSCAnyEM_COs1N-rk_1cgEMOnLA"
#define id        647924843

#define sensorHujan   2
#define limitSwitch1  4
#define limitSwitch2 17
#define sensorSuhu   27
#define motor1a      12
#define motor1b      14
#define blower       25
#define lampu        32
#define fan          33

CTBot myBot;
CTBotInlineKeyboard kbd, ya;
DHT dht(sensorSuhu, DHT11);
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27,16,2); 
byte Simbol_derajat = B11011111;
char days[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jum'at", "Sabtu"};
static int motor=0, statusBlow=0;
static int x=0, counter1=0,counter2=0, counter3=0;
unsigned short int pagiCerah=0,malamCerah=0,pagiHujan=0,malamHujan=0;
int temp = 60;

void connectWifi(){
    Serial.println("Connecting To Wifi");
    lcd.setCursor(0,0);
    lcd.print("   Connecting   ");
    lcd.setCursor(0,1);
    lcd.print("    To  Wifi    ");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("Wifi Connected");
    Serial.println(WiFi.SSID());
    Serial.println(WiFi.RSSI());
    Serial.println(WiFi.macAddress());
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.gatewayIP());
    Serial.println(WiFi.dnsIP());
    lcd.setCursor(0,0);
    lcd.print("     WiFi       ");
    lcd.setCursor(0,1);
    lcd.print("   Connected    ");
    delay(1500);
}

void loginTelegram(){
    lcd.setCursor(0,0);
    lcd.print("Menghubungkan Ke");
    lcd.setCursor(0,1);
    lcd.print("  Telegram Bot  ");
    Serial.println("login in telegram...");
    while (!myBot.testConnection())    {
        myBot.setTelegramToken(token);
        Serial.print(".");
        delay(100);}
        
  if(myBot.testConnection()){
       Serial.println("Telegram connection OK!");
       lcd.setCursor(0,0);
       lcd.print("  Telegram Bot  ");
       lcd.setCursor(0,1);
       lcd.print("   Terhubung    ");
  } else {
       Serial.println("Connection OK");
       lcd.setCursor(0,0);
       lcd.print(" Telegram  Bot  ");
       lcd.setCursor(0,1);
       lcd.print("Gagal  Terhubung");
  }delay(1500);
}

void setup(){
  pinMode(sensorHujan, INPUT);
  pinMode(limitSwitch1, INPUT_PULLUP);
  pinMode(limitSwitch2, INPUT_PULLUP);
  pinMode(sensorSuhu, INPUT);
  pinMode(motor1a, OUTPUT);
  pinMode(motor1b, OUTPUT);
  pinMode(blower, OUTPUT);
  pinMode(lampu, OUTPUT);
  pinMode(fan, OUTPUT);

  digitalWrite(blower,HIGH);
  digitalWrite(lampu,HIGH);
  digitalWrite(fan,HIGH);

  lcd.begin(); lcd.backlight();
  dht.begin(); lcd.clear();
  Serial.begin(9600); 
  connectWifi();
  loginTelegram();

  kbd.addButton("15 Menit", "m15", CTBotKeyboardButtonQuery); 
  kbd.addButton("30 Menit", "m30", CTBotKeyboardButtonQuery);
  kbd.addButton("45 Menit", "m45", CTBotKeyboardButtonQuery);
  kbd.addRow();
  kbd.addButton("1 Jam", "j1", CTBotKeyboardButtonQuery);
  kbd.addButton("2 Jam", "j2", CTBotKeyboardButtonQuery);
  kbd.addButton("3 Jam", "j3", CTBotKeyboardButtonQuery);
  kbd.addButton("5 Jam", "j5", CTBotKeyboardButtonQuery);
  ya.addButton("YAA!!", "yes", CTBotKeyboardButtonQuery);
  ya.addButton("TIDAK", "no", CTBotKeyboardButtonQuery);
  
  if (! rtc.begin()) {
    Serial.println("Could not find RTC! Check circuit.");
    while (1); }
  
//  rtc.adjust(DateTime(__DATE__, __TIME__));
}

void loop(){
  TBMessage msg; 
  DateTime now = rtc.now();
  static DateTime future;
  int detik = now.second();
  int menit = now.minute();
  int jam = now.hour();
  int tanggal = now.day();
  int bulan = now.month();
  int tahun = now.year();
  String hari = days[now.dayOfTheWeek()];
  
  float kelembapan = dht.readHumidity();
  float suhu = dht.readTemperature();
  int hujan = digitalRead(sensorHujan);
  int LSwitch1 = digitalRead(limitSwitch1);
  int LSwitch2 = digitalRead(limitSwitch2);

  if (myBot.getNewMessage(msg)){
    if (msg.text.equalsIgnoreCase("/start")) {
      if (x==0){
        future = now;
        myBot.sendMessage(id, "Pengering kerupuk otomatis Diaktifkan!! 🙂\n\nSilahkan atur pewaktu Pemanas !! 👇", kbd);
      } else if (x>=1 && ((future - now).hours()>0 || (future - now).minutes()>0 || (future - now).seconds()>0)){
        myBot.sendMessage(id, "Mesin Telah diaktifkan! \nTunggu Selama\n"+String((future - now).hours())+" Jam : "+String((future - now).minutes())+" Menit : "+String((future - now).seconds())+ " Detik\nHingga Kerupuk Kering 😁");
      } else if (x>=1 && (future - now).hours()<=0 && (future - now).minutes()<=0 && (future - now).seconds()<=0){
        myBot.sendMessage(id, "Silahkan atur pewaktu dibawah Terlebih dahulu 👇" ,kbd);
      } x++;
    } else if(msg.text.equalsIgnoreCase("/status")) {
      if (statusBlow==1){
        myBot.sendMessage(id, "Pemanas Diaktifkan!\n\nWaktu Tersisa  \n"+String((future - now).hours())+" Jam : "+String((future - now).minutes())+" Menit : "+String((future - now).seconds())+" Detik\nSuhu : "+String(suhu)+"°C\nKelembapan : "+String(kelembapan)+" %");
      } else if (statusBlow==0){
         myBot.sendMessage(id, "Pemanas Tidak Aktif! \n\nSuhu : "+String(suhu)+" °C\nKelembapan : "+String(kelembapan)+" %");
      }
    } else if(msg.text.equalsIgnoreCase("/time")) {
      myBot.sendMessage(id, "Silahkan Pilih Tambahan Waktu!",kbd);
    } else if(msg.text.equalsIgnoreCase("/off")) {
      digitalWrite(blower,HIGH);
      digitalWrite(fan,HIGH);
      statusBlow = 0; x=0;
      future = now + TimeSpan(0, 0, 0, 0);
      myBot.sendMessage(id, "Pemanas Telah dimatikan  🙃");
    } else if (msg.messageType == CTBotMessageQuery ) {
        if (msg.callbackQueryData.equals("m15") && x>0) {
        myBot.endQuery(msg.callbackQueryID, "Waktu ditambahkan 15 Menit 🕒");
        future = future + TimeSpan(0, 0, 15, 0);
      } else if (msg.callbackQueryData.equals("m30") && x>0) {
        myBot.endQuery(msg.callbackQueryID, "Waktu ditambahkan 30 Menit 🕒");
        future = future + TimeSpan(0, 0, 30, 0);
      } else if (msg.callbackQueryData.equals("m45") && x>0) {
        myBot.endQuery(msg.callbackQueryID, "Waktu ditambahkan 45 Menit 🕒");
        future = future + TimeSpan(0, 0, 45, 0);
      } else if (msg.callbackQueryData.equals("j1") && x>0) {
        myBot.endQuery(msg.callbackQueryID, "Waktu ditambahkan 1 Jam 🕒");
        future = future + TimeSpan(0, 1, 0, 0);
      } else if (msg.callbackQueryData.equals("j2") && x>0) {
        myBot.endQuery(msg.callbackQueryID, "Waktu ditambahkan 2 Jam 🕒");
        future = future + TimeSpan(0, 2, 0, 0);
      } else if (msg.callbackQueryData.equals("j3")&& x>0) {
        myBot.endQuery(msg.callbackQueryID, "Waktu ditambahkan 3 Jam 🕒");
        future = future + TimeSpan(0, 3, 0, 0);
      } else if (msg.callbackQueryData.equals("j5")&& x>0) {
        myBot.endQuery(msg.callbackQueryID, "Waktu ditambahkan 5 Jam 🕒");
        future = future + TimeSpan(0, 5, 0, 0);
      } else if (msg.callbackQueryData.equals("yes")) { x=1;
        myBot.sendMessage(id, "Silahkan Pilih Tambahan Waktu!",kbd);
      } else if (msg.callbackQueryData.equals("no")) {
        myBot.endQuery(msg.callbackQueryID, "OKE BOSQUEE! 🫡🫡");
      }
    } else if (msg.text.equalsIgnoreCase("/menu")){
      myBot.sendMessage(id, "Silahkan pilih menu dibawah 👇\n/start --> Menyalakan Pemanas \n/status --> Melihat Status pemanas\n/time --> Menambah timer Pemanas\n/off --> Mematikan Pemanas\n/menu --> Pilihan Menu");
    } else if(msg.text.equalsIgnoreCase("/jam")) {
      myBot.sendMessage(id, "Jam Sekarang\n"+String(now.hour())+" : "+String(now.minute())+" : "+String(now.second())+" WIB\n"+String(hari)+", "+String(tanggal)+"-"+String(bulan)+"-"+String(tahun));
    } else {
      myBot.sendMessage(id, "Perintah tidak dikenali! ☠️☠️\nSilahkan pilih menu dibawah 👇\n/start --> Menyalakan Pemanas \n/status --> Melihat Status pemanas\n/time --> Menambah timer Pemanas\n/off --> Mematikan Pemanas\n/menu --> Pilihan Menu");
    }
  }

  if(suhu<=temp && counter1==0 && ((future - now).hours()>0 || (future - now).minutes()>0 || (future - now).seconds()>0 )){
    digitalWrite(blower,LOW);
    digitalWrite(fan,LOW);
    lcd.setCursor(0,0); lcd.print("  Suhu  Rendah  ");
    lcd.setCursor(0,1); lcd.print("   Pemanas ON   ");
    myBot.sendMessage(id, "Suhu Rendah! ❄️\nMenyalakan Pemanas!");
    statusBlow = 1; counter1=1; counter2=0;
  } else if (suhu>temp && counter2==0){
    digitalWrite(blower,HIGH);
    digitalWrite(fan,HIGH);
    lcd.setCursor(0,0); lcd.print("  Suhu  Tinggi  ");
    lcd.setCursor(0,1); lcd.print("  Pemanas OFF   ");
    myBot.sendMessage(id, "Suhu Terlalu Tinggi!🔥 \nMematikan Pemanas! ");
    statusBlow = 0; counter1=0; counter2=1;
  }

  if (jam>=7 && jam<18 && hujan==1){
    motor=1; pagiCerah++;       //membuka
  } else if ((jam<7 || jam>=18) && hujan==1){
    motor=2; malamCerah++;      //menutup
  } else if (jam>=7 && jam<18 && hujan==0){
    motor=2; pagiHujan++;      //menutup
  } else if ((jam<7 || jam>=18) && hujan==0){
    motor=2; malamHujan++;     //menutup
  } else {motor=0; }

  if (LSwitch1==0 || LSwitch2==0){
    motor=0;
  }

   switch(motor){
    case 0 : //diam
      digitalWrite(motor1a,LOW);
      digitalWrite(motor1b,LOW);
      break;
    case 1 :
      digitalWrite(motor1a,LOW);
      digitalWrite(motor1b,HIGH);
      digitalWrite(lampu,HIGH);
      break;
    case 2 :
      digitalWrite(motor1a,HIGH);
      digitalWrite(motor1b,LOW);
      digitalWrite(lampu,LOW);
      break;
    default :
      digitalWrite(motor1a,LOW);
      digitalWrite(motor1b,LOW);
      break;
   }

   if (pagiCerah==1){
        lcd.clear();
        lcd.setCursor(0,0); lcd.print("Cuaca Cerah     ");
        lcd.setCursor(0,1); lcd.print("Pukul");
        lcd.setCursor(6,1); lcd.print(jam);
        lcd.setCursor(8,1); lcd.print(":");
        lcd.setCursor(9,1); lcd.print(menit);
        lcd.setCursor(11,1); lcd.print(":");
        lcd.setCursor(12,1); lcd.print(detik);
        myBot.sendMessage(id, "Cuaca Cerah 🌤 \n\n"+String(hari)+", Pukul "+String(jam)+" : "+String(menit)+" : "+String(detik)+" WIB.\nAtap Telah dibuka!! 😇");
        malamCerah=0;pagiHujan=0; malamHujan=0;
   } else if (malamCerah==1){
        lcd.clear();
        lcd.setCursor(0,0); lcd.print("Cuaca  Cerah    ");
        lcd.setCursor(0,1); lcd.print("Pukul ");
        lcd.setCursor(6,1); lcd.print(jam);
        lcd.setCursor(8,1); lcd.print(":");
        lcd.setCursor(9,1); lcd.print(menit);
        lcd.setCursor(11,1); lcd.print(":");
        lcd.setCursor(12,1); lcd.print(detik);
        myBot.sendMessage(id, "Cuaca Cerah 🌤 \nNamun Malam Telah tiba! 🌙 \n\n"+String(hari)+", Pukul "+String(jam)+" : "+String(menit)+" : "+String(detik)+" WIB.\nAtap Telah Ditutup!! 😇");
        pagiCerah=0; pagiHujan=0; malamHujan=0;
   } else if (pagiHujan==1){
        lcd.clear();
        lcd.setCursor(0,0); lcd.print("Cuaca  Hujan    ");
        lcd.setCursor(0,1); lcd.print("Pukul");
        lcd.setCursor(6,1); lcd.print(jam);
        lcd.setCursor(8,1); lcd.print(":");
        lcd.setCursor(9,1); lcd.print(menit);
        lcd.setCursor(11,1); lcd.print(":");
        lcd.setCursor(12,1); lcd.print(detik);
        myBot.sendMessage(id, "Terjadi Hujan!!! 🌧\n\n"+String(hari)+", Pukul "+String(jam)+" : "+String(menit)+" : "+String(detik)+" WIB.\nAtap Telah Ditutup!! 😇");
        pagiCerah=0; malamCerah=0; malamHujan=0;
   } else if (malamHujan==1){
        lcd.clear();
        lcd.setCursor(0,0); lcd.print("Cuaca  Hujan    ");
        lcd.setCursor(0,1); lcd.print("Pukul");
        lcd.setCursor(6,1); lcd.print(jam);
        lcd.setCursor(8,1); lcd.print(":");
        lcd.setCursor(9,1); lcd.print(menit);
        lcd.setCursor(11,1); lcd.print(":");
        lcd.setCursor(12,1); lcd.print(detik);
        myBot.sendMessage(id, "Terjadi Hujan pada malam hari 🌧\n\n"+String(hari)+", Pukul "+String(jam)+" : "+String(menit)+" : "+String(detik)+" WIB.\nAtap Telah ditutup!! 😇");
        pagiCerah=0; malamCerah=0; pagiHujan=0;
   } else {
    if ((future - now).hours()<=0 && (future - now).minutes()<=0 && (future - now).seconds()<=0 && counter3==0 && statusBlow==1){
      lcd.clear();
      lcd.setCursor(0,0); lcd.print(" Waktu Pemanas  ");
      lcd.setCursor(0,1); lcd.print("  Telah Habis   ");
      digitalWrite(blower,HIGH);
      digitalWrite(fan,HIGH);
      myBot.sendMessage(id, "Waktu Pemanasan Telah Habis! \nApakah Anda ingin Menambah Waktu?",ya);
      counter3=1; statusBlow=0; x=0;
    } else if ((future - now).hours()>0 || (future - now).minutes()>0 || (future - now).seconds()>0 ){
      counter3=0; statusBlow=1; lcd.clear();
        lcd.setCursor(0,0); lcd.print("Suhu : ");
        lcd.setCursor(8,0); lcd.print(suhu);
        lcd.setCursor(14,0); lcd.write(Simbol_derajat);
        lcd.setCursor(15,0); lcd.print("C");
        lcd.setCursor(0,1); lcd.print("Waktu: ");
        lcd.setCursor(7,1); lcd.print((future - now).hours());
        lcd.setCursor(9,1); lcd.print(":");
        lcd.setCursor(10,1); lcd.print((future - now).minutes());
        lcd.setCursor(12,1); lcd.print(":");
        lcd.setCursor(13,1); lcd.print((future - now).seconds());
        delay(800);
        if(suhu<=temp){
          digitalWrite(blower,LOW);
          digitalWrite(fan,LOW);
        } else if (suhu>temp){
          digitalWrite(blower,HIGH);
          digitalWrite(fan,HIGH);
        }
    } else{
      lcd.clear();
      lcd.setCursor(0,0); lcd.print("Suhu : ");
      lcd.setCursor(8,0); lcd.print(String(suhu));
      lcd.setCursor(14,0); lcd.write(Simbol_derajat);
      lcd.setCursor(15,0); lcd.print("C");
      lcd.setCursor(0,1); lcd.print("Pemanas OFF     ");
      if(x>=1){future = now;}
    }
   }
}
