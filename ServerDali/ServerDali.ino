/*
 * Demonstrate using an http server and an HTML form to control an LED.
 * The http server runs on the ESP8266.
 * https://gist.github.com/bbx10/5a2885a700f30af75fc5
 *
 * Connect to "http://esp8266WebForm.local" or "http://<IP address>"
 * to bring up an HTML form to control the LED connected GPIO#0. This works
 * for the Adafruit ESP8266 HUZZAH but the LED may be on a different pin on
 * other breakout boards.
 *
 * Imperatives to turn the LED on/off using a non-browser http client.
 * For example, using wget.
 * $ wget http://esp8266webform.local/ledon
 * $ wget http://esp8266webform.local/ledoff
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// Fill in your WiFi router SSID and password
const char* ssid = "MySSID2";
const char* password = "12345678";
MDNSResponder mdns;
String in1 = "10";
String in2 = "2";


void createHotSpot();
void joinWifiNetwork();
void returnOK();
void returnFail(String msg);
String getHTML();


ESP8266WebServer server(80);


// GPIO#0 is for Adafruit ESP8266 HUZZAH board. Your board LED might be on 13.
const int LEDPIN = LED_BUILTIN;


void handleRoot()
{
  if (server.hasArg("IN1") || server.hasArg("IN2")) {
    handleSubmit();
  }
  server.send(200, "text/html", getHTML());
}

void handleSubmit()
{

  if (!server.hasArg("IN1") || !server.hasArg("IN2")) 
    return returnFail("BAD ARGS");
  //String LEDvalue;
  in1 = server.arg("IN1");
  in2 = server.arg("IN2");

  Serial.println("IN1: " + in1);
  Serial.println("IN2: " + in2);

  writeLED(true);
  delay(500);
  writeLED(false);
  
}

void returnOK()
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "OK\r\n");
}

void returnFail(String msg)
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(500, "text/plain", msg + "\r\n");
}


void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void writeLED(bool LEDon)
{
  // Note inverted logic for Adafruit HUZZAH board
  if (LEDon)
    digitalWrite(LEDPIN, 0);
  else
    digitalWrite(LEDPIN, 1);
}

void setup(void)
{
  pinMode(LEDPIN, OUTPUT);
  writeLED(false);

  Serial.begin(115200);
 /* 
*/
  joinWifiNetwork();
}


void loop(void)
{
  server.handleClient();
}


void joinWifiNetwork(){

  const char* ssid = "VANESA";
  const char* password = "master2020";
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin("esp8266WebForm", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  //server.on("/ledon", handleLEDon);
  //server.on("/ledoff", handleLEDoff);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.print("Connect to http://");
  Serial.println(WiFi.localIP());
  
}

void createHotSpot(){
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.on("/", handleRoot);
  //server.on("/ledon", handleLEDon);
  //server.on("/ledoff", handleLEDoff);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.print("Connect to http://");
  Serial.println(myIP);
  
}

String getHTML(){

  String INDEX_HTML =
  "<!DOCTYPE HTML>"
  "<html>"
  "<head>"
  "<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\">"
  "<title>Botonera DALI</title>"
  "<style>"
  "\"body { background-color: #808080; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }\""
  "</style>"
  "</head>"
  "<body>"
  "<h1>Botonera DALI</h1>"
  "<FORM >"
  "<P>"
  "IN1:<BR>"
  "<INPUT type=\"number\" name=\"IN1\" value="+in1+" min=\"1\" max=\"16\"><BR><BR>"
  "IN2:<BR>"
  "<INPUT type=\"number\" name=\"IN2\" value="+in2+" min=\"1\" max=\"16\"><BR><BR>"
  "<INPUT type=\"submit\" value=\"Send\"> <INPUT type=\"reset\">"
  "</P>"
  "</FORM>"
  "</body>"
  "</html>";
  
  return INDEX_HTML;

}


