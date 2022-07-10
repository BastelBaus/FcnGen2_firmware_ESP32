// Note use Minimal SPIFFS with OTA !

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include "AiEsp32RotaryEncoder.h"
#include <Adafruit_ST7735.h>
#include <SPI.h>


#include "Config.h"
#include "controlTask.h"
#include "defines.h"
#include "HWControl.h"
#include "handleControls.h"
#include "MyWebServer.h"
#include "MyWiFi.h"
#include "MySerial.h"


//#include <BluetoothSerial.h>

// https://github.com/nkolban/ESP32_BLE_Arduino/tree/master/examples
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//BluetoothSerial SerialBT;




AiEsp32RotaryEncoder enc = AiEsp32RotaryEncoder(ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};



  

//const char* ssid = "BausOG";
//const char* password = "5u8bnGi6NVPcFre";



#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void testDisplay(uint8_t no);
void introLogo(uint8_t no);


#include <U8g2lib.h>
U8G2_SH1106_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, PIN_I2C_SCL, PIN_I2C_SDA, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

  
void IRAM_ATTR readEncoderISR() {
  enc.readEncoder_ISR();
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
    }
};



 
#include "Utils.h"
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
//Adafruit_ST7735 tft = Adafruit_ST7735(&spiClass, TFT_CS, TFT_DC, TFT_RST);


SPIClass mySPI; 
  // Note: never use SPI1 !

       
void setup() {

  
  mySerial.begin();
  mySerial.debug("Starting Setup");
  mySerial.debug("Runing on Core: ",String(xPortGetCoreID()));
  mySerial.debug("------ SETTING UP HW -------\n");

  mySerial.debug("configure SPI");
  mySPI.begin(PIN_SPI_SCK,PIN_SPI_MISO,PIN_SPI_MOSI);  

  mySerial.debug("start function generators");
  hwc1.begin(&mySPI);
  hwc2.begin(&mySPI);
  hwc1.setOutputMode(3);
  hwc2.setOutputMode(3);
  
  mySerial.debug("Loading configurations");
  conf.begin();
  conf.load();
  
  for(uint k=0; k<1;k++) {
    memcpy( conf.wlan[k].ssid, ssid[k],33);
    memcpy( conf.wlan[k].password, password[k],64);  
  }
  //conf.store();


// dodo: move all this in the loop to enable 
  mySerial.debug("Setting Wifi");
  connectWiFi();
  
  mySerial.debug("Setting Flash Over The Air");
  configureOTA();

  mySerial.debug("Setting up server");
  initWebServer();

  mySerial.debug("start socket server");
  initWiFi();
      
  mySerial.debug("starting control task loop");
  startControlTask();

  


  // initializeing I2C and look for devices
  Serial.println("Setting up I2C and looking for devices");
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);

  Serial.println("Initialising display");
  //u8g2.begin();
  //u8g2.firstPage();
  //do {
  //  u8g2.setFont(u8g2_font_ncenB14_tr);
  //  u8g2.drawStr(0,20,"Hello World!");
  //} while ( u8g2.nextPage() );
  //delay(1000);



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
  
  if(!display1.begin(SSD1306_SWITCHCAPVCC, LCD1_I2C)) { 
    Serial.println(F("... SSD1306 allocation failed (display 1)"));
  }
  if(!display2.begin(SSD1306_SWITCHCAPVCC, LCD2_I2C)) { 
    Serial.println(F("... SSD1306 allocation failed (display 2)"));
  }
  //testDisplay(0);
  //testDisplay(1);
  introLogo(0);
  introLogo(1);
  
  //findI2C();

  // setting up the input controls
  enc.begin();
  enc.setup(readEncoderISR);
  bool circleValues = false; //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
  enc.setBoundaries(0, 255, circleValues); 
  //enc.disableAcceleration(); //acceleration is now enabled by default - disable if you dont need it
  enc.setAcceleration(50); //or set the value - larger number = more accelearation; 0 or 1 means disabled acceleration

  
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
  Serial.print("------ SETTING done, starting main loop -------\n");
  
}


void rotary_loop() {
 //dont print anything unless value changed
  if (enc.encoderChanged()){
    Serial.print("Value: ");
    Serial.println(enc.readEncoder());
    //display.ssd1306_setContrast(enc.readEncoder());
    //display.ssd1306_command(SSD1306_SETCONTRAST);display.ssd1306_command(enc.readEncoder());
    
  }
  if (enc.isEncoderButtonClicked()) {
    Serial.println("Button Click");
    //rotary_onButtonClick();
  }
}
 

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


//uint32_t loop_count = 0;

void loop(void) { // main loop of core 1
  ArduinoOTA.handle();
  handleSerial();
  handleSocketClient();
  //handleBluetooth();
  //handleManualIO();
  
  //mySerial.debug("Loop main @core:", xPortGetCoreID());
  //if (SerialBT.available()) SerialBT.write('4');
  rotary_loop();
  myWDG.loop();

} // void loop(void) {





  
  //Serial.println("Test Screen");
  //spiClass.begin(TFT_SCK, -1, TFT_MOSI, TFT_CS);
  //tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
  //tft.fillScreen(COLOR2);
  //tft.setRotation(1); // landscape, set to 0,1,2 or 3
  
  // Let's draw a small table
  //tft.setTextWrap(false);
  //tft.setTextSize(2); 
  //tft.setTextColor(ST7735_BLUE);
  //tft.setCursor(0,0);
  //tft.print(" A*sin(f)+b");
//  tft.drawRect(0, 0, tft.width(), tft.height(), COLOR1);
//  tft.drawLine(0, 50, tft.width(), 50, COLOR1);
//  tft.drawLine(0, 100, tft.width(), 100, COLOR1);
//  tft.drawLine(0, 50, tft.width(), 50, COLOR1);
//  tft.setTextColor(ST7735_GREEN);
//  tft.setCursor(0,16);tft.print(" A*sin(f)+b");
//  tft.setTextColor(ST7735_WHITE);
//  tft.setCursor(0,32);tft.print(" A*sin(f)+b");
//
//  tft.setTextSize(1); 
//  tft.setCursor(0,64);tft.println(" Textsize 1");
//  tft.println(" Textsize 1");
