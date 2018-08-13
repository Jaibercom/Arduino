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
*/
/*
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
*/
#include "MyServer.h"

// Functions
void handleRoot();
void handleNotFound();
void handleSubmit();
void writeLED(bool LEDon);
/*
// Fill in your WiFi router SSID and password
const char* ssid = "MySSID2";
const char* password = "12345678";
*/
String in1 = "10";
String in2 = "2";


// GPIO#0 is for Adafruit ESP8266 HUZZAH board. Your board LED might be on 13.
const int LEDPIN = LED_BUILTIN;

extern MyServer myServer;
extern ESP8266WebServer server;
extern MDNSResponder mdns;


void setup(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  writeLED(true);

  Serial.begin(115200);
 
  myServer.joinWifiNetwork("VANESA", "master2020");
}


void loop(void)
{
  myServer.handle_Client();
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

void handleSubmit()
{
  if (!server.hasArg("IN1") || !server.hasArg("IN2")) 
    return myServer.returnFail("BAD ARGS");
  //String LEDvalue;
  in1 = server.arg("IN1");
  in2 = server.arg("IN2");

  Serial.println("IN1: " + in1);
  Serial.println("IN2: " + in2);

  writeLED(true);
  delay(500);
  writeLED(false);
}

void handleRoot()
{
  if (server.hasArg("IN1") || server.hasArg("IN2")) {
    //handleSubmit();
    in1 = server.arg("IN1");
    in2 = server.arg("IN2");

    Serial.println("IN1: " + in1);
    Serial.println("IN2: " + in2);

    writeLED(true);
    delay(500);
    writeLED(false);
  }
  server.send(200, "text/html", myServer.getHTML(in1, in2));
}

void writeLED(bool LEDon)
{
  // Note inverted logic for Adafruit HUZZAH board
  if (LEDon)
    digitalWrite(LEDPIN, 0);
  else
    digitalWrite(LEDPIN, 1);
}



