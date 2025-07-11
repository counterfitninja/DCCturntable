#include "configuration.h"
#include "configurationVariables.h"

#ifdef LCD12864SCREEN_PRESENT

#include "DCCInterface_Screen_Operate.h"
#include "DCCInterface_Screen.h"
#include "ButtonHelper.h"
#include <rotaryEncoderHelper.h>
#include "ScreenHelper.h"

#ifdef U8GLIBLIBRARY
  #include <U8glib.h>
#endif

#ifdef U8G2LIBLIBRARY
  #include <U8g2lib.h>
#endif

#include <Arduino.h>

// which index do we show first on the screen, 1 or 11 ?
byte startIndex = 1;

// are we selecting HEAD or TAIL movement of the bridge or selecting backup ?
// OP_STATE_MOVE_HEAD = 0
// OP_STATE_MOVE_TAIL = 1
// OP_STATE_MOVE_BACK = 2
byte currentSelectionOperation = 0;

// are we selecting an index, or the orientation of the bridge ?
// OP_STATE_INDEX_SEL = 0
// OP_STATE_MOVE_SEL 1
byte currentOperationState = 0;


// ************************************************************************************

// This is the routine for getting the input from the user in Operate Mode.
// return true or false if we should redraw the screen
bool processUserInputOperateMode(ButtonHelper &encoderButton, DCCInterfaceScreenOperateCallBack callBackFunction)
{
  byte prevIndex = currentIndex;
  byte previousMode = currentOperationState;
  byte prevSelectionOperation = currentSelectionOperation;
  char actionStr[19];
  
  bool returnReDrawScreen = false;
  
  // check if we have had a button press
  encoderButton.processButton(2, currentOperationState);
    
  // have we pressed the rotary button to "select" an index or "select" head/tail ?
  if (currentOperationState != previousMode)
  {
    // if so, then make sure we redraw
    returnReDrawScreen = true;
  }
  
  if (encoderButton.SinglePress())
  {

    if (previousMode == OP_STATE_MOVE_SEL)
    {
      if (currentSelectionOperation != OP_STATE_MOVE_BACK)
      {
        if (currentSelectionOperation == OP_STATE_MOVE_HEAD)
        {
          sprintf(actionStr, "%s%d%s", "Moving to [", currentIndex + 1, "] Head");
        }
        else
        if (currentSelectionOperation == OP_STATE_MOVE_TAIL)
        {
          sprintf(actionStr, "%s%d%s", "Moving to [", currentIndex + 1, "] Tail");
        }
  
        // stop the redraw
        showMessageBox(10, 7, 110, 50, actionStr, false, false, MESSAGEBOX_OPERATE_SHOW_TIME);
  
        // this is where we would call the function to move the turntable.
        callBackFunction(controllerIndexes[currentIndex].address, currentSelectionOperation == OP_STATE_MOVE_HEAD);
      }
    }
  }

  // if we are in the index selection mode,
  if (currentOperationState == OP_STATE_INDEX_SEL)
  {
    // use the rotary encoder to get the index we are selecting
    currentIndex = getRotaryPosition(ENCODER_A_PIN, ENCODER_B_PIN, 0, configurationSettings.numberOfIndexes-1, currentIndex, 1);  
    if (prevIndex != currentIndex) 
    {
      processEncoderForIndexSelection(currentIndex);

      // now refresh the screen
      returnReDrawScreen = true; 

      // always set this to HEAD as default
      currentSelectionOperation = 0;
    }
  }

  // else if we are in Movement mode.....
  if (currentOperationState == OP_STATE_MOVE_SEL)
  {
    // this selects the head/tail/back items
    // currentSelectionOperation is what is used when drawing the menu item
    currentSelectionOperation = getRotaryPosition(ENCODER_A_PIN, ENCODER_B_PIN, 0, 2, currentSelectionOperation, 1);  
    
    // have we just had a change in the variable ?
    if (prevSelectionOperation != currentSelectionOperation) 
    {
      // now refresh the screen
      returnReDrawScreen = true; 
    }
  } 

   return returnReDrawScreen;
}

