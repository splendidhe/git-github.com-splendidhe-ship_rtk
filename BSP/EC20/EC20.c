#include "ec20.h"
#include <stdlib.h>
#include <string.h>
#include "usart.h"
#include <stdio.h>
#include "delay.h"
#include "cJSON.h"
#include "math.h"

uint8_t watertemperature = 0; // ˮ��
int seepage = 0;         // ��ˮ
int pressure = 0;        // ��ѹ
float windspeed = 0;     // ����
float turbidity = 0;     // �Ƕ�
float ph = 0;            // phֵ
int tds = 0;             // TDSֵ
float pitch = 0;         // ������
float roll = 0;          // �����
float yaw = 0;           // ƫ����
double latitude = 0;     // γ��
double longitude = 0;    // ����
int distance = 0;        // ����

static char *strx;
static uint8_t count = 0; // �������

void EC20_4G_Reset(void) // ����ģ��
{
    printf("AT+CFUN=1,1\r\n"); // ����ATָ�� ����ģ�� ��ϵ�
}

uint8_t EC20_init_check(uint8_t res)
{
    USART2_RX_Flag = 0;
    count = res;           // ��ȡ��ʼ��״̬
    uint8_t reset_cnt = 0; // ������������
    while (count != 0x0F)
    {
        switch (count)
        {
        case 0:
            memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF));                 // ��ջ���
            printf("ATI\r\n");                                               // ����ATָ��
            delay_ms(300);                                                   // ��ʱ300ms
            strx = strstr((const char *)&USART2_RX_BUF, (const char *)"OK"); // �ж��Ƿ���OK
            printf1(">--EC20-->> ATI\r\n");                                  // ����ATָ��
            printf1("<--EC20--<< \r\n");
            printf1(" %s\r\n", USART2_RX_BUF);
            delay_ms(300);  // ��ʱ300ms
            HAL_Delay(500); // ��ʱ500ms
            USART2_RX_Flag = 0;
            reset_cnt++;
            if (strx != NULL) // �ж��Ƿ���OK
            {
                count = 1; // ��OK��������һ��
            }
            else
            {
                count = 0; // û��OK�����·���ATָ��
                if (reset_cnt % 3 == 1)
                {
                    printf1(">--print-->> EC20 RESET\r\n");
                    printf("\r\n");
                    EC20_4G_Reset(); // ����ģ��
                }
                delay_ms(3000); // ��ʱ3s
                break;
            }
            // break;
        case 1:
            memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF));                // ��ջ���
            printf("ATE0\r\n");                                             // ����ATE0ָ�� �رջ���
            delay_ms(300);                                                  // ��ʱ300ms
            strx = strstr((const char *)USART2_RX_BUF, (const char *)"OK"); // �ж��Ƿ���OK
            printf1(">--EC20-->> ATE0\r\n");                                // ����ATE0ָ�� �رջ���
            delay_ms(300);
            printf1("<--EC20--<< \r\n");
            printf1(" %s\r\n", USART2_RX_BUF);
            HAL_Delay(500); // ��ʱ500ms
            USART2_RX_STA = 0;
            if (strx != NULL)
            {
                count = 2;
            }
            else
            {
                count = 1;
                break;
            }
            // break;
        case 2:
            memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF)); // ��ջ���
            printf("AT+CPIN?\r\n");                          // ����AT+CPIN?ָ�� ���SIM���Ƿ����
            delay_ms(300);                                   // ��ʱ300ms
            strx = strstr((const char *)USART2_RX_BUF, (const char *)"READY");
            printf1(">--EC20-->> AT+CPIN?\r\n"); // ����AT+CPIN?ָ�� ���SIM���Ƿ����
            delay_ms(300);
            printf1("<--EC20--<< \r\n");
            printf1(" %s\r\n", USART2_RX_BUF);
            HAL_Delay(500); // ��ʱ500ms
            USART2_RX_STA = 0;
            if (strx != NULL)
            {
                count = 3;
            }
            else
            {
                count = 2;
                break;
            }
            // break;
        case 3:
            memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF)); // ��ջ���
            printf("AT+CREG?\r\n");                          // ����AT+CREG?ָ�� ��������Ƿ�ע��
            delay_ms(300);                                   // ��ʱ300ms
            strx = strstr((const char *)USART2_RX_BUF, (const char *)"+CREG: 0,1");
            printf1(">--EC20-->> AT+CREG?\r\n"); // ����AT+CREG?ָ�� ��������Ƿ�ע��
            delay_ms(300);
            printf1("<--EC20--<< \r\n");
            printf1(" %s\r\n", USART2_RX_BUF);
            HAL_Delay(500); // ��ʱ500ms
            USART2_RX_STA = 0;
            if (strx != NULL)
            {
                count = 4;
            }
            else
            {
                count = 3;
                break;
            }
            // break;
        case 4:
            memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF)); // ��ջ���
            printf("AT+CSQ\r\n");                            // ����AT+CSQָ�� ����ź�ǿ��
            delay_ms(300);                                   // ��ʱ300ms
            strx = strstr((const char *)USART2_RX_BUF, (const char *)"+CSQ:");
            printf1(">--EC20-->> AT+CSQ\r\n"); // ����AT+CSQָ�� ����ź�ǿ��
            delay_ms(300);
            printf1("<--EC20--<< \r\n");
            printf1(" %s\r\n", USART2_RX_BUF);
            HAL_Delay(500); // ��ʱ500ms
            USART2_RX_STA = 0;
            if (strx != NULL)
            {
                count = 5;
            }
            else
            {
                count = 4;
                break;
            }
            // break;
        case 5:
            memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF)); // ��ջ���
            printf("AT+CGREG?\r\n");                         // ����AT+CGREG?ָ�� ����Ƿ�ע�ᵽGPRS����
            delay_ms(300);                                   // ��ʱ300ms
            strx = strstr((const char *)USART2_RX_BUF, (const char *)"+CGREG: 0,1");
            printf1(">--EC20-->> AT+CGREG?\r\n"); // ����AT+CGREG?ָ�� ����Ƿ�ע�ᵽGPRS����
            delay_ms(300);
            printf1("<--EC20--<< \r\n");
            printf1(" %s\r\n", USART2_RX_BUF);
            HAL_Delay(500); // ��ʱ500ms
            USART2_RX_STA = 0;
            if (strx != NULL)
            {
                count = 6;
            }
            else
            {
                count = 5;
                break;
            }
            // break;
        case 6:
            memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF)); // ��ջ���
            printf("AT+CGATT?\r\n");                         // ����AT+CGATT?ָ�� ����Ƿ�������
            delay_ms(300);                                   // ��ʱ300ms
            strx = strstr((const char *)USART2_RX_BUF, (const char *)"+CGATT: 1");
            printf1(">--EC20-->> AT+CGATT?\r\n"); // ����AT+CGATT?ָ�� ����Ƿ�������
            delay_ms(300);
            printf1("<--EC20--<< \r\n");
            printf1(" %s\r\n", USART2_RX_BUF);
            HAL_Delay(500); // ��ʱ500ms
            USART2_RX_STA = 0;
            if (strx != NULL)
            {
                count = 7;
            }
            else
            {
                count = 6;
                break;
            }
            // break;
        case 7:
            memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF)); // ��ջ���
            printf("AT+QGPS=1\r\n");                         // ����AT+QGPS=1ָ�� ��GPS
            delay_ms(300);                                   // ��ʱ300ms
            strx = strstr((const char *)USART2_RX_BUF, (const char *)"OK");
            printf1(">--EC20-->> AT+QGPS=1\r\n"); // ����AT+QGPS=1ָ�� ��GPS
            delay_ms(300);
            printf1("<--EC20--<< \r\n");
            printf1(" %s\r\n", USART2_RX_BUF);
            HAL_Delay(500); // ��ʱ500ms
            USART2_RX_STA = 0;
            count = 8;
            // if(strx != NULL)
            // {
            //     count = 8;
            // }
            // else
            // {
            //     count = 7;
            //     break;
            // }
            // break;
        case 8:
            memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF)); // ��ջ���
            printf("AT+QMTDISC=0\r\n");                      // ����AT+QMTDISC=0ָ�� �Ͽ�MQTT����
            delay_ms(300);                                   // ��ʱ300ms
            strx = strstr((const char *)USART2_RX_BUF, (const char *)"OK");
            printf1(">--EC20-->> AT+QMTDISC=0\r\n"); // ����AT+QMTDISC=0ָ�� �Ͽ�MQTT����
            delay_ms(300);
            printf1("<--EC20--<< \r\n");
            printf1(" %s\r\n", USART2_RX_BUF);
            USART2_RX_STA = 0;
            HAL_Delay(500); // ��ʱ500ms
            if (strx != NULL)
            {
                count = 9;
            }
            else
            {
                count = 8;
                break;
            }
            // break;
        case 9:
            memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF)); // ��ջ���
            printf("AT+QIDEACT=1\r\n");                      // ����AT+QIDEACT=1ָ�� �Ͽ�GPRS����
            delay_ms(300);                                   // ��ʱ300ms
            strx = strstr((const char *)USART2_RX_BUF, (const char *)"OK");
            printf1(">--EC20-->> AT+QIDEACT=1\r\n"); // ����AT+QIDEACT=1ָ�� �Ͽ�GPRS����
            delay_ms(300);
            printf1("<--EC20--<< \r\n");
            printf1(" %s\r\n", USART2_RX_BUF);
            HAL_Delay(500); // ��ʱ500ms
            USART2_RX_STA = 0;
            if (strx != NULL)
            {
                count = 10;
            }
            else
            {
                count = 8;
                break;
            }
            // break;
        case 10:
            memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF)); // ��ջ���
            printf("AT+QIACT=1\r\n");                        // ����AT+QICLOSE=0ָ�� �Ͽ�TCP����
            delay_ms(300);                                   // ��ʱ300ms
            strx = strstr((const char *)USART2_RX_BUF, (const char *)"OK");
            printf1(">--EC20-->> AT+QIACT=1\r\n"); // ����AT+QICLOSE=0ָ�� �Ͽ�TCP����
            delay_ms(300);
            printf1("<--EC20--<< \r\n");
            printf1(" %s\r\n", USART2_RX_BUF);
            USART2_RX_STA = 0;
            HAL_Delay(500); // ��ʱ500ms
            if (strx != NULL)
            {
                count = 11;
            }
            else
            {
                count = 8;
                break;
            }
            // break;
        case 11:
            memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF)); // ��ջ���
            printf("AT+QMTCFG=\"qmtping\",0,30\r\n");        // ����AT+QMTCFG='qmtping',0,30ָ�� ����MQTT����ʱ��Ϊ30��
            delay_ms(300);                                   // ��ʱ300ms
            strx = strstr((const char *)USART2_RX_BUF, (const char *)"OK");
            printf1(">--EC20-->> AT+QMTCFG='qmtping',0,30\r\n"); // ����AT+QMTCFG='qmtping',0,30ָ�� ����MQTT����ʱ��Ϊ30��
            delay_ms(300);
            printf1("<--EC20--<< \r\n");
            printf1(" %s\r\n", USART2_RX_BUF);
            USART2_RX_STA = 0;
            HAL_Delay(500); // ��ʱ500ms
            if (strx != NULL)
            {
                count = 12;
            }
            else
            {
                count = 8;
                break;
            }
            // break;
        case 12:
            memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF)); // ��ջ���
            printf("AT+QMTCFG=\"recv/mode\",0,0,1\r\n");     // ����AT+QMTCFG='recv/mode',0,0,1ָ�� ����MQTT����ģʽΪ1
            delay_ms(300);                                   // ��ʱ300ms
            strx = strstr((const char *)USART2_RX_BUF, (const char *)"OK");
            printf1(">--EC20-->> AT+QMTCFG=\"recv/mode\",0,0,1\r\n"); // ����AT+QMTCFG='recv/mode',0,0,1ָ�� ����MQTT����ģʽΪ1
            delay_ms(300);
            printf1("<--EC20--<< \r\n");
            printf1(" %s\r\n", USART2_RX_BUF);
            USART2_RX_STA = 0;
            HAL_Delay(500); // ��ʱ500ms
            if (strx != NULL)
            {
                count = 13;
            }
            else
            {
                count = 8;
                break;
            }
            // break;
        case 13:
            memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF));               // ��ջ���
            printf("AT+QMTOPEN=0,\"124.220.4.71\",1883\r\n");              // ����AT+QMTOPEN=0, "",1883ָ�� ����MQTT������
            delay_ms(300);                                                 // ��ʱ300ms
            printf1(">--EC20-->> AT+QMTOPEN=0,\"124.220.4.71\",1883\r\n"); // ����AT+QMTOPEN=0, "",1883ָ�� ����MQTT������
            delay_ms(300);
            printf1("<--EC20--<< \r\n");
            strx = strstr((const char *)USART2_RX_BUF, (const char *)"+QMTOPEN: 0,0");
            printf1(" %s\r\n", USART2_RX_BUF);
            HAL_Delay(500); // ��ʱ500ms
            USART2_RX_STA = 0;
            if (strx != NULL)
            {
                count = 14;
            }
            else
            {
                count = 8;
                break;
            }
            // break;
        case 14:
            memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF));         // ��ջ���
            printf("AT+QMTCONN=0,\"ship\",\"root\",\"123456\"\r\n"); // ����AT+QMTCONN=0, "",1883ָ�� �������ݿ�
            delay_ms(300);                                           // ��ʱ300ms
            strx = strstr((const char *)USART2_RX_BUF, (const char *)"+QMTCONN: 0,0,0");
            printf1(">--EC20-->> AT+QMTCONN=0,\"ship\",\"root\",\"123456\"\r\n"); // ����AT+QMTCONN=0, "",1883ָ�� �������ݿ�
            delay_ms(300);
            printf1("<--EC20--<< \r\n");
            printf1(" %s\r\n", USART2_RX_BUF);
            USART2_RX_STA = 0;
            HAL_Delay(500); // ��ʱ500msHAL_Delay(500);
            if (strx != NULL)
            {
                count = 15;
            }
            else
            {
                count = 8;
                break;
            }
            // break;
        case 15:
            printf1(">--print-->> EC20��ʼ���ɹ�\r\n");
            count = 15;
            return count;
        default:
            printf1(">--print-->> EC20��ʼ��ʧ��\r\n");
            break;
        }
    }
    return count;
}

