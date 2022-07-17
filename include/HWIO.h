
#ifndef __HWIO_H__
#define __HWIO_H__

#include <Arduino.h>
#include "AiEsp32RotaryEncoder.h"

class HWIO {
  public:
    void loop(void);
    void setup(void);

  private:
    static AiEsp32RotaryEncoder enc;
    static void IRAM_ATTR readEncoderISR(void);

};

extern HWIO hwIO;

#endif
