#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

IPAddress apIP(192, 168, 1, 1);
ESP8266WebServer webServer(80);

void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("DNSServer example");

  // simple HTTP server to see that DNS server is working
  webServer.onNotFound([]() {
    String message = "Hello World!\n\n";
    message += "URI: ";
    message += webServer.uri();

    webServer.send(200, "text/plain", message);
  });
  webServer.begin();
}

void loop() {
  webServer.handleClient();
}
