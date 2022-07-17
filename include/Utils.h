#ifndef __UTILS_H__
#define __UTILS_H__

#include <Arduino.h>
#include "defines.h"

void feedTheDogs();
void feedTheDog0();
void feedTheDog1();

void findI2C(void);

String splitString(String data, char separator, int index);


class myWatchdog {
  public:
    void loop(void);
    void set_looptime(uint32_t time_ms,bool set_on = true);
    void setOn();
    void setOff();

  private:
   uint32_t time_ms = DEFAULT_WATCHDOG_TIME;
   bool     state = false;
   int64_t  lastCall_us = 0;
   uint32_t loopCount = 0;
};

extern myWatchdog myWDG;



template<class T>  String typeToString(T a) ;
template<>         String typeToString(float a) ;
template<>         String typeToString(uint32_t a) ;
template<>         String typeToString(bool a) ;


/*template <> struct TypeName<float> {
    static const char* Get() { return "gloat"; } };
template <> struct TypeName<uint32_t> {
    static const char* Get() { return "uint32_t"; } };
template <> struct TypeName<bool> {
    static const char* Get() { return "bool"; } };
*/


#endif
