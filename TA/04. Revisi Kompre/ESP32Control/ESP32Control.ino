#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

const char* ssid = "ESP32 Drone Controll";
const char* password = "123456789";

#define ledPin      2
#define buzzerPin  13
#define sprayer     5

bool SprayState = 0;
Adafruit_BMP280 bmp; 

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
    <title>Drone Control Web Server</title>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="icon" href="data:,">
    
    <style>
        html {
            font-family: Arial, Helvetica, sans-serif;
            text-align: center;
        }
        h1 {
            font-size: 1.5rem;
            color: white;
        }
        h2{
            font-size: 1.5rem;
            font-weight: bold;
            color: #143642;
        }
        body {
            margin: 0;
        }
        .topnav {
            overflow: hidden;
            background-color: #143642;
        }
        .content {
            padding: 30px;
            max-width: 600px;
            margin: 0 auto;
            display: block;
        }
        .card {
            background-color: #F8F7F9;;
            box-shadow: 2px 2px 12px 1px rgba(172, 159, 159, 0.5);
            padding-top:10px;
            padding-bottom:20px;
            border-radius: 20px;
        }
        .command {
            display: flex;
            gap: 10px;
            justify-content: center;
            margin-bottom: 10px;
            margin-top: 20px;
        }
        .command button {
            display: inline;
            background-color: #00e1f1;
            outline: auto;
            width: 120px;
            height: 50px;
            font-size: 20px;
            font-family: "Lucida Console", "Courier New", monospace;
        }
        .command button:hover{
            cursor: pointer;
            background-color: #0f8b8d;
        }
        .command button:active{
            box-shadow: 2 2px #CDCDCD;
            transform: translateY(2px);
            background-color: #00b0bd;
        }
        .mode{
            display: flex;
            justify-content: center;
            gap: 10px;
            margin-bottom: 5px;
            padding: 5px;
        }
        .mode form #mode{
            width: 200px;
            height: 30px;
            text-align: center;
        }
        .mode button , .takeoff button{
            box-sizing: auto;
            background-color: #00e1f1;
            outline: auto;
            width: 100px;
            height: auto;
            font-size: auto;
            font-family: "Lucida Console", "Courier New", monospace;
        }
        .mode button:hover, .takeoff button:hover{
            cursor: pointer;
            background-color: #0f8b8d;
        }
        .mode button:active, .takeoff button:active{
            box-shadow: 2 2px #CDCDCD;
            transform: translateY(2px);
            background-color: #00b0bd;
        }
        .takeoff{
            display: flex;
            justify-content: center;
            gap: 10px;
        }
        .takeoff form #Takeoff{
            width: 200px;
            height: 30px;
            text-align: center;
        }
        .yaw-button{
            display: flex;
            justify-content: center;
            gap: 70px;
            margin-bottom: 10px;
            margin-top: 10px;
        }
        .yaw-button button {
            width: 100px;
            height: 50px;
            font-size: 30px;
            border-radius: 20px;
            margin: 5px;
        }
        .yaw-button button:hover, .left-right button:hover, button.up:hover, button.down:hover{
            cursor: pointer;
            background-color: #c6feff;
        }
        .yaw-button button:active, .left-right button:active, button.up:active, button.down:active{
            box-shadow: 2 2px #CDCDCD;
            transform: translateY(2px);
            background-color: #85b1b4;
        }
        .stick button.up, .stick button.down{
            width: 60px;
            height: 60px;
            font-size: 30px;
            border-radius: 50px;
            text-align: center;
        }
        .left-right {
            display: flex;
            justify-content: center;
            gap: 50px;
        }
        .left-right button {
            width: 60px;
            height: 60px;
            font-size: 30px;
            border-radius: 50px;
        }
        .state {
            font-size: 1.5rem;
            color:#8c8c8c;
            font-weight: bold;
        }
        .sprayer {
            padding: 15px 50px;
            font-size: 24px;
            text-align: center;
            outline: none;
            color: #fff;
            background-color: #0f8b8d;
            border: none;
            border-radius: 10px;
            -webkit-touch-callout: none;
            -webkit-user-select: none;
            -khtml-user-select: none;
            -moz-user-select: none;
            -ms-user-select: none;
            user-select: none;
        }
        .sprayer:hover {
            background-color: #04d1d4;
            cursor: pointer;
        }

        .sprayer:active {
            background-color: #0f8b8d;
            box-shadow: 2 2px #CDCDCD;
            transform: translateY(2px);
        }
        .flight-status {
            display: grid;
            margin: auto;
            margin-top: 20px;
            background-color: #bcffc1;;
            box-shadow: 2px 2px 12px 1px rgba(172, 159, 159, 0.5);
            padding-top:10px;
            padding-bottom:20px;
            grid-template-columns: auto auto ;
            padding: 10px;
            gap: 10px;
            font-size: 20px;
            border-radius: 20px;
        }
        .flight-status div span {
            font-size: 30px;
        }
    </style>
