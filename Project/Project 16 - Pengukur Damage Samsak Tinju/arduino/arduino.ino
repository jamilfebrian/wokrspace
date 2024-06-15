#include <LiquidCrystal_I2C.h>
#include <NewPing.h>
 
#define FSR1 A0
#define FSR2 A1
#define FSR3 A3
#define FSR4 A6

#define TRIG_1 2
#define ECHO_1 3
#define TRIG_2 8
#define ECHO_2 9
#define TRIG_3 6
#define ECHO_3 7

#define BUTTON_PIN_1 10
#define BUTTON_PIN_2 11 
#define BUTTON_PIN_3 12
#define BUTTON_PIN_4 4

LiquidCrystal_I2C lcd(0x27, 20, 4);

NewPing sonar1(TRIG_1, ECHO_1, 200);
NewPing sonar2(TRIG_2, ECHO_2, 200);
NewPing sonar3(TRIG_3, ECHO_3, 200);

#define jarak_ultrasonik 5

unsigned int previousTime, currentTime, stopTime;

static bool buttonActive[4];
bool hasilNilai;

bool a = true;
int x = 0; //index array untuk damage dan speed
int pukulan = 1, tendangan = 1; 

static double PUKUL_KIRI=0,PUKUL_KANAN=0,TENDANG_KIRI=0,TENDANG_KANAN=0,TENDANG_TENGAH=0;
static double damage[3], speed[3]; 
static double avarageDamage, avarageSpeed; //variabel untuk menyimpan rata-rata damage dan kecepatan

void displayLCD(byte level=0, byte punch=0, byte kick=0){
  if(level==1 || level==2 || level==3 || level==4){
    lcd.setCursor(0,0); lcd.print("===== LEVEL "+String(level)+" =====");
    lcd.setCursor(0,1); lcd.print("Lakukan :            ");
    lcd.setCursor(0,2); lcd.print("    "+String(punch)+" Pukulan        ");
    lcd.setCursor(0,3); lcd.print("    "+String(kick)+" Tendangan       ");
  } else if(level==0){
    lcd.setCursor(0,0); lcd.print("===== UNKNOWN =====");
    lcd.setCursor(0,1); lcd.print("   Silahkan Pilih   ");
    lcd.setCursor(0,2); lcd.print("   Level Terlebih   ");
    lcd.setCursor(0,3); lcd.print("      Dahulu        ");
  } else if(level==10){
    lcd.setCursor(0,0); lcd.print("===== RESULTS =====");
    lcd.setCursor(0,1); lcd.print("                      ");
    lcd.setCursor(0,2); lcd.print("Damage:" + String(avarageDamage)+" %               ");
    lcd.setCursor(0,3); lcd.print("Speed :" + String(avarageSpeed) +" cm/s              ");
   
  }
}

void setup() {
  Serial.begin(115200);
  lcd.init(); lcd.backlight(); lcd.clear();
  
  pinMode(FSR1, INPUT); pinMode(FSR2, INPUT);
  pinMode(FSR3, INPUT); pinMode(FSR4, INPUT); 
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  pinMode(BUTTON_PIN_3, INPUT_PULLUP);
  pinMode(BUTTON_PIN_4, INPUT_PULLUP);

  lcd.setCursor(0,0); lcd.print("                   ");
  lcd.setCursor(0,1); lcd.print("      WELCOME      ");
  lcd.setCursor(0,2); lcd.print("                   ");
  lcd.setCursor(0,3); lcd.print("                   ");
  delay(1000);
}

void tombol(int index){
    for(int i=0; i<4; i++){
      buttonActive[i] = 0;
      damage[i]=0; speed[i]=0;
      if(i==index){
        buttonActive[i] = 1;
      }
    }
}

