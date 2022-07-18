// Note use Minimal SPIFFS with OTA !

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Wire.h>

#include <Arduino.h>


#include <SPI.h>


#include "Config.h"
#include "controlTask.h"
#include "defines.h"
#include "control/HWControl.h"
#include "handleControls.h"
#include "MyWebServer.h"
#include "MyWiFi.h"
#include "MySerial.h"
#include "MyDisplay.h"
#include "Utils.h"
#include "HWIO.h"


// & 'C:\Program Files\doxygen\bin\doxygen.exe' .\Doxyfile

#include "control/OutputModes.h"







SPIClass mySPI;
  // Note: never use SPI1 in ESP32 since it is used for FLashUploard!




#include <iostream>

void setup() {


  mySerial.begin();
  mySerial.debug("------ STARTING SETUP -------\n");
  mySerial.debug("Runing on Core: ",String(xPortGetCoreID()));
  mySerial.debug("------ SETTING UP HW -------\n");

  //std::cout << " XXX : " << outMode[0]->getName().c_str() << "\n";
  //std::cout << " XXX : " << outMode[0]->paramList()[0]->getName().c_str() << "\n";
  //std::cout << " \n\nCH0 \n; outMode[0].debugOutput();
  //std::cout << " \n\nCH1 \n; outMode[1].debugOutput();



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

  // todo: move all this in the loop to enable
  mySerial.debug("Setting Wifi");
  connectWiFi();

  mySerial.debug("Setting Flash Over The Air");
  configureOTA();

  mySerial.debug("Setting up web server");
  initWebServer();

  mySerial.debug("start socket server");
  initWiFi();

  mySerial.debug("starting control task loop");
  startControlTask();

  // initializeing I2C and look for devices
  Serial.println("Setting up I2C");
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
  Serial.println("Seting up inputs");
  hwIO.setup();

  Serial.print("IP address: "); Serial.println(WiFi.localIP());
  Serial.print("------ SETTING done, starting main loop -------\n");

}

void loop(void) { // main loop of core 1
  ArduinoOTA.handle();
  handleSerial();
  handleSocketClient();
  //handleBluetooth();
  //handleManualIO();

  //mySerial.debug("Loop main @core:", xPortGetCoreID());
  //if (SerialBT.available()) SerialBT.write('4');
  hwIO.loop();

  myWDG.loop();

} // void loop(void) {
