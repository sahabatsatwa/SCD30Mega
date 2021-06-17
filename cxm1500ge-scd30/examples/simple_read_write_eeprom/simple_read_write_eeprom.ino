
/*
 * this program sample is using CXM1500GE LPWA IC library with Arduino ATmega2560
 * the library is using pin D9, D12, D18, D19. Do not reuse them.
 * this example is using library version 0.2
*/

#include "cxm1500geInterface.h"
#include <stdio.h>
#include "generatePayload.h"

cxm1500geInterface *eltres;

class myEltresCallback : public cxm1500geCallback
{
	
};
char version[6];
char resp[100];

void setup()
{
  /* This part is belongs to ELTRES Library
   * do not modify it.
   */
  Serial.begin(115200);
  eltres = new cxm1500geInterface(1, "10"); // can remove reboot 24 hours. example eltres = new cxm1500geInterface(1); ->  this main variable timeReboot is NULL
  eltres->setCallback(new myEltresCallback); // this function must call to set callback class

  eltres->getVersion(version);
  Serial.print(F("Library's version  = "));
  Serial.print(version);
  Serial.print("\n");
  eltres->setInitSys();
  eltres->setSysReset();
  eltres->setSysMode(5);
  char valueEeprom[11];
  eltres->writeSysEeprom("0x0210", "0x00000000");
  eltres->readSysEeprom("0x0210", valueEeprom);

  /* --end of ELTRES Library */  

}

void loop()
{

  
}
