
#include "HWIO.h"
#include "defines.h"


HWIO hwIO;

AiEsp32RotaryEncoder HWIO::enc = AiEsp32RotaryEncoder(ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);

void IRAM_ATTR  HWIO::readEncoderISR(void) {
    enc.readEncoder_ISR();
}


void HWIO::loop(void) {
 //dont print anything unless value changed
  if (enc.encoderChanged()){
    Serial.print("Value: ");
    Serial.println(enc.readEncoder());
    //display.ssd1306_setContrast(enc.readEncoder());
    //display.ssd1306_command(SSD1306_SETCONTRAST);display.ssd1306_command(enc.readEncoder());

  }
  if (enc.isEncoderButtonClicked()) {
    Serial.println("Button Click");
    //rotary_onButtonClick();
  }
} // void HWIO::loop(void) {


void HWIO::setup(void) {
  enc.begin();
  enc.setup(HWIO::readEncoderISR);
  bool circleValues = false; //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
  enc.setBoundaries(0, 255, circleValues);
  //enc.disableAcceleration(); //acceleration is now enabled by default - disable if you dont need it
  enc.setAcceleration(50); //or set the value - larger number = more accelearation; 0 or 1 means disabled acceleration
} // void HWIO::setup() {
