
/******************************************************************************/
/* 環境モニター Rev1.00 for 4G Shield                                           */
/*                                          2021.05.28 Created by Sugirani    */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/* WEB Setting  */
#include "a4gim2.h"
const char *server  = "pnps.a-rms.com";
//const char *server  = "dev.iyu.one";
//const char *token = "/api/?token=cHLrL0PGln2lhcgGXe2Ldw&payload="; 
const char *token = "/api/?token=cHLrL0PGln2lhcgGXe2Ldw&payload=%7B%22ID%22:%22SCD0001%22,%22CO2%22:636.803588864,%22Tmp%22:26.69306752,%22Hum%22:49.538444544,%22Rssi%22:-97%7D";
//const char *token = "/test/?token=cHLrL0PGln2lhcgGXe2Ldw&payload=";
const uint16_t port = 443;
//const int port = 80;
//const int port = "";
//const int port = a4gsDEFAULT_PORT;

const char* c_str;

/******************************************************************************/
/* シリアルNo                                                                  */
char asid[] = "SCD0001";

/******************************************************************************/
/* デバッグモードの切り替え　　　　　　　　　　　　　　　　　　　　　　　　　            */
//#define DEBUG_ON

/******************************************************************************/
/* ライブラリ読込   */

#include <TimerOne.h>
#include <Arduino_JSON.h>
//#include "Uri.h"
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




/******************************************************************************/
/* ----- httpGET Command ----- */

char path[500];
char res[a4gsMAX_RESULT_LENGTH+1];
int len;

/******************************************************************************/
/* 各種変数設定 */
// ----- SystemReset -----
#define SRST 7 //7
#define POW4GIM 8  //8
// ----- RetryCount -----
int retry = 0;
// ----- 4GIM ( rssi ) -----
int rssi = -128;
// ----- ステート制御用変数 ----- 
int loop_state;

/******************************************************************************/
/* その他設定  */
String jsonString;
int jsonString_len;
char jsonChar[500];
char encjson[500];

/******************************************************************************/
/* ステート制御用定数 */

#define STATE_4G_WAIT     0
#define STATE_MAIN_LOOP   1

//####################################################
//起動時の処理
//####################################################
void setup() {

  Serial.begin(9600);  // パソコンとのシリアル通信
  delay(3000);  // Wait for Start Serial Monitor
  Serial2.begin(9600); //4gim
  loop_state = STATE_4G_WAIT;
  
  // ----- 4GIM POWER ON/OFF
  digitalWrite(POW4GIM, LOW);
  pinMode(POW4GIM, OUTPUT);
  // ----- End Of 4GIM POWER ON/OFF
  // ----- SystemReset
  digitalWrite(SRST, HIGH);
  pinMode(SRST,OUTPUT);
  // ----- End Of SystemReset
  

  // JSON Setting
  while (!Serial);
  //demoCreation();

  // SCD Setting
  Wire.begin();
  SERIAL.begin(9600);
  SERIAL.println("SCD30 Raw Data");
  scd30.initialize();
  
}

