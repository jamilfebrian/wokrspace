/* Author : M. Qolbi Al Zikri
 *  
 *  Program Ini merupakan program Bell pada TA Ridho, 
  jam getar sebagai isyarat utk mengetahui adanya tamu oleh 
  penderita tuna rungu/ gangguan pendengaran

  Sistem melalui komunikasi wireless melewati jaringan (ip address) yang sama dengan library WiFi
  
  */ 

// server

#include <WiFi.h>

const char* ssid = "POCOM3";                  // Your wifi Name       
const char* password = "bayudef1"; // Your wifi Password

const char * host = "192.168.194.94";        // IP Server

const int httpPort = 80;

const char* Commands;                       // The command variable that is sent to the server                     

int button = 15;                             // push button is connected
bool btn_press = true;                      // The variable to detect the button has been pressed
int con = 0;                                // Variables for mode

void setup() {
  // put your setup code here, to run once:
  pinMode(button, INPUT_PULLUP);            // initialize the pushbutton pin as an input:
  Serial.begin(115200);                     // initialize serial:

  Serial.println("");
  Serial.println("Client-------------------------------");
  Serial.print("Connecting to Network");
  WiFi.mode(WIFI_STA);                      // Mode Station
  WiFi.begin(ssid, password);               // Matching the SSID and Password
  delay(1000);

  // Waiting to connect to wifi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("Successfully Connecting");  
  Serial.println("Status : Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("-------------------------------------");
  Serial.println("");
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if (digitalRead(button) == LOW) {
    Serial.println("Client-------------------------------");
    Serial.print("Send Command = ");
    if (btn_press == true){
      if (con >= 2) {
        con=0;
      }
      con++;

      switch (con){
        case 1:
          Commands="LED_On";
          Serial.println(Commands);
          send_commands();
          break;
        case 2:
          Commands="LED_Off";
          Serial.println(Commands);
          send_commands();
          break; 
      }
      
      btn_press = false;
    }
  }
  else {
    btn_press = true;
  }
  delay(100);
}


void send_commands(){
  Serial.println("Sending command...");
  Serial.println("Don't press the button for now...");
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }

  // We now create a URI for the request  
  Serial.print("Requesting URL : ");
  Serial.println(Commands);

  // This will send the request to the server
  client.print(String("GET ") + Commands + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: Close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {      
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  Serial.print("Server Reply = "); 
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println("Now you can press the button ...");
  Serial.println("-------------------------------------");
  Serial.println("");
}
