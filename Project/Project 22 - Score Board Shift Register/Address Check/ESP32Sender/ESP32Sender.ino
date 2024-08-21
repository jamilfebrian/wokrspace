#include <esp_now.h>
#include <WiFi.h>

#define buttonPin 14
#define ledPin 2

uint8_t broadcastAddressESP32[] = {0xE8, 0x6B, 0xEA, 0xD4, 0xDA, 0xC0};
uint8_t broadcastAddressESP8266[] = {0xCC, 0x50, 0xE3, 0xFA, 0x03, 0xFC};

typedef struct struct_message {
  char a[32];
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if(status == ESP_NOW_SEND_SUCCESS){
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
    strcpy(myData.a, "THIS IS A Fail Address");
    esp_now_send(broadcastAddressESP8266, (uint8_t *) &myData, sizeof(myData));
  }
}
 
void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo.peer_addr, broadcastAddressESP32, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  bool buttonValue = digitalRead(buttonPin);
  if(buttonValue == 0){
    strcpy(myData.a, "THIS IS A CHAR");
    esp_err_t result = esp_now_send(broadcastAddressESP32, (uint8_t *) &myData, sizeof(myData));
      if (result == ESP_OK) {
        Serial.println("Sent with success");
      } else {
        Serial.println("Error sending the data");
      } delay(2000);
  } 

}
