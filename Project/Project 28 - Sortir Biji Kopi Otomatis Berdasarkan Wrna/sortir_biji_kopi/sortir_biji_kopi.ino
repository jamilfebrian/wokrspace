#define BLYNK_TEMPLATE_ID "TMPL6QH1SkZkE"
#define BLYNK_TEMPLATE_NAME "Sortir Buah Kopi Otomatis"
#define BLYNK_AUTH_TOKEN "tQsrCeFfmu-DPwaTOZ-eNmKBBDZVCqPw"

#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

LiquidCrystal_I2C lcd(0x27,16,2);

#define ssid      "iPhone"   
#define password  "Mi1234567" 
#define auth  BLYNK_AUTH_TOKEN 

#define ServoPin_1 12
#define ServoPin_2 14
#define BuzzerPin  15
#define LDR_Pin     4 

#define S0 19
#define S1 18
#define S2 33
#define S3 32
#define OUT 25

Servo myServo1, myServo2;

static int red = 0; //Variabel frekuensi
static int green = 0;
static int blue = 0;

#define max_array 20

static int red_sum, green_sum, blue_sum;
static int red_avarage, green_avarage, blue_avarage;

int maxValueRed = 0;
int minValueRed = 10000;
int maxValueGreen = 0;
int minValueGreen = 10000;
int maxValueBlue = 0;
int minValueBlue = 10000;

unsigned long cTime = 0;
unsigned long timerBacaData = 0;

bool deteksi_biji_kopi = false;
bool buahOk = false;
bool logicDisplayLCD = false;

int count = 0;
static int kopi_hijau=0, kopi_kuning=0, kopi_merah=0, kopi_hitam=0;

void buzzer(int index = 1, int timer = 75){
    for(int i=0; i<index; i++){
      digitalWrite(BuzzerPin, HIGH);
      delay(timer);
      digitalWrite(BuzzerPin, LOW);
      delay(timer);
    }
}

void connectWifi(){
  WiFi.mode(WIFI_STA);
  lcd.setCursor(0,0); lcd.print("Menghubungkan Ke");
  lcd.setCursor(0,1); lcd.print(ssid);
  WiFi.begin(ssid, password);
  Blynk.begin(auth, ssid, password, "blynk.cloud", 80);
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    } buzzer(3);
  lcd.setCursor(0,0); lcd.print("Koneksi Internet");
  lcd.setCursor(0,1); lcd.print("   Terhubung    ");
  delay(1000);
}

void bacaWarna(int s2,int s3){
  count++;
  //Setting baca merah
  digitalWrite(s2,LOW);
  digitalWrite(s3,LOW);
  red = pulseIn(OUT, LOW);
//  red = map(red, 21,420,255,0);
  Serial.print(count);
  Serial.print(" R=");
  Serial.print(red);
 
//Setting baca hijau
  digitalWrite(s2,HIGH);
  digitalWrite(s3,HIGH);
  green = pulseIn(OUT, LOW);
//  green = map(green, 25,530,255,0);
  Serial.print(" G=");
  Serial.print(green);

//Setting baca biru
  digitalWrite(s2,LOW);
  digitalWrite(s3,HIGH);
  blue = pulseIn(OUT, LOW);
//  blue = map(blue, 7,155,255,0);
  Serial.print(" B=");
  Serial.println(blue);

  if(count <= max_array){
      red_sum += red;
      green_sum += green;
      blue_sum += blue;

      if(count == max_array){
        red_avarage = red_sum / max_array;
        green_avarage = green_sum / max_array;
        blue_avarage = blue_sum / max_array;
        Serial.println(red_avarage);
      }
  } else{
    red_sum = 0;
    green_sum = 0;
    blue_sum = 0;
    Serial.println("debug");
  }
  delay(20);
}

