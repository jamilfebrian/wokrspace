#include <Wire.h>
#include <NewPing.h> 
#include <WiFi.h>
#include <LiquidCrystal_I2C.h> 
#include <ESP32Servo.h> 
#include "CTBot.h" 

#define ssid     "Wi-fi"
#define password "12345678"
#define token "6494936221:AAFsp5hLMXUwWG7LBnqmQogjYEvRQgRYuBw"
#define chatId 900864729

#define s0 13
#define s1 12
#define s2 14
#define s3 27
#define outPin 26

#define irPin1       25 // Sensor IR Deteksi
#define irPin2       15 // Sensor IR Warna
#define irPin3       36 // Sensor IR Gold
#define irPin4       39 // Sensor IR Silver
#define irPin5       34  // Sensor IR No
#define trigPin      18 // Ultrasonic 
#define echoPin      19 // Ultrasonic 
#define Relay1       4  // Relay Konveyor
#define Relay2       23 // Relay LED Deteksi 
#define Relay3       16 // Relay LED TCS
#define Photodioda   35 // Photodioda
#define BuzzerPin     2 // Buzzer
#define Start        17  // Tombol Start
#define Reset        5  // Tombol Reset
#define MIN_DISTANCE 5  // Jarak Ultrasonic

NewPing sonar(trigPin,echoPin);
LiquidCrystal_I2C lcd(0x27, 16, 2);
CTBot myBot;
Servo Servo1,Servo2;

bool objectDetected1 = false;
bool objectDetected2 = false;
bool objectDetected3 = false;
bool TCS = false;
bool Ir = false;
bool servo1 = false;
bool servo2 = false;
bool defaultReplySend= true;
String defaultReply;

int PH = 0;
int pos = 0;

int mark = 0;
int cacah;
int tanda = 0;
int angka = 0;

static int red, grn, blu;
bool logicDisplayLCD = false;
int maxValueRed = 0;
int minValueRed = 10000;
int maxValueGreen = 0;
int minValueGreen = 10000;
int maxValueBlue = 0;
int minValueBlue = 10000;
unsigned long timerBacaData = 0;

bool telurBusuk = false;
bool warna = false;

int jumlahTelurPutih = 0;
int jumlahTelurCklat = 0;
int jumlahTelurBusuk = 0;

void buzzer(int index = 1, int timer = 75){
    for(int i=0; i<index; i++){
      digitalWrite(BuzzerPin, HIGH);
      delay(timer);
      digitalWrite(BuzzerPin, LOW);
      delay(timer);
    }
}

void connectWifi(){
  buzzer(1);
  Serial.println("Connecting to WiFi...");
      lcd.setCursor(0,0); lcd.print("Menghubungkan Ke");
      lcd.setCursor(0,1); lcd.print("Jaringan Wifi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); 
    Serial.print("."); lcd.print(".");
  }  Serial.println("Wifi Terhubung");
       lcd.setCursor(0,0); lcd.print(" Jaringan Wifi  ");
       lcd.setCursor(0,1); lcd.print("   Terhubung    ");
       buzzer(2);
}

void connectTelegram(){
       lcd.setCursor(0,0); lcd.print("Menghubungkan Ke");
       lcd.setCursor(0,1); lcd.print(" Bot Telegram   ");
    Serial.println("login in telegram...");
    while (!myBot.testConnection()){
        myBot.setTelegramToken(token);
        delay(100);}
  if(myBot.testConnection()){
    lcd.setCursor(0,0); lcd.print("Koneksi Telegram");
    lcd.setCursor(0,1); lcd.print("   Terhubung    ");
    buzzer(3);
       Serial.print("Telegram connection OK!"); 
       delay(1500);
  } else {
       Serial.println("Connection Not OK");
       lcd.setCursor(0,0); lcd.print("Koneksi Telegram");
       lcd.setCursor(0,1); lcd.print("     GAGAL      ");
       buzzer(5);
       while(true);
  }
  lcd.setCursor(3,0); lcd.print("TUGAS AKHIR");
  lcd.setCursor(4,1); lcd.print("Jefriadi");
  delay(1000);
}


