
#include "MySerial.h"
#include "Config.h"
#include "defines.h"

void MySerial::begin(void) {
  Serial.begin(115200);  
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


MySerial mySerial;
