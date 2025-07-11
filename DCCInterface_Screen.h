#ifndef DCCINTERFACE_SCREEN_h
#define DCCINTERFACE_SCREEN_h

#include "configurationVariables.h"

extern bool debug;

void DCCInterface_Screen_ProcessUserInput(DCCInterfaceScreenOperateCallBack operateCallBackFunction, DCCInterfaceScreenMoveStepperCallBack moveStepperCallBackFunction);


void setup_encoder();
void setup_screen();
void initializeButtons();

void drawScreen();
void draw();
void DCCInterface_Screen_ProcessMessageBox();

void showMessageBox(byte x, byte y, byte w, byte h, char *strText, bool centreX, bool centreY, byte timeShown);
void hideMessageBox();
bool isMessageDisplayed();


#endif
