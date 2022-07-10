#include "handleControls.h"

#include "HWControl.h"
#include <Arduino.h>
#include "Utils.h"
#include "Config.h"
#include "MySerial.h"
String command = "";

/* commands 
 *  !x\n  : restart
 *    returns:  >reboot\n
 *  !y\n  : search I2C devices
 *    returns:  >0x3C,0x37\n (list of I2C addersses
 *  >1:*\n channel 1 
 *  >2:*\n channel 2
 *  >3:*\n both channels
 *     A?
 *     O?
 *     F?
 *     m?\n  mode (? = sin,square,triangle)
 *     o?\n  output mode (? = 0,1,2,3)
 *     e?\n  enable output (? = 0,1,empty)
 *     q?\n  query parameters (? = comma seperated list of maxf,minf, fo 3 alternating 1 and 2 channel for each value)
 *  $: configuration commands
 *  ?: status commands
 */


String readConfigCommand(String command) {
  String ret("");
  varInfo_t cValue;
  if( ! conf.readConfig(command,&cValue) ) return RESPONCE_ERR(ERROR_COMMAND_UNKNOWN,command);
  switch(cValue.type) {
    case FLOAT:    ret += String(conf.readFloat(cValue.id),10); break;
    case STRING:   ret += conf.readString(cValue.id); break;
    case INT:      ret += conf.readInt(cValue.id); break;
    default: return RESPONCE_ERR(ERROR_COMMAND_UNKNOWN,command);    
  }
  ret += ":";ret += cValue.name;ret += ":";ret += cValue.unit;ret += ":";ret += cValue.id;
  return RESPONCE_OK("",ret);
} // String readConfigCommand(String command) {

String writeConfigCommand(String command) {
  String key = splitString(command,':',0);
  String val = splitString(command,':',1); // .toFloat();
  varInfo_t cValue;
  if( ! conf.readConfig(key,&cValue) ) return RESPONCE_ERR(ERROR_COMMAND_UNKNOWN,command);

  String ret("");
  ret += cValue.id;
  ret += ":";
  
  switch(cValue.type) {
    case FLOAT:    conf.updateFloat(val.toFloat(),cValue.id); ret += val.toFloat(); break;
    case STRING:   ret += val; break;
    //case INT:      conf.updateInt(val.toInt(),cValue.id);break;
    default: return RESPONCE_ERR(ERROR_COMMAND_UNKNOWN,command);    
  }
  return RESPONCE_OK("",ret); 
 
} // String writeConfigCommand(String command) {



// "!x": restart ESP
// "!A": response with an alive message
// "!A:[t:uint32_t]": response every t ms with an alive message t>0
// "!A:0": turn of alive responce
// L[t:uint8_t]:  return list of configuration variables, 
//                  t=0 : channel 0
//                  t=1 : channel 1
//                  t=2 : generic
// D: return error and output info, debug and error messages
String controlCommand(String command) {
  switch ( command[0] ) {
    case 'x': ESP.restart(); 
              return RESPONCE_OK("x","doing reboot"); 
              break;
    case 'A': if ( command.length()>1 ) {
                uint32_t a = command.substring(1).toInt();
                myWDG.set_looptime(a); 
              }
              return RESPONCE_OK("A","alive"); 
              break;
    case 'L': { // return comma seperated list of configuration parameters
                String ret("");
                switch ( command[2] ) {
                  case '0': ret += conf.getConfigList(0); break;
                  case '1': ret += conf.getConfigList(1); break;
                  case '2': ret += conf.getConfigList(2); break;
                  default: return RESPONCE_ERR(ERROR_COMMAND_UNKNOWN,command);
                }
                return RESPONCE_OK("L",ret); 
              } break; 
    case 'D': mySerial.debug("This is a debug message");
              mySerial.info ("This is a info message");
              mySerial.error("This is a error message");
              return RESPONCE_ERR(ERROR_COMMAND_UNKNOWN,"forced error return"); 
              break;
    default:  break;
  } // switch (command[0]) {
  return RESPONCE_ERR(ERROR_COMMAND_UNKNOWN,command);
};


