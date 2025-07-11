#ifndef BUTTONHELPER_h
#define BUTTONHELPER_h

#include "Arduino.h"


class ButtonHelper
{
  private:
    byte          	_buttonPin;
	bool		  	_analog;
	bool			_debug;
    uint16_t      	_longPressPeriod = 500;
    unsigned long 	_timePressed;
    boolean       	_prevSwitchState = HIGH;
    boolean       	_singlePress;
    boolean       	_longPress;
   
  public:
    ButtonHelper(byte pButtonPin, uint16_t pLongPressPeriod, bool pAnalogPin = false, bool pDebug = false)
    {
		this-> _buttonPin = pButtonPin;
		this-> _longPressPeriod = pLongPressPeriod;
		this-> _analog = pAnalogPin;
		this-> _debug = pDebug;

		pinMode(pButtonPin, INPUT_PULLUP); 
    }

    bool SinglePress() {return _singlePress;};
    bool LongPress() {return _longPress;};
    

    void initButton()
    {
	  _timePressed = millis();
	  _singlePress = false;
      _longPress = false;
    }

    void processButton(byte pSequenceLength, byte& pState)
    {
        // variables to hold the new and old switch states
        boolean newSwitchState1 = LOW;
        boolean newSwitchState2 = LOW;
        boolean newSwitchState3 = LOW;
    
        // set the press states to false first of all
        _singlePress = false;
        _longPress = false;
        
		if (not _analog)
		{
			newSwitchState1 = digitalRead(_buttonPin);
			delay(1);
			newSwitchState2 = digitalRead(_buttonPin);
			delay(1);
			newSwitchState3 = digitalRead(_buttonPin);
		}
		else
		{
			newSwitchState1 = analogRead(_buttonPin);
			delay(1);
			newSwitchState2 = analogRead(_buttonPin);
			delay(1);
			newSwitchState3 = analogRead(_buttonPin);
		}
     
        // if all 3 values are the same we can continue
        if (  (newSwitchState1==newSwitchState2) && (newSwitchState1==newSwitchState3) )
        {
            if ( newSwitchState1 != _prevSwitchState ) 
            {
               // has the button switch been closed?
               if ( newSwitchState1 == HIGH )
               {
                  if (millis() - _timePressed > _longPressPeriod)  //test whether the period has elapsed to make it a Long Press
                  {
                    if (_debug)
					{
						Serial.print(F("Long Press Detected on pin "));
						Serial.println(_buttonPin);
					}
					
					_longPress = true;
                  }
                  else
                  {
                    if (_debug)
					{
						Serial.print(F("Short Press Detected on pin "));
						Serial.println(_buttonPin);
					}
					
					_singlePress = true;
                    pState++;
                    if (pState > (pSequenceLength -1) )
                    { 
                      pState = 0; 
                    }
                  }             
               }
               else
               {
                _singlePress = false;
                _longPress = false; 
                _timePressed = millis();
               }
               _prevSwitchState = newSwitchState1;
            }  
        }  
    }
};

#endif
