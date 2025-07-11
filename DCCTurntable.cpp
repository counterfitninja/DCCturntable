#include "DCCTurnTable.h"
#include "Configuration.h"
#include "configurationVariables.h"

#include <Wire.h>
#include <AccelStepper.h>
#include <NmraDcc.h>

volatile bool bInterruptDetected = false;
bool bLastIsRunningState;
bool bHomePositionFound = false;

// Now we'll wrap the stepper in an AccelStepper object
AccelStepper stepperMotorDriver(1, A4988_STEP_PIN, A4988_DIRECTION_PIN);

// DCC Library
NmraDcc  Dcc;
// Last Address variable to stop duplicate events
uint16_t lastAddr = 0xFFFF;
uint8_t lastDirection = 0xFF;
uint8_t lastIndex = 0;

void interruptEvent()
{
  detachInterrupt(digitalPinToInterrupt(HOME_SENSOR_PIN));
  bInterruptDetected = true;
}

void moveToHomePosition()
{
  pinMode(HOME_SENSOR_PIN, INPUT_PULLUP);
  
  stepperMotorDriver.setCurrentPosition(0);

  if (digitalRead(HOME_SENSOR_PIN)== HOME_SENSOR_TRIGGERED)
  {
    Serial.println(F("Moving Bridge Away from Home Sensor"));
    stepperMotorDriver.runToNewPosition(-100);  
  }
  
  
  attachInterrupt(digitalPinToInterrupt(HOME_SENSOR_PIN), interruptEvent, HOME_SENSOR_ACTIVE_STATE);

  bInterruptDetected = false;

  stepperMotorDriver.move(configurationSettings.stepperCompleteRotation()/20);
  // Make sure we move back some steps first... incase we are on Home Sensor
  stepperMotorDriver.runToPosition();
  
  Serial.print(F("Looking for Home Sensor within 2 complete turns ("));
  Serial.print(configurationSettings.stepperCompleteRotation()*2);
  Serial.println(F(")..."));
    
  stepperMotorDriver.move(configurationSettings.stepperCompleteRotation()*2);

  showMessage("Finding Home Sensor..");

  lastAddr = 0xFFFF;
}


// This function is called whenever a normal DCC Turnout Packet is received
void notifyDccAccTurnoutOutput(uint16_t Addr, uint8_t Direction, uint8_t OutputPower)
{
  // Call OperateTurnTable to move turntable
  OperateTurnTable(Addr, Direction);
}

void MoveBridge(uint8_t index, uint8_t Direction)
{
  stepperMotorDriver.enableOutputs();

  if (Direction)
  {
    Serial.print(F("Move Stepper Motor to Position : "));
    Serial.println(controllerIndexes[index].stationFront, DEC);
    stepperMotorDriver.moveTo(controllerIndexes[index].stationFront);
  }
  else
  {
    // move the the opposite position
    Serial.print(F("Move Stepper Motor to Position : "));
    Serial.println(controllerIndexes[index].stationBack, DEC);
    stepperMotorDriver.moveTo(controllerIndexes[index].stationBack);
  }  
}

void MoveBridgeToPosition(uint16_t Position)
{
  Serial.print(F("Stepper Motor Position : "));
  Serial.println(Position, DEC);  

  stepperMotorDriver.enableOutputs();
  stepperMotorDriver.moveTo(Position);
}

void OperateTurnTable(uint16_t Addr, uint8_t Direction)
{
  for (uint8_t i = 0; i < configurationSettings.numberOfIndexes; i++)
  {
    // if we are responding to an index move
    if ((Addr == controllerIndexes[i].address) && ((Addr != lastAddr) || (Direction != lastDirection)) && Addr != RESET_DCC_ADDRESS && Addr != SPIN_180_ADDRESS )
    {
      Serial.print(F("DCC Accessory Address : "));
      Serial.println(Addr, DEC);
      
      lastAddr = Addr;
      lastDirection = Direction;
      lastIndex = i;
      
      // Move the Turntable Bridge
      MoveBridge(i,Direction);
      
      break;
    }
    // if we are performing a reset
    else if ((Addr == configurationSettings.resetAddress) && (Addr != lastAddr))
    {
      // perform a reset
      lastAddr = Addr;
      bHomePositionFound = false;

      Serial.print(F("Rehoming....."));
   
      stepperMotorDriver.enableOutputs();
      
      moveToHomePosition();
      break;
    }
    // if we are spining the turntable 180
    else if (Addr == SPIN_180_ADDRESS)
    {
      // perform a 180
      Serial.print(F("Spin the Bridge 180....."));
      
      // Move the Turntable Bridge
      MoveBridge(lastIndex,!lastDirection);
      // record the fact that we just span it 180
      lastDirection = !lastDirection;
      break;
    }
    
  }
}

