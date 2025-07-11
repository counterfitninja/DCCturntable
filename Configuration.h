// Configuration.h

#ifndef CONFIG_H
#define CONFIG_H

#include "configurationVariables.h"

#define COMPANY_NAME "DCCInterface"
#define APPLICATION_NAME "DCCInterface"
#define DCCINTERFACE_URL "www.dccinterface.com"
#define FIRMWARE_CODE "DCC_GOLD_Turntable_NanoEvery"

#define VERSION_NUMBER_MAJOR "3"
#define VERSION_NUMBER_MINOR "0"
#define VERSION_NUMBER_BUILD "3"
#define VERSION_NUMBER_REVISION "2"

#define VERSION_NUMBER_MAJORMINOR VERSION_NUMBER_MAJOR "." VERSION_NUMBER_MINOR
#define VERSION_NUMBER VERSION_NUMBER_MAJORMINOR "." VERSION_NUMBER_BUILD
#define INTERNAL_VERSION_NUMBER VERSION_NUMBER "." VERSION_NUMBER_REVISION

#define TURNTABLE
//#define TRAVERSER

#define SDCARDCONFIGURATION
#define EEPROMCONFIGURATION

#if defined(SDCARDCONFIGURATION)
  #if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega4809__)
    #define SDCARDLIBRARY
    #define LCD12864SCREEN_PRESENT
  #else
    #define PETITFSLIBRARY
  #endif
#endif  

//#define U8GLIBLIBRARY
#define U8G2LIBLIBRARY

#define OOGAUGE 1
//#define NGAUGE 1

#define STEPPER_MOTOR_STEPS 200
//#define STEPPER_MOTOR_STEPS 400

//#define STEPPER_DRIVER_A4988
#define STEPPER_DRIVER_TMC2208

#ifdef STEPPER_DRIVER_TMC2208
  #define STEPPERDIRECTION -1
#endif

#ifdef STEPPER_DRIVER_A4988
  // Uncomment to enable Powering-Off the Stepper if its not running . For TMC2208 this is not required as its silent.
  #define STEPPERDIRECTION 1
#endif

// The lines below define the pins used to connect to the A4988 driver module
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega4809__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
  #define A4988_STEP_PIN      4
  #define A4988_DIRECTION_PIN 5
  #define A4988_ENABLE_PIN 6
#endif  

#if defined(ESP32)
  #define A4988_STEP_PIN      12
  #define A4988_DIRECTION_PIN 14
  #define A4988_ENABLE_PIN    27
#endif  

// Uncomment to enable Powering-Off the Stepper if its not running . For TMC2208 this is not required as its silent.
//#define STEPPER_ENABLE_POWERDOWN 

// Home Position Sensor Input Pin
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega4809__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
  //If an Arduino is used.
  #define HOME_SENSOR_PIN 3
  
#endif

// Home Position Sensor TriggerStates
#define HOME_SENSOR_ACTIVE_STATE CHANGE
#define HOME_SENSOR_TRIGGERED LOW

#if defined(ESP32)
  //If an Arduino is used.
  #define HOME_SENSOR_PIN 33
  // Home Position Sensor TriggerState
  // Wifi reset Pin
  #define WIFI_RESET_PIN 4

  // board reset timeout
  #define ESP32_BOARD_RESET_WAIT 15
#endif


// Max number of Indexes available
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  //If an Arduino Mega is used.
  // Default number of Indexes
  #define NUMBER_OF_INDEXES 10
#endif

// Max number of Indexes available
#if defined(__AVR_ATmega4809__) || defined(ESP32)
  //If an Arduino Nano Every is used.
  // Default number of Indexes
  #define NUMBER_OF_INDEXES 10
#endif

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
  //If an Arduino Uno (or older) is used.

  // Default number of Indexes
  #define NUMBER_OF_INDEXES 10
#endif

#define ALLOW_RESET 1

#ifdef OOGAUGE
  #define MAXSPEED 100.0
  #define ACCELERATION 25
  #define SPEED 75
#endif

#ifdef NGAUGE
  #define MAXSPEED 60
  #define ACCELERATION 25
  #define SPEED 60
#endif

// The A4988 Driver Board has 3 pins that set the Stepping Mode which are connected to 3 jumpers on the board. 
// Uncomment the line below to match the Boards jumper setting        MS1,     MS2,     MS3
// --------------------------------------------------------------------------------------------
//#define MICROSTEPPING             1   // full steps - MS1=OFF, MS2=OFF, MS3=OFF
//#define MICROSTEPPING             2   // 1/2  steps - MS1=ON,  MS2=OFF, MS3=OFF
//#define MICROSTEPPING             4   // 1/4  steps - MS1=OFF, MS2=ON,  MS3=OFF
#define MICROSTEPPING             8   // 1/8  steps - MS1=ON,  MS2=ON,  MS3=OFF
//#define MICROSTEPPING               16  // 1/16 steps - MS1=ON,  MS2=ON,  MS3=ON

// home location
#define HOME_INDEX 0
// spin the table 180
#define SPIN_180_ADDRESS 180
// address starting point
#define POSITION_01_DCC_ADDRESS 200
// reset address
#define RESET_DCC_ADDRESS 300
// addresses where we save the current position to the eeprom, e.g. 310 will save current position for address 210
#define SET_CURRENT_POSITION_01_DCC_ADDRESS POSITION_01_DCC_ADDRESS + 100

