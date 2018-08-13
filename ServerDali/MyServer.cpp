#include "MyServer.h"

extern void writeLED(bool LEDon);
extern void handleRoot();
extern void handleNotFound();

MyServer myServer;
ESP8266WebServer server(80);
MDNSResponder mdns;

MyServer::MyServer(){}

void MyServer::joinWifiNetwork(const char* ssid, const char* password){

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

void MyServer::returnOK()
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "OK\r\n");
}

void MyServer::returnFail(String msg)
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(500, "text/plain", msg + "\r\n");
}

void MyServer::handle_Client()
{
  server.handleClient();
}


String MyServer::getHTML(String in1, String in2)
{
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

String MyServer::getHTML()
{
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
  "<INPUT type=\"number\" name=\"IN1\" value=1 min=\"1\" max=\"16\"><BR><BR>"
  "IN2:<BR>"
  "<INPUT type=\"number\" name=\"IN2\" value=2 min=\"1\" max=\"16\"><BR><BR>"
  "<INPUT type=\"submit\" value=\"Send\"> <INPUT type=\"reset\">"
  "</P>"
  "</FORM>"
  "</body>"
  "</html>";
  
  return INDEX_HTML;

}