String channelCommand(String command) {
 
  // extract channel to be controlled
  uint8_t ch = 0;
  char chStr = command[0];
  switch(chStr) {
    case '1': ch=1; break;
    case '2': ch=2; break;
    case '3': ch=3; break;
    default: return RESPONCE_ERR(ERROR_COMMAND_UNKOWN_CHANNEL,command);
  }
  // check if command is given
  if (command[1] != ':')  return RESPONCE_ERR(ERROR_COMMAND_SYNTAX,command);
  if (command.length()<4) return RESPONCE_ERR(ERROR_COMMAND_INCOMPLETE,command);
  char code = command[2];
  String params = command.substring(4);
  String responce("");
  switch( code ) {
      case 'A': { float value = params.toFloat(); responce += code;responce += value; 
                  if((ch%2) == 1) { hwc1.setAmplitude(value); }
                  if((ch&2) == 2) { hwc2.setAmplitude(value); }   
                  responce+="A:";responce+=String(value);               
                } break; // case 'A': 
      case 'O': { float value = params.toFloat(); responce += code;responce += value;
                  if((ch%2) == 1) { hwc1.setOffset(value); }
                  if((ch&2) == 2) { hwc2.setOffset(value); }
                  responce+="O:";responce+=String(value);               
                } break; // case 'O': 
      case 'F': { float value = params.toFloat(); responce += code;responce += value;
                  if((ch%2) == 1) { hwc1.setFrequency(value); }
                  if((ch&2) == 2) { hwc2.setFrequency(value); }
                  responce+="F:";responce+=String(value);               
                } break; // case 'F': 
      case 'q': { // query limits
                  String list = params;
                  int i = 0;
                  while (true) {
                    String par=splitString(list,',',i);
                    
                    if (par == String("")) break;
                    if (i!=0) responce += ":";
                    //responce += par + "=";
                                        
                    if( par == "maxF")  {
                      float Fmin, Fmax;
                      if((ch%2) == 1) { hwc1.getFrequencyLimits(&Fmin,&Fmax); responce += Fmax; }
                      if(ch==3) responce += ":";
                      if((ch&2) == 2) { hwc2.getFrequencyLimits(&Fmin,&Fmax); responce += Fmax; }
                    } else if( par == "minF")  {
                      float Fmin, Fmax;
                      if((ch%2) == 1) { hwc1.getFrequencyLimits(&Fmin,&Fmax); responce += Fmin; }
                      if(ch==3) responce += ":";
                      if((ch&2) == 2) { hwc2.getFrequencyLimits(&Fmin,&Fmax); responce += Fmin; }
                    } else if( par == "maxA")  {
                      float Amin, Amax;
                      if((ch%2) == 1) { hwc1.getAmplitudeLimits(&Amin,&Amax); responce += Amax; }
                      if(ch==3) responce += ":";
                      if((ch&2) == 2) { hwc2.getAmplitudeLimits(&Amin,&Amax); responce += Amax; }
                    } else if( par == "minA")  {
                      float Amin, Amax;
                      if((ch%2) == 1) { hwc1.getAmplitudeLimits(&Amin,&Amax); responce += Amin; }
                      if(ch==3) responce += ":";
                      if((ch&2) == 2) { hwc2.getAmplitudeLimits(&Amin,&Amax); responce += Amin; }
                    } else if( par == "maxO")  {
                      float Omin, Omax;
                      if((ch%2) == 1) { hwc1.getOffsetLimits(&Omin,&Omax); responce += Omax; }
                      if(ch==3) responce += ":";
                      if((ch&2) == 2) { hwc2.getOffsetLimits(&Omin,&Omax); responce += Omax; }
                    } else if( par == "minO")  {
                      float Omin, Omax;
                      if((ch%2) == 1) { hwc1.getOffsetLimits(&Omin,&Omax); responce += Omin; }
                      if(ch==3) responce += ":";
                      if((ch&2) == 2) { hwc2.getOffsetLimits(&Omin,&Omax); responce += Omin; }
                    } else {
                      responce += "*";
                    }

                    i++;
                  }// while (true) {
                 }; break; // case 'q':                  
      case 'e': { bool en=true;
                  if (! (params.length()<1) ) {
                    switch(params[0]) {
                      case '0': en= false;  break;
                      case '1': en= true;  break;
                      default: return RESPONCE_ERR(ERROR_COMMAND_SYNTAX,command);
                    }
                  } // if (params.length()>=1) {
                  if((ch%2) == 1) hwc1.enableOutput(en);
                  if((ch&2) == 2) hwc2.enableOutput(en);
                }; break; // case 'e': 
      case 'm': { // chnage signal mode (sin, triangle, ...
                  String mo = params;      
                  Serial.println(mo);
                  signalmode_t m = NONE;
                  if      (mo == "sin")      m = SIN;
                  else if (mo == "triangle") m = TRIANGLE;
                  else if (mo == "square")   m = SQUARE;
                  else return RESPONCE_ERR(ERROR_COMMAND_SYNTAX,command);                  
                  if((ch%2) == 1) hwc1.setMode(m);
                  if((ch&2) == 2) hwc2.setMode(m);
                  responce += code;responce += String(m);
                } break; // case 'm': 
      case 'o': { // changed output mode
                  uint8_t m;
                  if (params.length()<1) return RESPONCE_ERR(ERROR_COMMAND_INCOMPLETE,command);
                  switch(params[0]) {
                    case '0': m = 0; break;
                    case '1': m = 1; break;
                    case '2': m = 2; break;
                    case '3': m = 3; break;
                    default: return RESPONCE_ERR(ERROR_COMMAND_UNKNOWN_MODE,command);
                  } // switch(command[4]) {
                  if((ch%2) == 1) hwc1.setOutputMode(m);
                  if((ch&2) == 2) hwc2.setOutputMode(m);
                  responce += code; responce += String(m);
                }; break; // case 'o':  
      default: return RESPONCE_ERR(ERROR_COMMAND_UNKNOWN,command);
    } // switch( command[3]) {
  return RESPONCE_OK(chStr,responce); 
} // String handleCommandNew(String command) {



String handleCommand(String command) {
 if (command.length()<2) return RESPONCE_ERR(ERROR_NO_COMMAND,command); 
 switch(command[0]) {
    case SYM_READ_CONFIG:  return String(SYM_READ_CONFIG)  + readConfigCommand( command.substring(2) );
    case SYM_WRITE_CONFIG: return String(SYM_WRITE_CONFIG) + writeConfigCommand( command.substring(2) );
    case SYM_COMMAND:      return String(SYM_COMMAND)      + controlCommand( command.substring(1) );
    case SYM_CHANNEL:      return String(SYM_CHANNEL)      + channelCommand( command.substring(1) );
    default: break;
 } // switch(command[0]) {
 return RESPONCE_ERR(ERROR_NO_COMMAND,command);
} // String handleCommand(String command) {

 

void handleSerial(void) {    
  while ( Serial.available() ) {
    char r = Serial.read();
    //Serial.println(r);
    if ( (r == '\r') || (r == '\n') ) {
      if (command.length() > 0) {
        mySerial.command(handleCommand(command));
        command = "";
      }
      return;
    }
    command += r;
  } // while ( Serial.available() ) {  
} // void handleSerial(void) {
