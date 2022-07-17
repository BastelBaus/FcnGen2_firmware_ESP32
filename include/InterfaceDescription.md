

Assynchronous interfaces
- each command send will be responded
- oder of commadn responce will stay as
- commands could be interrupted by messages from slave (i.e. alive counter)




Control commands

// "!x": restart ESP
// "!A": response with an alive message
// "!A:[t:uint32_t]": response every t ms with an alive message t>0
// "!A:0": turn of alive responce
// L[t:uint8_t]:  return list of configuration variables,
//                  t=0 : channel 0
//                  t=1 : channel 1
//                  t=2 : generic
// D: return error and output info, debug and error messages

/* commands
 *  !x\n  : restart
 *    returns:  >reboot\n
 *  !y\n  : search I2C devices
 *    returns:  >0x3C,0x37\n (list of I2C addersses


 *  > channel commands
 *  >1:*\n channel 1
 *  >2:*\n channel 2
 *  >3:*\n both channels

 *     t:?   set output type (? = _s_in,s_q_uare, _t_riangle)

 *     c     get _c_ount of different modes (i.e. 4)
 *     n     get current output mode _n_ame
 *     n:?   get output mode _n_ame of mode ?
 *     o     get active _o_utput mode
 *     o:?   switch to _o_utput mode (? = 0,1,2,3,....)

 *     p:c        get _c_ount of different parameters of current modes
 *     p:n:?      get parameter ? name
 *     p:t(:?)+   get parameter ? type
 *     p:u(:?)+   get parameter ? unit
 *     p:a(:?)+   get parameter ? max
 *     p:i(:?)+   get parameter ? min
 *     p:s(:?)+   get parameter ? step
 *     p:v(:?)+   get parameter ? (current) value
 *     p:?:*      set parameter ? to new value * (if two channels, both get the same value ?????)

 *     e?  _e_nable output (? = 0,1,empty=1)

 *  $: configuration commands
 *  ?: status commands
 */












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