void Mqtt_Senddata(void)
{
    count = 1;  // ��ʼ��������
    static u8 import_state = 0;  // �ϱ�״̬
    static u8 import_count = 0;  // �ϱ�������
    static int import_flag = 1002;   // �ϱ���־
    import_count++;
    if (import_count > 100)  // 10��ѭ���ϱ�һ��
    {
        import_flag++;
    }
    // Ϊ�ϱ����ݱ�����Чλ��
    pitch = floor((double)pitch * 100 + 0.5) / 100;           // �����Ǳ�����λС��
    roll = floor((double)roll * 100 + 0.5) / 100;             // ����Ǳ�����λС��
    yaw = floor((double)yaw * 100 + 0.5) / 100;               // ƫ���Ǳ�����λС��
    ph = floor((double)ph * 10 + 0.5) / 10;                   // PHֵ����һλС��
    turbidity = floor((double)turbidity * 10 + 0.5) / 10;     // �Ƕȱ���һλС��
    temperature = floor((double)temperature * 10 + 0.5) / 10; // �¶ȱ���һλС��
    windspeed = floor((double)windspeed * 10 + 0.5) / 10;     // ���ٱ���һλС��
    watertemperature = floor((double)watertemperature * 10 + 0.5) / 10; // ˮ�±���һλС��
    /* ����JSON�ϱ����� */
    // ����JSON���ݶ���
    cJSON *payload_data = cJSON_CreateObject();
    // ����JSON�ַ���
    char *payload_message;
    // ����豸ID  cJSON_CreateString 
    cJSON_AddItemToObject(payload_data, "DeviceID", cJSON_CreateNumber(import_flag));
    if (import_state)
    {
        // ��Ӿ���
        cJSON_AddItemToObject(payload_data, "lon", cJSON_CreateNumber(longitude));
        // ���γ��
        cJSON_AddItemToObject(payload_data, "lat", cJSON_CreateNumber(latitude));
        // ��Ӹ�����
        cJSON_AddItemToObject(payload_data, "pitch", cJSON_CreateNumber(pitch));
        // ��Ӻ����
        cJSON_AddItemToObject(payload_data, "roll", cJSON_CreateNumber(roll));
        // ���ƫ����
        cJSON_AddItemToObject(payload_data, "yaw", cJSON_CreateNumber(yaw));
        // ��Ӿ���
        cJSON_AddItemToObject(payload_data, "distance", cJSON_CreateNumber(distance));
        // ���ˮ��״̬
        cJSON_AddItemToObject(payload_data, "Seepage", cJSON_CreateNumber(seepage));

        cJSON *longitude_item = cJSON_GetObjectItem(payload_data, "lon"); // ��ȡ������
        if (longitude_item != NULL)
        {                                                    // �жϸ����Ƿ����
            cJSON_SetNumberValue(longitude_item, longitude); // ���¾���ֵ
        }
        cJSON *latitude_item = cJSON_GetObjectItem(payload_data, "lat"); // ��ȡγ����
        if (latitude_item != NULL)
        {                                                  // �жϸ����Ƿ����
            cJSON_SetNumberValue(latitude_item, latitude); // ����γ��ֵ
        }
        cJSON *pitch_item = cJSON_GetObjectItem(payload_data, "pitch"); // ��ȡ��������
        if (pitch_item != NULL)
        {                                            // �жϸ����Ƿ����
            cJSON_SetNumberValue(pitch_item, pitch); // ���¸�����ֵ
        }
        cJSON *roll_item = cJSON_GetObjectItem(payload_data, "roll"); // ��ȡ�������
        if (roll_item != NULL)
        {                                          // �жϸ����Ƿ����
            cJSON_SetNumberValue(roll_item, roll); // ���º����ֵ
        }
        cJSON *yaw_item = cJSON_GetObjectItem(payload_data, "yaw"); // ��ȡƫ������
        if (yaw_item != NULL)
        {                                        // �жϸ����Ƿ����
            cJSON_SetNumberValue(yaw_item, yaw); // ����ƫ����ֵ
        }
        cJSON *distance_item = cJSON_GetObjectItem(payload_data, "distance"); // ��ȡ������
        if (distance_item != NULL)
        {                                                  // �жϸ����Ƿ����
            cJSON_SetNumberValue(distance_item, distance); // ���¾���ֵ
        }
        cJSON *seepage_item = cJSON_GetObjectItem(payload_data, "Seepage"); // ��ȡˮ��״̬��
        if (seepage_item != NULL)
        {                                                  // �жϸ����Ƿ����
            cJSON_SetNumberValue(seepage_item, seepage); // ����ˮ��״ֵ̬
        }
        import_state = !import_state; // �ϴ�״̬ȡ��
    }
    else
    {
        // ��ӷ���
        cJSON_AddItemToObject(payload_data, "WindSpeed", cJSON_CreateNumber(windspeed));
        // ����¶�
        cJSON_AddItemToObject(payload_data, "Temperature", cJSON_CreateNumber(temperature));
        // ���ʪ��
        cJSON_AddItemToObject(payload_data, "Humidity", cJSON_CreateNumber(humidity));
        // ���TDS
        cJSON_AddItemToObject(payload_data, "TDS", cJSON_CreateNumber(tds));
        // ����Ƕ�
        cJSON_AddItemToObject(payload_data, "turbidity", cJSON_CreateNumber(turbidity));
        // ���PH
        cJSON_AddItemToObject(payload_data, "PH", cJSON_CreateNumber(ph));
        // �����ѹ
        cJSON_AddItemToObject(payload_data, "Pressure", cJSON_CreateNumber(pressure));
        // ���ˮ��
        cJSON_AddItemToObject(payload_data, "WaterTemperature", cJSON_CreateNumber(watertemperature));

        cJSON *temperature_item = cJSON_GetObjectItem(payload_data, "Temperature"); // ��ȡ�¶���
        if (temperature_item != NULL)
        {                                                        // �жϸ����Ƿ����
            cJSON_SetNumberValue(temperature_item, temperature); // �����¶�ֵ
        }
        cJSON *windspeed_item = cJSON_GetObjectItem(payload_data, "WindSpeed"); // ��ȡ������
        if (windspeed_item != NULL)
        {                                                    // �жϸ����Ƿ����
            cJSON_SetNumberValue(windspeed_item, windspeed); // ���·���ֵ
        }
        cJSON *humidity_item = cJSON_GetObjectItem(payload_data, "Humidity"); // ��ȡʪ����
        if (humidity_item != NULL)
        {                                                  // �жϸ����Ƿ����
            cJSON_SetNumberValue(humidity_item, humidity); // ����ʪ��ֵ
        }
        cJSON *tds_item = cJSON_GetObjectItem(payload_data, "TDS"); // ��ȡTDS��
        if (tds_item != NULL)
        {                                        // �жϸ����Ƿ����
            cJSON_SetNumberValue(tds_item, tds); // ����TDSֵ
        }
        cJSON *ph_item = cJSON_GetObjectItem(payload_data, "PH"); // ��ȡPH��
        if (ph_item != NULL)
        {                                      // �жϸ����Ƿ����
            cJSON_SetNumberValue(ph_item, ph); // ����PHֵ
        }
        cJSON *turbidity_item = cJSON_GetObjectItem(payload_data, "turbidity"); // ��ȡ�Ƕ���
        if (turbidity_item != NULL)
        {                                                    // �жϸ����Ƿ����
            cJSON_SetNumberValue(turbidity_item, turbidity); // �����Ƕ�ֵ
        }
        cJSON *Pressure_item = cJSON_GetObjectItem(payload_data, "Pressure"); // ��ȡ��ѹ
        if (Pressure_item != NULL)
        {                                                  // �жϸ����Ƿ����
            cJSON_SetNumberValue(Pressure_item, pressure); // ����ˮλֵ
        }
        cJSON *watertemperature_item = cJSON_GetObjectItem(payload_data, "WaterTemperature"); // ��ȡˮ����
        if (watertemperature_item != NULL)
        {                                                                // �жϸ����Ƿ����
            cJSON_SetNumberValue(watertemperature_item, watertemperature); // ����ˮ��ֵ
        }
        import_state = !import_state; // �ϴ�״̬ȡ��
    }

    // ��JSON���ݶ���ת��Ϊ�ַ���
    payload_message = cJSON_PrintUnformatted(payload_data);
    while (count != 0x02)
    {
        switch (count)
        {
        case 1:
            memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF)); // ��ջ���
            if (import_state)
            {
                printf("AT+QMTPUBEX=0,0,0,0,\"ship_value\",%d\r\n", strlen((char *)payload_message)); // ����AT+QMTPUB=0,0,0,0,'port'ָ�� ��������
                delay_ms(300);                                                                        // ��ʱ300ms
                printf("%s\r\n", payload_message);                                                    // ����JSON�ַ���
                delay_ms(300);                                                                        // ��ʱ300ms
                strx = strstr((const char *)USART2_RX_BUF, (const char *)"+QMTPUBEX: 0,0,0");
                printf1(">--EC20-->> AT+QMTPUB=0,0,0,0,\"ship_value\",%d\r\n", strlen((char *)payload_message)); // ����AT+QMTPUB=0,0,0,0,'port'ָ�� ��������
            }
            else
            {
                printf("AT+QMTPUBEX=0,0,0,0,\"ship_state\",%d\r\n", strlen((char *)payload_message)); // ����AT+QMTPUB=0,0,0,0,'port'ָ�� ��������
                delay_ms(300);                                                                        // ��ʱ300ms
                printf("%s\r\n", payload_message);                                                    // ����JSON�ַ���
                delay_ms(300);                                                                        // ��ʱ300ms
                strx = strstr((const char *)USART2_RX_BUF, (const char *)"+QMTPUBEX: 0,0,0");
                printf1(">--EC20-->> AT+QMTPUB=0,0,0,0,\"ship_state\",%d\r\n", strlen((char *)payload_message)); // ����AT+QMTPUB=0,0,0,0,'port'ָ�� ��������
            }
            delay_ms(100);
            printf1(">--JSON-->> %s\r\n", payload_message);
            delay_ms(300); // ��ʱ300ms
            /********************/
            /*		�ͷ��ڴ�	 */
            /********************/
            cJSON_Delete(payload_data); // �ͷ�JSON���ݶ���
            free(payload_message);      // �ͷ�JSON�ַ���
            printf1("<--EC20--<< \r\n");
            printf1(" %s\r\n", USART2_RX_BUF);
            USART2_RX_STA = 0;
            if (strx != NULL)
            {
                count = 2;
            }
            else
            {
                count = 0;
                break;
            }
        case 2:
            printf1(">--print-->> �����ɹ�\r\n");
            break;
        case 0:
            printf1(">--print-->> ����ʧ��\r\n");
            // printf1(">--print-->> MQTT�����ؽ�\r\n");
            // EC20_4G_Reset();  // EC20����
            // delay_ms(3000);
            // EC20_init_check(0);  // MQTT�����ؽ�
            break;
        default:
            printf1(">--print-->> ����ʧ��\r\n");
            // printf1(">--print-->> MQTT�����ؽ�\r\n");
            // EC20_4G_Reset();  // EC20����
            // delay_ms(3000);
            // EC20_init_check(0);  // MQTT�����ؽ�
            break;
        }
    }
}

