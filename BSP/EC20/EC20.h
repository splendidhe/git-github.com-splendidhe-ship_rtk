#ifndef __EC20_H_
#define __EC20_H_
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "main.h"
#include "gpio.h"

extern uint8_t watertemperature; // ˮ��
extern uint8_t temperature;      // �¶�
extern uint8_t humidity;         // ʪ��
extern int seepage;              // ��͸ѹ
extern int waterlevel;           // ˮλ
extern int pressure;             // ��ѹ
extern float windspeed;          // ����
extern float winddirection;      // ����
extern float turbidity;          // �Ƕ�
extern float ph;                 // phֵ
extern int tds;                  // TDS
extern float pitch;              // ������
extern float roll;               // �����
extern float yaw;                // ƫ����
extern double latitude;          // γ��
extern double longitude;         // ����
extern int distance;             // ����

#define UTC_Lendth 11  // UTCʱ�䳤��
#define lat_length 11  // γ�ȳ���
#define LatH_Length 2  // γ�Ȱ��򳤶�
#define lon_Length 12  // ���ȳ���
#define lonH_length 2  // ���Ȱ��򳤶�
#define sta_length 2   // GPS״̬����
#define Time_Lendth 12 // ʱ�䳤��

// GPRMC���������ݽṹ��
typedef struct
{
    char GPS_sta[sta_length];        // GPS״̬
    char UTCtime[UTC_Lendth];        // UTCʱ��
    char latitude[lat_length];       // γ��
    char latitudeHemi[LatH_Length];  // γ�Ȱ���
    char longitude[lon_Length];      // ����
    char longitudeHemi[lonH_length]; // ���Ȱ���
    double lat;                      // γ��
    double lon;                      // ����
    char Time[Time_Lendth];          // ʱ��
} _GPRMC_Typedef;

void Mqtt_Senddata(void);
uint8_t EC20_init_check(uint8_t i);
void PARSE_GPRMC(void);     // ����GPRMC����
void GPS_Analysis(void);    // GPS���ݽ���
void EC20_4G_Reset(void);   // 4Gģ�鸴λ
void Parse_UTCTime(void);   // ����UTCʱ��
void Parse_gps_data(void);  // ����GPS����

#endif
