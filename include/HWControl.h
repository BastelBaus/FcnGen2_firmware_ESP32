
#ifndef __HWCONTROL_H__
#define __HWCONTROL_H__

#include <SPI.h>
#include <AD9833.h>     // Include the library
#include "MCP41_Simple.h"

#include "defines.h"

#define MAX_OFFSET_V +12
#define MIN_OFFSET_V -12
#define NO_OFFSET_V  -99

#define MAX_FREQUENCY_HZ 4e6
#define MIN_FREQUENCY_HZ   0
#define NO_FREQUENCY_HZ   -1

#define MAX_AMPLITUDE_V  12
#define MIN_AMPLITUDE_V   0
#define NO_AMPLITUDE_V   -1


typedef enum  {SIN=0,TRIANGLE=1,SQUARE=2 } signaltype_t ;


// definitions of the output signal at the 
// output with different settings. All at 1Hz.
//typedef struct {
  // A*sin(2*pi*f) + O
  // a*LSB
//  float AMP2LSB;       // for SW0=SW1=0 (i.e. 0.0 .. 0.6V
//  float OFS2LSB_ofs;
//  float OFS2LSB_gain;  // 
//  float AMP1vsAMP2;    // gain difference of amplitude with 
//} channelSetting_t;


struct {
  float STAGE1_gain0;
  float STAGE1_gain1;
  float STAGE2_gain;
  float OFS_Vat0LSB;
  float OFS_LSB2V;
  float AMP_sin;
  float AMP_tri;
  float AMP_squ;
  float OFS_sin;
  float OFS_tri;
  float OFS_squ;
} channelConfig[2] = { {1,1,1,0,1,0.3,0.3,5,0.3,0.3,2.5},
                       {1,1,1,0,1,0.3,0.3,5,0.3,0.3,2.5},
                     };

typedef struct {
  float STAGE1_gain0;
  float STAGE1_gain1;
  float STAGE2_gain;
  float OFS_Vat0LSB;
  float OFS_LSB2V;
  float AMP_sin;
  float AMP_tri;
  float AMP_squ;
  float OFS_sin;
  float OFS_tri;
  float OFS_squ;
} channelConfiguration_t;


/** Class to abstract all HW control interfaces
  */
class HWControl {

  public:

   // Initialization functions
   
   HWControl(uint8_t PIN_SW1, uint8_t PIN_SW2, uint8_t PIN_FCTGEN, uint8_t PIN_POTI, uint8_t PIN_PWM, uint8_t channel, SPIClass* spi = NULL);
   void begin( SPIClass* spi = NULL);

   // setting functions

   void setOffsetStageSW(bool on = true); // SW1
   void setGainStageSW(bool on = true);   // SW2

   void setFrequency(uint32_t frequency) { gen.SetFrequency ( REG0, frequency ); };
   void setGain(uint8_t gain) { poti.setWiper( gain ); };
   void setOffset(uint8_t offset) { ledcWrite(CHANNEL, offset ); };
   
   void setSignalType(signaltype_t mode);
   inline void setSignalTypeSinus()    { setSignalType(SIN); };
   inline void setSignalTypeTriangle() { setSignalType(TRIANGLE); };
   inline void setSignalTypeSquare()   { setSignalType(SQUARE); };

   void enableOutput(bool en=true);
   
   // configuration functions

   void loadSettings();
   void storeSettings();

