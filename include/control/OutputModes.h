#ifndef __OUTPUTMODES_H__
#define __OUTPUTMODES_H__


#include <vector>
#include <iostream>

#include "defines.h"
#include "MySerial.h"

#include "OutputParameter.h"
#include "HWControl.h"



class OutputMode : public std::vector<ParameterRaw*> {
  public:
    OutputMode(String name,HWControl* hwc, std::vector<ParameterRaw*> params) : std::vector<ParameterRaw*>{params}, name(name), hwc(hwc)  {};
    ~OutputMode(void) { /* ToDo delete alll parameters */ };

    String getName(void) const { return name; };

    virtual void enableOutput(bool en) { hwc->enableOutput(en); };

    // configure a mode
    virtual void start(void) = 0;
    // ToDo: need to set all parameters newly

    virtual std::vector<ParameterRaw*> paramList(void) { return *this; };

    virtual void setSignalType( signaltype_t signalType ) {
      hwc->setSignalType(signalType);
      refreshParameters();
      // ToDo: need to set all parameters newly
    };

    //uint8_t getNumberOfParams(void) const { return this->size(); };
    //void    setParam(uint8_t n,String value) { (*this)[n]->set(value); };
    //ParameterRaw   getParam(uint8_t n) const { return *((*this)[n]); };
    // todo: add handling of n > numberofParams

  private:
    String name;
    signaltype_t signalType;


  protected:
    HWControl* hwc;
    //std::vector<ParameterRaw*>     params;
    virtual void refreshParameters(void) { };
};


class OutputModeRaw : public OutputMode {
  public:
    OutputModeRaw(HWControl* hwc, std::vector<ParameterRaw*> params) : OutputMode("ModeRaw", hwc,params)  {};

    // configure a mode
    virtual void start(void) { mySerial.debug("start Mode Raw"); };
};


class OutputMode0 : public OutputMode {
  public:
    OutputMode0(HWControl* hwc, std::vector<ParameterRaw*> params) : OutputMode("Mode0", hwc,params)  {};

    // configure a mode
    virtual void start(void) {
      mySerial.debug("start Mode 0: OSW:0/GSW:0");
      hwc->setOffsetStageSW(false);
      hwc->setGainStageSW(false);
      refreshParameters();
    };
};

class OutputMode1 : public OutputMode {
  public:
    OutputMode1(HWControl* hwc, std::vector<ParameterRaw*> params) : OutputMode("Mode1", hwc,params)  {};

    // configure a mode
    virtual void start(void) {
      mySerial.debug("start Mode 1: OSW:1/GSW:0");
      hwc->setOffsetStageSW(true);
      hwc->setGainStageSW(false);
      refreshParameters();
    };
};

class OutputMode2 : public OutputMode {
  public:
    OutputMode2(HWControl* hwc, std::vector<ParameterRaw*> params) : OutputMode("Mode2", hwc,params)  {};

    // configure a mode
    virtual void start(void) {
      mySerial.debug("start Mode 2: OSW:0/GSW:1");
      hwc->setOffsetStageSW(false);
      hwc->setGainStageSW(true);
      refreshParameters();
    };
};

class OutputMode3 : public OutputMode {
  public:
    OutputMode3(HWControl* hwc, std::vector<ParameterRaw*> params) : OutputMode("Mode3", hwc,params)  {};

    // configure a mode
    virtual void start(void) {
      mySerial.debug("start Mode 3: OSW:1/GSW:1");
      hwc->setOffsetStageSW(true);
      hwc->setGainStageSW(true);
      refreshParameters();
    };
};


//!  Class which covers the complte settings of a channel and
//!  keeps track of teh currently choosen mode
/*!
  A more elaborate class description.
*/
class OutputConfiguration : public std::vector<OutputMode*> {
   public:
     OutputConfiguration(std::vector<OutputMode*> modes) : std::vector<OutputMode*> { modes }, n(0) {};

     /**
      * @brief   selects axplicitly the current mode (if n> #modes, rolls over)
      * @param   n: the number of the mode to be set
      * @return  none
      */
      void setMode(uint8_t n) {
        this->n = n % size();
        // Todo something!! because new mode was choosen
      };

      uint8_t getMode(void) {
        return this->n;
      };

     /**
      * @brief   returns the number of modes supported
      * @param   none
      * @return  the number of different modes supported
      */
     inline uint8_t count(void) { return size(); };

     // set the next mode, rolls from last mode to first mode
     void next(void) { n = (n+1)% size(); };

     // gets a pointer to the current set mode
     OutputMode* getCurrentModePtr(void) { return (*this)[n]; };
     OutputMode* operator ->() { return getCurrentModePtr(); };


     void debugOutput(void) {
       std::cout << "Current Modes \n";
       std::cout << " " << getCurrentModePtr()->getName().c_str() << "\n";
       std::cout << "All Modes \n";
        for(const OutputMode* const mode: *this) {
           std::cout << " " << mode->getName().c_str() << "\n";
           for(const ParameterRaw * const param: *mode) {
             std::cout << " -> " << param->getName().c_str() << "\n";
           }
         }
     }; // void debugOutput(void) {

  private:
     int n = 0;
};

extern OutputConfiguration outMode[];






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
