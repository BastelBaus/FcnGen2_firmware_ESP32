#ifndef __MYSERIAL_H__
#define __MYSERIAL_H__

#include <Arduino.h>
#include <String>

void handleSerial(void);

class MySerial {
  public:
    void begin(void);

    void debug(String s);
    void debug(String s1,String s2);
    void debug(String s1,int s2);

    void error(String s);
    void error(String s1,String s2);
    void error(String s1,int s2);

    void info(String s);
    void info(String s1,String s2);
    void info(String s1,int s2);

    void command(String s);

    void handle(void);

  private:
    String inputBuffer;

};

extern MySerial mySerial;

#endif
