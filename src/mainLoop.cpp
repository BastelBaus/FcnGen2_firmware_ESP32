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
#include "MyDisplay.h"
#include "Utils.h"

// & 'C:\Program Files\doxygen\bin\doxygen.exe' .\Doxyfile



AiEsp32RotaryEncoder enc = AiEsp32RotaryEncoder(ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);

void IRAM_ATTR readEncoderISR() {
  enc.readEncoder_ISR();
}



//const char* ssid = "BausOG";
//const char* password = "5u8bnGi6NVPcFre";



SPIClass mySPI;
  // Note: never use SPI1 in ESP32 since it is used for FLashUploard!


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
  //hwc1.setOutputMode(3);
  //hwc2.setOutputMode(3);

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
