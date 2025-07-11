#include "configuration.h"
#include "configurationVariables.h"

#ifdef LCD12864SCREEN_PRESENT

#include "DCCInterface_Screen_Config.h"
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



// what configuration option have we selected ?
// CONF_STATE_INDEXES 0
// CONF_STATE_SPEED 1
// CONF_STATE_STEPPER 2
// CONF_STATE_CONFINDEXES 3
byte currentConfigSelection = 0;

// used on the index configuration screen

// are we selecting an index attribute or amending it
//#define CONF_STATE_INDEX_SELECTING 0
//#define CONF_STATE_INDEX_SELECTED 1
byte currentConfigIndexOperation = 0;

// what are we configuring on the index
//#define CONF_STATE_INDEX_INDEX_NUM 0
//#define CONF_STATE_INDEX_HEAD 1
//#define CONF_STATE_INDEX_TAIL 2
//#define CONF_STATE_INDEX_ADDRESS 3
//#define CONF_STATE_INDEX_BACK 4

byte currentConfigIndexSelection = 0;

// are we on the general config screen or the index config screen ?
// CONF_STATE_MAINCONFIG 0
// CONF_STATE_INDEXCONFIG 1
byte currentConfigScreen = 0;


// used on the main config selection screen

// are we selecting a config item, or have we selected it?
// CONF_STATE_SELECTING 0
// CONF_STATE_SELECTED 1
byte currentConfigOperation = 0;

// this variable says how much to increase the position amount by. Long Press while moving will change this
uint16_t positionInc = 1;

