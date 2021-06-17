#pragma once

#include "Arduino.h"
#include "cxm1500geAPI.h"

// #define DEBUG_LEVEL_2
// #define DEBUG_LEVEL_3

class cxm1500geCore;
byte get_last_word(char *msg, char *word);

class cxm1500geCore
{

public:
  typedef void (cxm1500geCore::*handleUartFunction)(void (cxm1500geCore::*)());
  typedef void (cxm1500geCore::*handleUart)();
  /*--- uart command line ---*/
  #define CMD_SYS_GET_SET "< SYS STT GET\r\n"
  #define CMD_SYS_RESET_SET_ON "< SYS RESET SET ON\r\n"
  #define CMD_SYS_MODE_SET_00 "< SYS MODE SET 00\r\n"
  #define CMD_SYS_MODE_SET_05 "< SYS MODE SET 05\r\n"
  #define CMD_SYS_MODE_SET_07 "< SYS MODE SET 07\r\n"
  #define CMD_SYS_MODE_GET "< SYS MODE GET\r\n"
  
  #define CMD_SYS_EEPROM_SET  "< SYS EEPROM SET"
  #define CMD_SYS_EEPROM_GET_0x0218 "< SYS EEPROM GET 0x0218\r\n"
  #define CMD_SYS_EEPROM_GET_0x0400 "< SYS EEPROM GET 0x0400\r\n"
  #define CMD_SYS_EEPROM_GET_0x0600 "< SYS EEPROM GET 0x0600\r\n"
  #define CMD_SYS_STT_GET "< SYS STT GET\r\n"
  #define CMD_SYS_STT_SET_EVT_ON "< SYS STT SET_EVT ON\r\n"
  #define CMD_SYS_STT_SET_EVT_OFF "< SYS STT SET_EVT OFF\r\n"
  #define CMD_GNSS_HOST_FW_SENT_SET_EVT_0x0001 "< GNSS HOST_FW_SENT SET_EVT 0x0001\r\n"
  #define CMD_GNSS_HOST_FW_SENT_SET_EVT_0x0000 "< GNSS HOST_FW_SENT SET_EVT 0x0000\r\n"
  #define CMD_GNSS_STT_SET_EVT_ON "< GNSS STT SET_EVT ON\r\n"
  #define CMD_GNSS_STT_SET_EVT_OFF "< GNSS STT SET_EVT OFF\r\n"
  #define CMD_SYS_EEPROM_GET "< SYS EEPROM GET"

  #define timeOutDefault 10000
  /**
   * @brief      Constructs the object.
   */
  cxm1500geCore();
  

  byte flagGPSReady = 0; // inisiasi flag GPS ready 0-> off, 1-> on

  byte flagJapanCorrection = 0;

	/**
   * @brief      Sets configure the cxm shield.
   */
  void setCXM();

	/**
   * @brief      Sends a command.
   *
   * @param[in]  cmd   The command
   */
  void sendCmd(const char* cmd);

  /**
   * @brief      Gets the response message.
   *
   * @param      resp  The response
   *
   * @return     The response message.
   */
  byte getResponseMessage(char *resp);


  byte getEventsMessage(char *resp);
  
  /**
   * @brief      function to activate interrupt
   *
   * @param[in]  func  The function
   */
  void attachInterruptCallback(void (*func)());

  /**
   * @brief      function to deactivate interrupt
   */
  void detachInterruptCallback();

  /**
   * @brief      handle UART response form IC CXM
   */
  static void handleUartToGetResponse();
  
  /**
   * @brief      handle UART to collected the massage form CXM
   */
  static void handleUartToGetGNSSReady();

  /**
   * @brief      { handle UART response and event form IC CXM }
   */
  static void handleUartToGetResponseEvent();

  /**
   * @brief      { function_description }
   *
   * @return     { description_of_the_return_value }
   */
  byte checkResponse();


  /**
   * @brief      { function_description }
   *
   * @return     { description_of_the_return_value }
   */
  byte checkEvent();


  /**
   * @brief      { function_description }
   *
   * @return     { description_of_the_return_value }
   */
  byte checkSYSSTT();
  byte sendCMDgetResponse(const char *CMD, char *response, long timeOut = timeOutDefault);

  responseUart_t gResponseUart;
  response_t gResponse;
  events_t gEvent;

	//~cxm1500geCore();
private:

  static cxm1500geCore* addressClass; // save the address the this calls self.

  /**
   * @brief      Gets the buffer interrupt.
   *
   * @param[in]  func  The function
   */
  void getBufferInterrupt(handleUart func);


  /**
   * @brief      Gets the resource event gps ready.
   */
  void getResEvtGPSReady();

  
  String gBuffer = "";

  /**
   * @brief      Sets the enable wakeup CXM.
   */
  void setEnableWakeupCXM();

  
  /**
   * @brief      Gets the response.
   */
  void getResponse();

  /**
   * @brief      Gets the resource event.
   */
  void getResEvt();


  /**
   * @brief      to check the buffer, the buffer save massage form IC CXM.
   *             
   * @return     
   */
  byte checkBuffer();

  /**
   * @brief      checking Buffer to handle waiting the gps ready
   *
   * @return     { description_of_the_return_value }
   */
  byte checkBufferGPSReady();


  /**
   * @brief      { function_description }
   */
  void checkUartResponse();

  
  
	// char gEventGNSS[100];
	// char gGNSS[100];
  
  
  
};
