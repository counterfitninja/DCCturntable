// DCCInterface_SDCardConfiguration.h

#ifndef SDCARDCONFIG_H
#define SDCARDCONFIG_H

#define CONFIGURATION_FILENAME "config.cfg"

// defines for ini file lookup
#define BUFFERSIZE 40
#define IndexNameStart "IN_"
#define IndexNameAddress "_ADDR"
#define IndexNameFront "_FRNT"
#define ARRAYADJUSTER 1
#define LEDPin 13


const char nameStart[] = IndexNameStart;
const char nameAddress[] = IndexNameAddress;
const char nameFront[] = IndexNameFront;  


bool SDCardConfiguration_Setup();
void SDCardConfiguration_LoadConfiguration();

#endif
