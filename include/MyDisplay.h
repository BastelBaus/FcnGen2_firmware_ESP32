#ifndef __MYDISPLAY_H__
#define __MYDISPLAY_H__

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>


void testDisplay(uint8_t no);
void introLogo(uint8_t no);

extern Adafruit_SSD1306 display1;
extern Adafruit_SSD1306 display2;

#endif
