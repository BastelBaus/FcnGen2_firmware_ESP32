
#include "defines.h"
#include "Utils.h"
#include <WiFi.h>
#include <ArduinoOTA.h>
#include "Config.h"
#include <Wire.h>
#include "mySerial.h"


/****************************************************************************/
/* Watchdog on the interfaces                                               */
/****************************************************************************/

myWatchdog myWDG;
void myWatchdog::loop(void) {
  if(state) {
    int64_t m = esp_timer_get_time();
    if( (m-lastCall_us) > (time_ms*1000) ) {
      mySerial.command(String("!a:alive:") + String(loopCount++) );
      lastCall_us = m;
    }
  }
}

void myWatchdog::setOn(void)  { state = true; loopCount=0;};
void myWatchdog::setOff(void) { state = false; };
void myWatchdog::set_looptime(uint32_t time_ms,bool set_on) {
  this->time_ms = time_ms;
  if (set_on) this->setOn();
  else this->setOff();
}
    


String splitString(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
      found++;
      strIndex[0] = strIndex[1]+1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

/****************************************************************************/
/* Watchdog on the interfaces                                               */
/****************************************************************************/


void findI2C(void) {
  byte error, address;
  int nDevices;
  Serial.println("Scanning I2C ...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) { Serial.print("0"); }
      Serial.println(address,HEX);
      nDevices++;
    } else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) { Serial.print("0");}
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else { Serial.println("done\n"); }
}

/// Source: https://forum.arduino.cc/t/esp32-a-better-way-than-vtaskdelay-to-get-around-watchdog-crash/596889/13
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"
void feedTheDog0(){
  // feed dog 0
  TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE; // write enable
  TIMERG0.wdt_feed=1;                       // feed dog
  TIMERG0.wdt_wprotect=0;                   // write protect
}
void feedTheDog1(){
  // feed dog 1
  TIMERG1.wdt_wprotect=TIMG_WDT_WKEY_VALUE; // write enable
  TIMERG1.wdt_feed=1;                       // feed dog
  TIMERG1.wdt_wprotect=0;                   // write protect
}

void feedTheDogs(){
  feedTheDog0();
  feedTheDog1();
}
