#pragma once
#include "cxm1500geInterface.h"




void getDataWillSend(char* payload_data, CXM1500GENMEAGGAInfo* g_nmea_gga_info_buf,uint16_t dataSens = 0, byte flagJapanCorrection= 0, int battery = 3, uint8_t payload_Type = 0, uint8_t class_Service=0){
  
 // uint16_t data_sens = get_sens_data();
  // uint16_t data_sens = get_waterlevel_sens_data();
 

 int batt = battery;
 memset(payload_data,0xFF,16);
 uint8_t data_sens_8[2];
 memset(data_sens_8, '\0', sizeof(data_sens_8));
 memcpy(data_sens_8, &dataSens, sizeof(dataSens));
 // load data_sens to payload
 if(payload_Type == 1){
   payload_data[15-2] &= (((data_sens_8[0]& 0xFF) << 2 ) | ~0xFC);
   payload_data[15-3] &= (((data_sens_8[0]& 0xFF) >> 6 ) | ~0x03);
   payload_data[15-3] &= (((data_sens_8[1]& 0xFF) << 2 ) | ~0xFC);
 }


  payload_data[15-4] &= ((g_nmea_gga_info_buf->m_pos_status & 0xFF) | ~0x01); // 1 bit + 1bit reserved --> 34
  int count_satelit = g_nmea_gga_info_buf->m_sat_used;
  if(count_satelit > 7) {
    count_satelit = 7;
  }
  payload_data[15-4] &= (((count_satelit & 0xFF) << 2) | ~0x1C); // 3 bit --> 37
  
  payload_data[15-4] &= (((batt & 0xFF) << 5) | ~0xE0); // 3 bit --> 40 

  Fixed atl;
  atl = FLOAT2FIXED(g_nmea_gga_info_buf->m_height); // change floating point to fix point
  uint8_t atl_8[2];
  memset(atl_8, '\0', sizeof(atl_8));
  memcpy(atl_8, &atl, sizeof(atl));
  payload_data[15-5] &= ((atl_8[0] & 0xFF) | ~0xFF); // 8bit 
  payload_data[15-6] &= ((atl_8[1] & 0xFF) | ~0x03); // 2 bit = 10 bit --> 50 

  
  uint8_t src[11];
  memset(src, '\0', sizeof(src));
  strncpy((char*)&src, (char*)g_nmea_gga_info_buf->m_lon, 10);
  uint8_t lon_deg[4];
  memset(lon_deg, '\0', sizeof(lon_deg));
  strncpy((char*)&lon_deg, (char*)&src, 3 );
  uint32_t lon_deg_unit = atoi((char*)lon_deg);
  uint32_t lon_int = lon_deg_unit * 10000;
  
  uint8_t lon_mm[3];
  memset(lon_mm, '\0', sizeof(lon_mm));
  uint8_t lon_ss[5];
  memset(lon_ss, '\0', sizeof(lon_mm));
  strncpy((char*)lon_mm, (char*)&src[3], 2);
  strncpy((char*)lon_ss, (char*)&src[6], 4);
  
  uint32_t lon_mm_unit = atoi((char*)lon_mm);
  uint32_t lon_ss_unit = atoi((char*)lon_ss);
  lon_mm_unit = (lon_mm_unit *10000)/60;
  lon_ss_unit = lon_ss_unit / 60;
  
  
  lon_int = lon_int + lon_mm_unit + lon_ss_unit;
  
  
  memset(src, '\0', sizeof(src));
  strncpy((char*)&src, (char*)g_nmea_gga_info_buf->m_lat, 10);
  uint8_t lat_deg[3];
  memset(lat_deg, '\0', sizeof(lat_deg));
  strncpy((char*)&lat_deg, (char*)&src, 2 );
  uint32_t lat_deg_unit = atoi((char*)lat_deg);
  uint32_t lat_int = lat_deg_unit * 10000;
  
  uint8_t lat_mm[3];
  memset(lat_mm, '\0', sizeof(lat_mm));
  uint8_t lat_ss[5];
  memset(lat_ss, '\0', sizeof(lat_ss));
  strncpy((char*)&lat_mm, (char*)&src[2], 2);
  strncpy((char*)&lat_ss, (char*)&src[5], 4);
  
  uint32_t lat_mm_unit = atoi((char*)lat_mm);
  uint32_t lat_ss_unit = atoi((char*)lat_ss);
  lat_mm_unit = (lat_mm_unit * 10000)/60;
  lat_ss_unit = lat_ss_unit / 60;
  
  
  lat_int = lat_int + lat_mm_unit + lat_ss_unit;
  //Serial.println(lat_int);
  if (flagJapanCorrection == 1){
    if (((lat_int > 864000)&&(lat_int<1044000))&&((lon_int>4410000)&&(lon_int<4752000))){
        lat_int += (864000-1535080);
        lon_int += (4410000-4608000);
    }
  }
  

  uint8_t longitude[4];
  memset(longitude, '\0', sizeof(longitude));
  memcpy(longitude, &lon_int, sizeof(lon_int));
  
  payload_data[15-6] &= (((longitude[0] & 0xFF) << 2) | ~0xFC);// 6 bit
  payload_data[15-7] &= (((longitude[0] & 0xFF) >> 6) | ~0x03);// 2 bit
  payload_data[15-7] &= (((longitude[1] & 0xFF) << 2) | ~0xFC);// 6 bit
  payload_data[15-8] &= (((longitude[1] & 0xFF) >> 6) | ~0x03);// 2 bit
  payload_data[15-8] &= (((longitude[2] & 0xFF) << 2) | ~0x7C);// 6 bit = 21 bit --> 71 bit
  

  uint8_t latitude[4];
  memset(latitude, '\0', sizeof(latitude));
  memcpy(latitude, &lat_int, sizeof(lat_int));
  
  payload_data[15-8] &= (((latitude[0] & 0xFF) << 7) | ~0x80);// 1 bit
  payload_data[15-9] &= (((latitude[0] & 0xFF) >> 1) | ~0x7F);// 7 bit
  payload_data[15-9] &= (((latitude[1] & 0xFF) << 7) | ~0x80);// 1 bit
  payload_data[15-10] &= (((latitude[1] & 0xFF) >> 1) | ~0x7F);// 7 bit
  payload_data[15-10] &= (((latitude[2] & 0xFF) << 7) | ~0x80);// 1 bit
  payload_data[15-11] &= (((latitude[2] & 0xFF) >> 1) | ~0x0F);// 4 bit = 21 bit
  

  memset(src, '\0', sizeof(src));
  strncpy((char*)&src, (char*)g_nmea_gga_info_buf->m_utc, 9 );
  
  uint8_t utc_temp[3];
  memset(utc_temp, '\0', sizeof(utc_temp));
  strncpy((char*)&utc_temp, (char*)&src[0], 2);
  uint8_t utc_hh_unit = atoi((char*)&utc_temp);


  memset(utc_temp, '\0', sizeof(utc_temp));
  strncpy((char*)&utc_temp, (char*)&src[2], 2);
  uint8_t utc_mm_unit = atoi((char*)&utc_temp);

  memset(utc_temp, '\0', sizeof(utc_temp));
  strncpy((char*)&utc_temp, (char*)&src[4], 2);
  int utc_ss_unit = atoi((char*)&utc_temp);
  
  uint8_t utcSS[4];
  memset(utcSS, '\0', sizeof(utcSS));
  memcpy(utcSS, &utc_ss_unit, sizeof(utc_ss_unit));
  
  payload_data[15-11] &= (((utcSS[0] & 0xFF) << 4) | ~0xF0);// 4 bit
  payload_data[15-12] &= (((utcSS[0] & 0xFF) >> 4) | ~0x03);// 2 bit
  payload_data[15-12] &= (((utc_mm_unit & 0xFF) << 2) | ~0xFC);// 6 bit
  payload_data[15-13] &= (((utc_hh_unit & 0xFF)) | ~0x1F);// 5 bit = 17 bit
 
  Fixed hdop = 0;

  hdop = FLOAT2FIXED(g_nmea_gga_info_buf->m_hdop);
  uint8_t hdop_8[2];
  memset(hdop_8, '\0', sizeof(hdop_8));
  memcpy(hdop_8, &hdop, sizeof(hdop));
  
  payload_data[15-13] &= (((hdop_8[0] & 0xFF) << 5) | ~0xE0);// 3 bit
  payload_data[15-14] &= ((payload_Type & 0xFF) | ~0x0F);// 4 bit
  payload_data[15-15] &= (((class_Service & 0xFF)<< 5) | ~0xE0);// 3 bit
  
  printf(payload_data);
  
};
