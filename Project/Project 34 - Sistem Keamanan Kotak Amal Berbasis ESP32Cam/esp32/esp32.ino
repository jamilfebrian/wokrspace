#include <SPI.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <MFRC522.h>
#include <esp_now.h>
#include <WiFi.h>

SoftwareSerial softSerial(/*rx =*/16, /*tx =*/17);
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // ganti dengan address receiver

typedef struct struct_message {
  bool a;
} struct_message;
struct_message myData;
esp_now_peer_info_t peerInfo;

#define FPSerial softSerial
#define buzzerPin  2 
#define RelayPin  15 
#define SensorGetarPin  35 

#define RST_PIN  4
#define SS_PIN   5

char keys[4][4] = { {'1','2','3','A'},
                    {'4','5','6','B'},
                    {'7','8','9','C'},
                    {'*','0','#','D'}};
byte rowPins[4] = {13, 12, 14, 27}; // konektor baris ke pin ESP32
byte colPins[4] = {26, 25, 33, 32}; // konektor kolom ke pin ESP32

static String cursorLCD = "";
static unsigned long prevMillis = 0, timerMillis = 500;

String validPassword = "1234";
String verifikasiPassword;
static String password = "";
static String Mode = "B";
bool updatePassword = false;
bool updateRFID = false;

bool bukaKunci;
bool resetMode;
unsigned long timerKunci = 0;
unsigned long timerMode = 0;

const int maxSize = 3; // Ukuran maksimum array
String myArray[maxSize] = {};  // Array untuk menyimpan nilai
int currentSize = 0;  

DFRobotDFPlayerMini myDFPlayer;
MFRC522 mfrc522(SS_PIN, RST_PIN); 
Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void buzzer(int index, int timer=50){
    for(int i=0; i<index; i++){
      digitalWrite(buzzerPin, HIGH);
      delay(timer);
      digitalWrite(buzzerPin, LOW);
      delay(timer);
    }
}

void DFConnect(){
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(FPSerial, /*isACK = */true, /*doReset = */true)) { 
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); 
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(30); 
}

void passKey(){
  
  char key = myKeypad.getKey();

  if (key != NO_KEY) {
    
      if(key != '#'){
        buzzer(1);
      } 
      timerMode = millis();
      
      password += key;

      if(key == 'A'){
        Mode = "A"; password = "";
        updatePassword = false;
        verifikasiPassword = "";
        myDFPlayer.play(13);
      } else if(key == 'B'){
        Mode = "B"; password = "";
      } else if(key == 'C'){
        Mode = "C"; password = "";
        updateRFID = false;
        myDFPlayer.play(13);
      } else if(key == 'D'){
        Mode = "D"; password = "";
        myDFPlayer.play(7);
      }

      if(key == '*'){
          password.remove(password.length()-2);
      } else if(key == '#'){
          password.remove(password.length()-1);
          if(Mode == "A"){
            if(updatePassword){
              if(verifikasiPassword != ""){
                if(verifikasiPassword == password){
                  myDFPlayer.play(3);
                  lcd.setCursor(0,0); lcd.print(" Kode Berhasil  ");
                  lcd.setCursor(0,1); lcd.print("  Diperbaharui  ");
                  validPassword = verifikasiPassword;
                  saveStringToEEPROM(0, validPassword);
                  updatePassword = false;
                  verifikasiPassword = "";
                  Mode = "B";
                  buzzer(3);
                  delay(1000);
                } else {
                  myDFPlayer.play(6);
                  lcd.setCursor(0,0); lcd.print("MAAF! Kode yang ");
                  lcd.setCursor(0,1); lcd.print("Dimasukkan Salah"); 
                  buzzer(1,1000);
                }
              } else {
                if(password.length() < 4){
                  myDFPlayer.play(4);
                  lcd.setCursor(0,0); lcd.print("  Kode Minimal  ");
                  lcd.setCursor(0,1); lcd.print(" 4 Digit Angka  "); 
                  buzzer(1,1000);
                } else if(password.length() >= 8 ){
                  myDFPlayer.play(16);
                  lcd.setCursor(0,0); lcd.print(" Kode Maksimal  ");
                  lcd.setCursor(0,1); lcd.print(" 8 Digit Angka  "); 
                  buzzer(1,1000);
                } else {
                  verifikasiPassword = password; 
                  myDFPlayer.play(2);
                }
              }
            } else {
              if(password == validPassword){
                myDFPlayer.play(1);
                lcd.setCursor(0,0); lcd.print("   Kode Benar   ");
                lcd.setCursor(0,1); lcd.print("                "); 
                updatePassword = true;
                buzzer(3);
                delay(1000);
              } else {
                myDFPlayer.play(6);
                lcd.setCursor(0,0); lcd.print("MAAF! Kode yang "); 
                lcd.setCursor(0,1); lcd.print("Dimasukkan Salah");
                buzzer(1,1000);
              } 
            }
          } else if(Mode == "B"){
            if(password == validPassword){
              myDFPlayer.play(5); 
              digitalWrite(RelayPin, LOW); 
              bukaKunci = true; 
              timerKunci = millis();
              lcd.setCursor(0,0); lcd.print("   Kode Benar   ");
              lcd.setCursor(0,1); lcd.print("                ");
              buzzer(3);
              delay(1000);
            } else {
              myDFPlayer.play(6);
              lcd.setCursor(0,0); lcd.print("MAAF! Kode yang ");
              lcd.setCursor(0,1); lcd.print("Dimasukkan Salah");
              buzzer(1,1000);
            } 
          } else if(Mode == "C"){
            if(password == validPassword){
              myDFPlayer.play(15); 
              lcd.setCursor(0,0); lcd.print("   Kode Benar   ");
              lcd.setCursor(0,1); lcd.print("                ");
              updateRFID = true;
              buzzer(15);
              delay(1000);
            } else {
              myDFPlayer.play(6);
              lcd.setCursor(0,0); lcd.print("MAAF! Kode yang "); 
              lcd.setCursor(0,1); lcd.print("Dimasukkan Salah");
              buzzer(1,1000);
            } 
          } else if(Mode == "D"){
            
          }
          password = "";
      }
    }
}