void loop() {

  bool level1 = digitalRead(BUTTON_PIN_1);
  bool level2 = digitalRead(BUTTON_PIN_2);
  bool level3 = digitalRead(BUTTON_PIN_3);
  bool level4 = digitalRead(BUTTON_PIN_4);
  
  long DISTANCE_1 = sonar1.ping_cm();
  long DISTANCE_2 = sonar2.ping_cm();
  long DISTANCE_3 = sonar3.ping_cm();

  double pukulKiri = analogRead(FSR1); 
  double pukulKanan = analogRead(FSR2);
  double tendangKiri = analogRead(FSR3);
  double tendangKanan  = analogRead(FSR4);

  pukulKiri = pukulKiri*100/1023;
  pukulKanan = pukulKanan*100/1023;
  tendangKiri = tendangKiri*100/1023;
  tendangKanan = tendangKanan*100/1023;

  Serial.print(DISTANCE_1); Serial.print(" ");
  Serial.print(DISTANCE_2); Serial.print(" ");
  Serial.print(DISTANCE_3); Serial.print(" | ");

  Serial.print(level1); Serial.print(level2);
  Serial.print(level3); Serial.print(level4);Serial.print(" | ");

  Serial.print(pukulKiri); Serial.print(" ");
  Serial.print(pukulKanan); Serial.print(" ");
  Serial.print(tendangKiri); Serial.print(" ");
  Serial.print(tendangKanan); Serial.println(" ");
  
  if(!level1){
      tombol(0);
      tendangan = 1;
      pukulan = 1;
      x=0;
  } else if(!level2){
      tombol(1);
      tendangan = 2;
      pukulan = 2;
      x=0;
  } else if(!level3){
      tombol(2);
      tendangan = 2;
      pukulan = 1;
      x=0;
  } else if(!level4){
      tombol(3);
      tendangan = 1;
      pukulan = 2;
      x=0;
  }

  if((DISTANCE_1!=0 && DISTANCE_1<=30) || (DISTANCE_2!=0 && DISTANCE_2<=30) || (DISTANCE_3!=0 && DISTANCE_3<=30)){
      if(a){ previousTime = millis(); a=false;}
      else{}
  } else {a=true;}
  
  if(pukulKiri>10){                       //Lakukan Pukulan di Kiri
    delay(10);
    currentTime = millis();
    if(pukulKiri>PUKUL_KIRI){
      PUKUL_KIRI = pukulKiri;
    } else if(pukulKiri<PUKUL_KIRI){
        if(pukulan != 0){
          damage[x] = PUKUL_KIRI;
          speed[x] = jarak_ultrasonik*1000/((currentTime-previousTime));
          x++; 
      }
      delay(500);
      pukulan--;
    }
  }
  
  if(pukulKanan>10){                     // Lakukan Pukul di kanan
    delay(10);
    currentTime = millis();
    if(pukulKanan>PUKUL_KANAN){
      PUKUL_KANAN = pukulKanan;
    } else if(pukulKanan<PUKUL_KANAN){
        if(pukulan != 0){
          damage[x] = PUKUL_KANAN;
          speed[x] = jarak_ultrasonik*1000/((currentTime-previousTime));
          x++; 
      }
      delay(500);
      pukulan--;
    }
  }
  if(tendangKanan>10){                      // Lakukan Tendangan di Kanan
    delay(10);
    currentTime = millis();
    if(tendangKanan>TENDANG_KANAN){
      TENDANG_KANAN = tendangKanan;
    } else if(tendangKanan<TENDANG_KANAN){
        if(tendangan != 0){
          damage[x] = TENDANG_KANAN;
          speed[x] = jarak_ultrasonik*1000/((currentTime-previousTime));
          x++; 
      } 
      delay(500);
      tendangan--;
    }
  }
  if(tendangKiri>10){                     // Lakukan tendangan di Kiri
    delay(10);
    currentTime = millis();
    if(tendangKiri>TENDANG_KIRI){
      TENDANG_KIRI = tendangKiri;
    } else if(tendangKiri<TENDANG_KIRI){
        if(tendangan != 0){
          damage[x] = TENDANG_KIRI;
          speed[x] = jarak_ultrasonik*1000/((currentTime-previousTime));
          x++; 
      } 
      delay(500);
      tendangan--;
    }
  }
  
x = (buttonActive[0] && x==2) ? 0:x ; 
x = (buttonActive[1] && x==4) ? 0:x ; 
x = (buttonActive[2] && x==3) ? 0:x ;
x = (buttonActive[3] && x==3) ? 0:x ;

tendangan = (tendangan<=0) ? 0 : tendangan;
pukulan = (pukulan<=0) ? 0 : pukulan;


 if(tendangan<=0 && pukulan<=0){
   hasilNilai=1;
   buttonActive[0]=0;
   buttonActive[1]=0;
   buttonActive[2]=0;
   buttonActive[3]=0;
 } else {
   hasilNilai=0;
 }

  if(buttonActive[0]){
      avarageDamage = (damage[0]+damage[1])/2;
      avarageSpeed  = (speed[0]+speed[1])/2;
      displayLCD(1, pukulan, tendangan);
  } else if(buttonActive[1]){
      avarageDamage = (damage[0]+damage[1]+damage[2]+damage[3])/4;
      avarageSpeed  = (speed[0]+speed[1]+speed[2]+speed[3])/4;
      displayLCD(2, pukulan, tendangan);
  } else if(buttonActive[2]){
      avarageDamage = (damage[0]+damage[1]+damage[2])/3;
      avarageSpeed  = (speed[0]+speed[1]+speed[2])/3;
      displayLCD(3, pukulan, tendangan);
  } else if(buttonActive[3]){
      avarageDamage = (damage[0]+damage[1]+damage[2])/3;
      avarageSpeed  = (speed[0]+speed[1]+speed[2])/3;
      displayLCD(4, pukulan, tendangan);
  } else if(hasilNilai){
    displayLCD(10);
  } else {
    displayLCD(0);
  }

}