void bacaWarna(){
  red = 0, grn = 0, blu = 0;
  int n = 30;
  for(int i = 0; i<n; ++i){
    digitalWrite(s2, LOW);
    digitalWrite(s3, LOW);
    red = red + pulseIn(outPin, LOW);
    digitalWrite(s2, HIGH);
    digitalWrite(s3, HIGH);
    grn = grn + pulseIn(outPin, LOW);
    digitalWrite(s2, LOW);
    digitalWrite(s3, HIGH);
    blu = blu + pulseIn(outPin, LOW);
    delay(3);
  }
    red = red/n;
    grn = grn/n;
    blu = blu/n;
}

void KalibrasiWarna(){

  if(maxValueRed < red){
    maxValueRed = red;
  } if(minValueRed > red && red!=0){
    minValueRed = red;
  }
  if(maxValueGreen < grn){
    maxValueGreen = grn;
  } if(minValueGreen > grn && grn!=0){
    minValueGreen = grn;
  }
  if(maxValueBlue < blu){
    maxValueBlue = blu;
  } if(minValueBlue > blu && blu!=0){
    minValueBlue = blu;
  }

  if(millis()-timerBacaData > 2000){
    logicDisplayLCD =! logicDisplayLCD;
    timerBacaData = millis();
  } 
  
  if(logicDisplayLCD){
      lcd.setCursor(0,0);
      lcd.print("Min R: ");
      lcd.print(minValueRed);
      lcd.print("                    ");
      lcd.setCursor(0,1);
      lcd.print("G: ");
      lcd.print(minValueGreen);
      lcd.print(" B: ");
      lcd.print(minValueBlue );
      lcd.print("                ");
    } else {
      lcd.setCursor(0,0);
      lcd.print("Max R: ");
      lcd.print(maxValueRed);
      lcd.print("                    ");
      lcd.setCursor(0,1);
      lcd.print("G: ");
      lcd.print(maxValueGreen);
      lcd.print(" B: ");
      lcd.print(maxValueBlue );
      lcd.print("                ");
    }    
}

void displayLCD(){
  if(millis()-timerBacaData > 2000){
    logicDisplayLCD =! logicDisplayLCD;
    timerBacaData = millis();
  } 

  if(logicDisplayLCD){
      lcd.setCursor(0,0);
      lcd.print("Telur 1 : "); lcd.print(jumlahTelurPutih);
      lcd.print("          ");
      lcd.setCursor(0,1);
      lcd.print("Telur 2 : "); lcd.print(jumlahTelurCklat);
      lcd.print("          ");
    } else {
      lcd.setCursor(0,0);
      lcd.print("Telur NO : "); lcd.print(jumlahTelurBusuk );
      lcd.setCursor(0,1);
      lcd.print("                ");
    }    
}

void Telegram(){
   TBMessage msg;
   if(myBot.getNewMessage(msg)){
    
      if (msg.text.equalsIgnoreCase("/Start")){
        digitalWrite(BuzzerPin,HIGH);
        myBot.sendMessage(chatId, "Buzzer On, Pendeteksian Siap!");
      }
    
      else if (msg.text.equalsIgnoreCase("/Cek1")){
         myBot.sendMessage(chatId, "Counter Telur 1: " + String(jumlahTelurPutih));
      }
      
      else if (msg.text.equalsIgnoreCase("/Restart(1)")){
        jumlahTelurPutih = 0;
        myBot.sendMessage(chatId, "Counter: " + String(jumlahTelurPutih));
      }
    
      else if (msg.text.equalsIgnoreCase("/Cek2")){
        myBot.sendMessage(chatId, "Counter Telur 2: " + String(jumlahTelurCklat));
      }
      
      else if (msg.text.equalsIgnoreCase("/Restart(2)")){
        jumlahTelurCklat = 0;
        myBot.sendMessage(chatId, "Counter: " + String(jumlahTelurCklat));
      }
    
      else if (msg.text.equalsIgnoreCase("/Cek3")){
        myBot.sendMessage(chatId, "Counter Telur 3: " + String(jumlahTelurBusuk));
      }
      
      else if (msg.text.equalsIgnoreCase("/Restart(3)")){
        jumlahTelurBusuk = 0;
        myBot.sendMessage(chatId, "Counter: " + String(jumlahTelurBusuk));
      } 
    
      else {
          String welcome = "Welcome To TA Jefriadi :\n";
          welcome += "/Start : Cek Alat (Buzzer On)\n";
          welcome += "/Cek1 : Menampilkan Kondisi Telur 1\n";
          welcome += "/Cek2 : Menampilkan Kondisi Telur 2\n";
          welcome += "/Cek3 : Menampilkan Kondisi Telur Tidak Baik\n";
          welcome += "/Cek4 : Menampilkan Seluruh Deteksi\n";
          welcome += "/Restart1-Restart Counter 1\n";
          welcome += "/Restart2-Restart Counter 2\n";
          welcome += "/Restart3-Restart Counter 3\n";
          myBot.sendMessage(chatId,welcome);
      }
   }
}

