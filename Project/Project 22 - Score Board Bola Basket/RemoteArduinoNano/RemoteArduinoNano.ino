#include <Keypad.h>
#include <EEPROM.h>

#define strobePin  A1
#define dataPin   A2
#define clockPin  A3

#define LED1 11
#define LED2 12
#define BuzzerPin 13

byte segChar[]={
  0b10000001, // 0
  0b11101101, // 1
  0b01000011, // 2
  0b01001001, // 3
  0b00101101, // 4
  0b00011001, // 5
  0b00010001, // 6
  0b11001101, // 7
  0b00000001, // 8
  0b00001001, // 9
  0b00000000, // 10 ON
  0b11111111, // 11 OFF
};

char keys[4][4] = { {'1','2','3','A'},
                    {'4','5','6','B'},
                    {'7','8','9','C'},
                    {'*','0','#','D'}};
byte rowPins[4] = {9, 8, 7, 6}; // konektor baris arduino
byte colPins[4] = {5, 4, 3, 2}; // konektor arduino
Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

int guestScore[12] = {0,0,0,0,0,0,0,0,0,0,0,0}; 
int HomeScore[12] = {0,0,0,0,0,0,0,0,0,0,0,0}; 

unsigned long currentTime = 0;
String keyword = "00"; int keywords;
int x = 0;
bool guest,home;

void buzzer(int index = 1, int timer = 40){
    for(int i=0; i<index; i++){
      digitalWrite(BuzzerPin, HIGH);
      delay(timer);
      digitalWrite(BuzzerPin, LOW);
      delay(timer);
    }
}

String getLastTwoChars(String str) {
  if (str.length() >= 2) {
    return str.substring(str.length() - 2);
  } else {
    return str;
  }
}

void update_display(String index){
 int digit1 = index[0] - '0';
 int digit2 = index[1] - '0';
  
  digitalWrite(strobePin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, segChar[digit1]);
    shiftOut(dataPin, clockPin, MSBFIRST, segChar[digit2]);
  digitalWrite(strobePin, HIGH);
}

void setup() {
  for(int i=0; i<12; i++){
    EEPROM.read(i+1);
    EEPROM.read(i+13);
  }
  
  for(int i=0; i<12; i++){
    HomeScore[i] = EEPROM.read(i+1);
    guestScore[i] = EEPROM.read(i+13);
  }
  
  Serial.begin(115200);
 
  pinMode(strobePin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  pinMode(LED1 ,OUTPUT);
  pinMode(LED2 ,OUTPUT);
  pinMode(BuzzerPin ,OUTPUT);
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,HIGH);

  update_display("00");
  Serial.println("Hres");
  Serial.println("Gres");

}

void loop() {
  char key = myKeypad.getKey();
  
     if (key != NO_KEY) {
      buzzer();
  
        keyword = (isDigit(key)) ? (keyword+key) : keyword;
        
        if(isDigit(key)){
          keyword = getLastTwoChars(keyword);
          keywords = keyword.toInt();
          update_display(keyword);
        }
          
        if(key == '*'){
          keyword = "00"; keywords = 0;
          update_display("00");
          if(home){
            Serial.println("Hres");
            for(int i=0; i<12; i++){
              HomeScore[i] = 0;
              EEPROM.write(i+1 ,HomeScore[i]);
            }
          } else if(guest){
            Serial.println("Gres");
            for(int i=0; i<12; i++){
              guestScore[i] = 0;
              EEPROM.write(i+13 ,guestScore[i]);
            }
          }
        } else if(key == 'A'){ //==== HOME
          digitalWrite(LED1,HIGH);
          digitalWrite(LED2,LOW);
          home = true; guest = false;
          keyword = "00";
          
        } else if(key == 'B'){ //==== GUEST
          digitalWrite(LED1,LOW);
          digitalWrite(LED2,HIGH);
          home = false; guest = true;
          keyword = "00";
          
        } else if(key == 'C'){  //==== Plus
          if(home && keywords>=4 && keywords<=15){
            HomeScore[keywords-4] = (HomeScore[keywords-4]<5) ? HomeScore[keywords-4]+1 : HomeScore[keywords-4]; 
            EEPROM.write(keywords-4+1 ,HomeScore[keywords-4]);
          } else if(guest && keywords>=4 && keywords<=15){
            guestScore[keywords-4] = (guestScore[keywords-4]<5) ? guestScore[keywords-4]+1 : guestScore[keywords-4];
            EEPROM.write(keywords-4+13 ,guestScore[keywords-4]);
          }
          if(home){
            String dataKirim = "H";
            for(int i=0; i<12; i++){
              dataKirim += String(HomeScore[i]);
            } Serial.println(dataKirim);
          } else if(guest){
            String dataKirim = "G";
            for(int i=0; i<12; i++){
              dataKirim += String(guestScore[i]);
            } Serial.println(dataKirim);
          } 
        } else if(key == 'D'){  //==== Minus
          if(home && keywords>=4 && keywords<=15){
            HomeScore[keywords-4] = (HomeScore[keywords-4]>0) ? HomeScore[keywords-4]-1 : HomeScore[keywords-4]; 
            EEPROM.write(keywords-4+1 ,HomeScore[keywords-4]);
          } else if(guest && keywords>=4 && keywords<=15){
            guestScore[keywords-4] = (guestScore[keywords-4]>0) ? guestScore[keywords-4]-1 : guestScore[keywords-4];
            EEPROM.write(keywords-4+13 ,guestScore[keywords-4]);
          }
          if(home){
            String dataKirim = "H";
            for(int i=0; i<12; i++){
              dataKirim += String(HomeScore[i]);
            } Serial.println(dataKirim);
          } else if(guest){
            String dataKirim = "G";
            for(int i=0; i<12; i++){
              dataKirim += String(guestScore[i]);
            } Serial.println(dataKirim);
          }
        } else if(key == '#'){ //==== Refresh
            keyword = "00"; 
            if(home){
            String dataKirim = "H";
            for(int i=0; i<12; i++){
              dataKirim += String(HomeScore[i]);
            } Serial.println(dataKirim);
          } else if(guest){
            String dataKirim = "G";
            for(int i=0; i<12; i++){
              dataKirim += String(guestScore[i]);
            } Serial.println(dataKirim);
          }
        }
  }
}
