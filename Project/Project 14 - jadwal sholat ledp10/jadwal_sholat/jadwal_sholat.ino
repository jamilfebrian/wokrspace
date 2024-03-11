#include <SPI.h>       
#include <DMD.h>    
#include <TimerOne.h>  
#include "Arial_black_16.h"
#include "Arial_Black_16_ISO_8859_1.h"
#include "Arial14.h"
#include "SystemFont5x7.h"

#include <EEPROM.h>
#include <SoftwareSerial.h>      
#include "DFRobotDFPlayerMini.h"
#include "RTClib.h"

SoftwareSerial mySerial(A1, A2); 
SoftwareSerial bluetooth(2, 3); 
DFRobotDFPlayerMini myDFPlayer; 
RTC_DS3231 rtc;

#define DISPLAYS_ACROSS 1 
#define DISPLAYS_DOWN 1
#define buzz 4
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

char print_waktu1[7], print_waktu2[7], print_waktu3[7], print_waktu4[7] ;
char imsak[7], subuh[7], dzuhur[7], ashar[7], magrib[7], isya[7];
char imsak1[7], subuh1[7], dzuhur1[7], ashar1[7], magrib1[7], isya1[7];
static String jam1, tanggal1, tahun1,menit1, jadwal;
static String Imsak, Subuh, Dzuhur, Ashar, Magrib, Isya;
int detik, menit, jam,tanggal, bulan, tahun;
unsigned long currTime=0, buzzTime=0;
bool logicArray[6] = {0,0,0,0,0,0};
static int no=0;
static int JAM,MENIT;
static DateTime future, now;
bool SubuhA,DzuhurA, AsharA, MagribA, IsyaA;

void ScanDMD() { 
  dmd.scanDisplayBySPI();
}

void DFConnect(){
  Serial.println();
  Serial.println(F("Menghidupkan DF PLAYER"));
  Serial.println(F("Tunggu Sekitar 3~5 detik"));
  if (!myDFPlayer.begin(mySerial)) {  
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); 
    }}
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(15); 
}

void logikaArray(int logic){
  for(int i=0 ; i<6; i++){
    if(i==logic){
      logicArray[i] = 1;
      continue;
  } logicArray[i] = 0;
} no=0;}

void buzzer(){
  if(millis()-buzzTime<=500){
    digitalWrite(buzz,HIGH);
  } else if(millis()-buzzTime>500 && millis()-buzzTime<=1000){
    digitalWrite(buzz,LOW);
  } else {
    buzzTime=millis();
  }
}

void ledscreen(int logic){
  //==================text waktu=====================
  if(logic==1){
     jam1 = String(jam)+":"+String(menit)+"  ";
     jam1.toCharArray(print_waktu1, sizeof(print_waktu1)); 
     menit1 = String(detik)+" ";
     menit1.toCharArray(print_waktu2, sizeof(print_waktu2)); 
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(0,0, print_waktu1,7, GRAPHICS_NORMAL);
    dmd.drawString(0,8, print_waktu2,5, GRAPHICS_NORMAL);
    dmd.drawString(12,8, "WIB",3, GRAPHICS_NORMAL);
  } else if(logic==2){ 
     tanggal1 = String(tanggal)+"-"+String(bulan)+"  ";
     tanggal1.toCharArray(print_waktu3, sizeof(print_waktu3)); 
     tahun1 = String(tahun)+"   ";
     tahun1.toCharArray(print_waktu4, sizeof(print_waktu4)); 
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(0,0, print_waktu3,7, GRAPHICS_NORMAL);
    dmd.drawString(0,8, print_waktu4,7, GRAPHICS_NORMAL);
  } else if (logic==3){
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(0,0," JAM  ",7, GRAPHICS_NORMAL);
    dmd.drawString(0,8,"SOLAT ",5, GRAPHICS_NORMAL);
  } else if (logic==4){
    dmd.selectFont(SystemFont5x7);
    Imsak = String(EEPROM.read(1))+":"+String(EEPROM.read(2))+"  ";
    Imsak.toCharArray(imsak, sizeof(imsak)); 
    dmd.drawString(0,0,"IMSAK ",7, GRAPHICS_NORMAL);
    dmd.drawString(0,8,imsak ,5, GRAPHICS_NORMAL);
  } else if (logic==5){
    dmd.selectFont(SystemFont5x7);
    Subuh = String(EEPROM.read(3))+":"+String(EEPROM.read(4))+"  ";
    Subuh.toCharArray(subuh, sizeof(subuh)); 
    dmd.drawString(0,0,"SUBUH ",7, GRAPHICS_NORMAL);
    dmd.drawString(0,8,subuh,5, GRAPHICS_NORMAL);
  } else if (logic==6){
    dmd.selectFont(SystemFont5x7);
    Dzuhur = String(EEPROM.read(5))+":"+String(EEPROM.read(6))+"  ";
    Dzuhur.toCharArray(dzuhur, sizeof(dzuhur)); 
    dmd.drawString(0,0,"ZUHUR  ",7, GRAPHICS_NORMAL);
    dmd.drawString(0,8,dzuhur,5, GRAPHICS_NORMAL);
  } else if (logic==7){
    dmd.selectFont(SystemFont5x7);
    Ashar = String(EEPROM.read(7))+":"+String(EEPROM.read(8))+"  ";
    Ashar.toCharArray(ashar, sizeof(ashar)); 
    dmd.drawString(0,0,"ASHAR ",7, GRAPHICS_NORMAL);
    dmd.drawString(0,8,ashar,5, GRAPHICS_NORMAL);
  } else if (logic==8){
    dmd.selectFont(SystemFont5x7);
    Magrib = String(EEPROM.read(9))+":"+String(EEPROM.read(10))+"  ";
    Magrib.toCharArray(magrib, sizeof(magrib)); 
    dmd.drawString(0,0,"MAGRIB",7, GRAPHICS_NORMAL);
    dmd.drawString(0,8,magrib,5, GRAPHICS_NORMAL);
  } else if (logic==9){
    dmd.selectFont(SystemFont5x7);
    Isya = String(EEPROM.read(11))+":"+String(EEPROM.read(12))+"  ";
    Isya.toCharArray(isya, sizeof(isya)); 
    dmd.drawString(0,0,"ISYA  ",7, GRAPHICS_NORMAL);
    dmd.drawString(0,8,isya,5, GRAPHICS_NORMAL);
  } 
}

