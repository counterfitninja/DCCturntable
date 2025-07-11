// DCC turntable Kit Premium and Standard
//
// Author: Ian Jeffery 01/07/2018
// 
// This requires two Arduino Libraries:
//
// 1) The AccelStepper library from: http://www.airspayce.com/mikem/arduino/AccelStepper/index.html
//
// 2) The NmraDcc Library from: http://mrrwa.org/download/
//
// Both libraries can be installed via the Arduino IDE Library Manager 
//

// all configuration variables are in configurationVariables.h
// all configuration is stored in the configuration.h file
#include "configurationVariables.h"
#include "Configuration.h"

void setup()
{
	Serial.begin(115200);fd

	Serial.println("www.dccinterface.com");
  Serial.println(FIRMWARE_CODE); 
	Serial.print("Version Number : ");
  Serial.println(INTERNAL_VERSION_NUMBER);

  delay(1000);

  setupController();
}

void loop()
{
	processController(); 
 
  
}