void setup() {
  Serial.begin(115200);
  pinMode(irPin1, INPUT);
  pinMode(irPin2, INPUT);
  pinMode(irPin3, INPUT);
  pinMode(irPin4, INPUT);
  pinMode(irPin5, INPUT);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(BuzzerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(Photodioda, INPUT);
  pinMode(Start, INPUT_PULLUP);
  pinMode(Reset, INPUT_PULLUP);
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(outPin, INPUT);
 
  digitalWrite(s0,HIGH);  // Setting frequency scaling to 100%
  digitalWrite(s1,HIGH);
  
  digitalWrite(Relay1, HIGH); // Setting relay 1,2,3 HIGH
  digitalWrite(Relay2, HIGH);
  digitalWrite(Relay3, LOW);
  
  Servo1.attach(33);
  Servo2.attach(32);
  
  lcd.init(); lcd.backlight(); lcd.clear();

  connectWifi();
  connectTelegram();
}

void loop() {
  KalibrasiWarna();
//  displayLCD();
  
  int distance = sonar.ping_cm();
  if (distance <= MIN_DISTANCE){
    digitalWrite(Relay1, LOW);
    buzzer(3); 
    digitalWrite(Relay2, LOW);
  }

  int PH = analogRead(Photodioda);
  if(PH>0 && PH<3000){  // atur rentang nilai PH untuk mendeteksi telur busuk
    telurBusuk = true;
  }

  int irValue1 = !digitalRead(irPin1);
  if(irValue1){
    digitalWrite(Relay2, HIGH);
    warna = true;
  }
  
  int irValue2 = !digitalRead(irPin2);
  if(warna && irValue2){
    bacaWarna();
    warna = false;
  }

  if(red>700 && red<780 && grn>650 && grn<726 && blu>520 && blu<580 && !telurBusuk){
    for(int pos=0; pos<=180; pos +=1){  // Warna Telur Terdeteksi Putih
      Servo1.write(pos); delay(15);
    } 
    jumlahTelurPutih++;
  } else if(red>360 && red<435 && grn>600 && grn<676 && blu>510 && blu<560 && !telurBusuk){
    for(int pos=150; pos>=0; pos-=1){  //Warna Telur terdekesi Coklat
      Servo2.write(pos); delay(15);
    }
    jumlahTelurCklat++;
  } else if(telurBusuk){
    jumlahTelurBusuk++;            // Telur Busuk Terdeksi
  }
  
  bool IR_Putih = !digitalRead(irPin3);
  bool IR_Cklat = !digitalRead(irPin4);
  bool IR_Busuk = !digitalRead(irPin5);
  
  if(IR_Putih){
    digitalWrite(Relay1, HIGH);
    for(int pos=180; pos>=0; pos -=1){  
      Servo1.write(pos); delay(15);
    } 
  }
  if(IR_Cklat){
    digitalWrite(Relay1, HIGH);
    for(int pos=0; pos<=150; pos+=1){ 
      Servo2.write(pos); delay(15);
    }
  }
  if(IR_Busuk){
    digitalWrite(Relay1, HIGH);
  }
}
