#ifndef __OUTPUTMODE_H__
#define __OUTPUTMODE_H__


#include "HWCOntrol.h"


// Channel 0,1
  // Signal: *s*inus // *t*riangle // s*q*uare
  // Output: mode 0 (raw) // mode 1 2 3 4 
    // PARAM_i
      // id
      // Name
      // Unit
      // max / min // step 
      // type
  // modifiert: AM, FM, Burst

// raw :
  // setFrequency(uint32_t)
  // setGainPoti(uint8_t)
  // setOffset(uint16_t)
  // setOutputSW2Full(bool)
  // setGainSW2High(bool)

// mode 0..1:
  // setFrequency(float)
  // setAmplitute(float)
  // setOffset(float)

// mode 2..3:
  // setFrequency(float)
  // setAmplitute(float)


// float HWControl.getRawAmp()
// float HWControl.getRawOfs()
// float HWControl.getGain1a()
// float HWControl.getGain1b()
// float HWControl.getGain2()
// float HWControl.getOfs_a()
// float HWControl.getOfs_b()

class Modifier {};
class NULLModifier  : Modifier { };
class AMmodifier    : Modifier { };
class FMmodifier    : Modifier { };
class BURSTmodifier : Modifier { };
// AMBust, FMBurst, ....


typedef void setAmplitude(HWControl* hwc,float ofs);

typedef void (*paramFunction)(HWControl* ,float );

void setOffset_Mode1_Mode3(HWControl* hwc,float ofs);
void setAmplitude(HWControl* hwc,float ofs);

class Parameter {
  public:
    Parameter(const String& name,const String& unit, paramFunction fun) : name(name), unit(unit), fun(fun) {};
    String name;
    String unit; 
    var_t type;
    paramFunction fun;
    
    // range+
    // range -
    // step
};

Parameter Amp ("Amplitude","V",setAmplitude);
Parameter Ofs ("Offset","V",setOffset_Mode1_Mode3);
Parameter Freq("Frequency","Hz",setAmplitude);


class OutputMode {
  public:
    OutputMode(String name,HWControl* hwc);

    virtual void use(void) = 0;
    
    uint8_t getNumberOFParams(void);
    String  getParam(uint8_t n);


    // default fnctions to configure frequency and amplitude
        
    inline void loop(void) {};
    
  private:
    HWControl* hwc;
};

class OutputMode0 : public OutputMode  {
  public:
    void setFrequency(float freq);
    void setAmplitute(float amp);
    void setOffset(float ofs);
   
};

class OutputModeRaw : public OutputMode  {
  public:
  // setFrequency(uint32_t)
  // setGainPoti(uint8_t)
  // setOffset(uint16_t)
  
   void setOffsetStageSW(bool on = true) { hwc->setOffsetStageSW(on); };
   void setGainStageSW(bool on = true)   { hwc->setGainStageSW(on); };  
   
};

class OutputModeSin : public OutputMode  {
  public:
    OutputModeSin(String name,uint8_t channel, uint8_t SWsetting);


  private:
};

class OutputModeSinAM : public OutputModeSin{
  public:
    OutputModeSinAM(String name,uint8_t channel, uint8_t SWsetting);

  private:
};

// modifiert

extern outputMode outputModes[2][];

outputMode outputModes[2][] = 
  {
    { OutputModeSin("Sin[0V..0.6 @ 50mA]",0,0x00), OutputModeSin("Sin[-15V..15V @ 250mA]",0,0x01), OutputModeSinAM("SinAM[-15V..15V @ 250mA]",0,0x02), OutputModeSq("Square[0V..5V @ 50mA]",0,0x00),},
    { OutputModeSin("name",1,0x00), OutputModeSin("Power",1,0x01)}, 
  };
  

/*
 
 
typedef struct {
  // A*sin(2*pi*f) + O
  // a*LSB
  float AMP2LSB;
  float OFS2LSB_ofs;
  float OFS2LSB_gain;  // 
  float AMP1vsAMP2;    // gain difference of first stage
} channelSetting_t;

channelSetting_t channelSetting[2];



// the structure for a mode
typedef struct {
  char* name;
  
  parameterDefinition_t paramsList[];
} outputMode_t;

class OutputMode {
  public:
    OutputMode(String name,uint8_t channel, uint8_t SWsetting, signalmode_t signal);
    
    uint8_t getParamN(void);
    String  getParam(uint8_t n);
  private:
};

class OutputModeSin : public OutputMode  {
  public:
    OutputModeSin(String name,uint8_t channel, uint8_t SWsetting);

    void getParam(void);
  private:
};



extern const outputMode_t outputModeList[];

//channel setting: gain, ogain oofs
// signal setting: triangle, sin, square
// mode: name, n_params*( Name, unit,type, range+/range-,step)
const outputMode_t outputModeList[] = { "name", {"parName","unitName",FLOAT}, {"parName2","unitName2",FLOAT} };


*/
#endif
