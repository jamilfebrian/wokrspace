#include <ESP8266WiFi.h>
#include <ThingerESP8266.h>

#define USERNAME "novani123"
#define DEVICE_ID "esp8266"
#define DEVICE_CREDENTIAL "S_KHvWqQX-ep0s&R"
#define WLAN_SSID "Oppo"
#define WLAN_PASS "novani19"

ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

unsigned long previousMillis = 0;
const long interval = 2000;
String arrData[2];

float v,h;

void setup() {
  Serial.begin(9600);
  delay(10);

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); digitalWrite(LED_BUILTIN, 1);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  pinMode (LED_BUILTIN, OUTPUT);
  digitalWrite (LED_BUILTIN, 0);
  thing.add_wifi(WLAN_SSID, WLAN_PASS);

  thing["PLTA_Prototype"] >> [](pson & out) {
    out["tegangan"] = v;
    out["ketinggian"] = h;
  };
}

void loop(){
  thing.handle();
  unsigned long currenMillis = millis();
  if(currenMillis - previousMillis >= interval){
    previousMillis = currenMillis;

    String data = "";
    while(Serial.available()>0){
      data += char(Serial.read());
    } data.trim();

    if (data != ""){
      int index = 0;
      for(int i=0; i<=data.length();i++){
        char delimiter = '#';
        if(data[i] != delimiter){
          arrData[index] += data[i];
        } else { 
          index++;
        }
      }
      
      if (index == 1){
        String data1 = arrData[0];
        String data2 = arrData[1];
         v = float(data1.toInt())/100; 
         h = data2.toInt();  

      }
      arrData[0] = "";
      arrData[1] = "";
    }
    Serial.println("YA");
  }
   
}