void processEncoderForIndexSelection(byte encoderValue)
{
  for (int i = 0; i < configurationSettings.numberOfIndexes+1; i = i + INDEXES_PER_PAGE) 
  {
    if (encoderValue >= i && encoderValue < i + INDEXES_PER_PAGE)
    {
      startIndex = i+1;  
    }
  }
}

// screen drawing routines

void drawOperate(LCDLIB &lcd)
{
  drawOperateHeader(lcd);
  drawOperateMainArea(lcd);
  drawOperateFooter(lcd);  
}

void drawOperateFooter(LCDLIB &lcd)
{
  if (currentOperationState == OP_STATE_MOVE_SEL)
  {
    // we use varaible [currentSelectionOperation] to determine what option to present as "selected"
    drawButton(lcd, 6, 50, OPERATION_BOX_WIDTH, 12, "Head", 6, (currentOperationState == OP_STATE_MOVE_SEL && currentSelectionOperation == OP_STATE_MOVE_HEAD)); 
    drawButton(lcd, 54, 50, OPERATION_BOX_WIDTH, 12, "Tail", 6, (currentOperationState == OP_STATE_MOVE_SEL && currentSelectionOperation == OP_STATE_MOVE_TAIL)); 
    drawButton(lcd, 108, 50, INDEX_BOX_WIDTH, 12, "<", 6, (currentOperationState == OP_STATE_MOVE_SEL && currentSelectionOperation == OP_STATE_MOVE_BACK)); 
  }
  else
  {
    lcd.drawStr( 10, 58, DCCINTERFACE_URL);  
  }
}

void drawOperateHeader(LCDLIB &lcd)
{
  lcd.drawStr( 10, 8, APPLICATION_NAME);  
  lcd.drawStr( 82, 8, INTERNAL_VERSION_NUMBER);  
}

void drawOperateMainArea(LCDLIB &lcd)
{
  byte indexesToDraw = INDEXES_PER_PAGE;

  if (configurationSettings.numberOfIndexes <= INDEXES_PER_PAGE)
  {
    indexesToDraw = configurationSettings.numberOfIndexes;
  }
  else
  {
    for (int i = 0; i < configurationSettings.numberOfIndexes+1; i = i + INDEXES_PER_PAGE) 
    {
     
      if ((currentIndex >= i) && (currentIndex < i + INDEXES_PER_PAGE))
      {
        
        if (configurationSettings.numberOfIndexes <= i + INDEXES_PER_PAGE)
        {
          indexesToDraw = configurationSettings.numberOfIndexes - i;  
        }
        else
        {
            indexesToDraw = INDEXES_PER_PAGE;    
        }
      }
    }  
  }

  drawIndexes(lcd, startIndex,indexesToDraw,currentIndex);
}

void drawIndexes(LCDLIB &lcd, byte start, byte numberOfIndexes, byte selectedIndex)
{
  byte xLocation = 0;
  byte yLocation = 14;
  byte rowCounter = 0;
  byte xOffSet = 6;

  char indexStr[4];
  
  // reduce the start by 1 to make it clearer in the code
  start = start - 1;
  
  for (int i = start; i < start + numberOfIndexes; i++) 
  {
    if (rowCounter>4)
    {
      rowCounter = 0;
      yLocation = 32;  
    }
        
    xLocation = ((6*(rowCounter+1)) + (18 * rowCounter));
    sprintf (indexStr, "%d", i + 1 );

    if (i + 1 < 10)
    {
      xOffSet = 6;
    }
    else
    {
      xOffSet = 3;
    } 

    if ((currentOperationState == OP_STATE_INDEX_SEL) || (currentOperationState == OP_STATE_MOVE_SEL && (i == selectedIndex)))
    {
      drawButton(lcd, xLocation, yLocation, INDEX_BOX_WIDTH, 12, indexStr, xOffSet, (i == selectedIndex));
    }
    
    rowCounter = rowCounter + 1;
  }    
}
#endif
