#include "handleControls.h"

#include "control/HWControl.h"
#include <Arduino.h>
#include "Utils.h"
#include "Config.h"
#include "defines.h"
#include "MySerial.h"
#include "control/OutputModes.h"



String readConfigCommand(String command) {
  String ret("");
  varInfo_t cValue;
  if( ! conf.readConfig(command,&cValue) ) return RESPONCE_ERR(ERROR_COMMAND_UNKNOWN,command);
  //switch(cValue.type) {
    //case FLOAT:    ret += String(conf.readFloat(cValue.id),10); break;
    //case STRING:   ret += conf.readString(cValue.id); break;
    //case INT:      ret += conf.readInt(cValue.id); break;
    //default: return RESPONCE_ERR(ERROR_COMMAND_UNKNOWN,command);
//  }
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
/*
  switch(cValue.type) {
    case FLOAT:    conf.updateFloat(val.toFloat(),cValue.id); ret += val.toFloat(); break;
    case STRING:   ret += val; break;
    //case INT:      conf.updateInt(val.toInt(),cValue.id);break;
    default: return RESPONCE_ERR(ERROR_COMMAND_UNKNOWN,command);
  }
  */
  return RESPONCE_OK("",ret);

} // String writeConfigCommand(String command) {




String controlCommand(String command) {
  switch ( command[0] ) {
    case 'x': // to do, sentz back message before restarting .... but here we do not know the interface, maybe to all ???
              ESP.restart();
              return RESPONCE_OK("x","doing reboot"); // not effectives
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

  // note: comand>0 ensued ???
  const char& chStr = command[0];
  uint8_t     ch    = (chStr=='1'?1:(chStr=='2'?2:3));

  // check if command is given
  if (command[1] != ':')  return RESPONCE_ERR(ERROR_COMMAND_SYNTAX,command);
  if (command.length()<3) return RESPONCE_ERR(ERROR_COMMAND_INCOMPLETE,command);
  const char& code = command[2];
  String params = (command.length()>3 ?  command.substring(4) : "");

  String responce(""); responce += code;
  switch( code ) {
      case 'A': { float value = params.toFloat(); responce += code;responce += value;
                  //if((ch%2) == 1) { hwc1.setAmplitude(value); }
                  //if((ch&2) == 2) { hwc2.setAmplitude(value); }
                  responce+="A:";responce+=String(value);
                } break; // case 'A':
      case 'O': { float value = params.toFloat(); responce += code;responce += value;
                  //if((ch%2) == 1) { hwc1.setOffset(value); }
                  //if((ch&2) == 2) { hwc2.setOffset(value); }
                  responce+="O:";responce+=String(value);
                } break; // case 'O':
      case 'F': { float value = params.toFloat(); responce += code;responce += value;
                  //if((ch%2) == 1) { hwc1.setFrequency(value); }
                  //if((ch&2) == 2) { hwc2.setFrequency(value); }
                  responce+="F:";responce+=String(value);
                } break; // case 'F':

      case 'e': { bool en = ( params.length()<1 || params[0] == '1' );
                  if IS_CHANNEL1(ch) outMode[0]->enableOutput(en);
                  if IS_CHANNEL2(ch) outMode[1]->enableOutput(en);
                  ADD_PARAMETER(responce, en);
                }; break; // case 'e':

    case 'c': // return number of modes available for all channels
              if IS_CHANNEL1(ch) ADD_PARAMETER(responce, outMode[0].count());
              if IS_CHANNEL2(ch) ADD_PARAMETER(responce, outMode[1].count());
              break; // case 'c': parameter _c_ount

    case 'n': { // get mode name
                if ( params.length() == 0 ) {
                  if IS_CHANNEL1(ch) ADD_PARAMETER(responce, outMode[0]->getName() );
                  if IS_CHANNEL2(ch) ADD_PARAMETER(responce, outMode[1]->getName() );
                } else {
                  uint8_t m = params.toInt();
                  uint8_t max[] = { outMode[0].count(), outMode[1].count() }   ;
                  if IS_CHANNEL1(ch) ADD_PARAMETER(responce, outMode[0][ (m<max[0]?m:0) ]->getName() );
                  if IS_CHANNEL2(ch) ADD_PARAMETER(responce, outMode[1][ (m<max[1]?m:0) ]->getName() );
                }
              } break;

     case 'p': { // get parameter properties
                String subCom = splitString(params,':',0);
                String subPar = splitString(params,':',1);
                int    n      = subPar.toInt();
                    // ToDO: error handling
                    // ToDO: multiple parameter at once
                ADD_PARAMETER(responce,subCom);
                if (subCom=="c") { // name
                  if IS_CHANNEL1(ch)  ADD_PARAMETER(responce,outMode[0]->paramList().size() );
                  if IS_CHANNEL2(ch)  ADD_PARAMETER(responce,outMode[1]->paramList().size() );
                } else if (subCom=="n") { // name
                    if IS_CHANNEL1(ch)  ADD_PARAMETER(responce,outMode[0]->paramList()[n]->getName() );
                    if IS_CHANNEL2(ch)  ADD_PARAMETER(responce,outMode[1]->paramList()[n]->getName() );
                  } else if (subCom=="u") { // unit
                    if IS_CHANNEL1(ch)  ADD_PARAMETER(responce,outMode[0]->paramList()[n]->getUnit() );
                    if IS_CHANNEL2(ch)  ADD_PARAMETER(responce,outMode[1]->paramList()[n]->getUnit() );
                  } else if (subCom=="t") { // type
                    if IS_CHANNEL1(ch)  ADD_PARAMETER(responce,outMode[0]->paramList()[n]->getType() );
                    if IS_CHANNEL2(ch)  ADD_PARAMETER(responce,outMode[1]->paramList()[n]->getType() );
                  } else if (subCom=="v") { // value
                    if IS_CHANNEL1(ch)  ADD_PARAMETER(responce,outMode[0]->paramList()[n]->getAsString() );
                    if IS_CHANNEL2(ch)  ADD_PARAMETER(responce,outMode[1]->paramList()[n]->getAsString() );
                  } else if (subCom=="a") { // max
                    if IS_CHANNEL1(ch)  ADD_PARAMETER(responce,outMode[0]->paramList()[n]->getMaxAsString() );
                    if IS_CHANNEL2(ch)  ADD_PARAMETER(responce,outMode[1]->paramList()[n]->getMaxAsString() );
                  } else if (subCom=="i") { // min
                    if IS_CHANNEL1(ch)  ADD_PARAMETER(responce,outMode[0]->paramList()[n]->getMinAsString() );
                    if IS_CHANNEL2(ch)  ADD_PARAMETER(responce,outMode[1]->paramList()[n]->getMinAsString() );
                  } else if (subCom=="s") { // step
                    if IS_CHANNEL1(ch)  ADD_PARAMETER(responce,outMode[0]->paramList()[n]->getStepAsString() );
                    if IS_CHANNEL2(ch)  ADD_PARAMETER(responce,outMode[1]->paramList()[n]->getStepAsString() );
                } else {
                  uint8_t id = subCom.toInt();
                  if IS_CHANNEL1(ch) outMode[0]->paramList()[id]->set(subPar);
                  if IS_CHANNEL2(ch) outMode[1]->paramList()[id]->set(subPar);
                    // ToDO: error handling
                    // return RESPONCE_ERR(ERROR_COMMAND_SYNTAX,command);
                    // ToDO: return value which is set afterwarsd ! ADD_PARAMETER(responce,outMode[0]
                }
            } break; // case 'p': // get parameter properties

      case 't': { // change signal mode (sin, triangle, ...
                  signaltype_t m = SIN;
                  if      (params == "s") m = SIN;
                  else if (params == "t") m = TRIANGLE;
                  else if (params == "q") m = SQUARE;
                  else return RESPONCE_ERR(ERROR_COMMAND_SYNTAX,command);
                  if IS_CHANNEL1(ch)  outMode[0]->setSignalType(m);
                  if IS_CHANNEL2(ch)  outMode[1]->setSignalType(m);
                  ADD_PARAMETER(responce,m);
                } break; // case 'm':

      case 'o': { // set or get output mode by id
                  if (params.length()<1) {
                    if IS_CHANNEL1(ch)  ADD_PARAMETER(responce,outMode[0].getMode());
                    if IS_CHANNEL2(ch)  ADD_PARAMETER(responce,outMode[1].getMode());
                  } else {
                    uint8_t m    = params.toInt(); // ToDo: add error handling: a) if string no int and b) if bigger than max value
                    if IS_CHANNEL1(ch)  {
                       uint8_t maxM = outMode[0].count(); m = (m<maxM?m:0);
                       outMode[0].setMode( m ); ADD_PARAMETER(responce,m);
                    }
                    if IS_CHANNEL2(ch)  {
                       uint8_t maxM = outMode[1].count(); m = (m<maxM?m:0);
                       outMode[1].setMode( m ); ADD_PARAMETER(responce,m);
                    }
                  }
                }; break; // case 'o':

      default: return RESPONCE_ERR(ERROR_COMMAND_UNKNOWN,command);
    } // switch( command[3]) {
  return RESPONCE_OK(chStr,responce); // todo: should be ch ??

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
