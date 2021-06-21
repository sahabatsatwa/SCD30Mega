
/*
 * this program sample is using CXM1500GE LPWA IC library with Arduino ATmega2560
 * the library is using pin D9, D12, D18, D19. Do not reuse them.
 * this example is using library version 0.2
*/

#include "cxm1500geInterface.h"
#include <stdio.h>
#include "generatePayload.h"
#include "SCD30.h"

#if defined(ARDUINO_ARCH_AVR)
    #pragma message("Defined architecture for ARDUINO_ARCH_AVR.")
    #define SERIAL Serial
#elif defined(ARDUINO_ARCH_SAM)
    #pragma message("Defined architecture for ARDUINO_ARCH_SAM.")
    #define SERIAL SerialUSB
#elif defined(ARDUINO_ARCH_SAMD)
    #pragma message("Defined architecture for ARDUINO_ARCH_SAMD.")
    #define SERIAL SerialUSB
#elif defined(ARDUINO_ARCH_STM32F4)
    #pragma message("Defined architecture for ARDUINO_ARCH_STM32F4.")
    #define SERIAL SerialUSB
#else
    #pragma message("Not found any architecture.")
    #define SERIAL Serial
#endif

cxm1500geInterface *eltres;

float tempCo2;
float tempTemperature;
float tempHum;

byte flagReadySend = 0;

class myEltresCallback : public cxm1500geCallback
{
  void onGPSconnected()
  {
    Serial.println("---GPS connected---");
    flagReadySend = 1;
  };
  /*the SetDataSend function edit-able,  but the structure of function can't edit*/
  void onSetDataSend(char *dataWillSend, CXM1500GENMEAGGAInfo *GGAInfo, byte japanCorrection)
  {
    float result[3] = {0};

    if (scd30.isAvailable()) {
      tempCo2 = result[0];
      tempTemperature = result[1];
      tempHum = result[2];
    }

    tempCo2 *= 10;
    tempTemperature *= 10;
    tempHum *= 10;

    int16_t hum = (int16_t) tempHum;
    int16_t temperature = (int16_t) tempTemperature;
    int16_t co2 = (int16_t) tempCo2;    

    byte typePayload = 0;
    byte classService = 0;
    getDataWillSend(dataWillSend, GGAInfo, japanCorrection, typePayload, classService, hum, temperature, co2);
  };
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
  eltres->init();
  eltres->setStateEvent(1);

  /* --end of ELTRES Library */

  scd30.initialize();
}

byte test = 0;
void loop()
{

  if (flagReadySend == 1)
  {
    eltres->send(); // This function to send payload if GPS ready, the payload was setting on onSetDataSend function.
    eltres->sleepCxm(80000, 100000);
    delay(10);
  }
  else
  {
    eltres->checkSYSEvent();
  }
  /*
    86400000 milliseconds in a day
    3600000 milliseconds in an hour
    60000 milliseconds in a minute
    1000 milliseconds in a second
    */
  eltres->rebootInterval(1800000); // 30min Reboot for speed check. if no-reboot interval, can remove this function
                                   /* note :  if reboot interval active in log appear message "--- Reboot Interval "
       but reboot 24 hours base UTC active in log appear message "--- Reboot 24 Hours Time ---"
       and reboot also can reboot-interval only.
    */
}
