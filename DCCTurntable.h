// DCCTurnTable.h
#include <AccelStepper.h>
#include "Configuration.h"

#ifndef _DCCTURNTABLE_h
#define _DCCTURNTABLE_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "arduino.h"
#else
  #include "WProgram.h"
#endif


extern AccelStepper stepperMotorDriver;

void setupTurnTableController();
void processTurnTableController();
void OperateTurnTable(uint16_t Addr, uint8_t Direction);
void MoveBridgeToPosition(uint16_t Position);
void ConfigureIndexes();
void setStepperMotorMotionParameters();

#endif