bool processUserInputConfigMode(ButtonHelper &encoderButton, DCCInterfaceScreenMoveStepperCallBack moveStepperCallBack)
{
  bool returnReDrawScreen = false;
  byte dummy = 0;
  // make sure we know the previous config selecting item
  byte prevCurrentConfigSelection = currentConfigSelection;
  byte prevCurrentConfigIndexSelection = currentConfigIndexSelection;
  // previous values for the configuration items
  byte prevIndexCount = configurationSettings.numberOfIndexes;
  uint16_t prevStepperSpeed = configurationSettings.stepperMaxspeed;
  uint16_t previousStepperSteps = configurationSettings.stepperFullSteps;
  // previous values for the index configuration
  byte prevIndex = currentIndex;
  uint16_t prevStationFront = controllerIndexes[currentIndex].stationFront;
  uint16_t prevStationBack = controllerIndexes[currentIndex].stationBack;
  uint16_t prevAddress = controllerIndexes[currentIndex].address;

  
    
  encoderButton.processButton(2, dummy);

  // what screen are we wanting input on?
  // what are we doing, selected an index, or amending an index
  if (currentConfigScreen == CONF_STATE_MAINCONFIG)
  {
    // *****************************************************************
    // the main config screen -> this is the screen where 
    // we select the speed, stepper resolution, number of indexes
    // *****************************************************************
    
    if (currentConfigOperation == CONF_STATE_SELECTING)
    {
      // *****************************************************************
      // if we are selecting a config item
      // *****************************************************************
      
      // get the rotary movement
      currentConfigSelection = getRotaryPosition(ENCODER_A_PIN, ENCODER_B_PIN, 0, 3, currentConfigSelection, 1);
  
      // has there been a change ?
      if (prevCurrentConfigSelection != currentConfigSelection) 
      {
        // now refresh the screen
        returnReDrawScreen = true; 
      }

      // are we selecting config items, BUT just selected 
      if(encoderButton.SinglePress() && currentConfigSelection == CONF_STATE_CONFINDEXES)
      {
        currentConfigScreen = 1 - currentConfigScreen;
        // reset the index amendment screen
        // set it to be selecting an item to amend
        // and set the first item to be highlighted
        currentConfigIndexOperation = CONF_STATE_INDEX_SELECTING;
        currentConfigIndexSelection = CONF_STATE_INDEX_INDEX_NUM;
        returnReDrawScreen = true;
      }
    }
    else
    if (currentConfigOperation == CONF_STATE_SELECTED)
    {
      //***
      // what are we going to update???
      switch (currentConfigSelection)
      {
        case CONF_STATE_INDEXES:
          configurationSettings.numberOfIndexes = getRotaryPosition(ENCODER_A_PIN, ENCODER_B_PIN, 1, MAX_NUMBER_OF_INDEXES, configurationSettings.numberOfIndexes, 1);
          
          if (prevIndexCount != configurationSettings.numberOfIndexes) 
          {
            // now refresh the screen
            returnReDrawScreen = true; 
          }
          break;
    
        case CONF_STATE_SPEED:
          configurationSettings.stepperMaxspeed = getRotaryPosition(ENCODER_A_PIN, ENCODER_B_PIN, 1, 500, configurationSettings.stepperMaxspeed, 10);
          
          if (prevStepperSpeed != configurationSettings.stepperMaxspeed) 
          {
            // now refresh the screen
            returnReDrawScreen = true; 
          }
          break;
    
        case CONF_STATE_STEPPER:
          
          // ok, did we get a long press to tell us the micro stepping ?
          if (encoderButton.LongPress())
          {
            // amend the configurationSettings.stepperMicroStepping setting
            configurationSettings.stepperMicroStepping = configurationSettings.stepperMicroStepping * 2;
            if (configurationSettings.stepperMicroStepping > 16)
            {
              configurationSettings.stepperMicroStepping = 1;  
            }

            returnReDrawScreen = true;
          }
          
          
          
          configurationSettings.stepperFullSteps = getRotaryPosition(ENCODER_A_PIN, ENCODER_B_PIN, 200, 400, configurationSettings.stepperFullSteps, 200);
          
          if (previousStepperSteps != configurationSettings.stepperFullSteps) 
          {
            // now refresh the screen
            returnReDrawScreen = true; 
          }
          break;
      }
    }

    // Here we are in the main config screen did we get a button press to select a config item, or confirm an config item?
    // Check for a button press here?
    if (encoderButton.SinglePress())
    {
      currentConfigOperation = 1 - currentConfigOperation;
      returnReDrawScreen = true;
    }  
  }
  else
  // ***********************************
  // ***  the index amending screen  ***
  // ***********************************
  
  if (currentConfigScreen == CONF_STATE_INDEXCONFIG)
  {
    // what are we doing, selected an index, or amending an index
    if (currentConfigIndexOperation == CONF_STATE_INDEX_SELECTING)
    {
      // if we are selecting an index property.....
      currentConfigIndexSelection = getRotaryPosition(ENCODER_A_PIN, ENCODER_B_PIN, 0, 5, currentConfigIndexSelection, 1);
  
      if (prevCurrentConfigIndexSelection != currentConfigIndexSelection) 
      {
        // now refresh the screen
        returnReDrawScreen = true; 
      }

      // are se selecting config items, BUT just selected 
      if(encoderButton.SinglePress() && currentConfigIndexSelection == CONF_STATE_INDEX_BACK )
      {

        currentConfigScreen = 1 - currentConfigScreen;
        // reset the Main Config screen
        currentConfigOperation = CONF_STATE_SELECTING;
        currentConfigSelection = CONF_STATE_INDEXES;
        returnReDrawScreen = true;
      }
    }
    else if (currentConfigScreen == CONF_STATE_INDEX_SELECTED)
    {
      // what are we configuring on the index
      //#define CONF_STATE_INDEX_INDEX_NUM 0
      //#define CONF_STATE_INDEX_HEAD 1
      //#define CONF_STATE_INDEX_TAIL 2
      //#define CONF_STATE_INDEX_ADDRESS 3
      //#define CONF_STATE_INDEX_BACK 4
      switch (currentConfigIndexSelection)
      {
        case CONF_STATE_INDEX_INDEX_NUM:
          // use the rotary encoder to get the index we are selecting
          currentIndex = getRotaryPosition(ENCODER_A_PIN, ENCODER_B_PIN, 0, configurationSettings.numberOfIndexes-1, currentIndex, 1);  
          if (prevIndex != currentIndex) 
          {
            
            // now refresh the screen
            returnReDrawScreen = true; 
          }
          break;

        case CONF_STATE_INDEX_HEAD:
          
          
          // ok, did we get a long press to tell how much to increase the steps by ?
          if (encoderButton.LongPress())
          {
            // move the step increase on by a factor of 10
            positionInc = positionInc * 10;
            if (positionInc > 100)
            {
              positionInc = 1;  
            }

            returnReDrawScreen = true;
          }
          
          // use the rotary encoder to get the indexes StationFront Value
          controllerIndexes[currentIndex].stationFront = getRotaryPosition(ENCODER_A_PIN, ENCODER_B_PIN, 0, configurationSettings.stepperCompleteRotation(), controllerIndexes[currentIndex].stationFront, positionInc);  
          if (prevStationFront != controllerIndexes[currentIndex].stationFront) 
          {
            
            // now refresh the screen
            returnReDrawScreen = true;
            // now call the callback function to move the stepper to the right place
            moveStepperCallBack(controllerIndexes[currentIndex].stationFront); 
          }

          break;

        case CONF_STATE_INDEX_TAIL:
          
          // ok, did we get a long press to tell how much to increase the steps by ?
          if (encoderButton.LongPress())
          {
            // move the step increase on by a factor of 10
            positionInc = positionInc * 10;
            if (positionInc > 100)
            {
              positionInc = 1;  
            }

            returnReDrawScreen = true;
          }
          
          // use the rotary encoder to get the indexes StationFront Value
          controllerIndexes[currentIndex].stationBack = getRotaryPosition(ENCODER_A_PIN, ENCODER_B_PIN, 0, configurationSettings.stepperCompleteRotation(), controllerIndexes[currentIndex].stationBack, positionInc);  
          if (prevStationBack != controllerIndexes[currentIndex].stationBack) 
          {
            
            // now refresh the screen
            returnReDrawScreen = true;

             // now call the callback function to move the stepper to the right place
            moveStepperCallBack(controllerIndexes[currentIndex].stationBack);
          }
          break;

        case CONF_STATE_INDEX_ADDRESS:
          // use the rotary encoder to get the address we are selecting
          controllerIndexes[currentIndex].address = getRotaryPosition(ENCODER_A_PIN, ENCODER_B_PIN, 200, 300, controllerIndexes[currentIndex].address, 1);

          if (prevAddress != controllerIndexes[currentIndex].address) 
          {
            
            // now refresh the screen
            returnReDrawScreen = true; 
          }
          break;
      }
    }

    // did we get a button press while selecting Index Noo, Head or Tail ?
    // Here we are in the main config screen did we get a button press to select a config item, or confirm an config item?
    // Check for a button press here?
    if (encoderButton.SinglePress())
    {
      currentConfigIndexOperation = 1 - currentConfigIndexOperation;
      returnReDrawScreen = true;
    }
  }

  return returnReDrawScreen;
}
// ****************************************************************