//####################################################
// メイン ルーチン
//####################################################
void loop() {
  
  delay(1);
  
  switch(loop_state){
    case STATE_4G_WAIT: // 4Gシールドの起動
      #ifndef DEBUG_ON
        Serial.println("4GIM Power Off");
        digitalWrite(POW4GIM, HIGH);
        delay(5000);
        Serial.println("4GIM Power On");
        digitalWrite(POW4GIM, LOW);
        delay(5000);
        Serial.println("Ready.");
        Serial.print("Initializing.. ");
        while(!(a4gs.start(POW4GIM) == 0 && a4gs.begin() == 0)) ;
        Serial.println("Succeeded.");
      #endif
      loop_state = STATE_MAIN_LOOP;
      delay(5000);
    break;
    
    case STATE_MAIN_LOOP: // メインループ

/****************************************************************************/
//電波強度取得   RSSI

      #ifndef DEBUG_ON
        if (a4gs.getRSSI(rssi) == 0) {
          Serial.print("RSSI = ");
          Serial.print(rssi);
          Serial.println(" dBm");
        }
        else {
          Serial.println("Failed.");
          rssi = -128;
        }
      #endif
      #ifndef DEBUG_ON
        Serial.print("RETRY = ");
        Serial.println(retry);
        if(retry>=10){
          Serial.println("4GIM Power Off");
          digitalWrite(POW4GIM, HIGH);
          delay(5000);
          Serial.println("4GIM Power On");
          digitalWrite(POW4GIM, LOW);
          delay(5000);
          Serial.println("Ready.");
          Serial.print("Initializing.. ");
          while(!(a4gs.start() == 0 && a4gs.begin()==0)) ;
          Serial.println("Succeeded.");
        } 

/******************************************************************************/
// SCD30 sequencesequence

    float result[3] = {0};

    if (scd30.isAvailable()) {
        scd30.getCarbonDioxideConcentration(result);
        SERIAL.println(" ");
        SERIAL.println("****** SCD30 creation ****** ");
        SERIAL.print("Carbon Dioxide Concentration is: ");
        SERIAL.print(result[0]);
        SERIAL.println(" ppm");
        SERIAL.println(" ");
        SERIAL.print("Temperature = ");
        SERIAL.print(result[1]);
        SERIAL.println(" ℃");
        SERIAL.println(" ");
        SERIAL.print("Humidity = ");
        SERIAL.print(result[2]);
        SERIAL.println(" %");
        SERIAL.println(" ");
        SERIAL.println(" ");
    }

/******************************************************************************/
//JSON OBJECT CREATE

//String jsonString;

    Serial.println("****** JSON creation ******");
    //String jsonString;
    JSONVar myObject;

    myObject["ID"] = asid; //シリアルNo
    myObject["CO2"] = result[0];
    myObject["Tmp"] = result[1];
    myObject["Hum"] = result[2];
    myObject["Rssi"] = rssi;
    
    Serial.print("myObject.keys() = ");
    Serial.println(myObject.keys());

    // JSON.stringify(myVar) can be used to convert the json var to a String
    jsonString = JSON.stringify(myObject);

    Serial.print("JSON.stringify(myObject) = ");
    Serial.println(jsonString);

    Serial.println();
    Serial.println();

    delay(60000); //60sec loop

//Convert String to Charactor
    //int jsonString_len = jsonString.length() + 1;
    jsonString_len = jsonString.length() + 1;
    //char jsonChar[jsonString_len];
 
    jsonString.toCharArray(jsonChar, jsonString_len);

    Serial.print("jsonString_len = ");
    Serial.println(jsonString_len);

    Serial.print("jsonChar = ");
    Serial.println(jsonChar); //確認


    len = sizeof(res);
    Serial.print("len = ");
    Serial.println(len);
    
    sprintf(path,"%s%s",token,jsonChar);
    Serial.print("path = ");
    Serial.println(path);
    Serial.println();
    Serial.println();

    Serial.print("port = ");
    Serial.println(port);
    Serial.println();
    Serial.println();

// encodeURL
    String encodedUri = uriEncode(jsonChar);
    
    //send_data();

      #endif
    break;
  }
}

/******************************************************************************/
/* サブルーチン（サーバーへの送信）                                           */
/******************************************************************************/
void send_data(){
  #ifndef DEBUG_ON

  //Serial.print("jsonChar = ");
  //Serial.println(jsonChar);
  
    Serial.print("httpGET(server,port,path,res,len,true) requesting.. ");
    len = sizeof(res);
    //sprintf(path,"%s%s",token,jsonChar);
    sprintf(path,"%s",token);
    Serial.println(path);
    if (a4gs.httpGET(server, port, path, res, len, true) == 0) {
      Serial.println("OK!");
      Serial.println(res);
      retry=0;
      if(strncmp(res,"11",2)==0){
        Serial.println("state 11");
        Serial.print("httpGET(server,port,path,res,len,true) requesting.. ");
        len = sizeof(res);
        //sprintf(path,"%s%s",token,jsonChar);
        sprintf(path,"%s",token);
        Serial.println(path);
        if (a4gs.httpGET(server, port, path, res, len, true) == 0) {
          Serial.println("OK!");
        }
        else {
          Serial.print("Can't get HTTP response from ");
          Serial.println(server);
        }
        Serial.println("Arduino System Reset");
        digitalWrite(SRST, LOW);
        }
      else {
        Serial.println("state xx");
      }
    }
    else {
      Serial.print("Can't get HTTP response from ");
      Serial.println(server);
      Serial.println("Ready.");
      Serial.print("Initializing.. ");
      while(!(a4gs.start() == 0 && a4gs.begin()==0)) ;
      Serial.println("Succeeded.");
      retry=retry+1;
      Serial.print("httpGET(server,port,path,res,len,true) requesting.. ");
      len = sizeof(res);
      //sprintf(path,"%s%s",token,jsonChar);
      sprintf(path,"%s",token);
      Serial.println(path);
      if (a4gs.httpGET(server, port, path, res, len, true) == 0) {
        Serial.println("OK!");
        Serial.println(res);
        retry=0;
      }
      else {
        retry=retry+1;
      }
    }
  #endif
  return;
}

String uriEncode (String str)
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i =0; i < str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '%20';
      } else if (isalnum(c)){
        encodedString+=c;
      } else if (c == '.'){
        encodedString+=c;
      } else if (c == '-'){
        encodedString+=c;
      } else if (c == ':'){
        encodedString+=c;
      } else if (c == ','){
        encodedString+=c;
      } else {
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
      }
      yield();
    }
    return encodedString;
}
