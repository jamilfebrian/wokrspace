#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

Servo myservo1; 
Servo myservo2; 

#define RST_PIN   9  
#define SS_PIN   10  

#define RST_PIN2   7  
#define SS_PIN2    8  

#define IR1 A0 
#define IR2 A1
#define IR3 A2
#define IR4 A3

#define servo1 6
#define servo2 5

MFRC522 mfrc522(SS_PIN, RST_PIN); 
MFRC522 mfrc522_2(SS_PIN2, RST_PIN2); 
LiquidCrystal_I2C lcd(0x27,16,2);  

const int maxSize = 10; // Ukuran maksimum array
String myArray[maxSize] = {};  // Array untuk menyimpan nilai
int currentSize = 0;  

bool buka, tutup;
bool tutupAtas, tutupBawah;
int slotAtas = 8;
int slotBawah = 8;
unsigned long timer = 0;

bool isValueExists(String value) {
    for (int i = 0; i < currentSize; i++) {
        if (myArray[i] == value) {
            return true; 
        }
    }
    return false;
}

void push(String value) {
  if (currentSize < maxSize) {
      myArray[currentSize] = value; 
      currentSize++;
  } else {
      Serial.println("ID penuh! Tidak bisa menambahkan nilai.");
  }
}

void removeValue(String value) {
    int index = -1;

    for (int i = 0; i < currentSize; i++) {
        if (myArray[i] == value) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        for (int i = index; i < currentSize - 1; i++) {
            myArray[i] = myArray[i + 1];
        }
        currentSize--; 
        Serial.print("ID ");
        Serial.print(value);
        Serial.println(" berhasil dihapus.");
        
    } else {
        Serial.print("ID ");
        Serial.print(value);
        Serial.println(" tidak ditemukan.");
    }
}

void printArray() {
    Serial.print("Array: \n");
    for (int i = 0; i < currentSize; i++) {
        Serial.print(i);
        Serial.print(". ");
        Serial.print(myArray[i]);
        Serial.print("\n");
    }
    Serial.println();
}

void readRFID2(){
  SPI.begin();  mfrc522_2.PCD_Init(); 
  String content = "";
  if (!mfrc522_2.PICC_IsNewCardPresent()) {
     return;
  }
  if (!mfrc522_2.PICC_ReadCardSerial()) {
    return;
  }

  for (byte i = 0; i < mfrc522_2.uid.size; i++){
     content.concat(String(mfrc522_2.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522_2.uid.uidByte[i], HEX));
  }  content.toUpperCase();
  String UID = content.substring(1);
  Serial.println(UID);

  if(isValueExists(UID)){
    Serial.print("ID ");
    Serial.print(UID);
    Serial.println(" Succes. Silahkan Keluar");
    if(tutupAtas){
      slotAtas++;
      myservo1.write(90); 
      tutupAtas = false;
   }else if(tutupBawah){
      slotBawah++;
      myservo1.write(90); 
      tutupBawah = false;
    }
    timer = millis();
  } else {
    Serial.println("Mobil Tdak terdaftar");
    return;
  }
  
  removeValue(UID);
  tutup = false;
}


void readRFID(){
  SPI.begin();  mfrc522.PCD_Init(); 
  String content = "";
  if (!mfrc522.PICC_IsNewCardPresent()) {
     return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  for (byte i = 0; i < mfrc522.uid.size; i++){
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }  content.toUpperCase();
  String UID = content.substring(1);
  Serial.println(UID);

  if(isValueExists(UID) ){
    Serial.print("ID ");
    Serial.print(UID);
    Serial.println(" Sudah Terdaftar. Tidak ditambahkan.");
    return;
  } else if(slotAtas>0 || slotBawah>0){
    if(buka){
      Serial.println("Mobil belum memilih parkir, Harap Bersabar");
      return;
    } else {
      push(UID);
      Serial.println("Pintu parkir telah dibuka");
      myservo2.write(90); 
      buka = true;
    }
  }
}

void printScreen(int value1, int value2){
  if(value1 == 0 && value2 == 0){
    lcd.setCursor(0,0); lcd.print("Semua Slot Parkir ");
    lcd.setCursor(0,1); lcd.print("   Telah Penuh    ");
  } else {
    if(value1 == 0){
      lcd.setCursor(0,0); lcd.print("Prkir Atas Penuh  ");
    } else if(value1 > 0){
      lcd.setCursor(0,0); lcd.print("Atas  : "); lcd.print(value1); lcd.print(" Slot  ");
    } 
    if(value2 == 0){
      lcd.setCursor(0,1); lcd.print("Prkir Bwh Penuh   ");
    } else if(value2 > 0){
      lcd.setCursor(0,1); lcd.print("Bawah : "); lcd.print(value2); lcd.print(" Slot  ");
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("inisialisasi");

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);

  myservo1.attach(servo1);
  myservo2.attach(servo2);
    myservo1.write(180); 
    myservo2.write(180); 
    
  lcd.init(); lcd.backlight(); lcd.clear();
}

void loop() {
  int ir1_value = digitalRead(IR1);
  int ir2_value = digitalRead(IR2);
  int ir3_value = digitalRead(IR3);
  int ir4_value = digitalRead(IR4);

  readRFID();

  if(!ir1_value && buka){
    myservo2.write(180); 
    slotAtas--;
    buka = false;
  }

  if(!ir4_value && buka){
    myservo2.write(180); 
    slotBawah--;
    buka = false;
  }

  if(!ir2_value && !tutupAtas && !tutup){
    tutupAtas = true;
    tutup = true;
  }

  if(!ir3_value && !tutupBawah && !tutup){
    tutupBawah = true;
    tutup = true;
  }

  if(tutup){
    readRFID2();
  }

  if((!tutupBawah || !tutupAtas)  && millis() - timer > 5000){
    myservo1.write(180); 
  }

  if(slotAtas <= 0){
    slotAtas = 0;
  } else if(slotAtas >= 8){
    slotAtas = 8;
  }
  if(slotBawah <= 0){
    slotBawah = 0;
  } else if(slotBawah >= 8){
    slotBawah = 8;
  }

  printScreen(slotAtas, slotBawah);

}