// ****************************************************************
// Config Screen drawing routines
// ****************************************************************

void drawConfig(LCDLIB &lcd)
{
  drawConfigHeader(lcd);
  drawConfigMainArea(lcd);
  drawConfigFooter(lcd);  
}

void drawConfigHeader(LCDLIB &lcd)
{
  if (currentConfigScreen == CONF_STATE_MAINCONFIG)
  {
    lcd.drawStr( 6, 8, "Configuration");
    lcd.drawStr( 96, 8, APPLICATION_EEPROMVERSIONNUMBER);      
  }
  else if(currentConfigScreen == CONF_STATE_INDEXCONFIG)
  {
    lcd.drawStr( 6, 8, "Configure Indexes");
  }
  
  
}

void drawConfigMainArea(LCDLIB &lcd)
{
  if (currentConfigScreen == CONF_STATE_MAINCONFIG)
  {
    drawConfigIndexCount(lcd);
    drawConfigStepperSpeed(lcd);
    drawConfigStepperMotor(lcd);
  }
  else if(currentConfigScreen == CONF_STATE_INDEXCONFIG)
  {
    drawConfigIndexAmend(lcd, currentIndex);
  }
}

void drawConfigIndexCount(LCDLIB &lcd)
{
  byte xOffSet = 6;
  char indexCountStr[2];
  itoa(configurationSettings.numberOfIndexes,indexCountStr,10);
 

  if (configurationSettings.numberOfIndexes < 10)
  {
    xOffSet = 6;
  }
  else
  {
    xOffSet = 3;
  } 

  lcd.drawStr( 6, 22, "Indexes");
    
  drawButton(lcd, 80, 12, INDEX_BOX_WIDTH, 12, indexCountStr, xOffSet, (currentConfigSelection==CONF_STATE_INDEXES)); 
  
}

