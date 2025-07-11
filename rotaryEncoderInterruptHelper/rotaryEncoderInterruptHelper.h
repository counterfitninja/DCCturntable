#ifndef ROTARYENCODERINTERRUPTHELPER_h
#define ROTARYENCODERINTERRUPTHELPER_h

#include <arduino.h>

typedef struct
{
  byte        encoderPinA = 21;
  byte        encoderPinB = 22;
  int16_t     encoderValue = 0;
  uint8_t     encoderState;
  int8_t      encoderDirection = 1;
  bool        debug = false;
}
RotaryEncoder;

extern RotaryEncoder rotaryEncoder;

void initializeEncoder(byte pEncoderPinA , byte pEncoderPinB, int8_t pEncoderDirection = 1);
void setRotaryEncoder(int8_t pEncoderValue = 0, uint8_t pEncoderState = 0, int8_t pEncoderDirection = 1);
void resetEncoder();
uint16_t getRotaryPosition(uint16_t minValue, uint16_t maxValue, uint16_t  currentValue, uint16_t stepIncrease = 1, bool pResetEncoder = true);



#endif
