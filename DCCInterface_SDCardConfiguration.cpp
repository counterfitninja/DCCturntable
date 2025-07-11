#include "Configuration.h"
#include <SPI.h>

#if defined(SDCARDLIBRARY)
  #include <SD.h>
#endif


#if defined(PETITFSLIBRARY)
  #include "PetitFS.h"
  #include "pffArduino.h"
#endif

#include <ini.h>
#include "DCCInterface_SDCardConfiguration.h"
#include "configurationVariables.h"

#if defined(SDCARDLIBRARY)
  File configurationFile;

  
  #if defined(__AVR_ATmega2560__)
    //If an Arduino Mega is used.
    // Default number of Indexes
    #define SD_CS_PIN 53
  #endif

  #if defined(__AVR_ATmega4809__)
    //If an Arduino Mega is used.
    // Default number of Indexes
    #define SD_CS_PIN 10
  #endif

  
#endif

#if defined(PETITFSLIBRARY)
  FATFS fs;
#endif

bool indexValueSet = false;

void getDynamicIndexName(byte index, const char * postName, char returnName[])
{
  char dynamicNameStart[8] = "";
  //char dynamicName[20] = "";

  strcpy(returnName, nameStart);
  char* ptr2 = returnName + strlen(returnName);
  ultoa(index, ptr2, 10);
  strcat(returnName, postName); 
 
}

void checkForIndexName(byte index, const char* name, const char* value)
{
 
  char iniNameAddress[20];
  char iniNameFront[20];

  getDynamicIndexName(index + ARRAYADJUSTER,nameAddress, iniNameAddress);
  getDynamicIndexName(index + ARRAYADJUSTER,nameFront, iniNameFront);
  
  if (strcmp(name,iniNameAddress) == 0)
  {
    controllerIndexes[index].address = atoi(value);
  }

  if (strcmp(name,iniNameFront) == 0)
  {
    controllerIndexes[index].stationFront = atoi(value);
  }
}

static int myini_handler(void* user, const char* section, const char* name, const char* value)
{
  
  char internalValue[5];

  if (strcmp(name,"STEPS") == 0)
  {
    configurationSettings.stepperFullSteps = atoi(value);
  }

  if (strcmp(name,"MAXSPEED") == 0)
  {
    configurationSettings.stepperMaxspeed = atoi(value);
  }

  if (strcmp(name,"ACCEL") == 0)
  {
    configurationSettings.stepperAcceleration = atoi(value);
  }

  if (strcmp(name,"MICROS") == 0)
  {
    configurationSettings.stepperMicroStepping = atoi(value);
  }

  if (strcmp(name,"HOMEINDEX") == 0)
  {
    configurationSettings.homeIndex = atoi(value) - ARRAYADJUSTER;
  }

  if (strcmp(name,"RESETADDRESS") == 0)
  {
    configurationSettings.resetAddress = atoi(value);
  }

  if (strcmp(name,"KEYPADADDRESS") == 0)
  {
    configurationSettings.i2cAdress = strtol(value, NULL, 16);
  }

  if (strcmp(name,"INDEXES") == 0)
  {
    indexValueSet = true;
    configurationSettings.numberOfIndexes = atoi(value);
  }

  if (indexValueSet)
  {
    for (byte i = 0; i < configurationSettings.numberOfIndexes;i++)
    {
      checkForIndexName(i,name,value);  
    }
  }

  return 1;
}

bool SDCardConfiguration_Setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.print("ChipSelect : ");
  Serial.println(SD_CS_PIN);

  #if defined(SDCARDLIBRARY)
    if (!SD.begin(SD_CS_PIN)) 
    {
      Serial.println("Failed to Initialize SD Card. Using Default Configuration");
      return false;
    }

    Serial.println("Opened SD Card config.cfg");

    configurationFile = SD.open("CONFIG.CFG");
    if (!configurationFile)
    {
      Serial.println("Failed to Open SD Card config.cfg");
      return false;  
    } 
    
  #endif

  #if defined(PETITFSLIBRARY)
    // Initialize SD and file system.
    if (pf_mount(&fs)) 
    {
      Serial.println("Failed to Initialize SD Card. Using Default Configuration");
      return false;
    }
    
    // Open test file.
    if (pf_open("CONFIG.CFG")) 
    {
      Serial.println("Failed to Open SD Card config.cfg");
      return false;
    }
  #endif
  

  return true;
}

void SDCardConfiguration_LoadConfiguration()
{
  char buf[BUFFERSIZE];
  char parseBuf[BUFFERSIZE];
  uint16_t currentLinePos = 0;
  bool newLineFound = false;
  uint8_t newLinePosition = 0;
  
  while (1) 
  {
    unsigned int nr;

    #if defined(SDCARDLIBRARY)
      nr = configurationFile.read(buf, sizeof(buf));
    #endif

    #if defined(PETITFSLIBRARY)
      if (pf_read(buf, sizeof(buf), &nr)) 
        Serial.println("Error Reading SD Card");
    #endif
    
    if (nr == 0) 
      break;

    for (byte i=0; i < nr; i++)
    {
      if ((buf[i] == '\n' && i > 0))
      {
        newLinePosition = i;
        currentLinePos = currentLinePos + newLinePosition;
        
        strncpy(parseBuf, buf, i);
       
        parseBuf[i] = '\0';

        if (ini_parse_string(parseBuf, myini_handler, 0) < 0)
        {
          Serial.print("\nIni parse failed");
        }

        #if defined(PETITFSLIBRARY)
          pf_lseek(currentLinePos+1);
        #endif

        #if defined(SDCARDLIBRARY)
          configurationFile.seek(currentLinePos+1);
        #endif
        
        
        newLineFound = true;
      }
      
      if (newLineFound)
      {
        newLineFound = false;
        break;  
      }
    }   
  }
}
