// configurationVariables.h

#ifndef CONFIGVARIABLES_H
#define CONFIGVARIABLES_H

#include <arduino.h>

// Max number of Indexes available
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega4809__) || defined(ESP32)
  //If an Arduino Mega is used.
  #define MAX_NUMBER_OF_INDEXES 12
#endif

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
  //If an Arduino Uno (or older) is used.
  #define MAX_NUMBER_OF_INDEXES 12
#endif

// *************************************************************************************************
// **
// **  Define the Struct to hold the index positions
// **
// *************************************************************************************************


typedef struct
{
  byte      stepperMaxspeed;
  byte      stepperAcceleration;
  uint8_t   numberOfIndexes;
  uint16_t  stepperFullSteps;
  uint16_t  stepperCompleteRotation()
  {
    return  stepperFullSteps * stepperMicroStepping;
  }
  uint8_t   stepperMicroStepping;
  uint8_t   homeIndex;
  uint16_t  resetAddress;
  byte      i2cAdress;  
}
ConfigurationSettings;

extern ConfigurationSettings configurationSettings;

typedef struct
{
  uint16_t address;
  uint16_t stationFront;
  uint16_t stationBack;
  //{
  //  return (stationFront + ((configurationSettings.stepperMicroStepping * configurationSettings.stepperFullSteps) / 2)) % (configurationSettings.stepperMicroStepping * configurationSettings.stepperFullSteps);  
  //}
  void ToDebug()
  {
    Serial.print(F(", Address : ")); Serial.print(address);
    Serial.print(F(", Front : ")); Serial.print(stationFront);
    Serial.print(F(", "));
    Serial.print(F("Back : ")); Serial.println(stationBack);
  }
}
DCCAccessoryAddress;

extern byte     currentIndex;

// Traverser
extern uint16_t StepsPerMM;
extern uint8_t  LeadScrewPitch;
extern uint8_t  LeadScrewStarts;
extern uint16_t TraverserMaxLength;

extern DCCAccessoryAddress controllerIndexes[MAX_NUMBER_OF_INDEXES];



typedef void (*DCCInterfaceScreenOperateCallBack)(uint16_t Addr, uint8_t Direction);
typedef void (*DCCInterfaceScreenMoveStepperCallBack)(uint16_t Position);

#endif
