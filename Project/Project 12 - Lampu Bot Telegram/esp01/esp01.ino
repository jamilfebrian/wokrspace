#include <ESP8266WiFi.h>
#include "CTBot.h"

#define ssid "vivo 1819"
#define password "Nisa2227"
#define token "5675736417:AAGJAJ-WKl3ydoBihx2S7eb4OJYa1zTGvo0"
#define id 1151344444

CTBot myBot;
CTBotReplyKeyboard Tbl;
CTBotInlineKeyboard TbLp1on, TbLp2on, TbLp3on, TbLp4on, TbLp5on;
CTBotInlineKeyboard TbLp1off, TbLp2off, TbLp3off, TbLp4off, TbLp5off;
CTBotInlineKeyboard TbLpAllon, TbLpAlloff;

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
 
void setup(){
  Serial.begin(9600);  
  pinMode (LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); 
  }    digitalWrite(LED_BUILTIN, 0); delay(1000);
       digitalWrite(LED_BUILTIN, 1);
  while (!myBot.testConnection()){
        myBot.setTelegramToken(token);
        delay(100);
  } digitalWrite (LED_BUILTIN, 0);

           Tbl.addButton("Lampu 1");
           Tbl.addButton("Lampu 2");
           Tbl.addButton("Lampu 3");
           Tbl.addRow();
           Tbl.addButton("Lampu 4");
           Tbl.addButton("Lampu 5");
           Tbl.addRow();
           Tbl.addButton("Semua Lampu ON");
           Tbl.addButton("Semua Lampu OFF");
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

           TbLp5on.addButton("✅ Ya", "5on", CTBotKeyboardButtonQuery);
           TbLp5on.addButton("❌ Tidak", "no", CTBotKeyboardButtonQuery);

           TbLp1off.addButton("✅ Ya", "1off", CTBotKeyboardButtonQuery);
           TbLp1off.addButton("❌ Tidak", "no", CTBotKeyboardButtonQuery);
           
           TbLp2off.addButton("✅ Ya", "2off", CTBotKeyboardButtonQuery);
           TbLp2off.addButton("❌ Tidak", "no", CTBotKeyboardButtonQuery);

           TbLp3off.addButton("✅ Ya", "3off", CTBotKeyboardButtonQuery);
           TbLp3off.addButton("❌ Tidak", "no", CTBotKeyboardButtonQuery);

           TbLp4off.addButton("✅ Ya", "4off", CTBotKeyboardButtonQuery);
           TbLp4off.addButton("❌ Tidak", "no", CTBotKeyboardButtonQuery);

           TbLp5off.addButton("✅ Ya", "5off", CTBotKeyboardButtonQuery);
           TbLp5off.addButton("❌ Tidak", "no", CTBotKeyboardButtonQuery);
           
           TbLpAllon.addButton("✅ Ya", "allon", CTBotKeyboardButtonQuery);
           TbLpAllon.addButton("❌ Tidak", "no", CTBotKeyboardButtonQuery);

           TbLpAlloff.addButton("✅ Ya", "alloff", CTBotKeyboardButtonQuery);
           TbLpAlloff.addButton("❌ Tidak", "no", CTBotKeyboardButtonQuery);
}