void Kalibrasi(){

  if(maxValueRed < red_avarage){
    maxValueRed = red_avarage;
  } if(minValueRed > red_avarage && red_avarage!=0){
    minValueRed = red_avarage;
  }
  if(maxValueGreen < green_avarage){
    maxValueGreen = green_avarage;
  } if(minValueGreen > green_avarage && green_avarage!=0){
    minValueGreen = green_avarage;
  }
  if(maxValueBlue < blue_avarage){
    maxValueBlue = blue_avarage;
  } if(minValueBlue > blue_avarage && blue_avarage!=0){
    minValueBlue = blue_avarage;
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
      lcd.print("Jumlah Buah Kopi");
      lcd.setCursor(0,1);
      lcd.print("Hjau:");
      lcd.print(kopi_hijau);
      lcd.print(" Kng : ");
      lcd.print(kopi_kuning );
      lcd.print("                ");
    } else {
      lcd.setCursor(0,0);
      lcd.print("Jumlah Buah Kopi");
      lcd.setCursor(0,1);
      lcd.print("Mrah:");
      lcd.print(kopi_merah);
      lcd.print(" Htam: ");
      lcd.print(kopi_hitam );
      lcd.print("                ");
    }    
}

void setup() {
  Serial.begin(115200);
  pinMode(BuzzerPin, OUTPUT);
  pinMode(LDR_Pin, INPUT);

  pinMode(S0, OUTPUT); //Setting S0 sebagai output
  pinMode(S1, OUTPUT); //Setting S1 sebagai output
  pinMode(S2, OUTPUT); //Setting S2 sebagai output
  pinMode(S3, OUTPUT); //Setting S3 sebagai output
  pinMode(OUT, INPUT); //Setting Outsensor sebagai Input
  digitalWrite(S0,HIGH); // setting 20% frequency
  digitalWrite(S1,LOW);
  
  myServo1.attach(ServoPin_1); myServo1.write(170);
  myServo2.attach(ServoPin_2); myServo2.write(90);

  buzzer();
  lcd.init(); lcd.backlight(); lcd.clear();
  connectWifi();

  Blynk.virtualWrite(V9, 170);
  lcd.clear();
}

void loop() {
 Blynk.run(); 
 bool LDR_Value = digitalRead(LDR_Pin);
 displayLCD();
// Kalibrasi();

 if(LDR_Value && !deteksi_biji_kopi){
   deteksi_biji_kopi = true;
   buzzer(2,100); 
   myServo1.write(113);
   cTime = millis();
 } else {
    if(millis() - cTime > 3000){
      if(buahOk){
        if(red_avarage>700 && red_avarage<780 && green_avarage>650 && green_avarage<726 && blue_avarage>520 && blue_avarage<580){
          Serial.println("Hijau"); myServo2.write(50); kopi_hijau++;
        } else if(red_avarage>360 && red_avarage<435 && green_avarage>600 && green_avarage<676 && blue_avarage>510 && blue_avarage<560){
          Serial.println("Kuning"); myServo2.write(80); kopi_kuning++;
        } else if(red_avarage>519 && red_avarage<720 && green_avarage>750 && green_avarage<844 && blue_avarage>584 && blue_avarage<640){
          Serial.println("Merah"); myServo2.write(100); kopi_merah++;
        } else {
          Serial.println("Unknown"); myServo2.write(130); kopi_hitam++;
        }
          Blynk.virtualWrite(V0, kopi_merah); 
          Blynk.virtualWrite(V1, kopi_kuning); 
          Blynk.virtualWrite(V2, kopi_hijau); 
          Blynk.virtualWrite(V3, kopi_hitam); 
        myServo1.write(45);
        delay(1000); 
        buahOk = false;
        count = 0;
      } 
      myServo1.write(170); delay(1000);
      deteksi_biji_kopi = false;
    } else {
      bacaWarna(S2,S3);
      buahOk = true;
    }
 }
}

BLYNK_WRITE(V4){
  int value = param.asInt();  
  if (value == 1) {
   kopi_hijau=0; kopi_kuning=0; kopi_merah=0; kopi_hitam=0;
    Blynk.virtualWrite(V0, kopi_merah); 
    Blynk.virtualWrite(V1, kopi_kuning); 
    Blynk.virtualWrite(V2, kopi_hijau); 
    Blynk.virtualWrite(V3, kopi_hitam); 
  } else {
   
  }
}
