
#include "MySerial.h"
#include "Config.h"
#include "defines.h"
#include "handleControls.h"
#include <String>

void MySerial::begin(void) {
  Serial.begin(115200);
  inputBuffer.reserve(100);
  inputBuffer = "";
}


void MySerial::debug(String s1, String s2) {
  this->debug(s1+s2);
}
void MySerial::debug(String s1,int s2) {
  this->debug(s1+s2);
}
void MySerial::debug(String s) {
  Serial.print(SYM_DEBUG);
  Serial.print(":");
  Serial.println(s);
  Serial.flush();
}

void MySerial::error(String s1, String s2) {
  this->error(s1+s2);
}
void MySerial::error(String s1,int s2) {
  this->error(s1+s2);
}
void MySerial::error(String s) {
  Serial.print(SYM_ERROR);
  Serial.print(":!!");
  Serial.println(s);
  Serial.flush();
}

void MySerial::info(String s1, String s2) {
  this->info(s1+s2);
}
void MySerial::info(String s1,int s2) {
  this->info(s1+s2);
}
void MySerial::info(String s) {
  Serial.print(SYM_INFO);
  Serial.print(":>>");
  Serial.println(s);
  Serial.flush();
}

void MySerial::command(String s) {
  Serial.println(s);
  Serial.flush();
}


void handleSerial(void) { mySerial.handle(); };

void MySerial::handle(void) {
  while ( Serial.available() ) {
    char r = Serial.read();
    if ( (r == '\r') || (r == '\n') ) {
      if (inputBuffer.length() > 0) {
        mySerial.command(handleCommand(inputBuffer));
        inputBuffer = "";
      }
      return;
    }
    inputBuffer += r;
  } // while ( Serial.available() ) {
} // void handleSerial(void) {

MySerial mySerial;