void tampiltext(int logic, String jadwal){
  if (logic==1){
    dmd.selectFont(SystemFont5x7);
    jadwal.toCharArray(subuh1, sizeof(subuh1)); 
    dmd.drawString(0,0,"SUBUH ",7, GRAPHICS_NORMAL);
    dmd.drawString(0,8,subuh1,5, GRAPHICS_NORMAL);
  } else if (logic==2){
    dmd.selectFont(SystemFont5x7);
    jadwal.toCharArray(dzuhur1, sizeof(dzuhur1)); 
    dmd.drawString(0,0,"ZUHUR  ",7, GRAPHICS_NORMAL);
    dmd.drawString(0,8,dzuhur1,5, GRAPHICS_NORMAL);
  } else if (logic==3){
    dmd.selectFont(SystemFont5x7);
    jadwal.toCharArray(ashar1, sizeof(ashar1)); 
    dmd.drawString(0,0,"ASHAR ",7, GRAPHICS_NORMAL);
    dmd.drawString(0,8,ashar1,5, GRAPHICS_NORMAL);
  } else if (logic==4){
    dmd.selectFont(SystemFont5x7);
    jadwal.toCharArray(magrib1, sizeof(magrib1)); 
    dmd.drawString(0,0,"MAGRIB",7, GRAPHICS_NORMAL);
    dmd.drawString(0,8,magrib1,5, GRAPHICS_NORMAL);
  } else if (logic==5){
    dmd.selectFont(SystemFont5x7);
    jadwal.toCharArray(isya1, sizeof(isya1)); 
    dmd.drawString(0,0,"ISYA  ",7, GRAPHICS_NORMAL);
    dmd.drawString(0,8,isya1,5, GRAPHICS_NORMAL);
  } 
}

void setup() {
  pinMode(buzz,OUTPUT);
  Serial.begin(9600); 
  mySerial.begin(9600);
  DFConnect();
  bluetooth.begin(9600); 
  Timer1.initialize(1000);          
  Timer1.attachInterrupt(ScanDMD);   
  dmd.clearScreen(true);  

   if (!rtc.begin()) {
    Serial.println("Could not find RTC! Check circuit.");
    while (1); }
    rtc.adjust(DateTime(__DATE__, __TIME__));
    
  dmd.selectFont(Arial_Black_16_ISO_8859_1);
  dmd.drawString(4,0, "UPI",3, GRAPHICS_NORMAL);
  delay(2500);
  dmd.clearScreen(true);  
}

