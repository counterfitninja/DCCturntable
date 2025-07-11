#ifndef SCREENHELPER_h
#define SCREENHELPER_h

#include "configuration.h"
#include "Arduino.h"




extern unsigned long startMessageMillis;

void drawButton(LCDLIB &lcd, byte x, byte y, byte w, byte h, char *strText, byte textXOffset, bool bSelected);
void drawMessageBox(LCDLIB &lcd, byte x, byte y, byte w, byte h, char *strText, bool centreX, bool centreY);

#endif
