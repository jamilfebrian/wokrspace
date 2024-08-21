#include <esp_now.h>
#include <WiFi.h>

#define buzzerPin 14

static int count = 0;

typedef struct struct_message {
  char a[32]; 
} struct_message;
struct_message myData;

void buzzer(int index = 1, int timer = 80){
    for(int i=0; i<index; i++){
      digitalWrite(buzzerPin, HIGH);
      delay(timer);
      digitalWrite(buzzerPin, LOW);
      delay(timer);
    }
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  String dataKirim = String(myData.a);

    Serial.println(dataKirim);
    count = (count>10) ? 0 : count++;
    
    buzzer(2,80);
}

void setup() {
  Serial.begin(115200);
  pinMode(buzzerPin,OUTPUT);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  
}
