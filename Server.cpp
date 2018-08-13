#include "Server.h"
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


ESP8266WebServer server(80);
MDNSResponder mdns;

void Server::joinWifiNetwork(const char* ssid, const char* password){

//const char* ssid = "VANESA";
//  const char* password = "master2020";
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

void handleRoot()
{
  if (server.hasArg("IN1") || server.hasArg("IN2")) {
    handleSubmit();
  }
  server.send(200, "text/html", getHTML());
}

void Server::handleSubmit()
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

void Server::returnOK()
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "OK\r\n");
}

void Server::returnFail(String msg)
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(500, "text/plain", msg + "\r\n");
}

void Server::handle_Client()
{
  server.handleClient();
}

void Server::handleNotFound()
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

String Server::getHTML(String in1, String in2)
{
  String INDEX_HTML =
  "<!DOCTYPE HTML>"
  "<html>", 
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
