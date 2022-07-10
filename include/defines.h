
#ifndef __DEFINES_H__
#define __DEFINES_H__

#define HOSTNAME    "FcnGen2_esp32"
#define SOCKET_PORT   2000
#define WEB_PORT      80

#define DEFAULT_WATCHDOG_TIME 1000

#define PIN_FCT_A_SW1       12
#define PIN_FCT_A_SW2       13
#define PIN_FCT_A_CS_GEN    33
#define PIN_FCT_A_CS_POTI   32
#define PIN_FCT_A_PWM       14

#define PIN_FCT_B_SW1        2
#define PIN_FCT_B_SW2       15
#define PIN_FCT_B_CS_GEN     5
#define PIN_FCT_B_CS_POTI   19
#define PIN_FCT_B_PWM        4

#define PIN_SPI_SCK    18
#define PIN_SPI_MOSI   21
#define PIN_SPI_MISO   35 // note: PIN Unused does not work since then the default pin is used!!


#define PIN_I2C_SCL    22
#define PIN_I2C_SDA    23

#define LCD1_I2C        0x3C
#define LCD2_I2C        0x3D

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)


#define AD9833_INIT_FREQ    1000 // Hz

#define PWM_FREQ     40000
#define PWM_BITS        11  // bits // 10bit @80kHz // 11bits=2048
#define PWM_INIT_PWM   256  // of 2048 @ 11bit


#define ROTARY_ENCODER_A_PIN       39
#define ROTARY_ENCODER_B_PIN       36
#define ROTARY_ENCODER_BUTTON_PIN  34
#define ROTARY_ENCODER_VCC_PIN     -1
#define ROTARY_ENCODER_STEPS        4



#define SYM_DEBUG         '*'
#define SYM_INFO          '?'
#define SYM_ERROR         '#'

#define SYM_COMMAND       '!'
#define SYM_CHANNEL       '>'
#define SYM_READ_CONFIG   '$'
#define SYM_WRITE_CONFIG  '%'


#define ERROR_GENERAL                F("ERROR0: unknown error")
#define ERROR_NO_COMMAND             F("ERROR1: no command")
#define ERROR_COMMAND_INCOMPLETE     F("ERROR2: command incomplete")
#define ERROR_COMMAND_UNKNOWN        F("ERROR3: command unknown")
#define ERROR_COMMAND_SYNTAX         F("ERROR4: command syntax wrong")
#define ERROR_COMMAND_UNKOWN_CHANNEL F("ERROR5: unknown channel")
#define ERROR_COMMAND_UNKNOWN_MODE   F("ERROR6: unknown mode")
#define COMMAND_OK                   F("OK")



#define RESPONCE_OK(x,y)  ( String(x) + ":"  + COMMAND_OK + ":" + y )
#define RESPONCE_ERR(x,y)  ( String("E:") + String(x) + ":" + y )

#define RESPONCE(x,y) ( String( SYM_RESPONCE ) + x + ":" + y )
#define RESPONCE_ERROR(CODE,TEXT) ( String( COMMAND_OK ) )



#define TFT_MOSI        19
#define TFT_SCK         21
#define TFT_RST        17
#define TFT_DC         16
#define TFT_CS         2



#define COLOR1 ST7735_WHITE
#define COLOR2 ST7735_BLACK
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

/*
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };
*/

#endif
