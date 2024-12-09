#include <WiFi.h>
#include "CTBot.h"

#define ssid "Aulia"
#define password "12345678"
#define token "7871398250:AAHpFR-GI9OyQutRKfao6POATb4QqrLc1SI"
#define id  msg.sender.id

CTBot myBot;
CTBotInlineKeyboard PompaOn, PompaOff;

#define water_pump  22
#define BuzzerPin   23

#define level1 18
#define level2 19
#define level3 21

bool severOn;
bool logic1, logic2, logic3, logic4;
bool pompaOn = false;
bool buzzerOn = false;

void buzzer(int index = 1, int timer = 50){
    for(int i=0; i<index; i++){
      digitalWrite(BuzzerPin, HIGH);
      delay(timer);
      digitalWrite(BuzzerPin, LOW);
      delay(timer);
    }
}

void setup() {
  Serial.begin(115200);  
  pinMode (2, OUTPUT);
  digitalWrite(2, LOW);

  pinMode(water_pump, OUTPUT);
  pinMode(BuzzerPin, OUTPUT);
  
  pinMode(level1, INPUT_PULLDOWN);
  pinMode(level2, INPUT);
  pinMode(level3, INPUT);

  digitalWrite(water_pump, HIGH);
  digitalWrite(BuzzerPin, LOW);

   Serial.print("Menghubungkan ke Wifi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);   Serial.print(".");
  }    digitalWrite(2, 1); delay(500);
       digitalWrite(2, 0);
       Serial.println("");
       Serial.println("Menghubungkan ke Telegram");
  while (!myBot.testConnection()){
        myBot.setTelegramToken(token);
        delay(100);
  } digitalWrite (2, 1);
  Serial.println("Telegram terhubung");

  PompaOn.addButton("✅ Ya", "pompaOn", CTBotKeyboardButtonQuery);
  PompaOn.addButton("❌ Tidak", "no", CTBotKeyboardButtonQuery);
  PompaOff.addButton("✅ Ya", "pompaOff", CTBotKeyboardButtonQuery);
  PompaOff.addButton("❌ Tidak", "no", CTBotKeyboardButtonQuery);

  logic1 = true;
}

void loop(){
  TBMessage msg;
  int level1_value = !digitalRead(level1);
  int level2_value = !digitalRead(level2);
  int level3_value = !digitalRead(level3);

  Serial.print(level1_value);
  Serial.print(level2_value);
  Serial.print(level3_value);

  Serial.print(" ");
  Serial.print(logic1);
  Serial.print(logic2);
  Serial.print(logic3);
  Serial.print(logic4);
  Serial.println();

  
  
  delay(50);
  
  if (myBot.getNewMessage(msg)) {
    if (msg.text.equalsIgnoreCase("/start")) {                                      
          myBot.sendMessage(id, "Halo...\nSelamat Datang 👇");
          severOn = true;
    } else if (msg.text.equalsIgnoreCase("/OnPompa")) {                                      
            digitalWrite(water_pump, LOW);
            myBot.sendMessage(id, "🚿 Pompa Telah Dinyalakan");
            pompaOn = true;
            buzzerOn = false;
    } else if (msg.text.equalsIgnoreCase("/OffPompa")) {                                      
            myBot.sendMessage(id, "🚫 Pompa Telah Dimatikan"); 
           digitalWrite(water_pump, HIGH); 
           pompaOn = false;
    } else if (msg.text.equalsIgnoreCase("/status")) {
        if(pompaOn){
          myBot.sendMessage(id, "🚿 Pompa Sedang Dinyalakan");
        } else {
          myBot.sendMessage(id, "🚫 Pompa Tidak Menyala");
        }
        if(level1_value && level2_value && !level3_value){
            myBot.sendMessage(id, "Air Terisi 50%");
        } else if(level1_value && level2_value && level3_value){
            myBot.sendMessage(id, "Air Terisi Penuh");
        } else if(!level1_value && !level2_value && !level3_value){
           myBot.sendMessage(id, "Air Habis");
        }
    } else if (msg.messageType == CTBotMessageQuery) {
        if (msg.callbackQueryData.equals("pompaOn")) 
        {
            digitalWrite(water_pump, LOW);
            myBot.sendMessage(id, "🚿 Pompa Telah Dinyalakan");
            pompaOn = true; 
            buzzerOn = false;
        }
    } else {
       myBot.sendMessage(id, "Perintah Tidak Valid! ❌\nSilahkan Coba Perintah dibawah 👇\n/OnPompa -> Menyalakan Pompa \n/OffPompa -> Mematikan Pompa\n/status -> Cek Status air");
    }
  }

  if(severOn){
    if(!level1_value && !pompaOn && logic1){
      myBot.sendMessage(id, "Air Kosong, Hidupkan Pompa Air?", PompaOn); 
      buzzerOn = true;
      logic1 = false;  logic2 = true;
    } else if(level1_value && level2_value && !level3_value && pompaOn && logic2){
      myBot.sendMessage(id, "Air Terisi 50%"); 
      logic2 = false; logic3 = true;
    } else if(level1_value && level2_value && !level3_value && !pompaOn && logic4){
      myBot.sendMessage(id, "Air tersisa 50%"); 
      logic4 = false; logic1 = true;
    } else if(level1_value && level2_value && level3_value && pompaOn && logic3){
       myBot.sendMessage(id, "Air Penuh, Mematikan Pompa"); 
       digitalWrite(water_pump, HIGH); 
       pompaOn = false;  logic4 = true;
    }

    if(buzzerOn){
      buzzer();
    }
  }
}
