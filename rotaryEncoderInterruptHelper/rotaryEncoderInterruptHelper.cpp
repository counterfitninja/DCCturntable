#include "rotaryEncoderInterruptHelper.h"


RotaryEncoder rotaryEncoder;


uint16_t getRotaryPosition(uint16_t minValue, uint16_t maxValue, uint16_t  currentValue, uint16_t stepIncrease, bool pResetEncoder)
{
  int16_t returnValue = currentValue;
  
  if (rotaryEncoder.encoderValue != 0)
  {
      if (rotaryEncoder.debug) {
        Serial.print("\nActual Encoder Value :");
        Serial.println(rotaryEncoder.encoderValue);
        Serial.print("\nPassed-in Value :");
        Serial.println(currentValue);
      }

      returnValue = currentValue + ((rotaryEncoder.encoderValue * stepIncrease)  * rotaryEncoder.encoderDirection );

      if (rotaryEncoder.debug) {
        Serial.print("\nInitial Return Value :");
        Serial.println(returnValue);
      }

      if (returnValue <= minValue)
      {
        if (rotaryEncoder.debug) {
          Serial.println("\nSetting Encoder to Min Value");
        }
        returnValue = minValue;
      }

      if (returnValue >= maxValue)
      {
        if (rotaryEncoder.debug) {
          Serial.println("\nSetting Encoder to Max Value");
        }
        returnValue = maxValue;
      }

      if (pResetEncoder)
      {
        if (rotaryEncoder.debug) {
          Serial.println("\nResetting Encoder");
        }
        resetEncoder();
      }

      if (rotaryEncoder.debug) {
        Serial.print("\nReturn Value :");
        Serial.println(returnValue);
      }
  }

  return returnValue;
}

void readEncoder() 
{
  rotaryEncoder.encoderState = (rotaryEncoder.encoderState << 4) | (digitalRead(rotaryEncoder.encoderPinB) << 1) | digitalRead(rotaryEncoder.encoderPinA);
  switch (rotaryEncoder.encoderState) 
  {
    case 0x23:  {
                  (rotaryEncoder.encoderValue = rotaryEncoder.encoderValue + 1);
                  break;
                }
    case 0x32:  {
                  (rotaryEncoder.encoderValue = rotaryEncoder.encoderValue -1);
                  break;
                }
    default: break;
  }
}

void initializeEncoder(byte pEncoderPinA, byte pEncoderPinB, int8_t pEncoderDirection)
{
  rotaryEncoder.encoderPinA = pEncoderPinA;
  rotaryEncoder.encoderPinB = pEncoderPinB;
  rotaryEncoder.encoderDirection = pEncoderDirection;

  pinMode(rotaryEncoder.encoderPinA, INPUT_PULLUP);
  pinMode(rotaryEncoder.encoderPinB, INPUT_PULLUP);

  // enable pin change interrupts
  attachInterrupt(digitalPinToInterrupt(rotaryEncoder.encoderPinA), readEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rotaryEncoder.encoderPinB), readEncoder, CHANGE);
  rotaryEncoder.encoderState = (digitalRead(rotaryEncoder.encoderPinB) << 1) | digitalRead(rotaryEncoder.encoderPinA);
  interrupts();

  resetEncoder();
}

void setRotaryEncoder(int8_t pEncoderValue, uint8_t pEncoderState, int8_t pEncoderDirection)
{
  rotaryEncoder.encoderValue = pEncoderValue;
  rotaryEncoder.encoderState = pEncoderState;
  rotaryEncoder.encoderDirection = pEncoderDirection;
}

void resetEncoder()
{
  setRotaryEncoder(0,0, rotaryEncoder.encoderDirection);
}