void loop(){
  
  String pesan="";
    while (bluetooth.available()>0){
      pesan += char(bluetooth.read()); 
    }pesan.trim();
    if(pesan=="L"){
      bluetooth.write("Masukkan Jadwal Imsak\n");
      logikaArray(0);
    } else if(pesan=="S"){
      bluetooth.write("Masukkan Jadwal Sholat Subuh\n");
      logikaArray(1);
    } else if(pesan=="D"){
      bluetooth.write("Masukkan Jadwal Sholat Dzuhur\n");
      logikaArray(2);
    } else if(pesan=="A"){
      bluetooth.write("Masukkan Jadwal Sholat Ashar\n");
      logikaArray(3);
    } else if(pesan=="M"){
      bluetooth.write("Masukkan Jadwal Magrib\n");
      logikaArray(4);
    } else if(pesan=="I"){
      bluetooth.write("Masukkan Jadwal Isya\n");
      logikaArray(5);
    } 

//=====================logika waktu=============================
    if(logicArray[0]){  //==========Untuk waktu Imsak
      if(pesan!=""){
        no++; //Serial.println(no);
        if(no==1){
          bluetooth.write("Masukkan JAM\n");
        }
        if(no==2){
          JAM = pesan.toInt();
          bluetooth.write("Masukkan MENIT\n");
          Serial.println("JAM");
          Serial.println(JAM);
          EEPROM.write(1,JAM);
        } else if(no==3){
          MENIT = pesan.toInt();
          Serial.println("Menit");
          Serial.println(MENIT);
          EEPROM.write(2,MENIT);
          no=1;
        }
      } 
    } 

 //================Subuh================
 if(logicArray[1]){
      if(pesan!=""){
        no++; //Serial.println(no);
        if(no==1){
          bluetooth.write("Masukkan JAM\n");
        }
        if(no==2){
          JAM = pesan.toInt();
          bluetooth.write("Masukkan MENIT\n");
          Serial.println("JAM");
          Serial.println(JAM);
          EEPROM.write(3,JAM);
        } else if(no==3){
          MENIT = pesan.toInt();
          Serial.println("Menit");
          Serial.println(MENIT);
          EEPROM.write(4,MENIT);
          no=1;
        }
      } 
    }    
//=============Dzuhur================
if(logicArray[2]){
      if(pesan!=""){
        no++; //Serial.println(no);
        if(no==1){
          bluetooth.write("Masukkan JAM\n");
        }
        if(no==2){
          JAM = pesan.toInt();
          bluetooth.write("Masukkan MENIT\n");
          Serial.println("JAM");
          Serial.println(JAM);
          EEPROM.write(5,JAM);
        } else if(no==3){
          MENIT = pesan.toInt();
          Serial.println("Menit");
          Serial.println(MENIT);
          EEPROM.write(6,MENIT);
          no=1;
        }
      } 
    }   
//=============Ashar================
if(logicArray[3]){
      if(pesan!=""){
        no++; //Serial.println(no);
        if(no==1){
          bluetooth.write("Masukkan JAM\n");
        }
        if(no==2){
          JAM = pesan.toInt();
          bluetooth.write("Masukkan MENIT\n");
          Serial.println("JAM");
          Serial.println(JAM);
          EEPROM.write(7,JAM);
        } else if(no==3){
          MENIT = pesan.toInt();
          Serial.println("Menit");
          Serial.println(MENIT);
          EEPROM.write(8,MENIT);
          no=1;
        }
      } 
    }   

 //=============Magrib================
if(logicArray[4]){
      if(pesan!=""){
        no++; //Serial.println(no);
        if(no==1){
          bluetooth.write("Masukkan JAM\n");
        }
        if(no==2){
          JAM = pesan.toInt();
          bluetooth.write("Masukkan MENIT\n");
          Serial.println("JAM");
          Serial.println(JAM);
          EEPROM.write(9,JAM);
        } else if(no==3){
          MENIT = pesan.toInt();
          Serial.println("Menit");
          Serial.println(MENIT);
          EEPROM.write(10,MENIT);
          no=1;
        }
      } 
    }   

//=============Isya================
if(logicArray[5]){
      if(pesan!=""){
        no++; //Serial.println(no);
        if(no==1){
          bluetooth.write("Masukkan JAM\n");
        }
        if(no==2){
          JAM = pesan.toInt();
          bluetooth.write("Masukkan MENIT\n");
          Serial.println("JAM");
          Serial.println(JAM);
          EEPROM.write(11,JAM);
        } else if(no==3){
          MENIT = pesan.toInt();
          Serial.println("Menit");
          Serial.println(MENIT);
          EEPROM.write(12,MENIT);
          no=1;
        }
      } 
    }   
 // =============JAM RTC====================   
     DateTime now = rtc.now();
       detik = now.second();
       menit = now.minute();
       jam = now.hour(); 
       tanggal = now.day();
       bulan = now.month();
       tahun = now.year();

       
if(jam==EEPROM.read(11) && menit+2==EEPROM.read(12) && detik==0 ){
  IsyaA=true; SubuhA=false; DzuhurA=false; AsharA=false; MagribA=false;
  future = now + TimeSpan(0,0,2,0);
} else if(jam==EEPROM.read(3) && menit+2==EEPROM.read(4) && detik==0 ){
  SubuhA=true; IsyaA=false; DzuhurA=false; AsharA=false; MagribA=false;
  future = now + TimeSpan(0,0,2,0);
} else if(jam==EEPROM.read(5) && menit+2==EEPROM.read(6) && detik==0 ){
  DzuhurA=true; SubuhA=false; IsyaA=false; AsharA=false; MagribA=false;
  future = now + TimeSpan(0,0,2,0);
} else if(jam==EEPROM.read(7) && menit+2==EEPROM.read(8) && detik==0 ){
  AsharA=true; SubuhA=false; DzuhurA=false; IsyaA=false; MagribA=false;
  future = now + TimeSpan(0,0,2,0); 
} else if(jam==EEPROM.read(9) && menit+2==EEPROM.read(10) && detik==0 ){
  MagribA=true; SubuhA=false; DzuhurA=false; AsharA=false; IsyaA=false;
  future = now + TimeSpan(0,0,2,0);
} else if((future-now).hours()<=0 && (future-now).minutes()<=0 && (future-now).seconds()<=0){
  IsyaA=false; SubuhA=false; DzuhurA=false; AsharA=false; MagribA=false;
} 

if(IsyaA){
  jadwal = String((future-now).minutes())+":"+String((future-now).seconds())+" ";
  tampiltext(5, jadwal);
  if(((future-now).hours())==0 && ((future-now).minutes())==0 && ((future-now).seconds())<=10){
    buzzer();
  }
} else if(SubuhA){
  jadwal = String((future-now).minutes())+":"+String((future-now).seconds())+" ";
  tampiltext(1, jadwal);
  if(((future-now).hours())==0 && ((future-now).minutes())==0 && ((future-now).seconds())<=10){
    buzzer();
  }
} else if(DzuhurA){
  jadwal = String((future-now).minutes())+":"+String((future-now).seconds())+" ";
  tampiltext(2, jadwal);
  if(((future-now).hours())==0 && ((future-now).minutes())==0 && ((future-now).seconds())<=10){
    buzzer();
  }
} else if(AsharA){
  jadwal = String((future-now).minutes())+":"+String((future-now).seconds())+" ";
  tampiltext(3, jadwal);
  if(((future-now).hours())==0 && ((future-now).minutes())==0 && ((future-now).seconds())<=10){
    buzzer();
  }
} else if(MagribA){
  jadwal = String((future-now).minutes())+":"+String((future-now).seconds())+" ";
  tampiltext(4, jadwal);
  if(((future-now).hours())==0 && ((future-now).minutes())==0 && ((future-now).seconds())<=10){
    buzzer();
  }
} else {
 //==================Menampilkan TEXT=======================      
  if(millis()-currTime<3000){
    ledscreen(2);
  } else if(millis()-currTime>3000 && millis()-currTime<8000){
    ledscreen(1);
  } else if(millis()-currTime>8000 && millis()-currTime<11000){
    ledscreen(3);
  } else if(millis()-currTime>11000 && millis()-currTime<14000){
    ledscreen(4);
  } else if(millis()-currTime>14000 && millis()-currTime<17000){
    ledscreen(5);
  } else if(millis()-currTime>17000 && millis()-currTime<20000){
    ledscreen(6);
  } else if(millis()-currTime>20000 && millis()-currTime<23000){
    ledscreen(7);
  } else if(millis()-currTime>23000 && millis()-currTime<26000){
    ledscreen(8);
  } else if(millis()-currTime>26000 && millis()-currTime<29000){
    ledscreen(9);
  }  else if (millis()-currTime > 29000){ 
    currTime=millis();
  }
}
  if(jam==EEPROM.read(11) && menit==EEPROM.read(12) && detik==0  ){
    myDFPlayer.play(1);
    delay(1000);
  } else if(jam==EEPROM.read(3) && menit==EEPROM.read(4) && detik==0  ){
    myDFPlayer.play(1);
    delay(1000);
  } else if(jam==EEPROM.read(5) && menit==EEPROM.read(6) && detik==0  ){
    myDFPlayer.play(1);
    delay(1000);
  } else if(jam==EEPROM.read(7) && menit==EEPROM.read(8) && detik==0  ){
    myDFPlayer.play(1);
    delay(1000);
  } else if(jam==EEPROM.read(9) && menit==EEPROM.read(10) && detik==0  ){
    myDFPlayer.play(1);
    delay(1000);
  }
}
