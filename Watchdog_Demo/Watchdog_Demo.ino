#include <Ticker.h>

Ticker ticker;

volatile int watchdogCount = 5;

void ISRwatchdog(){
  watchdogCount--;
  if(watchdogCount == 0){
    //Serial.println();
    Serial.println("\nThe watchdog bites!!!");  
    ESP.reset();
  } 
  
}

void setup() {
  Serial.begin(9600);
  ticker.attach(0.2, ISRwatchdog);

}

void loop() {
  Serial.printf("Watchdog counter: %d\n", watchdogCount);  
  //watchdogCount = 5;
  delay(200);
}