void printLCD(){
  if((millis() - prevMillis) > timerMillis){
        if(cursorLCD == "_"){
          cursorLCD = "";
        } else {
          cursorLCD = "_";
        } 
        prevMillis = millis();
      }

      if(Mode == "A"){
        if(updatePassword == true && verifikasiPassword != ""){
          lcd.setCursor(0,0); lcd.print("Verifikasi Kode :  ");
          lcd.setCursor(0,1); lcd.print(password + cursorLCD + "                ");
        } else if(updatePassword == true && verifikasiPassword == ""){
          lcd.setCursor(0,0); lcd.print("Masukkan Kode   ");
          lcd.setCursor(0,1); lcd.print("Baru : "+ password + cursorLCD + "                ");
        } else {
          lcd.setCursor(0,0); lcd.print("Masukkan Kode   ");
          lcd.setCursor(0,1); lcd.print("Lama : "+ password + cursorLCD + "                ");
        }
      } else if (Mode == "B"){
        lcd.setCursor(0,0); lcd.print("Masukkan Kode : ");
        lcd.setCursor(0,1); lcd.print(password + cursorLCD + "                ");
      } else if (Mode == "C"){
        if(updateRFID){
          lcd.setCursor(0,0); lcd.print("  Silahkan Tap   ");
          lcd.setCursor(0,1); lcd.print("   Kartu Anda    ");
        } else {
          lcd.setCursor(0,0); lcd.print("Masukkan Kode   ");
          lcd.setCursor(0,1); lcd.print("Lama : " + password + cursorLCD + "                ");
        }
      } else if (Mode == "D"){
        lcd.setCursor(0,0); lcd.print("  Silahkan Tap   ");
        lcd.setCursor(0,1); lcd.print("   Kartu Anda    ");
      } 
}

bool isValueExists(String value) {
    for (int i = 0; i < currentSize; i++) {
        if (myArray[i] == value) {
            return true; 
        }
    }
    return false;
}

