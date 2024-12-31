#include <WiFi.h>
#include "CTBot.h"

#define ssid "Gilang"
#define password "12345678"
#define token "7719106147:AAHEydg1ONGim7ynR_SxTbQCTyAENca4HAA"
#define id msg.sender.id

CTBot myBot;
CTBotReplyKeyboard Tbl;
CTBotInlineKeyboard TbLp1on, TbLp2on, TbLp3on, TbLp4on, TbLp5on;
CTBotInlineKeyboard TbLp1off, TbLp2off, TbLp3off, TbLp4off, TbLp5off;
CTBotInlineKeyboard TbLpAllon, TbLpAlloff;

#define RELAY_1 19
#define RELAY_2 21
#define RELAY_3 22
#define RELAY_4 23

 int Lampu_1 = HIGH;
 int Lampu_2 = HIGH;
 int Lampu_3 = HIGH;
 int Lampu_4 = HIGH;
 int Lampu_5 = HIGH;

 String Status_Lampu_1;
 String Status_Lampu_2;
 String Status_Lampu_3;
 String Status_Lampu_4;
 String Status_Lampu_5;

void setup() {
  Serial.begin(115200);  
  pinMode (2, OUTPUT);
  digitalWrite(2, 0);

  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);
  
  digitalWrite(RELAY_1, HIGH);
  digitalWrite(RELAY_2, HIGH);
  digitalWrite(RELAY_3, HIGH);
  digitalWrite(RELAY_4, HIGH);

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

           Tbl.addButton("Lampu 1");
           Tbl.addButton("Lampu 2");
           Tbl.addRow();
           Tbl.addButton("Kipas");
           Tbl.addButton("Charger");
           Tbl.addRow();
           Tbl.addButton("Semua Perangkat ON");
           Tbl.addButton("Semua Perangkat OFF");
           Tbl.addRow();
           Tbl.addButton("Cek Status");
           Tbl.enableResize();

           TbLp1on.addButton("✅ Ya", "1on", CTBotKeyboardButtonQuery);
           TbLp1on.addButton("❌ Tidak", "no", CTBotKeyboardButtonQuery);
           
           TbLp2on.addButton("✅ Ya", "2on", CTBotKeyboardButtonQuery);
           TbLp2on.addButton("❌ Tidak", "no", CTBotKeyboardButtonQuery);

           TbLp3on.addButton("✅ Ya", "3on", CTBotKeyboardButtonQuery);
           TbLp3on.addButton("❌ Tidak", "no", CTBotKeyboardButtonQuery);

           TbLp4on.addButton("✅ Ya", "4on", CTBotKeyboardButtonQuery);
           TbLp4on.addButton("❌ Tidak", "no", CTBotKeyboardButtonQuery);

           TbLp1off.addButton("✅ Ya", "1off", CTBotKeyboardButtonQuery);
           TbLp1off.addButton("❌ Tidak", "no", CTBotKeyboardButtonQuery);
           
           TbLp2off.addButton("✅ Ya", "2off", CTBotKeyboardButtonQuery);
           TbLp2off.addButton("❌ Tidak", "no", CTBotKeyboardButtonQuery);

           TbLp3off.addButton("✅ Ya", "3off", CTBotKeyboardButtonQuery);
           TbLp3off.addButton("❌ Tidak", "no", CTBotKeyboardButtonQuery);

           TbLp4off.addButton("✅ Ya", "4off", CTBotKeyboardButtonQuery);
           TbLp4off.addButton("❌ Tidak", "no", CTBotKeyboardButtonQuery);
           
           TbLpAllon.addButton("✅ Ya", "allon", CTBotKeyboardButtonQuery);
           TbLpAllon.addButton("❌ Tidak", "no", CTBotKeyboardButtonQuery);

           TbLpAlloff.addButton("✅ Ya", "alloff", CTBotKeyboardButtonQuery);
           TbLpAlloff.addButton("❌ Tidak", "no", CTBotKeyboardButtonQuery);
}

