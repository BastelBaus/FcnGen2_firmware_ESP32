#ifndef __OUTPUTPARAMETER_H__
#define __OUTPUTPARAMETER_H__


#include <vector>
#include "MySerial.h"
#include "Utils.h"
#include "HWControl.h"


class Modifier {};
class NULLModifier  : Modifier { };
class AMmodifier    : Modifier { };
class FMmodifier    : Modifier { };
class BURSTmodifier : Modifier { };
// AMBust, FMBurst, ....


class ParameterRaw {
  public:
    ParameterRaw(String name,String unit, HWControl* hwc) : name(name), unit(unit), hwc(hwc) {};


    virtual void   set(String param) { };
    virtual void   reset(void) { };
    virtual String getAsString(void)           { return "none"; };

    virtual String getName(void) const { return name; };
    virtual String getUnit(void) const{ return unit; };
    virtual String getType(void) const{ return "none"; };


    virtual String getMaxAsString(void)  const { return "none"; };
    virtual String getMinAsString(void)  const { return "none"; };
    virtual String getStepAsString(void) const { return "none"; };

  protected:
     HWControl* hwc;

  private:
     String name;
     String unit;
};

template<class T>
class Parameter : public ParameterRaw {
  public:
    Parameter(String name,String unit, HWControl* hwc) : ParameterRaw(name,unit,hwc), par(0) {};

    virtual T getMax(void)  const { return 0; };
    virtual T getMin(void)  const { return 0; };
    virtual T getStep(void) const { return 0; };

    virtual String getMaxAsString(void)  const { return String(getMax()); };
    virtual String getMinAsString(void)  const { return String(getMin()); };
    virtual String getStepAsString(void) const { return String(getStep()); };

    virtual void   set(String param) = 0;
    virtual void   set(T param) { par = param; };
    virtual void   reset(void) { set(par); };
    virtual T      get(void) { return par; };
    virtual String getAsString(void) { return String(get()); };

    virtual String getType(void) const { return typeToString( (T) 0); };

  private:
    T par;
}; // template<class T> class Parameter {

class NoneParameter : public ParameterRaw {
  public:
    NoneParameter(void) : ParameterRaw("invalid","",NULL) {};
    virtual void set(String param) { ParameterRaw::set(param); };
};

class ParameterFloat : public Parameter<float> {
  public:
    ParameterFloat( String name,String unit, HWControl* hwc) : Parameter<float>(name,unit,hwc) {};
    virtual void set(String param) { this->set( param.toFloat() ); };

    using Parameter::set;
    
    //virtual float  get(void) { this->set( param.toFloat() ); };
    //virtual String get(void) { return this.}= 0;
};

class ParameterInt : public Parameter<uint32_t> {
  public:
    ParameterInt( String name,String unit, HWControl* hwc) : Parameter<uint32_t>(name,unit,hwc) {};

    virtual void set(String  param) { this->set( (uint32_t) param.toInt() ); };
    using Parameter::set;

    virtual uint32_t getStep(void) const { return 1; };
}; // class ParameterInt : public Parameter<uint32_t> {

class ParameterBool : public Parameter<bool> {
  public:
    ParameterBool( String name,String unit, HWControl* hwc) : Parameter<bool>(name,unit,hwc) {};

    virtual void set(String param) { this->set( param == "1"); };
    using Parameter::set;

    virtual bool getMax(void)  const { return 1; };
    virtual bool getMin(void)  const { return 0; };
    virtual bool getStep(void) const { return 1; };
}; // class ParameterBool : public Parameter<bool> {


class ParamAmplitudeLSB : public ParameterInt {
  public:
    ParamAmplitudeLSB( HWControl* hwc) :  ParameterInt("Amplitude","LSB",hwc) {};

    virtual void set(uint32_t  param) { Parameter::set(param); hwc->setGain(param); };
    using ParameterInt::set; // needed to handle in parallel to overloaded funtion

    virtual uint32_t getMax(void)  const { return 255; };

}; // class ParamAmplitudeLSB : public ParameterInt {

class ParamOffsetLSB : public ParameterInt {
  public:
    ParamOffsetLSB( HWControl* hwc) :  ParameterInt("Offset","LSB",hwc) {};

    virtual void set(uint32_t  param) { Parameter::set(param); hwc->setOffset(param); };
    using ParameterInt::set; // needed to handle in parallel to overloaded funtion

    virtual uint32_t getMax(void)  const { return 1024; };
}; // class ParamOffsetLSB : public ParameterInt {

class ParamOffsetStageSW : public ParameterBool {
  public:
    ParamOffsetStageSW( HWControl* hwc) :  ParameterBool("OffsetStageSW","",hwc) {};
    virtual void set(bool param) { Parameter::set(param); hwc->setOffsetStageSW(param); };
    using ParameterBool::set; // needed to handle in parallel to overloaded funtion
};

class ParamGainStageSW : public ParameterBool {
  public:
    ParamGainStageSW( HWControl* hwc) :  ParameterBool("GainStageSW","",hwc) {};
    virtual void set(bool param) { Parameter::set(param); hwc->setGainStageSW(param); };
    using ParameterBool::set; // needed to handle in parallel to overloaded funtion
};

class ParamAmplitudeV : public ParameterFloat {
  public:
    ParamAmplitudeV( HWControl* hwc) : ParameterFloat("Amplitude","V",hwc) {};
    void set(float param) {
      // conversion from out to gain setting
         //  A1 * val * gain1  * gain2 = out
         // val =out / (A1 * gain1(SW1) * gain2(SW2) )

         float gainStage2 = hwc->STAGE2_gain;
         float gainStage1 = (hwc->getGainStageSW() ? hwc->STAGE1_gain1 :  hwc->STAGE1_gain0);
         float amp = (hwc->isTriangle() ? hwc->AMP_tri : (hwc->isSquare() ? hwc->AMP_squ : hwc->AMP_sin));

         float val = param / ( amp *  gainStage1 * gainStage2);
         int   valSet = (uint8_t) round( val * 255.0 );
         mySerial.debug("Set Gain (V):",param);
         mySerial.debug("Set Gain (LSB):",valSet);
         hwc->setGain(valSet);
         Parameter::set(param);
    };
    using ParameterFloat::set; // needed to handle in parallel to overloaded funtion

    float getMax(void)  const { return 3; };
    float getMin(void)  const { return 0; };
    float getStep(void) const { return (getMax()-getMin())/256; };
};

class ParamOffsetV : public ParameterFloat {
  public:
    ParamOffsetV( HWControl* hwc) : ParameterFloat("Offset","V",hwc) {};
    void set(float param) { Parameter::set(param); hwc->setOffset(0); return; };
    using ParameterFloat::set; // needed to handle in parallel to overloaded funtion
};


#endif