void drawConfigStepperSpeed(LCDLIB &lcd)
{
  char speedStr[3];
  itoa(configurationSettings.stepperMaxspeed,speedStr,10);
    
  lcd.drawStr(6, 33, "Speed");
  drawButton(lcd, 80, 23, 30, 12, speedStr, 3, (currentConfigSelection==CONF_STATE_SPEED)); 
}

void drawConfigStepperMotor(LCDLIB &lcd)
{
  char stepsStr[3];
  char microSteppingStr[5];
  
  itoa(configurationSettings.stepperFullSteps,stepsStr,10);
  sprintf(microSteppingStr, "/%u", configurationSettings.stepperMicroStepping);
  
  drawButton(lcd, 80, 34, 30, 12, stepsStr, 3, (currentConfigSelection==CONF_STATE_STEPPER)); 

  lcd.drawStr( 6, 44, "Steps / rev");
  lcd.drawStr(112, 44, microSteppingStr);
}

void drawConfigConfigureIndexes(LCDLIB &lcd)
{
  drawButton(lcd, 100, 50, INDEX_BOX_WIDTH+8, 12, "idx", 6, (currentConfigSelection==CONF_STATE_CONFINDEXES));   
}

// *** draw the Index configuration ***

void drawConfigIndexAmend(LCDLIB &lcd, byte index)
{
  drawConfigIndexId(lcd, index);
  drawConfigIndexHead(lcd, index);
  drawConfigIndexTail(lcd, index);
  drawConfigIndexAddress(lcd, index);    
}

void drawConfigIndexId(LCDLIB &lcd, byte index)
{
  byte xOffSet = 6;
  char indexStr[2];
  // we always pass in a reference to the array item, so for display, we plus 1. e.g. 0 -> 1
  itoa(index+1,indexStr,10);

  if (index < 10)
  {
    xOffSet = 6;
  }
  else
  {
    xOffSet = 3;
  } 

  lcd.drawStr( 6, 22, "Index");
    
  drawButton(lcd, 40, 12, INDEX_BOX_WIDTH, 12, indexStr, xOffSet, (currentConfigIndexSelection==CONF_STATE_INDEX_INDEX_NUM)); 
}

void drawConfigIndexHead(LCDLIB &lcd, byte index)
{
  const byte yPos = 23;
  
  char headPositionStr[6];
  char stepIncreaseStr[6];
  sprintf(headPositionStr, "%u",controllerIndexes[index].stationFront); 
  sprintf(stepIncreaseStr, "/%u", positionInc);
    
  lcd.drawStr(6, yPos + 10, "Head");
  drawButton(lcd, 40, yPos, 30, 12, headPositionStr, 3, (currentConfigIndexSelection==CONF_STATE_INDEX_HEAD)); 
  lcd.drawStr(72, yPos + 10, stepIncreaseStr);
}

void drawConfigIndexTail(LCDLIB &lcd, byte index)
{
  const byte yPos = 34;
  
  char tailPositionStr[4];
  char stepIncreaseStr[5];
  sprintf(tailPositionStr, "%u",  controllerIndexes[currentIndex].stationBack);
  sprintf(stepIncreaseStr, "/%u", positionInc);
  
  lcd.drawStr( 6, yPos + 10, "Tail");
  drawButton(lcd, 40, yPos, 30, 12, tailPositionStr, 3, (currentConfigIndexSelection==CONF_STATE_INDEX_TAIL)); 
  lcd.drawStr( 72, yPos + 10, stepIncreaseStr);
}

void drawConfigIndexAddress(LCDLIB &lcd, byte index)
{
  const byte yPos = 45;
  char addrStr[4];
  sprintf(addrStr, "%u",controllerIndexes[index].address); 
    
  lcd.drawStr(6, yPos + 10, "Addr");
  drawButton(lcd, 40, yPos, 30, 12, addrStr, 3, (currentConfigIndexSelection==CONF_STATE_INDEX_ADDRESS)); 
}

void drawConfigFooter(LCDLIB &lcd)
{
  if (currentConfigScreen == CONF_STATE_MAINCONFIG)
  {
    drawConfigConfigureIndexes(lcd);
  }
  else if(currentConfigScreen == CONF_STATE_INDEXCONFIG)
  {
    drawButton(lcd, 108, 50, INDEX_BOX_WIDTH, 12, "<", 6, (currentConfigIndexSelection==CONF_STATE_INDEX_BACK)); 
  }
}

#endif
