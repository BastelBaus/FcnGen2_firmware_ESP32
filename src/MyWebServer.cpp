#include "Utils.h"
#include "MyWebServer.h"
#include "defines.h"

WebServer  webServer(WEB_PORT);

void initWebServer(void) {
  webServer.on("/", handleRoot);
//  server.on("/test.svg", drawGraph);
  //webServer.on("/t", demonstrateFrequencysEffectOnResolution );
  webServer.on("/x", []() { Serial.println("Rscv/x"); webServer.send(200, "text/plain", "this works as well"); });
  webServer.onNotFound(handleNotFound);
  webServer.begin();
}


void handleRoot() {
  Serial.println("Root");
  char temp[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, 400, "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP32 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP32!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <img src=\"/test.svg\" />\
  </body>\
</html>",

           hr, min % 60, sec % 60
          );
  webServer.send(200, "text/html", temp);
//  digitalWrite(led, 0);
}

void handleNotFound() {
//  digitalWrite(led, 1);
  Serial.println("NotFOund");
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += webServer.uri();
  message += "\nMethod: ";
  message += (webServer.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += webServer.args();
  message += "\n";

  for (uint8_t i = 0; i < webServer.args(); i++) {
    message += " " + webServer.argName(i) + ": " + webServer.arg(i) + "\n";
  }

  webServer.send(404, "text/plain", message);
}
