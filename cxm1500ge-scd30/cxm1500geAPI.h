#pragma once
#include "Arduino.h"

#define NMEA_TAKER_ID_SIZE       (2)
#define NMEA_SENTENCE_ID_SIZE    (3)
#define NMEA_UTC_SIZE            (9)
#define NMEA_LAT_SIZE            (9)
#define NMEA_N_S_SIZE            (1)
#define NMEA_LON_SIZE            (10)
#define NMEA_E_W_SIZE            (1)
#define NMEA_POS_STATUS          (1)
#define NMEA_SAT_USED            (2)
#define NMEA_HEIGHT_UNIT_SIZE    (1)
#define NMEA_GEOID_UNIT_SIZE     (1)
#define NMEA_DGPS_DATA_SIZE      (9)
#define NMEA_STATUS_SIZE         (1)
#define NMEA_DATE_SIZE           (7)
#define NMEA_POS_MODE_SIZE       (1)
#define NMEA_CS_SIZE             (2)
#define NMEA_GNS_MODE_SIZE       (2)
#define NMEA_MODE_SIZE           (1)
#define NMEA_T_SIZE              (1)
#define NMEA_M_SIZE              (1)
#define NMEA_N_SIZE              (1)
#define NMEA_K_SIZE              (1)

typedef struct
{
	String message;
	byte typeMessage;
} responseUart_t;

typedef struct
{
	responseUart_t response;
	byte typeResponse;
	byte flag = 0;
} response_t;

typedef struct
{
	responseUart_t events;
	byte typeEvent;
	byte flag = 0;
} events_t;

typedef struct {
    char m_talker_id[NMEA_TAKER_ID_SIZE + 1];	       // トーカID
    char m_sentence_id[NMEA_SENTENCE_ID_SIZE + 1];    // センテンスID
    char m_utc[NMEA_UTC_SIZE + 1];                    // UTC
    char m_lat[NMEA_LAT_SIZE + 1];                    // 緯度
    char m_n_s[NMEA_N_S_SIZE + 1];                    // N：北緯/S：南緯
    char m_lon[NMEA_LON_SIZE + 1];                    // 経度
    char m_e_w[NMEA_E_W_SIZE + 1];                    // E：東経/W：西経
    char m_pos_status_str[NMEA_POS_STATUS + 1 ];
    int m_pos_status;                            	  // 品質指標
    char m_sat_used_str[NMEA_SAT_USED + 1];
    int m_sat_used;                                		// 使用衛星数
    char* m_hdop_str;                                        // HDOP（Horizontal dilution of precision）：水平精度低下度
    float m_hdop;                                        // HDOP（Horizontal dilution of precision）：水平精度低下度
    char* m_height_str;                                      // 標高
    float m_height;                                      // 標高
    char m_height_unit[NMEA_HEIGHT_UNIT_SIZE + 1];   	 // M：標高の単位
    float m_geoid;                                       // ジオイド面との差
    char m_geoid_unit[NMEA_GEOID_UNIT_SIZE + 1];      // M：ジオイド面との差の単位
    char m_dgps_data[NMEA_DGPS_DATA_SIZE + 1];        // DGPSデータ
    int m_bs_id;                                    	// 基地局ID
    int m_cs;                                      		// チェックサム
} CXM1500GENMEAGGAInfo;