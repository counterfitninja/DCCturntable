#ifndef ROTARYENCODERHELPER_h
#define ROTARYENCODERHELPER_h

#include "Arduino.h"



uint16_t getRotaryPosition(byte encoderPin1, byte encoderPin2, uint16_t minValue, uint16_t maxValue, uint16_t  currentRotaryPosition, uint16_t stepIncrease = 1);
float getRotaryPositionf(byte encoderPin1, byte encoderPin2, float minValue, float maxValue, float  currentRotaryPosition, float stepIncrease = 1);

#endif