void setupStepperDriver()
{
  stepperMotorDriver.setPinsInverted(false, false, true); // Its important that these commands are in this order
  stepperMotorDriver.setEnablePin(A4988_ENABLE_PIN);    // otherwise the Outputs are NOT enabled initially

  setStepperMotorMotionParameters();

  bLastIsRunningState = stepperMotorDriver.isRunning();
}

void setStepperMotorMotionParameters()
{
  stepperMotorDriver.setMaxSpeed(configurationSettings.stepperMaxspeed);
  stepperMotorDriver.setAcceleration(configurationSettings.stepperAcceleration);
}

void setupDCCDecoder()
{
  Serial.println(F("Setting up DCC Decorder..."));

  // Setup which External Interrupt, the Pin it's associated with that we're using and enable the Pull-Up 
  Dcc.pin(2, 1);

  // Call the main DCC Init function to enable the DCC Receiver
  Dcc.init(MAN_ID_DIY, 10, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE, 0);
}



void DisplayIndexValues(byte index)
{
  if (index  == configurationSettings.homeIndex)
  {
    Serial.print(F("Index (Home) : "));   
  }
  else
  {
    Serial.print(F("Index        : "));   
  }
  
  Serial.print(index + ARRAYADJUSTER);
  controllerIndexes[index].ToDebug();  
}

void DisplayIndexes()
{
  Serial.println(F(""));
  for (byte i =0; i<  configurationSettings.numberOfIndexes;i++)
  {
    DisplayIndexValues(i); 
  } 
  Serial.println(F(""));
}

void setupTurnTableController()
{
  Serial.println(F("\nDefault Configuration"));
  setDefaultConfiguration();
  ConfigureIndexes();
  DisplayIndexes();
  setupStepperDriver();

  Serial.println(F("Finding home...."));
  moveToHomePosition();
}

void processTurnTableController()
{
  Dcc.process();
  if (bInterruptDetected)
  {
    bInterruptDetected = false;
    bHomePositionFound = true;

    Serial.println(F("Found Home - Setting Current Position to 0"));

    showMessage("Home Found.");

    stepperMotorDriver.setCurrentPosition(0);

    Serial.print(F("Moving to Entry position "));
    Serial.println(controllerIndexes[configurationSettings.homeIndex].address, DEC);

    stepperMotorDriver.enableOutputs();

    stepperMotorDriver.moveTo(controllerIndexes[configurationSettings.homeIndex].stationFront);

    setupDCCDecoder();
  }

  Dcc.process();

  if (bHomePositionFound)
  {
    // You MUST call the NmraDcc.process() method frequently from the Arduino loop() function for correct library operation
    Dcc.process();
  }

  // Process the Stepper Library
  stepperMotorDriver.run();
  Dcc.process();

  
  #ifdef LCD12864SCREEN_PRESENT
  if  (!stepperMotorDriver.isRunning() && isMessageDisplayed())
  {
    // hide the message box if we are now at the destination and its being shown.
    hideMessageBox();    
    Dcc.process();
  }
  #endif 

#ifdef STEPPER_ENABLE_POWERDOWN
  if (stepperMotorDriver.isRunning() != bLastIsRunningState)
  {
    bLastIsRunningState = stepperMotorDriver.isRunning();
    if (!bLastIsRunningState)
    {
      stepperMotorDriver.disableOutputs();
    }
  }
#endif  
}

void ConfigureIndexes()
{
  for (byte i =0; i<  configurationSettings.numberOfIndexes;i++)
  {
    controllerIndexes[i].address = POSITION_01_DCC_ADDRESS + i; 
    controllerIndexes[i].stationFront = ((configurationSettings.stepperCompleteRotation() / 2) / configurationSettings.numberOfIndexes) * i; 
    //stationBack
    controllerIndexes[i].stationBack = controllerIndexes[i].stationFront + ((configurationSettings.stepperCompleteRotation() / 2)); 
  }
    
  DisplayIndexes();

  loadConfiguration();
}
