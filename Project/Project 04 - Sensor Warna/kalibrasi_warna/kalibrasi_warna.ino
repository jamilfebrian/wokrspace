#define S0 13 //S0 di pin D8
#define S1 12 //S1 di pin D9
#define S2 14 //S2 di pin D10
#define S3 27 //S3 di pin D11
#define sensorOut 26 //Out di pin D12

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

static int red = 0; //Variabel frekuensi
static int green = 0;
static int blue = 0;

void setup() {
   lcd.init();  lcd.backlight();
  
  pinMode(S0, OUTPUT); //Setting S0 sebagai output
  pinMode(S1, OUTPUT); //Setting S1 sebagai output
  pinMode(S2, OUTPUT); //Setting S2 sebagai output
  pinMode(S3, OUTPUT); //Setting S3 sebagai output
  pinMode(sensorOut, INPUT); //Setting Outsensor sebagai Input

  digitalWrite(S0,HIGH); // setting 20% frequency
  digitalWrite(S1,LOW);
  Serial.begin(115200); //Mulai komunikasi serial
}

void bacaWarna(int s2,int s3){
  //Setting baca merah
  digitalWrite(s2,LOW);
  digitalWrite(s3,LOW);
  red = pulseIn(sensorOut, LOW);
//  red = map(red, 21,420,255,0);
  Serial.print("R=");
  Serial.print(red);
 
//Setting baca hijau
  digitalWrite(s2,HIGH);
  digitalWrite(s3,HIGH);
  green = pulseIn(sensorOut, LOW);
//  green = map(green, 25,530,255,0);
  Serial.print(" G=");
  Serial.print(green);


//Setting baca biru
  digitalWrite(s2,LOW);
  digitalWrite(s3,HIGH);
  blue = pulseIn(sensorOut, LOW);
//  blue = map(blue, 7,155,255,0);
  Serial.print(" B=");
  Serial.println(blue);
  delay(100);
}

void loop() {
  bacaWarna(S2,S3);
  lcd.setCursor(0,0);
  lcd.print("R: ");
  lcd.print(red);
  lcd.print(" G: ");
  lcd.print(green);
  lcd.print("                    ");
  lcd.setCursor(0,1);
  lcd.print("B: ");
  lcd.print(blue);
  lcd.print("                ");
  delay(100);
}
