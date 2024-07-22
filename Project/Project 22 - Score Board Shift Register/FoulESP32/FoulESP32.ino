//#include <ESP8266WiFi.h>
//void setup(){
//  Serial.begin(115200);
//}
//void loop(){
//  Serial.println(WiFi.macAddress());
//  delay(100);
//}


#include <ESP8266WiFi.h>
#include <espnow.h>

#define led 2 

typedef struct struct_message {
    char a[32];
} struct_message;

struct_message myData;

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.println(myData.a);
}
 
void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  
}
