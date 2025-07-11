#ifndef DCCInterface_Screen_Operate_h
#define DCCInterface_Screen_Operate_h

#include <Arduino.h>
#include "ButtonHelper.h"
#include "Configuration.h"
#include "configurationVariables.h"


//extern byte currentIndex;
extern byte startIndex;


bool processUserInputOperateMode(ButtonHelper &encoderButton, DCCInterfaceScreenOperateCallBack callBackFunction);
void processEncoderForIndexSelection(byte encoderValue);

void drawOperate(LCDLIB &lcd);

void drawOperateFooter(LCDLIB &lcd);
void drawOperateHeader(LCDLIB &lcd);
void drawOperateMainArea(LCDLIB &lcd);
void drawIndexes(LCDLIB &lcd, byte start, byte numberOfIndexes, byte selectedIndex);




#endif
