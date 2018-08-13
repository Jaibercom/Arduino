/*
   EEPROM Write
   Stores values read from analog input 0 into the EEPROM.
   These values will stay in the EEPROM when the board is
   turned off and may be retrieved later by another sketch.
*/
#include <EEPROM.h>

void blink();

// the current address in the EEPROM (i.e. which byte
// we're going to write to next)
int addr = 0;
bool LEDon = true;
bool flag = true;

void setup() {
  EEPROM.begin(512);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);
}

void loop() {
  // need to divide by 4 because analog inputs range from
  // 0 to 1023 and each byte of the EEPROM can only hold a
  // value from 0 to 255.
  int val = addr+1;//analogRead(A0) / 4;

  // write the value to the appropriate byte of the EEPROM.
  // these values will remain there when the board is
  // turned off.
  EEPROM.write(addr, val);

  // advance to the next address.  there are 512 bytes in
  // the EEPROM, so go back to 0 when we hit 512.
  // save all changes to the flash.
  addr = addr + 1;
  if (addr == 100) {
    addr = 0;
    EEPROM.commit();
    digitalWrite(LED_BUILTIN, 0);
    flag = false;
  }

  delay(100);
  blink();
  
}

void blink(){

  if(flag){
    if (LEDon){
      digitalWrite(LED_BUILTIN, 0);
      LEDon = false;
    }
    else{
      digitalWrite(LED_BUILTIN, 1);
      LEDon = true;
    }
  }
}
