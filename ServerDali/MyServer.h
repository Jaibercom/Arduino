#ifndef my_server_h
#define my_server_h

#include "Arduino.h"
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>


class MyServer
{
  private:
  	char* ssid;
  	char* password;
  	

	public:
  	MyServer();
  	//MyServer(const char* ssid, const char* password); //the constructor
    void createHotSpot();
  	void joinWifiNetwork(const char* ssid, const char* password);
  	
  	//String getHTML(String in1, String in2);
    String getHTML();
    String getHTML(String in1, String in2);
  	void handle_Client();
  	//void handleRoot();
  	//void handleSubmit();
  
  	void returnOK();
  	void returnFail(String msg);

};//end of class Dali

#endif
