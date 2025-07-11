#include <Arduino.h>
#include "DCCInterface_EEPROMConfiguration.h"
#include "configurationVariables.h"
#include "Configuration.h"

#ifdef INTERNAL_EEPROM
  #include "eepromAnything.h"
  #include <EEPROM.h>
  #define EEPROM_LOCATION "Internal"
#endif

#ifdef EXTERNAL_EEPROM
  #include "eepromi2c.h"
  #define EEPROM_LOCATION "External"
#endif


void readConfigEEPROM(uint16_t startingAddress)
{
  Serial.print(F("Reading Config from "));
  Serial.print(EEPROM_LOCATION);
  Serial.println(F(" EEPROM"));
  
  char eepromHeader[12] = "";

  int eeHeaderAddress = startingAddress;

  eeHeaderAddress = eeHeaderAddress + eeRead(eeHeaderAddress, eepromHeader);
  
  Serial.print(F("eepromHeader : "));
  Serial.print(eepromHeader);
  Serial.print(F(" , Checking against : "));
  Serial.println(APPLICATION_EEPROMHEADER);

  Serial.println(F("-------------------------------------------------"));

  if (strcmp(eepromHeader, APPLICATION_EEPROMHEADER) == 0)
  {
    Serial.println("Configuration found.  Retrieving Values.\n");

    int eeConfigAddress = startingAddress + EEPROM_CONFIG_MAIN_OFFSET;

    Serial.print(F("Reading Config from EEProm Address : "));Serial.println(eeConfigAddress);

    eeConfigAddress = eeConfigAddress + eeRead(eeConfigAddress, configurationSettings);

    Serial.print(F("configurationSettings.stepperMaxspeed")); Serial.print(F(": ")); Serial.println(configurationSettings.stepperMaxspeed);
    Serial.print(F("configurationSettings.stepperAcceleration")); Serial.print(F(": ")); Serial.println(configurationSettings.stepperAcceleration);
    Serial.print(F("configurationSettings.numberOfIndexes")); Serial.print(F(": ")); Serial.println(configurationSettings.numberOfIndexes);
    Serial.print(F("configurationSettings.stepperFullSteps")); Serial.print(F(": ")); Serial.println(configurationSettings.stepperFullSteps);
    Serial.print(F("configurationSettings.stepperCompleteRotation")); Serial.print(F(": ")); Serial.println(configurationSettings.stepperCompleteRotation());
    Serial.print(F("configurationSettings.stepperMicroStepping")); Serial.print(F(": ")); Serial.println(configurationSettings.stepperMicroStepping);
    Serial.print(F("configurationSettings.homeIndex")); Serial.print(F(": ")); Serial.println(configurationSettings.homeIndex);
    Serial.print(F("configurationSettings.resetAddress")); Serial.print(F(": ")); Serial.println(configurationSettings.resetAddress);
    Serial.print(F("configurationSettings.i2cAdress")); Serial.print(F(": ")); Serial.println(configurationSettings.i2cAdress, HEX);

    eeConfigAddress = startingAddress + EEPROM_CONFIG_INDEX_OFFSET;
    Serial.print(F("Reading Indexes from EEProm Address : "));Serial.println(eeConfigAddress);
    eeConfigAddress = eeConfigAddress + eeRead(eeConfigAddress, controllerIndexes);
    
  }
  else
  {
    // if we dont have any config saved, write defaults to eeprom
    Serial.println("Configuration not found.  Saving Default Values.\n");
    writeConfigEEPROM(EEPROM_CONFIG_START_ADDRESS);
  }
}

void writeConfigEEPROM(uint16_t startingAddress)
{
  Serial.println(F("Saving Config to EEProm"));
  
  char eepromHeader[12] = APPLICATION_EEPROMHEADER;

  int eepromMemAddress = startingAddress;

  Serial.print(F("eepromHeader : "));
  Serial.println(eepromHeader);

  eepromMemAddress = eepromMemAddress + eeWrite(eepromMemAddress, eepromHeader);
  
  Serial.print(F("Next Address : "));
  Serial.println(eepromMemAddress);

  // Save the main config to EEPROM
  eepromMemAddress = startingAddress + EEPROM_CONFIG_MAIN_OFFSET;

  Serial.println(F("-------------------------------------------------"));
  
  Serial.print(F("Saving Config to EEProm Address : "));Serial.println(eepromMemAddress);

  eepromMemAddress = eepromMemAddress + eeWrite(eepromMemAddress, configurationSettings);

  // Save the main indexes to EEPROM
  eepromMemAddress = startingAddress + EEPROM_CONFIG_INDEX_OFFSET;

  Serial.println(F("-------------------------------------------------"));
  
  Serial.print(F("Saving Indexes to EEProm Address : "));Serial.println(eepromMemAddress);

  eepromMemAddress = eepromMemAddress + eeWrite(eepromMemAddress, controllerIndexes);
  
  Serial.println(F("-------------------------------------------------"));

  Serial.print(F("End of Config EEProm Address : ")); Serial.println(eepromMemAddress);
}
