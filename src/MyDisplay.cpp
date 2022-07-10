
#include "MyDisplay.h"
#include <U8g2lib.h>


Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

U8G2_SH1106_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, PIN_I2C_SCL, PIN_I2C_SDA, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
//Adafruit_ST7735 tft = Adafruit_ST7735(&spiClass, TFT_CS, TFT_DC, TFT_RST);



//u8g2.begin();
//u8g2.firstPage();
//do {
//  u8g2.setFont(u8g2_font_ncenB14_tr);
//  u8g2.drawStr(0,20,"Hello World!");
//} while ( u8g2.nextPage() );
//delay(1000);

void introLogo(uint8_t no = 0){
  Adafruit_SSD1306 *disp;
  if (no==0) disp = &display1;
  else       disp = &display2;

  Serial.print("Bastel Baus Logo\n");
  disp->clearDisplay();
  disp->setTextWrap(false);
  disp->setTextSize(2); disp->setTextColor(WHITE);
  disp->setCursor(0,0);
  disp->println("BastelBaus");
  disp->display();
  //delay(2000); // Pause for 2 seconds
}

void testDisplay(uint8_t no) {
  Adafruit_SSD1306 *disp;
  if (no==0) disp = &display1;
  else       disp = &display2;

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  Serial.print("Initial Display Check\n");
  disp->display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  Serial.print("Bastel Baus Logo\n");
  disp->clearDisplay();
  disp->setTextWrap(false);
  disp->setTextSize(2); disp->setTextColor(WHITE);
  disp->setCursor(0,0);
  disp->println("BastelBaus");
  disp->display();
  delay(2000); // Pause for 2 seconds

  Serial.print("Parameter Menue\n");
  disp->clearDisplay();
  disp->setTextSize(1); disp->setTextColor(WHITE);
  disp->setCursor(0,0);
  disp->println(" A*sin(f)+b");
  disp->println(" A:  10.000mV");
  disp->println(" O:  10.000mV"); //mV // LSB
  disp->println(" F:  1MHz");
  disp->println("-- BastelBaus -- ");
  disp->display();
  delay(2000);

  return;

  disp->clearDisplay();
  disp->setTextSize(2); disp->setTextColor(WHITE);
  disp->setCursor(0,0);
  disp->println("O");
  disp->setTextSize(1);
  disp->setCursor(12,16);
  disp->println("O");
  disp->setCursor(18,24);
  disp->println("O");
  disp->display();
  delay(2000);

  disp->clearDisplay();
  disp->setTextSize(1); disp->setTextColor(WHITE);
  disp->setCursor(0,0);
  disp->println("2");
  disp->println("3");
  disp->setTextSize(2);
  disp->setCursor(1,4);
  disp->println("1");
  disp->display();

  return
  //display.dim(true);
  //display.ssd1306_command(SSD1306_SETCONTRAST);display.ssd1306_command(0x00);
  //display.ssd1306_setContrast(0x8f);

  // Draw a single pixel in white
  display1.clearDisplay();
  for(uint8_t x=0;x<128;x++) {
    display1.drawPixel(x, 0, WHITE);
    display1.drawPixel(x, 12, WHITE);
    display1.drawPixel(x, 31, WHITE);
    display1.drawPixel(x, 47, WHITE);
  }
  for(uint8_t y=0;y<128;y++) {
    display1.drawPixel(127, y, WHITE);
    display1.drawPixel(0, y, WHITE);
  }

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display1.display();
  //delay(2000);
}

void chkDislpay(void) {
u8g2_uint_t offset;
u8g2_uint_t width;
const char *text = "AZ-Delivery";
u8g2.begin();
u8g2.setFont(u8g2_font_logisoso32_tf);
width = u8g2.getUTF8Width(text);
u8g2.setFontMode(0);

for (int i = 0 ; i < 128 + width*3 ; i++ ){
  u8g2.firstPage();
  u8g2.setFont(u8g2_font_logisoso32_tf);
  u8g2.drawUTF8(128 - i, 48, text);
  u8g2.nextPage();
}
//u8g2.clearBuffer();
u8g2.setFont(u8x8_font_chroma48medium8_r);
//u8g2.drawString(0,1,"Hello World!");
//u8g2.drawString(0,0,"Line 0");
//u8g2.drawString(0,8,"Line 8");
//u8g2.drawString(0,9,"Line 9");
//u8g2.refreshDisplay();    // only required for SSD1606/7

} // void chkDislpay(void);
