#include <NewPing.h>

#define A0 34
#define D0 35

#define motor1 4
#define motor2 2

#define TRIGGER_PIN  27  
#define ECHO_PIN     14  
#define MAX_DISTANCE 200

bool bukaPagar, tutupPagar;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);
  pinMode(D0, INPUT);

  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  
}

void loop() {
  int analogValue = analogRead(A0);
  int digitalValue = digitalRead(D0);
  int distance = sonar.ping_cm();

  bukaPagar = (analogValue > 1000) ? true : bukaPagar;
  tutupPagar = (distance <= 10 && distance != 0) ? true : tutupPagar;

  if(bukaPagar){
    digitalWrite(motor1, HIGH);
    digitalWrite(motor2, LOW);
    delay(500); 
    bukaPagar = false; 
  } else {
    digitalWrite(motor1, LOW);
    digitalWrite(motor2, LOW);
  }

  if(tutupPagar){
    digitalWrite(motor1, LOW);
    digitalWrite(motor2, HIGH);
    delay(500); 
    tutupPagar = false;
  } else {
    digitalWrite(motor1, LOW);
    digitalWrite(motor2, LOW);
  }

  Serial.print(analogValue); 
  Serial.print(" "); 
  Serial.print(digitalValue); 

  Serial.print(" | "); 
  Serial.println(distance); 
  delay(50);

}
