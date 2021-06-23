
/*
 * this program sample is using CXM1500GE LPWA IC library with Arduino ATmega2560
 * the library is using pin D9, D12, D18, D19. Do not reuse them.
 * this example is using library version 0.2
*/

#include "cxm1500geInterface.h"
#include <stdio.h>
#include "generatePayload_scd30.h"
#include <Adafruit_SCD30.h>

cxm1500geInterface *eltres;
Adafruit_SCD30 scd30;

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
    if (scd30.dataReady()) {
      if (!scd30.read()){ Serial.println("Error reading sensor data"); return; }
      Serial.print("Temperature: ");
      Serial.print(scd30.temperature);
      Serial.println(" degrees C");
    
      Serial.print("Relative Humidity: ");
      Serial.print(scd30.relative_humidity);
      Serial.println(" %");
    
      Serial.print("CO2: ");
      Serial.print(scd30.CO2, 3);
      Serial.println(" ppm");
      Serial.println("");

      tempHum = scd30.relative_humidity;
      tempTemperature = scd30.temperature;
      tempCo2 = scd30.CO2;

      tempHum *= 10;
      tempTemperature *= 10;
      tempCo2 *= 10;
    }

    int16_t hum = (int16_t) tempHum;
    int16_t temperature = (int16_t) tempTemperature;
    int16_t co2 = (int16_t) tempCo2;    

    byte typePayload = 8;
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

  if (!scd30.begin()) {
    Serial.println("Failed to find SCD30 chip");
    while (1) { delay(10); }
  }
  Serial.println("SCD30 Found!");
  Serial.print("Measurement Interval: "); 
  Serial.print(scd30.getMeasurementInterval()); 
  Serial.println(" seconds");

  eltres = new cxm1500geInterface(1, "10"); // can remove reboot 24 hours. example eltres = new cxm1500geInterface(1); ->  this main variable timeReboot is NULL
  eltres->setCallback(new myEltresCallback); // this function must call to set callback class

  eltres->getVersion(version);
  Serial.print(F("Library's version  = "));
  Serial.print(version);
  Serial.print("\n");
  eltres->init();
  eltres->setStateEvent(1);

  /* --end of ELTRES Library */
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
