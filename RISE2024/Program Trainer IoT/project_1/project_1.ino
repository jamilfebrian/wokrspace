#include <ESP8266WiFi.h>

//mendeklarasikan variabel ssid dan password wifi
const char* ssid = "Tinkpad";
const char* password = "12345678";

//mendeklarasikan pin relay yang digunakan
#define Relay1 D1
#define Relay2 D2
#define Relay3 D3
#define Relay4 D4

// mendefinisikan pin saklar manual
#define Saklar1 D5
#define Saklar2 D6
#define Saklar3 D7
#define Saklar4 D8

//mendeklarasikan variabel 
WiFiServer server(80);
String header;

// deklarasi jeda waktu
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;


void setup() {
  Serial.begin(115200);
  pinMode(Relay1, OUTPUT);   // mendeklarasikan pin Relay sebagai OUTPUT
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  
  digitalWrite(Relay1, LOW); // inisialisasi pin Relay menjadi LOW
  digitalWrite(Relay2, LOW);
  digitalWrite(Relay3, LOW);
  digitalWrite(Relay4, LOW);

  pinMode(Saklar1, INPUT); // mendeklarasikan pin saklar sebagai INPUT
  pinMode(Saklar2, INPUT);
  pinMode(Saklar3, INPUT);
  pinMode(Saklar4, INPUT);

  connectWiFi();
}

void loop() {
  webConnection();
//  manualControl();
}

// fungsi menghubungkan nodeMCU dengan Internet Wifi
void connectWiFi(){
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

// fungsi kontrol relay manual menggunakan saklar 
void manualControl(){
  int saklarValue1 = digitalRead(Saklar1);
  int saklarValue2 = digitalRead(Saklar2);
  int saklarValue3 = digitalRead(Saklar3);
  int saklarValue4 = digitalRead(Saklar4);

  if(saklarValue1 == 0){
    digitalWrite(Relay1,LOW);
  } else if (saklarValue1 == 1){
    digitalWrite(Relay1,HIGH);
  }
  if(saklarValue2 == 0){
    digitalWrite(Relay2,LOW);
  } else if (saklarValue2 == 1){
    digitalWrite(Relay2,HIGH);
  }
  if(saklarValue3 == 0){
    digitalWrite(Relay3,LOW);
  } else if (saklarValue3 == 1){
    digitalWrite(Relay3,HIGH);
  }
  if(saklarValue4 == 0){
    digitalWrite(Relay4,LOW);
  } else if (saklarValue4 == 1){
    digitalWrite(Relay4,HIGH);
  }
}


// fungsi kontrol relay menggunakan web server
void webConnection(){
  WiFiClient client = server.available();

  if (client) {
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {

            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if (header.indexOf("/relay1on") > 0) {
              digitalWrite(Relay1, HIGH);
            }
            if (header.indexOf("/relay1off") > 0) {
              digitalWrite(Relay1, LOW);
            }
            if (header.indexOf("/relay2on") > 0) {
              digitalWrite(Relay2, HIGH);
            }
            if (header.indexOf("/relay2off") > 0) {
              digitalWrite(Relay2, LOW);
            }
            if (header.indexOf("/relay3on") > 0) {
              digitalWrite(Relay3, HIGH);
            }
            if (header.indexOf("/relay3off") > 0) {
              digitalWrite(Relay3, LOW);
            }
            if (header.indexOf("/relay4on") > 0) {
              digitalWrite(Relay4, HIGH);
            }
            if (header.indexOf("/relay4off") > 0) {
              digitalWrite(Relay4, LOW);
            }

            // membuat halaman tampilan website
            client.println ("<!DOCTYPE html> <html>\n");
            client.println ("<head><meta name=\"viewport\" content=\"width=device-width,initial-scale=1.0, user-scalable=no\">\n");
            client.println ("<title>Kontrol 4 Relay</title>\n");
            client.println ("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;color: #444444;}\n");
            client.println ("body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n");
            client.println (".button {display: block;width: 80px;background-color:#1abc9c;border: none;color: white;padding: 13px 30px;text-decoration:none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius:4px;}\n");
            client.println (".button-on {background-color: #1abc9c;}\n");
            client.println (".button-on:active {background-color: #16a085;}\n");
            client.println (".button-off {background-color: #34495e;}\n");
            client.println (".button-off:active {background-color: #2c3e50;}\n");
            client.println ("p {font-size: 14px;color: #888;margin-bottom: 10px;}\n");
            client.println ("</style>\n");
            client.println ("</head>\n");
            client.println ("<body>\n");
            client.println ("<h1>Kontrol 4 Relay Web Server (STA Mode)</h1>\n");
            client.println ("<h2>www.minangtechno.com</h2>\n");
            if (digitalRead(Relay1)) {
              client.println ("<p>Relay 1 Status: ON</p><a class=\"button button-off\" href=\"/relay1off\">OFF</a>\n");
            } else {
              client.println ("<p>Relay 1 Status: OFF</p><a class=\"button button-on\"href=\"/relay1on\">ON</a>\n");
            }
            if (digitalRead(Relay2)) {
              client.println ("<p>Relay 2 Status: ON</p><a class=\"button button-off\" href=\"/relay2off\">OFF</a>\n");
            } else {
              client.println ("<p>Relay 2 Status: OFF</p><a class=\"button button-on\" href=\"/relay2on\">ON</a>\n");
            }
            if (digitalRead(Relay3)) {
              client.println ("<p>Relay 3 Status: ON</p><a class=\"button button-off\" href=\"/relay3off\">OFF</a>\n");
            } else {
              client.println ("<p>Relay 3 Status: OFF</p><a class=\"button button-on\" href=\"/relay3on\">ON</a>\n");
            }
            if (digitalRead(Relay4)) {
              client.println ("<p>Relay 4 Status: ON</p><a class=\"button button-off\" href=\"/relay4off\">OFF</a>\n");
            } else {
              client.println ("<p>Relay 4 Status: OFF</p><a class=\"button button-on\" href=\"/relay4on\">ON</a>\n");
            }
            client.println ("</body>\n");
            client.println ("</html>\n");
            break;
          }
        }
      }
    }
    header = "";
    
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
