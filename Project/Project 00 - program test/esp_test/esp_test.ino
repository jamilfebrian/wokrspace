#define BLYNK_TEMPLATE_ID "TMPL6R6Zgjx6p"
#define BLYNK_TEMPLATE_NAME "Biodiagaster metana meter control"
#define BLYNK_AUTH_TOKEN "ufq-ukh8suzjr-a9XgfK-urLNl34Lc2I"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#define ssid "Tinkpad"
#define password "12345678"
#define token BLYNK_AUTH_TOKEN

WidgetLCD lcd(V9);

void setup (){
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Blynk.begin(token, ssid, password, "blynk.cloud", 80);

  lcd.clear();
  lcd.print(0,0,"Hello");
  lcd.print(4,1,"dunia");
}

void loop(){
  Blynk.run();
}
