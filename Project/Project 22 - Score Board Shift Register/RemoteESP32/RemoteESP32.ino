#include <esp_now.h>
#include <WiFi.h>
#include <Keypad.h>

#define ledRed   23
#define ledGreen 15
#define button1 19
#define button2 21
#define button3  5
#define button4 18
#define BuzzerPin 22

char keys[4][4] = { {'1','2','3','A'},
                    {'4','5','6','B'},
                    {'7','8','9','C'},
                    {'*','0','#','D'}};
byte rowPins[4] = {13, 12, 14, 27}; // konektor baris ke pin ESP32
byte colPins[4] = {26, 25, 33, 32}; // konektor kolom ke pin ESP32
Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

// {0x40, 0x91, 0x51, 0x9B, 0xD8, 0xA4};
uint8_t AddressBoard[] = {0xE8, 0x6B, 0xEA, 0xD4, 0x6F, 0x44};
uint8_t AddressHomeBoard[] = {0xE0, 0x98, 0x06, 0x8F, 0xD1, 0x79};
uint8_t AddressGuestBoard[] = {0x84, 0xF3, 0xEB, 0x98, 0xCD, 0xA7}; 


typedef struct struct_message {
  char a[32]; // Pesan ESP
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;
esp_now_peer_info_t peerInfoHome;
esp_now_peer_info_t peerInfoGuest;

unsigned long currentTime = 0;
String HomeScore  = "";
String GuestScore = "";

void buzzer(int index = 1, int timer = 40){
    for(int i=0; i<index; i++){
      digitalWrite(BuzzerPin, HIGH);
      delay(timer);
      digitalWrite(BuzzerPin, LOW);
      delay(timer);
    }
}

String dataPesan(){
  String data = "";
    while(Serial2.available()>0){
      data += char(Serial2.read());
    } data.trim(); 
  return data;
}

void splitString(String str, char delimiter, String &part1, String &part2) {
  int delimiterIndex = str.indexOf(delimiter);
  if (delimiterIndex != -1) {
    part1 = str.substring(0, delimiterIndex);
    part2 = str.substring(delimiterIndex + 1);
  } else {
    part1 = str;
    part2 = "";
  }
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if(status == ESP_NOW_SEND_SUCCESS){
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledRed, LOW);
  } else {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledRed, HIGH);
  }
}

void setup(){
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } 
  
  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo.peer_addr, AddressBoard, 6);
  memcpy(peerInfoHome.peer_addr, AddressHomeBoard, 6);
  memcpy(peerInfoGuest.peer_addr, AddressGuestBoard, 6);
  
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  peerInfoHome.channel = 0;  
  peerInfoHome.encrypt = false;
  peerInfoGuest.channel = 0;  
  peerInfoGuest.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  } if (esp_now_add_peer(&peerInfoHome) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  } if (esp_now_add_peer(&peerInfoGuest) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
  
  
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);
  pinMode(BuzzerPin, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  strcpy(myData.a, "<empty>");

  digitalWrite(ledRed, LOW);
  digitalWrite(ledGreen, LOW);
}
 
void loop(){

  bool value1 = digitalRead(button1);
  bool value2 = digitalRead(button2);
  bool value3 = digitalRead(button3);
  bool value4 = digitalRead(button4);

  char key = myKeypad.getKey();
  String Pesan = dataPesan();

  if(Pesan != ""){
    if(Pesan[0] == 'H'){
       Serial.println(Pesan.c_str());
       strcpy(myData.a, Pesan.c_str());
       esp_now_send(AddressHomeBoard, (uint8_t *) &myData, sizeof(myData));
    } else if(Pesan[0] == 'G'){
       Serial.println(Pesan.c_str());
       strcpy(myData.a, Pesan.c_str());
       esp_now_send(AddressGuestBoard, (uint8_t *) &myData, sizeof(myData));
    }
  } 
 
  if (key != NO_KEY) {
    buzzer();
    
    if(key == '1'){
      strcpy(myData.a, "HomeScore+");
    } else if(key == '2'){
      strcpy(myData.a, "HomeFoul+");
    } else if(key == '3'){
      strcpy(myData.a, "GuestFoul+");
    } else if(key == '4'){
      strcpy(myData.a, "HomeScore-");
    } else if(key == '5'){
      strcpy(myData.a, "HomeFoul-");
    } else if(key == '6'){
      strcpy(myData.a, "GuestFoul-");
    } else if(key == '7'){
      strcpy(myData.a, "Reset");
    } else if(key == '8'){
      strcpy(myData.a, "Set20m");
    } else if(key == '9'){
      strcpy(myData.a, "Timer+");
    } else if(key == '0'){
      strcpy(myData.a, "Start");
    } else if(key == 'A'){
      strcpy(myData.a, "GuestScore+");
    } else if(key == 'B'){
      strcpy(myData.a, "GuestScore-");
    } else if(key == 'C'){
      strcpy(myData.a, "ResetTout");
    } else if(key == 'D'){
      strcpy(myData.a, "BELL");
    } else if(key == '*'){
      strcpy(myData.a, "Quarter");
    } else if(key == '#'){
      strcpy(myData.a, "Timer-");
    } 
    esp_now_send(AddressBoard, (uint8_t *) &myData, sizeof(myData));
  }


  if(millis() - currentTime > 1000){
    if(!value1){
      strcpy(myData.a, "HomeTout+");
      esp_now_send(AddressBoard, (uint8_t *) &myData, sizeof(myData));
      currentTime = millis();
    } else if (!value2){
      strcpy(myData.a, "HomeTout-");
      esp_now_send(AddressBoard, (uint8_t *) &myData, sizeof(myData));
      currentTime = millis();
    } else if (!value3){
      strcpy(myData.a, "GuestTout+");
      esp_now_send(AddressBoard, (uint8_t *) &myData, sizeof(myData));
      currentTime = millis();
    }
    else if (!value4){
      strcpy(myData.a, "GuestTout-");
      esp_now_send(AddressBoard, (uint8_t *) &myData, sizeof(myData));
      currentTime = millis();
    }
  }
}
