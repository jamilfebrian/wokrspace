
#include <Wire.h>
#include <Adafruit_SSD1306.h> // Memanggil Library OLED SSD1306
#include <WiFi.h>
#include <WiFiUDP.h>
#include <NTPClient.h>
#include <TimeLib.h>

#define SCREEN_WIDTH 128 // Lebar Oled dalam Pixel
#define SCREEN_HEIGHT 64 // Tinggi Oled dalam Pixel

#define OLED_RESET     4 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



WiFiUDP ntpUDP;
NTPClient timeClient (ntpUDP);

int last_second ;


#include <WiFi.h>

//int led = 15; // utk getar,awalnya led
const char *ssid = "POCOM3";
const char *password = "bayudef1";
//const char* ssid = "Raspberrypi";                  // Your wifi Name       
//const char* password = "12345678"; // Your wifi Password
const char* Commands_Reply;                 // The command variable that is sent to the client

const char * host = "192.168.194.28";          // IP Client


WiFiServer server(80);

void setup() {
  Serial.begin (115200);
  WiFi.begin (ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay (500);
    Serial.print (".");
  }

  timeClient.begin();
 
  Serial.begin (9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // alamat I2C 0x3C untuk 128x64
  Serial.println(F("SSD1306 Gagal"));
  for(;;); // mengulang terus, hingga bisa menghubungkan ke I2C Oled
  }
  
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);// Declare the LED builtin as an output
  pinMode (4, OUTPUT); //getaran
  pinMode (2, OUTPUT); //getaran
  Serial.begin(115200);                     // initialize serial:
  delay(10);

  Serial.println("");
  Serial.println("Server-------------------------------");
  Serial.print("Configuring access point");
  WiFi.begin(ssid, password);

  // Waiting to connect to wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    Serial.println("");
    Serial.println("WiFi connected");

  // Start the server
    server.begin();
    Serial.println("Server started");

  // Print the IP address
    Serial.print("Use this URL to connect: ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
    Serial.println("-------------------------------------");
    Serial.println("");
}

void loop() {
  timeClient.update();
// Serial.println (timeClient.getFormattedTime());
// Serial.println (timeClient.getEpochTime());

  unsigned long rawTime = timeClient.getEpochTime() + 25200; // gmt jakarta
  if (rawTime != last_second){
    time_t t = rawTime ;
    int jam = hour (t);
    String jamStr = jam < 10 ? "0" + String(jam) : String (jam);
    int menit = minute(t);
    String menitStr = menit < 10 ? "0" + String(menit) : String (menit);
    int detik = second (t);
    String detikStr = detik <10 ? "0" + String (detik) : String (detik);

    String hari;
    switch (weekday(t)){
      case 1 :
        hari = "Ahad";
        break;
      case 2 :
        hari = "senin";
        break;
      case 3 :
        hari = "selasa";
        break;
      case 4 :
        hari = "Rabu";
        break;
      case 5 :
        hari = "Kamis";
        break;
      case 6 :
        hari = "Jumat";
        break;
      case 7 :
        hari = "sabtu";
        break;
    }
    int tgl = day(t);
    String tglStr = tgl < 10 ? "0" + String(tgl) : String (tgl);
    int bln = month(t);
    String blnStr = bln <10 ? "0" + String(bln) : String (bln);
    int thn = year(t);
    String thnStr = String(thn);

    String tanggal = hari + " " + tglStr + "/" + blnStr + "/" + thnStr;
    String waktu = jamStr + ":" + menitStr + ":" + detikStr;

    display.clearDisplay();
    display.setTextSize(2.5);
    display.setTextColor(WHITE);
    display.setCursor (15,27);
    display.println (waktu);
    //display.println (tanggal);

    display.display();
    delay (1000);
    last_second = rawTime;
  }
 delay(500);
  
  // put your main code here, to run repeatedly:
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("Server-------------------------------");
  Serial.println("New client");
  Serial.print("From client = ");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request -------------------------------------
   String req = client.readStringUntil('\r');
   Serial.println(req);
   client.flush();

   //Command LED -------------------------------------------------------------

    if (req.indexOf("LED_On") != -1){
      Commands_Reply = "LED Status : On";
      Serial.print("Server send = ");
      Serial.println(Commands_Reply);
      client.print(String("GET ") + Commands_Reply + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
      digitalWrite(13, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(2, HIGH);
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor (15,27);
      display.println ("BEGETAR");
      display.display();
      delay (500);
    }
   else if (req.indexOf("LED_Off") != -1){
    Commands_Reply = "LED Status : Off";
    Serial.print("Server send = ");
      Serial.println(Commands_Reply);
      client.print(String("GET ") + Commands_Reply + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
      digitalWrite(13, LOW);
      digitalWrite (4,LOW);
      digitalWrite(2, LOW);
    }
   else {
     Serial.println("invalid request");
     client.stop();
     return;
    }

   client.flush();
   Serial.println("Client disonnected");
   Serial.println("-------------------------------------");
   Serial.println("");
}
