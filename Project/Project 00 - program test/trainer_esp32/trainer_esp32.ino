#define ENABLE_USER_AUTH
#define ENABLE_DATABASE

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <WiFiClientSecure.h>
#include <FirebaseClient.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_TEXT_SIZE 1 // OLED display height, in pixels

#define WIFI_SSID "Tinkpad"
#define WIFI_PASSWORD "12345678"

#define Web_API_KEY "AIzaSyCV8R7BeV5um7nVpoXwMclI6zZl1whJmsM"
#define DATABASE_URL "https://iot-project-1800-default-rtdb.firebaseio.com/"
#define USER_EMAIL "jamilfebrian68@gmail.com"
#define USER_PASS "@DarkStar68"


static String password = "";
static String cursorLCD = "";
unsigned long timerMode = 0;
static unsigned long prevMillis = 0, timerMillis = 500;

int ledPin = 2;
int lampuStatus, kipasStatus;

void processData(AsyncResult &aResult);
UserAuth user_auth(Web_API_KEY, USER_EMAIL, USER_PASS);

// Firebase components
FirebaseApp app;
WiFiClientSecure ssl_client;
using AsyncClient = AsyncClientClass;
AsyncClient aClient(ssl_client);
RealtimeDatabase Database;

// Timer variables for sending data every 10 seconds
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 10000; 

// Variables to send to the database
int intValue = 0;
float floatValue = 0.01;
String stringValue = "";

int x = 0;
char keys[4][4] = { {'1','2','3','A'},
                    {'4','5','6','B'},
                    {'7','8','9','C'},
                    {'*','0','#','D'}};
byte rowPins[4] = {13, 12, 14, 27}; // konektor baris ke pin ESP32
byte colPins[4] = {26, 25, 33, 32}; // konektor kolom ke pin ESP32

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

void connectWifi(){

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  
  // Configure SSL client
  ssl_client.setInsecure();
  ssl_client.setConnectionTimeout(1000);
  ssl_client.setHandshakeTimeout(5);

  initializeApp(aClient, app, getAuth(user_auth), processData, "🔐 authTask");
  app.getApp<RealtimeDatabase>(Database);
  Database.url(DATABASE_URL);
}

void readDatabase(){
  if(app.ready()){
    int dataLampu = Database.get<int>(aClient, "/lamp/status");
    lampuStatus = dataLampu;

    int dataKipas = Database.get<int>(aClient, "/fan/status");
    kipasStatus = dataKipas;
  }
}

void sendDatabase(){
  if (app.ready()){ 
      // Periodic data sending every 10 seconds
      unsigned long currentTime = millis();
      if (currentTime - lastSendTime >= sendInterval){

        lastSendTime = currentTime;
        stringValue = "value_" + String(currentTime);
        Database.set<String>(aClient, "/test/string", stringValue, processData, "RTDB_Send_String");
        Database.set<int>(aClient, "/test/int", intValue, processData, "RTDB_Send_Int");
        intValue++;

        floatValue = 0.01 + random (0,100);
        Database.set<float>(aClient, "/test/float", floatValue, processData, "RTDB_Send_Float");
      }
  }
}

void processData(AsyncResult &aResult) {
  if (!aResult.isResult())
    return;

  if (aResult.isEvent())
    Firebase.printf("Event task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.eventLog().message().c_str(), aResult.eventLog().code());

  if (aResult.isDebug())
    Firebase.printf("Debug task: %s, msg: %s\n", aResult.uid().c_str(), aResult.debug().c_str());

  if (aResult.isError())
    Firebase.printf("Error task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.error().message().c_str(), aResult.error().code());

  if (aResult.available())
    Firebase.printf("task: %s, payload: %s\n", aResult.uid().c_str(), aResult.c_str());
}

void passKey(){
  char key = myKeypad.getKey();

  if (key != NO_KEY) {
    
      if(key != '#'){
        // buzzer(1);
      } 

      timerMode = millis();
      password += key;

      if(key == '*'){
          password.remove(password.length()-2);
      } else if(key == '#'){
          password.remove(password.length()-1);
          password = "";
      }
    }
}

void printLCD(){
  if((millis() - prevMillis) > timerMillis){
        if(cursorLCD == "_"){
          cursorLCD = "";
        } else {
          cursorLCD = "_";
        } 
        prevMillis = millis();
      }

      display.setCursor(0, 0);
      display.println("Masukkan Password : ");
      display.print(password); display.print(cursorLCD);
      display.display(); 
      delay(50);
      display.clearDisplay();
}

void setup() {
  Serial.begin(115200);

  connectWifi();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  delay(1000);
  display.clearDisplay();
  display.setTextSize(OLED_TEXT_SIZE);
  display.setTextColor(WHITE);

  pinMode(ledPin, OUTPUT);
 
}

void loop() {
  passKey();
  printLCD();

  app.loop();

  readDatabase();
  // sendDatabase();
  
  Serial.print("Lampu: ");
  Serial.println(lampuStatus);

  Serial.print("Kipas: ");
  Serial.println(kipasStatus);

  if(lampuStatus){
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }


  // Check if authentication is ready
  
}