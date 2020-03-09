#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid ="*********";//the network name to connect to
const char* password ="*********";//the password to get a connection

ESP8266WebServer server(80);   //instantiere server at port 80.
//the html code
const char index_html[]PROGMEM = R"=====( 
<!DOCTYPE html>
<html>

<head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
        body {
            background-color: whitesmoke;
        }

        h2 {
            font-family: 1em Georgia, 'Times New Roman', Times, serif;
            text-align: center;
        }

        .button {
            margin: auto;
            text-decoration: none;
            padding: 50px;
            border: none;
            width: 100%;
            color: black;
            padding: 5px;
            text-align: center;
            border-radius: 4px;
            font-size: 24px;
            border: 1px solid black;
        }

        .stop {
            background-color: red;
            border: 1px;
            color: white;
            padding: 20px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            margin: 4px 2px;
            cursor: pointer;
            border: 1px solid black;
            border-radius: 50%;
        }

        .stop:active {
            box-shadow: 0 2px #666;
            transform: translateY(2px);
        }

        .button:active {
            box-shadow: 0 2px #666;
            transform: translateY(2px);
        }

        .grid-container {
            display: grid;
            grid-template-columns: auto auto auto;
            grid-gap: 5px;
            padding: 2px;
        }

        .grid-container>div {
            text-align: center;
            border-style: solid;
            border-color: black;
            padding: 20px 0;
            font-size: 25px;
        }

        .item1 {
            grid-column: 2 / 3;
            background-color: lightgreen;
        }

        .item2 {
            grid-column: 1 / 2;
            background-color: rgb(255, 255, 255);
        }

        .item3 {
            grid-column: 3/ 4;
            background-color: rgb(255, 255, 255);
        }

        .item4 {
            grid-column: 2 / 3;
            background-color: red;
            color: #ffffff;
        }

        .footer {
            position: fixed;
            left: 0;
            bottom: 0;
            width: 100%;
            background-color: gray;
            color: white;
            text-align: center;
        }
    </style>
</head>

<body>
    <h2>Om de robot te besturen klik je op de gewenste richting.</h2>
    <div class="grid-container">
        <a href="Forward" class="item1 button" target="_blank">Vooruit</a>
        <a href="Left" class="item2 button" target="_blank">Links</a>
        <a href="Right" class="item3 button" target="_blank">Rechts</a>
        <a href="Backward" class="item4 button" target="_blank">Achteruit</a>
    </div>
    <div>
        <a href="Stop" class="stop" target="_blank">Stop</a>
    </div>
    <div class="footer">
        <p>Project 2 Rescuebot &copy;</p>
    </div>
</body>

</html>
)=====";
//------------------------------------------------------------------------------------------------------------

void handleRoot() {// function for showing the website
  server.send_P(200, "text/html", index_html);
}

void handleNotFound() {//function for showing that page is not found
  digitalWrite(LED_BUILTIN, HIGH);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(LED_BUILTIN, LOW);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // For status of WiFi connection
 
  Serial.begin(115200);//baudrate
  WiFi.mode(WIFI_STA);// starts the connection over WiFi
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }//when connecected this code shows the ip adress in the serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }//the different pages with their functions to controll the robot's.
  server.on("/", handleRoot);
  server.on("/Forward", Forward);
    server.send_P(200, "text/html","vooruit");
  server.on("/Backward", Backward );
    server.send_P(200, "text/html", "achteruit");
  server.on("/Left", Left );
    server.send_P(200, "text/html", "links");
  server.on("/Right", Right );
    server.send_P(200, "text/html", "rechts");
  server.on("/Stop", Stop ); // turns all the motor input pins low
    server.send_P(200, "text/html", "gestopt");
  //test page...
  server.on("/inline", []() {
    server.send(200, "text/html", "this works as well");
  });
 
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}
//the functions with the bytes to send to the arduino.
void Forward()
{
  int richting = 1;
  Serial.write(richting);
  Serial.println("    vooruit");
}
void Left()
{
  int richting = 4;
  Serial.write(richting);
  Serial.println("    links");
}
void Right()
{
  int richting = 3 ;
  Serial.write(richting);
  Serial.println("    rechts");
}
void Backward()
{
  int richting = 2;
  Serial.write(richting);
  Serial.println("   achteruit");
}
void Stop()
{
  int richting = 0;
  Serial.write(richting);
  Serial.println("    STOP!!");
}

void loop() {
  server.handleClient();//
  MDNS.update();
}