void loop() {
  TBMessage msg;

  if (myBot.getNewMessage(msg)) {
    if (msg.text.equalsIgnoreCase("/start")) {                                         
          myBot.sendMessage(id, "Halo...\nSelamat Datang Di Lampu Telegram bot Kakak.\n\nGunakan tombol dibawah untuk mengontrol lampu 👇", Tbl);
    } else if (msg.text.equalsIgnoreCase("Lampu 1")) {
        if (Lampu_1 == LOW) {
            myBot.sendMessage(id, "Saat ini Lampu 1 dalam keadaan ON\nApakah anda ingin mematikan Lampu 1?", TbLp1off);
        } else {
            myBot.sendMessage(id, "Saat ini Lampu 1 dalam keadaan OFF\nApakah anda ingin menyalakan Lampu 1?", TbLp1on);
        }
      } else if (msg.text.equalsIgnoreCase("Lampu 2")) {
        if (Lampu_2 == LOW) {
            myBot.sendMessage(id, "Saat ini Lampu 2 dalam keadaan ON\nApakah anda ingin mematikan Lampu 2?", TbLp2off);
       } else {
            myBot.sendMessage(id, "Saat ini Lampu 2 dalam keadaan OFF\nApakah anda ingin menyalakan Lampu 2?", TbLp2on);
       }
      } else if (msg.text.equalsIgnoreCase("Kipas")) {
        if (Lampu_3 == LOW) {
            myBot.sendMessage(id, "Saat ini Kipas dalam keadaan ON\nApakah anda ingin mematikan Kipas?", TbLp3off);
       } else {
            myBot.sendMessage(id, "Saat ini Kipas dalam keadaan OFF\nApakah anda ingin menyalakan Kipas?", TbLp3on);
        }
      } else if (msg.text.equalsIgnoreCase("Charger")) {
        if (Lampu_4 == LOW) {
            myBot.sendMessage(id, "Saat ini Charger dalam keadaan ON\nApakah anda ingin mematikan Charger?", TbLp4off);
        } else {
            myBot.sendMessage(id, "Saat ini Charger dalam keadaan OFF\nApakah anda ingin menyalakan Charger?", TbLp4on); 
        }
      }else if (msg.text.equalsIgnoreCase("Semua Perangkat ON")) {
            myBot.sendMessage(id, "Apakah anda ingin menyalakan Semua Perangkat?", TbLpAllon);
      } else if (msg.text.equalsIgnoreCase("Semua Perangkat OFF")) {
            myBot.sendMessage(id, "Apakah anda ingin mematikan Semua Perangkat?", TbLpAlloff);
      } else if (msg.text.equalsIgnoreCase("Cek Status")) {
            if (Lampu_1 == LOW) {
              Status_Lampu_1 = "Lampu 1 ON";
            } else {
              Status_Lampu_1 = "Lampu 1 OFF";
            }
            if (Lampu_2 == LOW) {
              Status_Lampu_2 = "Lampu 2 ON";
            } else {
              Status_Lampu_2 = "Lampu 2 OFF";
            }
            if (Lampu_3 == LOW) {
              Status_Lampu_3 = "Kipas ON";
            } else {
              Status_Lampu_3 = "Kipas OFF";
            }
            if (Lampu_4 == LOW) {
              Status_Lampu_4 = "Charger ON";
            } else {
              Status_Lampu_4 = "Charger OFF";
            }
       myBot.sendMessage(id, "Status saat ini : \n\n👉 " + Status_Lampu_1 + "\n\n👉 " + Status_Lampu_2 + "\n\n👉 " + Status_Lampu_3 + "\n\n👉 " + Status_Lampu_4 + "\n\n👉 " + Status_Lampu_5 + "\n\nGunakan tombol dibawah untuk mengontrol lampu 👇", Tbl);
      } else if (msg.messageType == CTBotMessageQuery) {
          if (msg.callbackQueryData.equals("1on")) 
          {
              Lampu_1 = LOW; digitalWrite(RELAY_1, LOW);
              myBot.sendMessage(id, "💡 Lampu 1 Telah Dinyalakan");
          }
          else if (msg.callbackQueryData.equals("1off")) 
          {
              Lampu_1 = HIGH;  digitalWrite(RELAY_1, HIGH);
              myBot.sendMessage(id, "🚫 Lampu 1 Telah Dimatikan");
          }
          else if (msg.callbackQueryData.equals("2on")) 
          {
              Lampu_2 = LOW;  digitalWrite(RELAY_2, LOW);
              myBot.sendMessage(id, "💡 Lampu 2 Telah Dinyalakan");
          } 
          else if (msg.callbackQueryData.equals("2off")) 
          {
              Lampu_2 = HIGH;  digitalWrite(RELAY_2, HIGH);
              myBot.sendMessage(id, "🚫 Lampu 2 Telah Dimatikan");
         } 
          else if (msg.callbackQueryData.equals("3on")) 
          {
              Lampu_3 = LOW;  digitalWrite(RELAY_3, LOW);
              myBot.sendMessage(id, "❄ Kipas Telah Dinyalakan");
         } 
          else if (msg.callbackQueryData.equals("3off")) 
          {
              Lampu_3 = HIGH;  digitalWrite(RELAY_3, HIGH);
              myBot.sendMessage(id, "🚫 Kipas Telah Dimatikan");
         } 
          else if (msg.callbackQueryData.equals("4on")) 
          {
              Lampu_4 = LOW;  digitalWrite(RELAY_4, LOW);
              myBot.sendMessage(id, "⚡ Charger Telah Dinyalakan");
          } 
          else if (msg.callbackQueryData.equals("4off")) 
          {
              Lampu_4 = HIGH;  digitalWrite(RELAY_4, HIGH);
              myBot.sendMessage(id, "🚫 Charger Telah Dimatikan");
         }
          else if (msg.callbackQueryData.equals("allon")) 
          {
              Lampu_1 = LOW; 
              Lampu_2 = LOW; 
              Lampu_3 = LOW; 
              Lampu_4 = LOW; 
              Lampu_5 = LOW; 
              Serial.println("allon");
               digitalWrite(RELAY_1, LOW);
               digitalWrite(RELAY_2, LOW);
               digitalWrite(RELAY_3, LOW);
               digitalWrite(RELAY_4, LOW);
              
              myBot.sendMessage(id, "Semua Perangkat Telah Dinyalakan");
          } 
          else if (msg.callbackQueryData.equals("alloff")) 
          {
              Lampu_1 = HIGH; 
              Lampu_2 = HIGH; 
              Lampu_3 = HIGH; 
              Lampu_4 = HIGH; 
              Lampu_5 = HIGH; 
              Serial.println("alloff");
               digitalWrite(RELAY_1, HIGH);
               digitalWrite(RELAY_2, HIGH);
               digitalWrite(RELAY_3, HIGH);
               digitalWrite(RELAY_4, HIGH);
              
              myBot.sendMessage(id, "🚫 Semua Perangkat Telah Dimatikan");
         } 
          else if (msg.callbackQueryData.equals("no")) 
          {
              myBot.sendMessage(id, "❌ Perintah Telah Dibatalkan!!!");
         }
        }
        else 
        {
          myBot.sendMessage(id, "⛔️ ERROR: Perintah tidak dikenal?!\n\nGunakan tombol dibawah untuk mengontrol lampu 👇", Tbl);
       }
}

}
