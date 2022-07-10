
#ifndef __CONTROLTASK_H__
#define __CONTROLTASK_H__

#include <Arduino.h>


// start control task and then return
void startControlTask(void);

// indefinite loop calling HWControl 
// class loop functions for both channels
void controlTaskLoop( void * parameter);

// flag is set to true every loop.
// set to false and check after a certain 
// time if is set back tro true
extern bool aliveFlag;

#endif
