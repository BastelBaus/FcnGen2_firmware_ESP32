
#include "MyWiFi.h"

#include <WiFi.h>
#include <ArduinoOTA.h>
//#include <ESPmDNS.h>
//#include <WiFiUdp.h>
#include "defines.h"
#include "Config.h"
#include "handleControls.h"
#include "MySerial.h"

#define RETRIES_PER_NETWORK 10

const char* ssid[] = {};
const char* password[] = {};

WiFiServer socketServer(SOCKET_PORT);
WiFiClient client;

void initWiFi(void) {
  socketServer.begin();
}

// rm pwd

void connectWiFi(void) {
  WiFi.mode(WIFI_STA);

  uint8_t N = sizeof(ssid) / sizeof (char*);
  if (N==0) return;
  uint8_t n = 0;
  do {
    mySerial.debug("Trying Network #" + String(n) + " of " + String(N) + " SID:" + String(ssid[n]) + " pass:" + String(password[n]) );
    WiFi.begin( (char*)  ssid[n], (char*) password[n]);
    uint8_t t = RETRIES_PER_NETWORK;
    while ( (WiFi.status() != WL_CONNECTED) && (t>0)) {
      delay(500);
      mySerial.debug(" ... retry");
      //Serial.print(".");Serial.flush();
      t--;
    }
    //Serial.println("");
    n=(n+1)%N;
  } while (WiFi.status() != WL_CONNECTED);

  mySerial.info(" Successfully connected, IP:", WiFi.localIP().toString() );
} // void connectWiFi(void) {


uint8_t lastProg=200;

void configureOTA(void) {
  ArduinoOTA.setHostname(HOSTNAME); // Hostname defaults to esp3232-[MAC]
  //ArduinoOTA.setPort(3232); // Port defaults to 3232

  ArduinoOTA.onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) type = "sketch";
      else type = "filesystem"; // U_SPIFFS
      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      mySerial.info("Start updating " + type);
    })
    .onEnd([]() { Serial.println("\nEnd"); })
    .onProgress([](unsigned int progress, unsigned int total) {
      uint8_t prog = (progress / (total / 100));
      if( prog != lastProg) {
        char buf[100]; sprintf(buf, "Progress: %u%%\r\n", prog);
        mySerial.debug(buf);
        lastProg = prog;
      }
    })
    .onError([](ota_error_t error) {
      mySerial.error("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR)         mySerial.error("Auth Failed");
      else if (error == OTA_BEGIN_ERROR)   mySerial.error("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) mySerial.error("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) mySerial.error("Receive Failed");
      else if (error == OTA_END_ERROR)     mySerial.error("End Failed");
    });

  ArduinoOTA.begin();
  mySerial.info("OTA activated");
}




void handleSocketClient(void) {
  static String recvBuf;
  if (!client) {
    client = socketServer.available();
    if(client) {
      Serial.print("Client connected: "); Serial.println(client.remoteIP() ) ;
      //client.write("Welcome customer: " + client.remoteIP());
      client.write("Welcome customer\n");
    }
    return;
  } // if (!client) {
  while (client.available()>0) {
    char c = client.read();
    //Serial.write(c); Serial.flush();
    if (c!='\n') recvBuf += c;
    else {
      String response = handleCommand(recvBuf);
      client.write(response.c_str());
      Serial.print("Processed command: ");Serial.print(recvBuf);
      Serial.print(" --> ");Serial.println(response);
      Serial.flush();
      recvBuf = "";
    }
  } // while (client.available()>0) {

//      WiFiClient nextClient = socketServer.available();
//      if(nextClient) {
//        Serial.println("Refusing incomming connection from" + nextClient.remoteIP());
//        nextClient.write("Server busy, quitting");
//        nextClient.stop();
//      }

} // void handleSocketClient(void) {
