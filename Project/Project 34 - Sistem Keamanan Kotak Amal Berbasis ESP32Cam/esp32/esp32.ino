#include <Keypad.h>

#define LED_PIN  2 

char keys[4][4] = { {'1','2','3','A'},
                    {'4','5','6','B'},
                    {'7','8','9','C'},
                    {'*','0','#','D'}};
byte rowPins[4] = {13, 12, 14, 27}; // konektor baris ke pin ESP32
byte colPins[4] = {26, 25, 33, 32}; // konektor kolom ke pin ESP32

String validPassword = "1234";
static String password = "";

Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

void blink_led(int index){
    for(int i=0; i<index; i++){
      digitalWrite(LED_PIN, HIGH);
      delay(50);
      digitalWrite(LED_PIN, LOW);
      delay(50);
    }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

}

void loop() {
  char key = myKeypad.getKey();

  if (key != NO_KEY) {
      if(key != '#'){
        blink_led(1);
      }
      password += key;
      Serial.println(password);
      if(key == '*'){
          password.remove(password.length()-2);
      } else if(key == '#'){
          password.remove(password.length()-1);
          if(password == validPassword){
            blink_led(3);
            Serial.println("Kode Benar");
          } else {
            Serial.println("Kode Salah");
          }
          password = "";
      }
    }

}