void loop(){
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
      } else if (msg.text.equalsIgnoreCase("Lampu 3")) {
        if (Lampu_3 == LOW) {
            myBot.sendMessage(id, "Saat ini Lampu 3 dalam keadaan ON\nApakah anda ingin mematikan Lampu 3?", TbLp3off);
       } else {
            myBot.sendMessage(id, "Saat ini Lampu 3 dalam keadaan OFF\nApakah anda ingin menyalakan Lampu 3?", TbLp3on);
        }
      } else if (msg.text.equalsIgnoreCase("Lampu 4")) {
        if (Lampu_4 == LOW) {
            myBot.sendMessage(id, "Saat ini Lampu 4 dalam keadaan ON\nApakah anda ingin mematikan Lampu 4?", TbLp4off);
        } else {
            myBot.sendMessage(id, "Saat ini Lampu 4 dalam keadaan OFF\nApakah anda ingin menyalakan Lampu 4?", TbLp4on); 
        }
      } else if (msg.text.equalsIgnoreCase("Lampu 5")) {
        if (Lampu_5 == LOW) {
            myBot.sendMessage(id, "Saat ini Lampu 5 dalam keadaan ON\nApakah anda ingin mematikan Lampu 5?", TbLp5off);
        } else {
            myBot.sendMessage(id, "Saat ini Lampu 5 dalam keadaan OFF\nApakah anda ingin menyalakan Lampu 5?", TbLp5on); 
        }
      } else if (msg.text.equalsIgnoreCase("Semua Lampu ON")) {
            myBot.sendMessage(id, "Apakah anda ingin menyalakan Semua Lampu?", TbLpAllon);
      } else if (msg.text.equalsIgnoreCase("Semua Lampu OFF")) {
            myBot.sendMessage(id, "Apakah anda ingin mematikan Semua Lampu?", TbLpAlloff);
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
              Status_Lampu_3 = "Lampu 3 ON";
            } else {
              Status_Lampu_3 = "Lampu 3 OFF";
            }
            if (Lampu_4 == LOW) {
              Status_Lampu_4 = "Lampu 4 ON";
            } else {
              Status_Lampu_4 = "Lampu 4 OFF";
            }
            if (Lampu_5 == LOW) {
              Status_Lampu_5 = "Lampu 5 ON";
            } else {
              Status_Lampu_5 = "Lampu 5 OFF";
            }
       myBot.sendMessage(id, "Status saat ini : \n\n👉 " + Status_Lampu_1 + "\n\n👉 " + Status_Lampu_2 + "\n\n👉 " + Status_Lampu_3 + "\n\n👉 " + Status_Lampu_4 + "\n\n👉 " + Status_Lampu_5 + "\n\nGunakan tombol dibawah untuk mengontrol lampu 👇", Tbl);
      } else if (msg.messageType == CTBotMessageQuery) {
          if (msg.callbackQueryData.equals("1on")) 
          {
              Lampu_1 = LOW; Serial.println("1on");
              myBot.sendMessage(id, "💡 Lampu 1 Telah Dinyalakan");
          }
          else if (msg.callbackQueryData.equals("1off")) 
          {
              Lampu_1 = HIGH; Serial.println("1off");
              myBot.sendMessage(id, "🚫 Lampu 1 Telah Dimatikan");
          }
          else if (msg.callbackQueryData.equals("2on")) 
          {
              Lampu_2 = LOW; Serial.println("2on");
              myBot.sendMessage(id, "💡 Lampu 2 Telah Dinyalakan");
          } 
          else if (msg.callbackQueryData.equals("2off")) 
          {
              Lampu_2 = HIGH; Serial.println("2off");
              myBot.sendMessage(id, "🚫 Lampu 2 Telah Dimatikan");
         } 
          else if (msg.callbackQueryData.equals("3on")) 
          {
              Lampu_3 = LOW; Serial.println("3on");
              myBot.sendMessage(id, "💡 Lampu 3 Telah Dinyalakan");
         } 
          else if (msg.callbackQueryData.equals("3off")) 
          {
              Lampu_3 = HIGH; Serial.println("3off");
              myBot.sendMessage(id, "🚫 Lampu 3 Telah Dimatikan");
         } 
          else if (msg.callbackQueryData.equals("4on")) 
          {
              Lampu_4 = LOW; Serial.println("4on");
              myBot.sendMessage(id, "💡 Lampu 4 Telah Dinyalakan");
          } 
          else if (msg.callbackQueryData.equals("4off")) 
          {
              Lampu_4 = HIGH; Serial.println("4off");
              myBot.sendMessage(id, "🚫 Lampu 4 Telah Dimatikan");
         }
         else if (msg.callbackQueryData.equals("5on")) 
          {
              Lampu_5 = LOW; Serial.println("5on");
              myBot.sendMessage(id, "💡 Lampu 5 Telah Dinyalakan");
          } 
          else if (msg.callbackQueryData.equals("5off")) 
          {
              Lampu_5 = HIGH; Serial.println("5off");
              myBot.sendMessage(id, "🚫 Lampu 5 Telah Dimatikan");
         } 
          else if (msg.callbackQueryData.equals("allon")) 
          {
              Lampu_1 = LOW; 
              Lampu_2 = LOW; 
              Lampu_3 = LOW; 
              Lampu_4 = LOW; 
              Lampu_5 = LOW; 
              Serial.println("allon");
              
              myBot.sendMessage(id, "Semua 💡💡💡💡 Lampu Telah Dinyalakan");
          } 
          else if (msg.callbackQueryData.equals("alloff")) 
          {
              Lampu_1 = HIGH; 
              Lampu_2 = HIGH; 
              Lampu_3 = HIGH; 
              Lampu_4 = HIGH; 
              Lampu_5 = HIGH; 
              Serial.println("alloff");
              
              myBot.sendMessage(id, "🚫 Semua Lampu Telah Dimatikan");
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