</head>
<body>
    <div class="topnav">
        <h1 class="judul">Sistem Kontrol Penyemprotan Cairan Pestisida Otomatis Menggunakan Drone UAV Hexacopter</h1>
    </div>

    <div class="content">
        <div class="card">
            <div class="command">
                <button type="button" class="connect" id="connect">Connect</button>
                <button type="button" class="armed" id="armed">Armed</button>
            </div>
            <div class="mode">
                <form>
                    <select title="mode" id="mode" name="mode">
                        <option value="usa">GUIDED</option>
                        <option value="stabilize">STABILIZE</option>
                        <option value="auto">AUTO</option>
                        <option value="rtl">RTL</option>
                        <option value="loiter">LOITER</option>
                        <option value="land">LAND</option>
                    </select>
                </form>
                <button type="submit" id="setMode">Set Mode</button>
            </div>
            <div class="takeoff">
                <form>
                    <select title="takeoff" id="Takeoff" name="takeoff">
                        <option value="alt1">1</option>
                        <option value="alt2">2</option>
                        <option value="alt3">3</option>
                        <option value="alt4">4</option>
                        <option value="alt5">5</option>
                    </select>
                </form>
                <button type="submit" id="takeoff">Take Off</button>
            </div>
            <p style="font-family: 'Courier New', Courier, monospace;">Mode : <strong><span id="Modes">GUIDED</span></strong><br>Take Off : <span id="altitude">12</span></p>
            <div class="stick">
                <div class="yaw-button">
                    <button type="button" class="yaw-left" id="yaw-left" onmousedown=mousedown(this) onmouseup=mouseup(this) value="◄">◄</button>
                    <button type="button" class="yaw-right" id="yaw-right" onmousedown=mousedown(this) onmouseup=mouseup(this) value="►">►</button>
                </div>
                <button type="button" class="up" id="up" onmousedown=mousedown(this) onmouseup=mouseup(this) value="▲">▲</button>
                <div class="left-right">
                    <button type="button" class="left" id="left" onmousedown=mousedown(this) onmouseup=mouseup(this) value="◄">◄</button>
                    <button type="button" class="right" id="right" onmousedown=mousedown(this) onmouseup=mouseup(this) value="►">►</button>
                </div>
                <button type="button" class="down" id="down" on onmousedown=mousedown(this) onmouseup=mouseup(this) value="▼">▼</button>
            </div>

            <h2 class="gpio2">Output - GPIO 2</h2>
            <p class="state">State: <span id="state">%STATE%</span></p>
            <p><button type="button" id="sprayer" class="sprayer">Spray</button></p>
        </div>
        <div class="flight-status">
            <div>Altitude <br><span id="alt">%ALT%</span>m</div>
            <div>Ground Speed <br><span id="gs">%GS%</span>m/s</div>
            <div>Sprayer<br><span id="spray">%SPRAY%</span></div>
            <div>Yaw<br><span id="yaw">%YAW%</span>deg</div>
            <div>Temperature<br><span id="temp">%TEMP%</span>°C</div>
            <div>Pressure<br><span id="press">%PRESS%</span>hPa</div>
        </div>
    </div>

    <script>
        var gateway = `ws://${window.location.hostname}/ws`;
        var websocket;
        window.addEventListener('load', onLoad);
        
        function initWebSocket() {
            console.log('Trying to open a WebSocket connection...');
            websocket = new WebSocket(gateway);
            websocket.onopen    = onOpen;
            websocket.onclose   = onClose;
            websocket.onmessage = onMessage; // <-- add this line
        }
        function onOpen(event) {
            console.log('Connection opened');
        }
        function onClose(event) {
            console.log('Connection closed');
            setTimeout(initWebSocket, 2000);
        }
        function onMessage(event) {
            var state;
            var mydata = event.data;
            var myArray = JSON.parse(mydata);
            console.log(myArray);

            if(myArray[0] == "1"){
              state = "ON";
            } else {
              state = "OFF";
            }
            
            document.getElementById('state').innerHTML = state;
            document.getElementById('spray').innerHTML = state;
            document.getElementById('temp').innerHTML = myArray[1];
            document.getElementById('press').innerHTML = myArray[2];
            document.getElementById('alt').innerHTML = myArray[3];
            document.getElementById('gs').innerHTML = 67;
            document.getElementById('yaw').innerHTML = 90;
        }
        function onLoad(event) {
            initWebSocket();
            initButton();
        }
        function initButton() {
            document.getElementById('sprayer').addEventListener('click', toggle);
        }

        function toggle(){
            websocket.send('spray');
        }

        function mousedown(obj){
            obj.style.backgroundColor = "#dddddd";
            obj.style.textIndent = "-9999px"; 
        }
        function mouseup(obj){
            obj.style.backgroundColor = "#ffffff";
            obj.style.textIndent = "1px"; 
        }
    </script>
</body>
</html>
)rawliteral";

void buzzer(int index = 3, int timer = 80){
    for(int i=0; i<index; i++){
      digitalWrite(buzzerPin, HIGH);
      delay(timer);
      digitalWrite(buzzerPin, LOW);
      delay(timer);
  }
}

//void notifyClients() {
//  ws.textAll(String(SprayState));
//}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "spray") == 0) {
      SprayState = !SprayState;
      digitalWrite(sprayer, SprayState);
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      digitalWrite(ledPin, HIGH);  buzzer(3);
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      digitalWrite(ledPin, LOW);
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if (SprayState){
      return "ON";
    }
    else{
      return "OFF";
    }
  }
  return String();
}

void setup(){
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(sprayer, OUTPUT);
   buzzer(1);
  
  Serial.println();
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  initWebSocket();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  bool status = bmp.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
  
  server.begin();
}
 
void loop(){
  String temperature = String(bmp.readTemperature());
  String altitude = String(bmp.readAltitude(1013.25));
  String pressure = String(bmp.readPressure() / 100.0F);  //   [String(SprayState) ,  temperature , pressure, altitude]

  String dataKirim = String( "[\"" + String(SprayState) + "\",\"" + temperature + "\",\"" + pressure+ "\",\"" + altitude +"\"]");
  
  ws.textAll(dataKirim);
  ws.cleanupClients();
  delay(1000);
}