// Traverser
#ifdef TRAVERSER
  #define LEAD_SCREW_PITCH 2.0
  #define LEAD_SCREW_STARTS 1
  
  #define STEPS_PER_MM  ((MICROSTEPPING * STEPPER_MOTOR_STEPS) / (LEAD_SCREW_PITCH * LEAD_SCREW_STARTS))

  // Physical Dimenstions
  #define TRAVERSER_LENGTH_MM 400
#endif

// End of Traverser

// Keypad Configuration
#define I2CADDR 0x38

#ifdef KEYPADCONTROLLER_PRESENT

#define KEYPAD_1 '1'
#define KEYPAD_2 '2'
#define KEYPAD_3 '3'
#define KEYPAD_4 '4'
#define KEYPAD_5 '5'
#define KEYPAD_6 '6'
#define KEYPAD_7 '7'
#define KEYPAD_8 '8'
#define KEYPAD_9 '9'
#define KEYPAD_0 '0'
#define KEYPAD_FRONT 'A'
#define KEYPAD_PROGRAM 'B'
#define KEYPAD_CLEAR 'C'
#define KEYPAD_BACK 'D'

#define KEYPAD_MOVECLOCKWISE '#'
#define KEYPAD_MOVEANTICLOCKWISE '*'
#define KEYPAD_CANCEL 'C'
#define KEYPAD_OK 'D'

// End of Keypad Configuration

#endif

#ifdef LCD12864SCREEN_PRESENT
// 12864 LCD Screen and Rotary Configuration

#ifdef U8GLIBLIBRARY
  #include <U8glib.h>
  typedef U8GLIB_ST7920_128X64_1X  LCDLIB;
#endif

#ifdef U8G2LIBLIBRARY
  #include <U8g2lib.h>
  typedef U8G2_ST7920_128X64_1_SW_SPI  LCDLIB;
#endif

#define OP_STATE_INDEX_SEL 0
#define OP_STATE_MOVE_SEL 1

#define OP_STATE_MOVE_HEAD 0
#define OP_STATE_MOVE_TAIL 1
#define OP_STATE_MOVE_BACK 2

// main config or index config
#define CONF_STATE_MAINCONFIG 0
#define CONF_STATE_INDEXCONFIG 1

// are we selecting a config item, or have we selected it
#define CONF_STATE_SELECTING 0
#define CONF_STATE_SELECTED 1

#define CONF_STATE_INDEXES 0
#define CONF_STATE_SPEED 1
#define CONF_STATE_STEPPER 2
#define CONF_STATE_CONFINDEXES 3

// are we selecting an index attribute or amending it
#define CONF_STATE_INDEX_SELECTING 0
#define CONF_STATE_INDEX_SELECTED 1

// what are we configuring on the index
#define CONF_STATE_INDEX_INDEX_NUM 0
#define CONF_STATE_INDEX_HEAD 1
#define CONF_STATE_INDEX_TAIL 2
#define CONF_STATE_INDEX_ADDRESS 3
#define CONF_STATE_INDEX_BACK 4

// pins for connections

#if defined(__AVR_ATmega328P__)
 
 // Rotary
  #define ENCODER_A_PIN         8
  #define ENCODER_B_PIN         7
  #define ENCODER_BUTTON_PIN    9
  
  // Config button
  #define CONFIG_BUTTON_PIN     18
  
  //LCD
  #define DOGLCD_CS             10
  #define DOGLCD_MOSI           11
  #define DOGLCD_SCK            13

#elif defined(__AVR_ATmega4809__)
 
 // Rotary
  #define ENCODER_A_PIN         A1
  #define ENCODER_B_PIN         A2
  #define ENCODER_BUTTON_PIN    A7
  
  // Config button
  #define CONFIG_BUTTON_PIN     A0
  
  //LCD
  #define DOGLCD_CS             7
  #define DOGLCD_MOSI           8
  #define DOGLCD_SCK            9

#elif defined(__AVR_ATmega2560__)
 
 // Rotary
  #define ENCODER_A_PIN         46
  #define ENCODER_B_PIN         31
  #define ENCODER_BUTTON_PIN    35
  
  // Config button
  #define CONFIG_BUTTON_PIN     47
  
  //LCD
  #define DOGLCD_CS             34
  #define DOGLCD_MOSI           33
  #define DOGLCD_SCK            32

#endif

//Drawing defines
#define OPERATION_BOX_WIDTH 42
#define INDEX_BOX_WIDTH 18
#define ENCODER_SPEED 3
#define INDEXES_PER_PAGE 10
#define MESSAGEBOX_OPERATE_SHOW_TIME 60
#define MESSAGEBOX_STANDARD_SHOW_TIME 5

// end of 12864 LCD Screen and Rotary Configuration
#endif

#define EEPROM_STORAGE

#ifdef EEPROM_STORAGE
  #define EXTERNAL_EEPROM 1
//#define INTERNAL_EEPROM 1
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

#ifdef EEPROMCONFIGURATION
  #include "DCCInterface_EEPROMConfiguration.h"
#endif

// end of include files


void setupController();
void processController();

void setDefaultConfiguration( uint16_t pStepperMaxspeed = MAXSPEED, 
                              uint16_t pStepperAcceleration = ACCELERATION, 
                              uint8_t pNumberOfIndexes = NUMBER_OF_INDEXES, 
                              uint16_t pStepperFullSteps = STEPPER_MOTOR_STEPS, 
                              uint8_t pStepperMicroStepping = MICROSTEPPING, 
                              uint8_t pHomeIndex = HOME_INDEX,
                              uint16_t pResetAddress = RESET_DCC_ADDRESS,
                              byte pI2CAddress = I2CADDR);

void loadConfiguration();
void saveConfiguration();

void showMessage(char *strText);


#endif