   float getRawSigAmp_V(void);
   float getRawSigOfs_V(void);
   float getOfs_V(uint16_t offset);
   float getGainStage1(uint16_t offset);
   float getGainStage2(uint16_t offset);

/*   
   void setS() { gen.ApplySignal(SINE_WAVE, REG0, AD9833_INIT_FREQ); gen.EnableOutput(true);  };
   void setT() { gen.ApplySignal(TRIANGLE_WAVE, REG0, AD9833_INIT_FREQ); gen.EnableOutput(true);  };
   void setQ() { gen.ApplySignal(SQUARE_WAVE, REG0, AD9833_INIT_FREQ); gen.EnableOutput(true);  };


  
   void setOutputMode(uint8_t output_mode);
   // set the switch settings which amplification and offset stages are activated
   //  valid for sin & triangle
   //  Mode    Name         Ofs   Amp    current  SW1   SW2
   //  0       FullRange    yes   15.0V  high     4     8
   //  1       MidRange     yes    3.0V  high     4     7
   //  2       SmallRange   -     -1.5V  low      3     8
   //  3       RawRange     -      0.3V  low      3     7

  

   void setFrequency(float f, float f1 = NO_FREQUENCY_HZ); // set frequencies in Hz
   void setChirp(float duration = 0);
   
   void setAmplitude(float A, float A2 = NO_AMPLITUDE_V); // set amplitude in V
   void setOffset(float o, float o2 = NO_OFFSET_V );    // set offset in V
   
   void getFrequency(float *f, float *f1 ); // set frequencies in Hz
   void getAmplitude(float *A, float *A2 ); // set amplitude in V
   void getOffset(float *o, float *o2 );    // set offset in V

   
   void getFrequencyLimits(float *fmin, float *fmax ) { *fmin=0.1; *fmax=5000000.0; }; 
   // return mode dependent parameters
   void getAmplitudeLimits(float *Amin, float *Amax ); 
   void getOffsetLimits(float *Omin, float *Omax ); 


   uint16_t amplitude2LSB(float amp);
   uint16_t offset2LSB(float ofs);


   void deacivateChirp(void);
   void activeFrequencyChirp(bool updown = false);
   void activeAmplitudeChirp(bool updown = false);
   void activeOffsetChirp(bool updown = false);
   void setChirpDuration(uint32_t d);
   void activePulseChirp(uint32_t pulsePause);
   void setChirpPhases(uint32_t A2F,uint32_t O2F);

   void setCalibrationFrequency();
   void setCalibrationOffset();
   void setCalibrationAmplitude();

   void loop(void) {};



 //protected: 
   void setOffsetStage(bool on = true); // SW1
   void setGainStage(bool on = true); // SW2
   void setF(String f) { gen.SetFrequency ( REG0, f.toInt() ); };
   void setG(String g) { poti.setWiper( g.toInt() ); };
   void setP(String p) { ledcWrite(CHANNEL, p.toInt() ); };
   void setS() { gen.ApplySignal(SINE_WAVE, REG0, AD9833_INIT_FREQ); gen.EnableOutput(true);  };
   void setT() { gen.ApplySignal(TRIANGLE_WAVE, REG0, AD9833_INIT_FREQ); gen.EnableOutput(true);  };
   void setQ() { gen.ApplySignal(SQUARE_WAVE, REG0, AD9833_INIT_FREQ); gen.EnableOutput(true);  };

  
              
   
 protected:
   void setFrequency(uint32_t f, uint32_t f2); // set frequency in LSB
   void setAmplitude(uint32_t A, uint32_t A2); // set amplitude in LSB
   void setOffset(uint32_t o, uint32_t o2); // set offset in LSB
*/
  
 private:
   uint32_t f = AD9833_INIT_FREQ;
   channelConfiguration_t conf;
   signaltype_t mode = SIN;

   //float f2 = 0;
   
   //uint16_t A = 0;
   //uint16_t A2 = 0;
   //uint16_t O = 0;
   //uint16_t O2 = 0;
   //uint16_t duty  = 0;
   //uint16_t pulse = 0;
   //uint8_t output_mode;
   
   //bool parametersChanged = false;
   
   //bool frequencyChirp = false;
   //bool offsetChirp = false;
   //bool amplitudeChirp = false;
   SPIClass* spi;

   uint8_t PIN_SW1,PIN_SW2,PIN_POTI,PIN_PWM;
   uint8_t CHANNEL;

   AD9833  gen;
   MCP41_Simple poti;
};

extern HWControl hwc1;
extern HWControl hwc2;

#endif