_GPRMC_Typedef GPRMC; // ����GPRMC�ṹ��

// ����UTCʱ��
void Parse_UTCTime(void)
{
    int hour, minute, second;
    int decimal;
    // ����� HH��MM��SS �� SS.SS ����
    sscanf(GPRMC.UTCtime, "%2d%2d%2d.%2d", &hour, &minute, &second, &decimal);
    // ת������ʱ��
    hour += 8;
    // ����ʱ��
    sprintf(GPRMC.Time, "%2d:%2d:%2d.%2d", hour, minute, second, decimal);
}

// ������γ��
void Parse_gps_data(void)
{
    int lat_d, lon_d; // dd����
    int lat_temp0, lon_temp0, lat_temp1, lon_temp1;

    // ����� ddmm.mmmm ����
    sscanf(GPRMC.latitude, "%2d%2d.%4d", &lat_d, &lat_temp0, &lat_temp1);
    sscanf(GPRMC.longitude, "%3d%2d.%4d", &lon_d, &lon_temp0, &lon_temp1);

    // ������Чλ��
    GPRMC.lat = (double)lat_d + (double)lat_temp0 / 60 + (double)((double)lat_temp1 / 600000);
    GPRMC.lat = floor(GPRMC.lat * 10000000 + 0.5) / 10000000;
    GPRMC.lon = (double)lon_d + (double)lon_temp0 / 60 + (double)((double)lon_temp1 / 600000);
    GPRMC.lon = floor(GPRMC.lon * 10000000 + 0.5) / 10000000;
}

