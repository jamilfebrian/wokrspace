#define PIR_PIN 33         // PIR sensor pin
#define TRIG_PIN 19         // HC-SR04 Trigger pin
#define ECHO_PIN 18        // HC-SR04 Echo pin
#define MOTOR_PIN_LEFT 27  // Motor driver pin for left movement
#define MOTOR_PIN_RIGHT 26 // Motor driver pin for right movement

// Variables for ultrasonic sensor
long duration;
int distance;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set pin modes
  pinMode(PIR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(MOTOR_PIN_LEFT, OUTPUT);
  pinMode(MOTOR_PIN_RIGHT, OUTPUT);

    pinMode(13, OUTPUT);
    digitalWrite(13,HIGH);

  
  Serial.println("System Initialized.");
}

void loop() {
  // Check if motion is detected by the PIR sensor
  if (digitalRead(PIR_PIN) == HIGH) {
    Serial.println("Motion detected! Checking distance...");

    // Send trigger pulse to ultrasonic sensor
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Calculate distance based on echo pulse duration
    duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration * 0.034 / 2;  // Convert to cm
    
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Motor movement based on distance
    if (distance < 20) {
      // Object detected nearby, move motor to the left
      Serial.println("Object nearby - Moving motor left.");
      digitalWrite(MOTOR_PIN_LEFT, HIGH);
      digitalWrite(MOTOR_PIN_RIGHT, LOW);
    } else {
      // No close object, move motor to the right
      Serial.println("No object nearby - Moving motor right.");
      digitalWrite(MOTOR_PIN_LEFT, LOW);
      digitalWrite(MOTOR_PIN_RIGHT, HIGH);
    }
  } else {
    // Stop motor if no motion detected
    Serial.println("No motion detected - Motor stopped.");
    digitalWrite(MOTOR_PIN_LEFT, LOW);
    digitalWrite(MOTOR_PIN_RIGHT, LOW);
  }

  // Delay to avoid rapid triggering
  delay(500);
}
