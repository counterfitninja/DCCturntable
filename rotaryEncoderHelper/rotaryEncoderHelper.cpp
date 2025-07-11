#include "rotaryEncoderHelper.h"

// quadrature decoder state machine
#define _IDLE 0
#define CW_1  1
#define CW_2  2
#define CW_3  3
#define CCW_1 4
#define CCW_2 5
#define CCW_3 6
uint8_t state = _IDLE;

uint16_t getRotaryPosition(byte encoderPin1, byte encoderPin2, uint16_t minValue, uint16_t maxValue, uint16_t  currentRotaryPosition, uint16_t stepIncrease = 1)
{
  uint8_t A = digitalRead( encoderPin1 );
  uint8_t B = digitalRead( encoderPin2 );
  uint16_t returnRotaryPosition = currentRotaryPosition;

  switch (state)
  {
    case _IDLE: // between detents (A & B open)
      if      (A == HIGH && B == LOW)  // B closed
        state = CW_1;
      else if (B == HIGH && A == LOW)  // A closed
        state = CCW_1;
      break; // A & B still high (or an illegal transition occurred)

    case CCW_1: // A closed, B open
      if      (A == LOW  && B == LOW)  // B closed
        state = CCW_2;
      else if (A == HIGH && B == HIGH) // contact bounce
        state = _IDLE;
      break;

    case CCW_2: // both A & B closed
      if      (A == HIGH && B == LOW)  // A opened
        state = CCW_3;
      else if (A == LOW  && B == HIGH) // contact bounce
        state = CCW_1;
      else if (A == HIGH && B == HIGH) // illegal
        state = _IDLE;
      break;

    case CCW_3: // A open, B closed
      if      (A == LOW  && B == LOW)  // contact bounce
        state = CCW_2;
      else if (A == HIGH && B == HIGH) // B opened
      {
        // CCW turn detected
        if (currentRotaryPosition > minValue) 
		{
			returnRotaryPosition = currentRotaryPosition - stepIncrease;
			if (returnRotaryPosition > maxValue)
			{
				returnRotaryPosition = minValue;
			}
		}
        //Serial.println(currentRotaryPosition);
        state = _IDLE;
      }
      break;

    case CW_1: // B closed, A open
      if      (A == LOW  && B == LOW)  // A closed
        state = CW_2;
      else if (A == HIGH && B == HIGH) // contact bounce
        state = _IDLE;
      break;

    case CW_2: // both A & B closed
      if      (A == LOW  && B == HIGH) // B opened
        state = CW_3;
      else if (A == HIGH && B == LOW)  // contact bounce
        state = CW_1;
      if      (A == HIGH && B == HIGH) // illegal
        state = _IDLE;
      break;

    case CW_3: // B open, A closed
      if      (A == LOW  && B == LOW)  // contact bounce
        state = CW_2;
      else if (A == HIGH && B == HIGH) // A opened
      {
        // CW turn detected
        if (currentRotaryPosition < maxValue) 
		{
			returnRotaryPosition = currentRotaryPosition + stepIncrease;
			if (returnRotaryPosition > maxValue)
			{
				returnRotaryPosition = maxValue;
			}
		}
        //Serial.println(currentRotaryPosition);
        state = _IDLE;
      }
	  
	  
  } 

  return returnRotaryPosition;  
}

float getRotaryPositionf(byte encoderPin1, byte encoderPin2, float minValue, float maxValue, float  currentRotaryPosition, float stepIncrease = 1)
{
  uint8_t A = digitalRead( encoderPin1 );
  uint8_t B = digitalRead( encoderPin2 );
  float returnRotaryPosition = currentRotaryPosition;

  switch (state)
  {
    case _IDLE: // between detents (A & B open)
      if      (A == HIGH && B == LOW)  // B closed
        state = CW_1;
      else if (B == HIGH && A == LOW)  // A closed
        state = CCW_1;
      break; // A & B still high (or an illegal transition occurred)

    case CCW_1: // A closed, B open
      if      (A == LOW  && B == LOW)  // B closed
        state = CCW_2;
      else if (A == HIGH && B == HIGH) // contact bounce
        state = _IDLE;
      break;

    case CCW_2: // both A & B closed
      if      (A == HIGH && B == LOW)  // A opened
        state = CCW_3;
      else if (A == LOW  && B == HIGH) // contact bounce
        state = CCW_1;
      else if (A == HIGH && B == HIGH) // illegal
        state = _IDLE;
      break;

    case CCW_3: // A open, B closed
      if      (A == LOW  && B == LOW)  // contact bounce
        state = CCW_2;
      else if (A == HIGH && B == HIGH) // B opened
      {
        // CCW turn detected
        if (currentRotaryPosition > minValue) 
		{
			returnRotaryPosition = currentRotaryPosition - stepIncrease;
			if (returnRotaryPosition > maxValue)
			{
				returnRotaryPosition = minValue;
			}
		}
        //Serial.println(currentRotaryPosition);
        state = _IDLE;
      }
      break;

    case CW_1: // B closed, A open
      if      (A == LOW  && B == LOW)  // A closed
        state = CW_2;
      else if (A == HIGH && B == HIGH) // contact bounce
        state = _IDLE;
      break;

    case CW_2: // both A & B closed
      if      (A == LOW  && B == HIGH) // B opened
        state = CW_3;
      else if (A == HIGH && B == LOW)  // contact bounce
        state = CW_1;
      if      (A == HIGH && B == HIGH) // illegal
        state = _IDLE;
      break;

    case CW_3: // B open, A closed
      if      (A == LOW  && B == LOW)  // contact bounce
        state = CW_2;
      else if (A == HIGH && B == HIGH) // A opened
      {
        // CW turn detected
        if (currentRotaryPosition < maxValue) 
		{
			returnRotaryPosition = currentRotaryPosition + stepIncrease;
			if (returnRotaryPosition > maxValue)
			{
				returnRotaryPosition = maxValue;
			}
		}
        //Serial.println(currentRotaryPosition);
        state = _IDLE;
      }
	  
	  
  } 

  return returnRotaryPosition;  
}
