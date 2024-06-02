#define BUTTON 2

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON, INPUT_PULLUP);
}

void loop() {
  int buttonValue = digitalRead(BUTTON);

  if(!buttonValue){
    Serial.println("1");
    delay(1000);
  }
}
