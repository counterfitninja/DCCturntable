#include "Configuration.h"

// include files

#ifdef WEBINTERFACE
  #include "DCCInterface_WebServer.h"
#endif

#ifdef TURNTABLE
  #include "DCCTurntable.h"
#endif

#ifdef TRAVERSER
  #include "DCCTraverser.h"
#endif

#ifdef LCD12864SCREEN_PRESENT
  #include "DCCInterface_Screen.h"
#endif

#ifdef SDCARDCONFIGURATION
  #include "DCCInterface_SDCardConfiguration.h"
#endif

#ifdef EEPROM_STORAGE
  #include "DCCInterface_EEPROMConfiguration.h"
#endif

// end of include files

void setupController()
{
  #ifdef LCD12864SCREEN_PRESENT
    setup_encoder();
    setup_screen();
    Serial.println(F("Init Buttons...."));
    initializeButtons();
    delay(1000);
  #endif
  
  #ifdef TURNTABLE
    setupTurnTableController();
  #endif 
  #ifdef TRAVERSER
    setupTraverserController();
  #endif

  #ifdef WEBINTERFACE
    setUpWebServer();
  #endif

  #ifdef LCD12864SCREEN_PRESENT
    drawScreen();
  #endif
}

void processController()
{
  #ifdef LCD12864SCREEN_PRESENT
    DCCInterface_Screen_ProcessUserInput(OperateTurnTable, MoveBridgeToPosition);
    DCCInterface_Screen_ProcessMessageBox();
  #endif 
  
  #ifdef TURNTABLE
    processTurnTableController();
  #endif  
  #ifdef TRAVERSER
    processTraverserController();
  #endif
  
  #ifdef KEYPADCONTROLLER_PRESENT  
    keypadController_loop();
  #endif

  #ifdef WEBINTERFACE
    processWebServer();
  #endif
}

void setDefaultConfiguration(  uint16_t pStepperMaxspeed = MAXSPEED, 
              uint16_t pStepperAcceleration = ACCELERATION, 
              uint8_t pNumberOfIndexes = NUMBER_OF_INDEXES, 
              uint16_t pStepperFullSteps = STEPPER_MOTOR_STEPS, 
              uint8_t pStepperMicroStepping = MICROSTEPPING, 
              uint8_t pHomeIndex = HOME_INDEX,
              uint16_t pResetAddress = RESET_DCC_ADDRESS,
              byte pI2CAddress = I2CADDR)
{
  // set up standard parameters of the stepper
  configurationSettings.stepperMaxspeed = pStepperMaxspeed;
  configurationSettings.stepperAcceleration = pStepperAcceleration;
  // set up the resolution of the stepper
  configurationSettings.stepperFullSteps = pStepperFullSteps;
  configurationSettings.stepperMicroStepping = pStepperMicroStepping; 

  //configurationSettings.stepperCompleteRotation = configurationSettings.stepperFullSteps * pStepperMicroStepping;
  
  // set up the number of indexes
  configurationSettings.numberOfIndexes = pNumberOfIndexes;
  configurationSettings.homeIndex = pHomeIndex;
  configurationSettings.resetAddress = pResetAddress;
  configurationSettings.i2cAdress = pI2CAddress;
  
  Serial.print(F("\nStepperMaxspeed : ")); Serial.print(configurationSettings.stepperMaxspeed);
  Serial.print(F("\nStepperAcceleration : ")); Serial.print(configurationSettings.stepperAcceleration);

  Serial.print(F("\nStepperFullSteps : ")); Serial.print(configurationSettings.stepperFullSteps);
  Serial.print(F("\n(stepperFullSteps * stepperMicroStepping) : ")); Serial.print((configurationSettings.stepperFullSteps * configurationSettings.stepperMicroStepping));
  Serial.print(F("\nstepperCompleteRotation : ")); Serial.print(configurationSettings.stepperCompleteRotation());
  Serial.print(F("\nStepperMicroStepping : ")); Serial.print(configurationSettings.stepperMicroStepping);

  Serial.print(F("\nNumberOfIndexes : ")); Serial.print(configurationSettings.numberOfIndexes);
  Serial.print(F("\nHomeIndex : ")); Serial.println(configurationSettings.homeIndex + ARRAYADJUSTER);

  #ifdef KEYPADCONTROLLER_PRESENT
    Serial.print(F("Keypad Controller Address : "));
    Serial.println(configurationSettings.i2cAdress, HEX);
  #endif
}

void setDefaultIndexConfiguration()
{
  for (byte i =0; i<  configurationSettings.numberOfIndexes;i++)
  {
    controllerIndexes[i].address = POSITION_01_DCC_ADDRESS + i; 
    controllerIndexes[i].stationFront = ((configurationSettings.stepperCompleteRotation() / 2) / configurationSettings.numberOfIndexes) * i; 
    //stationBack
    controllerIndexes[i].stationBack = controllerIndexes[i].stationFront + ((configurationSettings.stepperCompleteRotation() / 2)); 
  }
}


// Load configuration
void loadConfiguration()
{
  #ifdef EEPROM_STORAGE
    // read the config from the EEProm
    readConfigEEPROM(EEPROM_CONFIG_START_ADDRESS);
  #endif
  
  // Load Configuration from SD Card
  #ifdef SDCARDCONFIGURATION
    Serial.println(F("\nSD Card Configuration Module Included. Loading Config from CONFIG.CFG... on SD Card"));
    if (SDCardConfiguration_Setup())
    {
      SDCardConfiguration_LoadConfiguration();
      Serial.println(F("Configuration Loaded.... Applying New Configuration...."));
      setDefaultConfiguration(configurationSettings.stepperMaxspeed, 
                              configurationSettings.stepperAcceleration, 
                              configurationSettings.numberOfIndexes, 
                              configurationSettings.stepperFullSteps, 
                              configurationSettings.stepperMicroStepping, 
                              configurationSettings.homeIndex,
                              configurationSettings.resetAddress,
                              configurationSettings.i2cAdress);
      
      saveConfiguration();
    }
  #endif

  
}

// Save Configuration
void saveConfiguration()
{
  #ifdef EEPROM_STORAGE
    writeConfigEEPROM(EEPROM_CONFIG_START_ADDRESS);
  #endif
}

void showMessage(char *strText)
{
  #ifdef LCD12864SCREEN_PRESENT
    showMessageBox(10, 7, 110, 50, strText, false, false, MESSAGEBOX_STANDARD_SHOW_TIME);
    drawScreen();
  #endif   
}