bool push(String value) {
  if (currentSize < maxSize) {
      myArray[currentSize] = value; 
      currentSize++;
      return false;
  } else {
      myDFPlayer.play(14); Mode = "B";
      Serial.println("ID penuh! Tidak bisa menambahkan nilai.");
      lcd.setCursor(0,0); lcd.print("    ID Kartu      ");
      lcd.setCursor(0,1); lcd.print("   Telah Penuh    ");
      delay(2000); return true;
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

void readRFID(){
  SPI.begin();  mfrc522.PCD_Init(); 
  String content = "";
  
  if (!mfrc522.PICC_IsNewCardPresent()) {
     return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  timerMode = millis();

  for (byte i = 0; i < mfrc522.uid.size; i++){
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }  content.toUpperCase();
  String UID = content.substring(1);
  Serial.println(UID);

  if(Mode == "C"){
    if(updateRFID){
      if(isValueExists(UID)){
        myDFPlayer.play(13);
        lcd.setCursor(0,0); lcd.print(" ID Sudah Pernah");
        lcd.setCursor(0,1); lcd.print("    Terdaftar   ");
        buzzer(1,1000); 
        return;
      } else {
        if(push(UID)){
          return;
        }
        myDFPlayer.play(8); Mode = "B";
        lcd.setCursor(0,0); lcd.print("  ID Berhasil   ");
        lcd.setCursor(0,1); lcd.print("  Ditambahkan   ");
        buzzer(3); delay(1000);
        updatePassword = false;
      }
    } else {
      myDFPlayer.play(13);
      lcd.setCursor(0,0); lcd.print(" Masukkan Kode  "); 
      lcd.setCursor(0,1); lcd.print("Terlebih Dahulu ");
      buzzer(1,1000);
    } 
  } else if(Mode == "D"){
    if(isValueExists(UID)){
      removeValue(UID); myDFPlayer.play(9);  Mode = "B";
      lcd.setCursor(0,0); lcd.print("  ID Berhasil  ");
      lcd.setCursor(0,1); lcd.print("    Dihapus    ");
      buzzer(1,1000); 
      return;
    } else {
      myDFPlayer.play(11);
      lcd.setCursor(0,0); lcd.print(" ID Kartu Tidak ");
      lcd.setCursor(0,1); lcd.print("   Ditemukan    ");
      buzzer(1,1000);
    }
  } else if(Mode == "B"){
    if(isValueExists(UID)){
      myDFPlayer.play(10);
      digitalWrite(RelayPin, LOW); 
      bukaKunci = true; 
      timerKunci = millis();
      lcd.setCursor(0,0); lcd.print("    Valid ID    ");
      lcd.setCursor(0,1); lcd.print("                ");
      buzzer(3); delay(1000);
      return;
    } else {
      myDFPlayer.play(11);
      lcd.setCursor(0,0); lcd.print(" ID Kartu Tidak ");
      lcd.setCursor(0,1); lcd.print("   Ditemukan    ");
      buzzer(1,1000);
    }
  }
}

void saveStringToEEPROM(int addr, const String &str) {
  int len = str.length();
  EEPROM.write(addr, len); 
  for (int i = 0; i < len; i++) {
    EEPROM.write(addr + 1 + i, str[i]); 
  }
  EEPROM.commit(); 
}

String readStringFromEEPROM(int addr) {
  int len = EEPROM.read(addr);
  String str = "";
  for (int i = 0; i < len; i++) {
    str += char(EEPROM.read(addr + 1 + i));
  }
  return str;
}

void setup() {
  FPSerial.begin(9600);
  Serial.begin(115200);
  EEPROM.begin(100);
  WiFi.mode(WIFI_STA);

  DFConnect();
  
  lcd.init(); lcd.backlight(); lcd.clear();
  pinMode(buzzerPin, OUTPUT);
  pinMode(RelayPin, OUTPUT);
  pinMode(SensorGetarPin, INPUT);

  digitalWrite(RelayPin, HIGH);

  validPassword = readStringFromEEPROM(0);
  Serial.println(validPassword);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } esp_now_register_send_cb(OnDataSent);
  
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
      
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  passKey();
  readRFID();
  printLCD();

  bool getar = digitalRead(SensorGetarPin);

  if(!getar && Mode == "B"){
    myDFPlayer.play(17);
    myData.a = true;
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    delay(1000);
  }


  if(bukaKunci && millis() - timerKunci > 10000){
    digitalWrite(RelayPin, HIGH); 
    bukaKunci = false;
  }
  
  if(millis() - timerMode > 15000){
    Mode = "B"; 
  }
  
}
