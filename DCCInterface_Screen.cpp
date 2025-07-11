#include "configuration.h"
#include "configurationVariables.h"

#ifdef LCD12864SCREEN_PRESENT

#include "DCCInterface_Screen.h"
#include "ButtonHelper.h"
#include <rotaryEncoderHelper.h>

#include "DCCInterface_Screen_Operate.h"
#include "DCCInterface_Screen_Config.h"

#include "ScreenHelper.h"

#ifdef U8GLIBLIBRARY
  #include <U8glib.h>
#endif

#ifdef U8G2LIBLIBRARY
  #include <U8g2lib.h>
#endif

ButtonHelper encoderButton(ENCODER_BUTTON_PIN,500);
ButtonHelper configButton(CONFIG_BUTTON_PIN,1750);

#ifdef U8GLIBLIBRARY
  U8GLIB_ST7920_128X64_1X lcd(DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS);
#endif

#ifdef U8G2LIBLIBRARY
  U8G2_ST7920_128X64_1_SW_SPI lcd(U8G2_R0, DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS, 8);
#endif

// are we in config mode ?
byte inConfigMode = 0;

// used for drawing the message box

byte _x;
byte _y;
byte _w;
byte _h;
char *_strText;
bool _centreX;
bool _centreY;

// used to display a message on the screen
bool bDisplayingMessage = false;
byte _timeShown = 0;
unsigned long startMessageMillis = 0;

// ******************************************  User Input ********************************************

void setup_encoder()
{
  // encoder setup

  pinMode(ENCODER_A_PIN, INPUT_PULLUP);
  pinMode(ENCODER_B_PIN, INPUT_PULLUP);
  pinMode(ENCODER_BUTTON_PIN, INPUT_PULLUP); 

  pinMode(13,OUTPUT); //debug to led 13
}

void initializeButtons()
{
  encoderButton.initButton();
  configButton.initButton();  
}

// used to set up the drawing of the message box
void showMessageBox(byte x, byte y, byte w, byte h, char *strText, bool centreX, bool centreY, byte timeShown)
{
  _x = x;
  _y = y;
  _w = w;
  _h = h;
  _strText = strText;
  _centreX = centreX;
  _centreY = centreY;

  bDisplayingMessage = true;
  startMessageMillis = millis();
  _timeShown = timeShown;
}

void hideMessageBox()
{
  bDisplayingMessage = false; 
  drawScreen();  
}

bool isMessageDisplayed()
{
  return bDisplayingMessage;  
}

void DCCInterface_Screen_ProcessUserInput(DCCInterfaceScreenOperateCallBack operateCallBackFunction, DCCInterfaceScreenMoveStepperCallBack moveStepperCallBackFunction)
{
  byte dummy = 0;
  
  bool bRedrawScreen = false;

  if (!bDisplayingMessage)
  {
    configButton.processButton(2,dummy);

    // inConfigMode tells us which state we are in. set below using the LongPress() check
    if (inConfigMode == 1)
    {
      // In Config Mode
  
      bRedrawScreen = processUserInputConfigMode(encoderButton, moveStepperCallBackFunction); 
     
    }
    else
    {
      // Operate Mode
      
      // process input for the main Operate Mode - selecting the index
      
      bRedrawScreen = processUserInputOperateMode(encoderButton, operateCallBackFunction);
    }
  
  
    if (configButton.LongPress())
    {
      // if we detect a long press, then switch the state of the inConfigMode from  0 -> 1 -> 0
      Serial.println(F("LongPress on Config Button Detected..."));
      // switch the vlaue of inConfigMode
      inConfigMode = 1 - inConfigMode;
  
      // going to test this here....
      // TODO : Remove if wrong.
      currentIndex = 0;
      startIndex = 1;  
      
      bRedrawScreen = true;
  
      if (inConfigMode == 0)
      {
        // reset the current index back to 1 so that we can redraw 
        // the screen correctly based on the new maxcount of indexes
        currentIndex = 0;
        startIndex = 1;  

        showMessageBox(10, 7, 110, 50, "Config Saved", false, false, MESSAGEBOX_STANDARD_SHOW_TIME);

        saveConfiguration();
        loadConfiguration();
        
        setStepperMotorMotionParameters();
      }
    }
    
    if (bRedrawScreen)
    {
      drawScreen();  
    } 
  }
}

// return true or false if we should redraw the screen

// ******************************************* Drawing Routines **************************************

void drawScreen()
{
  lcd.firstPage();
  do 
  {  
    draw();
  } while( lcd.nextPage() );  
}

void draw()
{
  if (bDisplayingMessage)
  {
    // check if we need to hide it
    if (millis() - startMessageMillis > (_timeShown * 1000))
    {
      // hide the message box
      bDisplayingMessage = false; 
      drawScreen();  
    }
    else
    {
      // display the message box
      drawMessageBox(lcd,_x,_y,_w,_h,_strText,_centreX,_centreY);  
    }
  }
  
  if (!bDisplayingMessage)
  {
    // show the normal screen
    
    // set colour to white
    lcd.setColorIndex(0);
    // draw a filled in box to clear the screen
    lcd.drawBox(0,0,128,64);
    // set the colour to black
    lcd.setColorIndex(1);
    
    if (inConfigMode == 0)
    {
      // draw the normal Operating Screen
      drawOperate(lcd);
    }
    else
    {
      // draw the Config screen    
      drawConfig(lcd);
    }
  }
}

void DCCInterface_Screen_ProcessMessageBox()
{
  if (bDisplayingMessage)
  {
    if (millis() - startMessageMillis > (_timeShown * 1000))
    {
      hideMessageBox();
    }
  }
}



// ****************************************************************

void setup_screen()
{
  currentIndex = 0;
  inConfigMode = 0;

  // lcd screen setup
  #ifdef U8G2LIBLIBRARY
    lcd.begin();
    lcd.setFont(u8g2_font_helvR08_tf);
  #endif
  
  #ifdef U8GLIBLIBRARY
    lcd.setFont(u8g_font_helvR08r);        //Set the font for the display
  #endif
  
  lcd.setColorIndex(1);                 // Instructs the display to draw with a pixel on.  

  // draw the screen for the first time.
  showMessageBox(10, 7, 110, 50, "Please Wait..", false, false, MESSAGEBOX_STANDARD_SHOW_TIME);
  drawScreen();
}


#endif
