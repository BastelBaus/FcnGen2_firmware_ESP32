#include <Arduino.h>
#include "control/OutputModes.h"

OutputConfiguration outMode[] = {
          std::vector<OutputMode*>( {
            new OutputModeRaw(&hwc1, std::vector<ParameterRaw*>( { new ParamAmplitudeLSB(&hwc1), new ParamOffsetLSB(&hwc1), new ParamGainStageSW(&hwc1), new ParamOffsetStageSW(&hwc1) } )) ,
            new OutputMode0  (&hwc1, std::vector<ParameterRaw*>( { new ParamAmplitudeV(&hwc1), new ParamOffsetV(&hwc1) } ) )
          } ),
          std::vector<OutputMode*>( {
             new OutputModeRaw(&hwc2, std::vector<ParameterRaw*>( { new ParamAmplitudeLSB(&hwc2), new ParamOffsetLSB(&hwc2), new ParamGainStageSW(&hwc2), new ParamOffsetStageSW(&hwc2) } )) ,
             new OutputMode0  (&hwc2, std::vector<ParameterRaw*>( { new ParamAmplitudeV(&hwc2), new ParamOffsetV(&hwc2) } ) ),
             new OutputMode1  (&hwc2, std::vector<ParameterRaw*>( { new ParamAmplitudeV(&hwc2), new ParamOffsetV(&hwc2) } ) ),
             new OutputMode2  (&hwc2, std::vector<ParameterRaw*>( { new ParamAmplitudeV(&hwc2), new ParamOffsetV(&hwc2) } ) ),
             new OutputMode3  (&hwc2, std::vector<ParameterRaw*>( { new ParamAmplitudeV(&hwc2), new ParamOffsetV(&hwc2) } ) )
           } )
        };


/*
std::vector<OutputMode*> outputModes[] =
  { std::vector<OutputMode*>( {
      new OutputModeRaw(&hwc1, std::vector<ParameterRaw*>( { new ParamAmplitudeLSB(&hwc1), new ParamOffsetLSB(&hwc1), new ParamGainStageSW(&hwc1), new ParamOffsetStageSW(&hwc1) } )) ,
      new OutputMode0  (&hwc1, std::vector<ParameterRaw*>( { new ParamAmplitudeV(&hwc1), new ParamOffsetV(&hwc1) } ) )
    } ),
    std::vector<OutputMode*>( {
       new OutputModeRaw(&hwc2, std::vector<ParameterRaw*>( { new ParamAmplitudeLSB(&hwc2), new ParamOffsetLSB(&hwc2), new ParamGainStageSW(&hwc2), new ParamOffsetStageSW(&hwc2) } )) ,
       new OutputMode0  (&hwc2, std::vector<ParameterRaw*>( { new ParamAmplitudeV(&hwc2), new ParamOffsetV(&hwc2) } ) ) ,
       new OutputMode1  (&hwc2, std::vector<ParameterRaw*>( { new ParamAmplitudeV(&hwc2), new ParamOffsetV(&hwc2) } ) ) //,
       //new OutputMode0  (&hwc2, std::vector<ParameterRaw*>( { new ParamAmplitudeV(&hwc2), new ParamOffsetV(&hwc2) } ) ),
       //new OutputMode0  (&hwc2, std::vector<ParameterRaw*>( { new ParamAmplitudeV(&hwc2), new ParamOffsetV(&hwc2) } ) )
     } )
  };
*/
