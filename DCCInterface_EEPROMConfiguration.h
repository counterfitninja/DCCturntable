// DCCInterface_EEPROMConfiguration.h

#ifndef EEPROMCONFIG_H
#define EEPROMCONFIG_H

// EEProm Configuration

#define APPLICATION_EEPROMVERSIONNUMBER "1.5"
#define APPLICATION_CODE "DCCINT"
#define APPLICATION_EEPROMHEADER APPLICATION_CODE "_" APPLICATION_EEPROMVERSIONNUMBER

#define EEPROM_CONFIG_START_ADDRESS 0
#define EEPROM_CONFIG_MAIN_OFFSET 14
#define EEPROM_CONFIG_INDEX_OFFSET 40

// end of EEProm Configuration

void readConfigEEPROM(uint16_t startingAddress);
void writeConfigEEPROM(uint16_t startingAddress);


#endif
