#ifndef server_h
#define server_h
//using namespace std;
//#include <String.h>

#include "Arduino.h"
//#include <string>


class Server
{
  private:
  	char* ssid;
  	char* password;
  	

	public:
  	Server();
  	Server(const char* ssid, const char* password); //the constructor
    void createHotSpot();
  	void joinWifiNetwork(const char* ssid, const char* password);
  	
  	String getHTML(String in1, String in2);
  	void handle_Client();
  	void handleRoot();
  	void handleSubmit();
  
  	void returnOK();
  	void returnFail(String msg);

};//end of class Dali

extern Server myServer;

#endif
