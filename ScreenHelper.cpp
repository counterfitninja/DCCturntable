#include "ScreenHelper.h"
#include "configuration.h"

#ifdef U8GLIBLIBRARY
  #include <U8glib.h>
#endif

#ifdef U8G2LIBLIBRARY
  #include <U8g2lib.h>
#endif

#include "Arduino.h"

void drawButton(LCDLIB &lcd, byte x, byte y, byte w, byte h, char *strText, byte textXOffset, bool bSelected)
{
  // get the original ColorIndex to set back at the end
  byte currentColorIndex = lcd.getColorIndex();

  lcd.setColorIndex(bSelected);
  // draw a filled in box to clear the screen
  lcd.drawRBox( x, y, w, h, 1);
  // set the colour to black
  lcd.setColorIndex(not bSelected);
  
  // draw a frame if its not selected
  if (not bSelected)
  {
    lcd.drawRFrame( x, y, w, h, 1);
  }
  
  lcd.drawStr(x + textXOffset, y + 10, strText);

  // make sure we set it back to what it was.
  lcd.setColorIndex(currentColorIndex);
}


void drawMessageBox(LCDLIB &lcd, byte x, byte y, byte w, byte h, char *strText, bool centreX, bool centreY)
{
  byte xPos = 0;
  byte yPos = 0;

  // set colour to white
  lcd.setColorIndex(0);
  // draw a filled in box to clear the screen
  lcd.drawBox(0,0,128,64);
  // set the colour to black
  lcd.setColorIndex(1);
  
  lcd.drawFrame( x, y, w, h);

  xPos = x+(w/2) - ( lcd.getStrWidth(strText) / 2 );
  yPos = ((h/2)+8);

  lcd.drawStr( xPos, yPos, strText);
  
}
