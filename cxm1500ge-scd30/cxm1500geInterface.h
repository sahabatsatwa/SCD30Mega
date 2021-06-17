#pragma once

#include "cxm1500geCore.h"

class cxm1500geCallback;
class cxm1500geInterface;
typedef void (cxm1500geInterface::*handleMessageResponse)(const char*);
typedef void (cxm1500geCallback::*handleResponseCallback)(const char*);
typedef uint16_t Fixed;
#define FRACT_BITS 0
#define FLOAT2FIXED(x) ((uint16_t)((x) * (1 << FRACT_BITS)))

#define FRACT_BITS_sens 0
#define FLOAT2FIXED_sens(data) ((uint16_t)((data) * (1 << FRACT_BITS_sens)))


class cxm1500geInterface : public cxm1500geCore
{
public:
	#define TIMEDELAY 1000
	cxm1500geInterface(byte jpCorrection, const char* timeReboot=NULL);
	// ~cxm1500geInterface();

	byte flagInterrupt = 0;
	byte flagSysSTTEVT = 0; // inisiasi flag Event 0-> off, 1-> on
	byte flagGPSconnected = 0;
	byte flagGNSSFWHostEVT = 0;
	byte gpsCorrection = 0;

	byte flagTimerWK = 0;
	byte flagTimerSleep = 0;

	byte flagSetGNSS = 0; // inisiasi flag set GNSS Event 0-> off, 1-> on
	byte flagSetGNSSSTT = 0; // inisiasi flag set GNSS STT Event 0-> off, 1-> on
	byte flagResponse = 0; // inisiasi flag ready get Response 0-> off, 1-> on
	byte flagEventSTT = 0; // inisiasi flag ready get event stt 0-> off, 1-> on
	byte flagGNSS = 0; // inisiasi flag ready get GNSS data 0-> off, 1-> on
	byte flagSTTGNSS = 0; // inisiasi flag ready get STT GNSS event 0-> off, 1-> on
	
	byte flagSendPayload = 0; // inisiasi flag send payload 0-> off, 1-> on
	byte flagSleep = 0; // inisiasi flag send payload 0-> off, 1-> on

	/*to */
	void getVersion(char *ver);
	byte init();
	void setCallback(cxm1500geCallback *Callback);
	byte send();

	/* General Function*/
	
	byte checkSYSEvent();
	
	
	void sleepCxm(long timesInterfalSleep, long activTimes);
	
	void rebootInterval(unsigned long time);
	float getADC(byte channel);
	// void setHandleResponse(handleMessageResponse handleFunc);
	void setStateEvent(byte set);
	void setSysMode(byte set);
	void setSysReset();
	void initModeNormal();
	void setInitSys();
	byte readSysEeprom(const char *addressEeprom, char *result);
	byte writeSysEeprom(const char *addressEeprom, const char *value);
	byte getSysMode();

private:
	// handleMessageResponse m_pHandleResponse = nullptr;
	long timeReset = 0;
	
	unsigned long timeStartWk;
	unsigned long timeStartSlp;
	unsigned long timerWK;
	unsigned long timerSleep;
	unsigned long startCurrentTime;

	cxm1500geCallback *m_pCallback = nullptr;

	
	byte setGNSSFWON();
	byte setGNSSFWOFF();
	
	byte sendCMDHandleResponse(const char* CMD, handleMessageResponse func);
	
	byte sendCMDHandleResponseGetLastValue(const char* CMD, handleMessageResponse func, char* value);

	//byte stringCMD(const char* groupCMD, const char* targetString, const char* valueString, char* messageResponse, handleMessageResponse functionHandle, char* outputValue);
	byte checkSetup();
	void sendPayload(char* payload);


	byte setSysSTTEVTOff();
	byte setSysSTTEVTOn();

	// byte get_last_word(char *msg, char *word);
	// handle response
	void printResponse(const char* responseMessage);
	void handleSetModeNormal(const char *response);
	void handleResetSet(const char *response);
	void handleSetModeWriteEeprom(const char *response);
	void handleReadSysEeprom(const char *response);
	void handleGetSysMode(const char *response);
};

class cxm1500geCallback
{
public:
	
	//virtual ~cxm1500geCallback();
	virtual void onSetDataSend(char* dataWillSend, CXM1500GENMEAGGAInfo* GGAInfo, byte japanCorrection);
	virtual void onGPSconnected();
	virtual void onGPSdisconnected();
	virtual void onFechingTime();
	virtual void onReset();
	virtual void onDATAGNSS();
	virtual void onEndWakeUpCXM();
	virtual void onEndSleepCXM();
	virtual void onSetModeNormal(const char *response);
	virtual void onResetSet(const char *response);
	virtual void onSetModeWriteEeprom(const char *response);
	virtual void onReadSysEeprom(const char *response);
	virtual void onGetSysMode(const char *response);
};


