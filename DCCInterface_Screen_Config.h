#ifndef DCCInterface_Screen_Config_h
#define DCCInterface_Screen_Config_h

#include <Arduino.h>

#include "ButtonHelper.h"
#include "configuration.h"
#include "configurationVariables.h"

bool processUserInputConfigMode(ButtonHelper &encoderButton, DCCInterfaceScreenMoveStepperCallBack moveStepperCallBack);

void drawConfig(LCDLIB &lcd);


void drawConfigHeader(LCDLIB &lcd);
void drawConfigMainArea(LCDLIB &lcd);
void drawConfigIndexCount(LCDLIB &lcd);
void drawConfigStepperSpeed(LCDLIB &lcd);
void drawConfigStepperMotor(LCDLIB &lcd);
void drawConfigConfigureIndexes(LCDLIB &lcd);

void drawConfigIndexAmend(LCDLIB &lcd, byte index);

void drawConfigIndexId(LCDLIB &lcd, byte index);
void drawConfigIndexHead(LCDLIB &lcd, byte index);
void drawConfigIndexTail(LCDLIB &lcd, byte index);
void drawConfigIndexAddress(LCDLIB &lcd, byte index);
void drawConfigFooter(LCDLIB &lcd);


#endif
