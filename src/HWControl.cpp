
#include "HWControl.h"
#include "defines.h"
#include <Arduino.h>
#include "Config.h"
#include "MySerial.h"


HWControl hwc1(PIN_FCT_A_SW1, PIN_FCT_A_SW2, PIN_FCT_A_CS_GEN, PIN_FCT_A_CS_POTI, PIN_FCT_A_PWM,0);
HWControl hwc2(PIN_FCT_B_SW1, PIN_FCT_B_SW2, PIN_FCT_B_CS_GEN, PIN_FCT_B_CS_POTI, PIN_FCT_B_PWM,1);


HWControl::HWControl(uint8_t PIN_SW1, uint8_t PIN_SW2, uint8_t PIN_FCTGEN, uint8_t PIN_POTI, uint8_t PIN_PWM, uint8_t CHANNEL, SPIClass* spi) :
  PIN_SW1(PIN_SW1), PIN_SW2(PIN_SW2), PIN_POTI(PIN_POTI), PIN_PWM(PIN_PWM), CHANNEL(CHANNEL), gen(PIN_FCTGEN)
{
  if (spi != NULL) this->spi = spi;
  else this->spi = &SPI;

/*  // some PINs might be configures for JTAG. Put them as normal IO pins
  // see: https://www.esp32.com/viewtopic.php?t=2687
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[this->PIN_SW1], PIN_FUNC_GPIO);
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[this->PIN_SW2], PIN_FUNC_GPIO);
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[this->PIN_PWM], PIN_FUNC_GPIO);
*/
  
  pinMode(this->PIN_SW1, OUTPUT); digitalWrite(this->PIN_SW1, LOW);
  pinMode(this->PIN_SW2, OUTPUT); digitalWrite(this->PIN_SW2, LOW);
  
  ledcAttachPin(this->PIN_PWM, CHANNEL);
  ledcSetup(CHANNEL, PWM_FREQ , PWM_BITS);

  mySerial.debug(String("PIN: ") + String(this->PIN_SW1) + String(" configure to output") );
  mySerial.debug(String("PIN: ") + String(this->PIN_SW2) + String(" configure to output") );
  mySerial.debug(String("PIN: ") + String(this->PIN_PWM) + String(" configure to PWM output") );
}

void HWControl::begin( SPIClass* spi) {
  if (spi != NULL ) this->spi = spi;

  // Note: make shure to initialize here and not in constructur since other functions might wrongly initialized PINs
  //pinMode(this->PIN_SW1, OUTPUT); digitalWrite(this->PIN_SW1, LOW);
  //pinMode(this->PIN_SW2, OUTPUT); digitalWrite(this->PIN_SW2, LOW);

  // AD9833 function generator modul configuration
  gen.Begin(this->spi);
  gen.ApplySignal(SINE_WAVE, REG0, AD9833_INIT_FREQ);
  gen.EnableOutput(true);

  // gain poti configuration
  poti.begin(PIN_POTI, this->spi);
};


void HWControl::enableOutput(bool en) {
  gen.EnableOutput(en); 
}


void HWControl::setOffsetStageSW(bool on) {
  digitalWrite(this->PIN_SW1, (on ? HIGH : LOW));
  mySerial.debug(String("PIN: ") + String(this->PIN_SW1) + String(" to  ") + String( (on ? HIGH : LOW) ) );
}

void HWControl::setGainStageSW(bool on) {
  digitalWrite(this->PIN_SW2, (on ? HIGH : LOW));
  mySerial.debug(String("PIN: ") + String(this->PIN_SW2) + String(" to  ") + String( (on ? HIGH : LOW) ) );
}

   
void HWControl::setFrequency(uint32_t frequency) { 
  this->f = frequency;
  gen.SetFrequency ( REG0, this->f); 
  mySerial.debug(String("AD9833_frequency: ") + String(frequency) );
};

void HWControl::setGain(uint8_t gain) { 
  poti.setWiper( gain ); 
  mySerial.debug(String("AD9833_gain: ") + String(gain) );
};

void HWControl::setOffset(uint8_t offset) { 
  ledcWrite(CHANNEL, offset ); 
  mySerial.debug(String("PWM_offset: ") + String(offset) );
};
   
// toDo: AD9833 features only float frequency :-(
void HWControl::setSignalType(signaltype_t mode) {
  switch(mode) {
    case SIN:      mySerial.debug(String("AD9833_type: sin:") + String(mode) );
                   gen.ApplySignal(SINE_WAVE, REG0, f); 
                   this->mode = mode;
                   break;
    case TRIANGLE: mySerial.debug(String("AD9833_type: triangle:") + String(mode) );
                   gen.ApplySignal(TRIANGLE_WAVE, REG0, f); 
                   this->mode = mode;
                   break;
    case SQUARE:   mySerial.debug(String("AD9833_type: square:") + String(mode) );
                   gen.ApplySignal(SQUARE_WAVE, REG0, f); 
                   this->mode = mode;
                   break;
    default:       mySerial.debug(String("AD9833_type: ERROR:") + String(mode) );   
  } // switch(mode) {
}; // void HWControl::setSignalType(signaltype_t mode) {
  