// ���� $GPRMC ��Ϣ
void PARSE_GPRMC(void)
{
    char *subString;
    char *subStringNext;
    char i = 0;
    for (i = 0; i <= 6; i++) // ��ȡ6������
    {
        if (i == 0)
        {
            subString = strstr((const char *)USART2_RX_BUF, ","); // ���Ҷ���
        }
        else
        {
            subString++;                                          // ƫ�Ƶ����ź���
            if ((subStringNext = strstr(subString, ",")) != NULL) // ������һ������
            {
                switch (i)
                {
                case 1:
                    memcpy(GPRMC.UTCtime, subString, subStringNext - subString); // ��ȡUTCʱ��
                    break;
                case 2:
                    memcpy(GPRMC.GPS_sta, subString, subStringNext - subString); // ��ȡ��λ״̬��A=��Ч��λ��V=��Ч��λ
                    break;
                case 3:
                    memcpy(GPRMC.latitude, subString, subStringNext - subString); // ��ȡγ����Ϣ(�ȷ�)
                    break;
                case 4:
                    memcpy(GPRMC.latitudeHemi, subString, subStringNext - subString); // ��ȡγ�Ȱ���(N/S)
                    break;
                case 5:
                    memcpy(GPRMC.longitude, subString, subStringNext - subString); // ��ȡ������Ϣ(�ȷ�)
                    break;
                case 6:
                    memcpy(GPRMC.longitudeHemi, subString, subStringNext - subString); // ��ȡ���Ȱ���(E/W)
                    break;
                default:
                    break;
                }
                subString = subStringNext;
            }
        }
    }
    // printf1("UTCtime:%s\r\n",GPRMC.UTCtime); // ��ӡUTCʱ��
    // printf1("GPS_sta:%s\r\n",GPRMC.GPS_sta); // ��ӡ��λ״̬
    // printf1("latitude:%s\r\n",GPRMC.latitude); // ��ӡγ����Ϣ(�ȷ�)
    // printf1("latitudeHemi:%s\r\n",GPRMC.latitudeHemi); // ��ӡγ�Ȱ���(N/S)
    // printf1("longitude:%s\r\n",GPRMC.longitude); // ��ӡ������Ϣ(�ȷ�)
    // printf1("longitudeHemi:%s\r\n",GPRMC.longitudeHemi); // ��ӡ���Ȱ���(E/W)
    Parse_UTCTime();                    // ����UTCʱ��
    Parse_gps_data();                   // ������γ��
    printf1("Time:%s\r\n", GPRMC.Time); // ��ӡUTCʱ��
    printf1("lat:%f\r\n", GPRMC.lat);   // ��ӡγ����Ϣ(�ȷ�)
    printf1("lon:%f\r\n", GPRMC.lon);   // ��ӡ������Ϣ(�ȷ�)
    latitude = GPRMC.lat;               // γ��
    longitude = GPRMC.lon;              // ����
}

