
#include "controlTask.h"
#include "control/HWControl.h"
#include "Utils.h"

bool aliveFlag = false;
TaskHandle_t controlTaskHandle;

void startControlTask(void) {
  Serial.println("Starting control task");
  //disableCore0WDT(); // ToDo: better yielkd WDT in each loop
  xTaskCreatePinnedToCore(
      controlTaskLoop,    /* Function to implement the task */
      "controlTask",      /* Name of the task */
      10000,              /* Stack size in words */
      NULL,               /* Task input parameter */
      0,                  /* Priority of the task */
      &controlTaskHandle, /* Task handle. */
      0);                 /* Core where the task should run */
}

void controlTaskLoop( void * parameter) {
  for(;;) {
    //hwc1.loop();
    //hwc2.loop();
    aliveFlag = true;
    feedTheDog0();
  }
}
