#define REMOTEXY_MODE__ESP8266WIFI_LIB_CLOUD

#include <ESP8266WiFi.h>
#include <RemoteXY.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int TRIGPIN = D5;
const int ECHOPIN = D6;

unsigned int jarak, timer, T_air, La;
unsigned int Vol;

#define REMOTEXY_WIFI_SSID "Ardutech"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_CLOUD_SERVER "cloud.remotexy.com"
#define REMOTEXY_CLOUD_PORT 6376
#define REMOTEXY_CLOUD_TOKEN "token_sesuaikan_dari_remoteXY"
#pragma pack(push, 1)

uint8_t RemoteXY_CONF[] =
{ 255, 0, 0, 12, 0, 60, 0, 8, 16, 1,
  129, 0, 37, 66, 0, 6, 17, 0, 129, 0,
  10, 1, 42, 6, 190, 87, 65, 84, 69, 82,
  32, 76, 69, 86, 69, 76, 0, 67, 2, 10,
  8, 24, 13, 2, 26, 11, 129, 0, 36, 13,
  13, 6, 17, 76, 105, 116, 101, 114, 0, 66,
  1, 13, 23, 36, 61, 2, 26
};

struct {
  char text_1[11]; // string UTF8 end zero
  int8_t level_1; // =0..100 level position
  uint8_t connect_flag; // =1 if wire connected, else =0
} RemoteXY;
#pragma pack(pop)
//-----------------------------BACA SENSOR ULTRSONIC
void Baca_SRF04() {
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);
  timer = pulseIn(ECHOPIN, HIGH);
  jarak = timer / 58;
  if (jarak > 100)jarak = 100;
  T_air = 100 - jarak;
  Vol = La * T_air / 1000;
}

void setup() {
  pinMode(ECHOPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.print("Init RemotXY");
  RemoteXY_Init ();
  delay(3000);
  lcd.clear();
  // tinggi tangki 1 m (100 cm)
  // diameter alas 60 cm
  // Luas alas = 3.14x30x30 = 2826 cm2
  La = 2826;
}
void loop()
{
  RemoteXY_Handler ();
  Baca_SRF04();
  lcd.setCursor(0, 0);
  lcd.print("Water Level");
  lcd.setCursor(0, 1);
  lcd.print("Level:");
  lcd.print(Vol);
  lcd.print(" L ");
  dtostrf(Vol, 0, 0, RemoteXY.text_1);
  int level = map(Vol, 0, 280, 0, 100);
  RemoteXY.level_1 = level;
  delay(2000);
}