// GPS���ݽ���
void GPS_Analysis(void)
{
    static uint8_t GPS_ENABLE = 0; // GPSʹ�ܱ�־λ

    memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF)); // ��ջ���
    printf("AT+QGPSGNMEA=\"RMC\"\r\n");              // ����AT+QGPSGNMEA="RMC"ָ�� ��ȡGPS����
    delay_ms(300);                                   // ��ʱ300ms
    strx = strstr((const char *)USART2_RX_BUF, (const char *)"OK");
    printf1(">--EC20-->> AT+QGPSGNMEA=\"RMC\"\r\n"); // ����AT+QGPSGNMEA="RMC"ָ�� ��ȡGPS����
    delay_ms(100);                                   // ��ʱ100ms
    printf1("<--EC20--<< \r\n");
    printf1(" %s\r\n", USART2_RX_BUF);
    USART2_RX_STA = 0;
    if (strx != NULL)
    {
        GPS_ENABLE = 0; // GPSʹ��
                        // ������յ������� $GPRMC ��Ϣ
        if (strstr((const char *)USART2_RX_BUF, "$GPRMC") != NULL && strchr((const char *)USART2_RX_BUF, '\n') != NULL)
        {
            // ������Ϣ
            PARSE_GPRMC();
        }
    }
    else
    {
        if (ENABLE > 6)
        {
            printf1(">--printf1-->> GPS����\r\n");           // �����ɹ�
            memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF)); // ��ջ���
            printf("AT+QGPSEND\r\n");                        // ����AT+QGPSENDָ�� �ر�GPS
            delay_ms(300);                                   // ��ʱ300ms
            strx = strstr((const char *)USART2_RX_BUF, (const char *)"OK");
            printf1(">--EC20-->> AT+QGPSEND\r\n"); // ����AT+QGPSENDָ�� �ر�GPS
            delay_ms(100);                         // ��ʱ100ms
            printf1("<--EC20--<< \r\n");
            printf1(" %s\r\n", USART2_RX_BUF);
            USART2_RX_STA = 0;
            memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF)); // ��ջ���
            printf("AT+QGPS=0\r\n");                         // ����AT+QGPS=0ָ�� �ر�GPS
            delay_ms(300);                                   // ��ʱ300ms
            /* GPS���� */
            memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF)); // ��ջ���
            printf("AT+QGPS=1\r\n");                         // ����AT+QGPS=1ָ�� ��GPS
            delay_ms(300);                                   // ��ʱ300ms
            strx = strstr((const char *)USART2_RX_BUF, (const char *)"OK");
            printf("AT+QGPSCFG=\"gpsnmeatype\",1\r\n"); // ����AT+QGPSCFG="gpsnmeatype",1ָ�� ����GPS������ݸ�ʽΪGGA
            delay_ms(300);                              // ��ʱ300ms
            strx = strstr((const char *)USART2_RX_BUF, (const char *)"OK");
            if (strx != NULL)
            {
                printf1(">--printf1-->> GPS�����ɹ�\r\n"); // �����ɹ�
            }
            else
            {
                printf1(">--printf1-->> GPS����ʧ��\r\n"); // ����ʧ��
            }
        }
        printf1(">--printf1-->> GPS�������ڼ���,���Ժ�...\r\n"); // �����ɹ�
    }
    GPS_ENABLE++; // GPSʹ�ܱ�־λ����
}