/*

void HWControl::setMode(signalmode_t mode) {
  this->mode = mode;
  switch (mode) {
    case SIN:      gen.ApplySignal(SINE_WAVE, REG0, AD9833_INIT_FREQ); break;
    case TRIANGLE: gen.ApplySignal(TRIANGLE_WAVE, REG0, AD9833_INIT_FREQ); break;
    case SQUARE:   gen.ApplySignal(SQUARE_WAVE, REG0, AD9833_INIT_FREQ); break;
    default: return;
  }
}

void HWControl::setOutputMode(uint8_t output_mode) {
  this->output_mode = output_mode;
  switch (output_mode) {
    case 0:  this->setOffsetStage(false);
      this->setGainStage(false);
      break;
    case 1:  this->setOffsetStage(false);
      this->setGainStage(true);
      break;
    case 2:  this->setOffsetStage(true);
      this->setGainStage(false);
      break;
    case 3:  this->setOffsetStage(true);
      this->setGainStage(true);
      break;
    default: ; // do nothing
  }
  //  Mode    Name         Ofs   Amp    current  SW1   SW2
  //  0       FullRange    yes   15.0V  high     4     8
  //  1       MidRange     yes    3.0V  high     4     7
  //  2       SmallRange   -     -1.5V  low      3     8
  //  3       RawRange     -      0.3V  low      3     7
  // SW2: 7/8 schaltet first gani stage
  // SW1: 3/4 schaltet output select
}





void HWControl::setFrequency(float f, float f1 ) { // set frequencies in Hz
  this->f = f;
  if (f1 != NO_FREQUENCY_HZ) this->f2 = f1;
  gen.SetFrequency ( REG0, f );
}

uint16_t HWControl::amplitude2LSB(float amp) {
  return (uint16_t) round(amp * conf.channel[this->CHANNEL][this->output_mode].A2LSB);
}
uint16_t HWControl::offset2LSB(float ofs) {
  //return (uint16_t) round(ofs * conf.channel[this->CHANNEL][this->output_mode].O2LSB);
  return (uint16_t) round(ofs);
  return (uint16_t) round(( (float)(ofs - conf.channel[this->CHANNEL][this->output_mode].O2LSBofs ))* conf.channel[this->CHANNEL][this->output_mode].O2LSBgain);
}


#define MAX_LSB_GAIN 255
void HWControl::getAmplitudeLimits(float *Amin, float *Amax ) {
  *Amin = 0; *Amax = 0;
  float f = conf.channel[this->CHANNEL][this->output_mode].A2LSB;
  if (f==0) return; // ToDo add error message
  *Amax = MAX_LSB_GAIN/f;
  return;

}

void HWControl::getOffsetLimits(float *Omin, float *Omax ) {
  *Omin = 0; *Omax = 0;
  switch ( this->output_mode) {
    case 0: *Omax = 15.0; return;
    case 1: *Omax = 3.0; return;
    case 2: *Omax = 1.5; return;
    case 3: *Omax = 0.3; return;
  }
  // ToDO add failure handling
  return;
}

*/

/*  switch ( this->output_mode) {
    case 0: return (uint16_t) round(amp / 15.0 * 255.0);
    case 1: return (uint16_t) round(amp / 3.0 * 255.0);
    case 2: return (uint16_t) round(amp / 1.5 * 255.0);
    case 3: return (uint16_t) round(amp / 0.3 * 255.0);
  }
  // ToDO add failure handling
  return 0;
*/

/*
void HWControl::setAmplitude(float A, float A2 ) {
  uint16_t ampLSB = amplitude2LSB(A);
  poti.setWiper( ampLSB  );
  mySerial.debug(String("Channel:") + String(CHANNEL) + String(" setAmplitude:") + String(A) + "V ampLSB:" + String(ampLSB) );
}
void HWControl::setOffset(float o, float o2  ) {
  uint16_t ofsLSB = offset2LSB(o);
  ledcWrite(CHANNEL, ofsLSB );
  mySerial.debug(String("Channel:") + String(CHANNEL) + String(" setOffset:") + String(o) + "V ampLSB:" + String(ofsLSB) );
}

void HWControl::getFrequency(float *f, float *f1 ) { // set frequencies in Hz
}
void HWControl::getAmplitude(float *A, float *A2 ) { // set amplitude in V
}
void HWControl::getOffset(float *o, float *o2 ) {   // set offset in V
}

*/
