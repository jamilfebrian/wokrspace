#include <DS3231.h> //mengincludekan library DS3231
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 

LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
DS3231  rtc(SDA, SCL); // inisialisasi penggunaan i2c

Time  t;

#define buz 11

int Hor;

int Min;

int Sec;

void setup()
{
  Serial.begin(115200); //set komunikasi baut serial monitor pada 115200
  rtc.begin();
 
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  //setting pertama download program
  //rtc.setDate(21, 02, 2019);   //mensetting tanggal 07 april 2018
  //rtc.setTime(06, 30, 00);     //menset jam 22:00:00
 // rtc.setDOW(2);     //menset hari "Sabtu"
 
//setelah didownload awal selesai, download kedua dengan memberi tanda komen "//"
}
 
void loop()
{
  t = rtc.getTime();

  Hor = t.hour;

  Min = t.min;

  Sec = t.sec;
  Serial.print(rtc.getDOWStr()); //prosedur pembacaan hari
  Serial.print(" ");
  
  Serial.print(rtc.getDateStr()); //prosedur pembacaan tanggal
  Serial.print(" -- ");
 
  Serial.println(rtc.getTimeStr()); //prosedur pembacaan waktu


  lcd.setCursor(0,0);
  lcd.print("Tgl: "); 
  lcd.setCursor(5,0);
  lcd.print(rtc.getDateStr());

  lcd.setCursor(0,1);
  lcd.print("Jam: ");
  
  lcd.setCursor(5,1);
  lcd.print(rtc.getTimeStr());
  
  
  delay (1000); //waktu tunda 1 detik per cycle
}
