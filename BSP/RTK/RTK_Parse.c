#include "RTK_Parse.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

uint8_t RMC_Buffer[RMC_MAX_Len] = {0}; // RMC���ݻ���
uint8_t GGA_Buffer[GGA_MAX_Len] = {0}; // GGA���ݻ���
uint8_t GSA_Buffer[GSA_MAX_Len] = {0}; // GSA���ݻ���
uint8_t GSV_Buffer[GSV_MAX_Len] = {0}; // GSV���ݻ���
uint8_t VTG_Buffer[VTG_MAX_Len] = {0}; // VTG���ݻ���
uint8_t GLL_Buffer[GLL_MAX_Len] = {0}; // GLL���ݻ���
uint8_t WTZ_Buffer[WTZ_MAX_Len] = {0}; // WTZ���ݻ���

/*****************************************************/
//  ��������count_commas
//  ��  ���������ַ����ж��ŵĸ���
//  ��  �룺�ַ���
//  ��  �������ŵĸ���
/*****************************************************/
int count_commas(char *arr) // �����ַ����ж��ŵĸ���
{
    int count = 0;                       // ���ŵĸ���
    for (int i = 0; arr[i] != '\0'; i++) // �����ַ���
    {
        if (arr[i] == ',') // �ж��Ƿ�Ϊ����
        {
            count++; // ���Ÿ�����1
        }
    }
    return count; // ���ض��Ÿ���
}

/*****************************************************/
//  ��������Parse_UTCTime
//  ��  ��������UTCʱ��
//  ��  �룺UTCʱ�䡢����ʱ��
//  ��  ������
/*****************************************************/
void Parse_UTCtime(char *utc_time, char *time)
{
    int hour, minute, second; // ʱ���֡���
    int decimal;              // С������
    // ����� HH��MM��SS �� SS.SS ����
    sscanf(utc_time, "%2d%2d%2d.%2d", &hour, &minute, &second, &decimal);
    // ת������ʱ��
    hour += 8;
    // ����ʱ��
    sprintf(time, "%2d:%2d:%2d.%d", hour, minute, second, decimal);
}

/*****************************************************/
//  ��������Parase_UTCdate
//  ��  ��������UTC����
//  ��  �룺UTC���ڡ���������
//  ��  ������
/*****************************************************/
void Parase_UTCdate(char *utc_date, char *date)
{
    int year, month, day; // �ꡢ�¡���
    // ����� YY��MM��DD ����
    sscanf(utc_date, "%2d%2d%2d", &year, &month, &day);
    // ��������
    sprintf(date, "%2d/%2d/%2d", year, month, day);
}

/*****************************************************/
//  ��������Parse_latitude
//  ��  ��������γ��
//  ��  ��: γ�ȡ�γ�����ݡ�γ�Ȱ���
//  ��  ������
/*****************************************************/
void Parse_latitude(char *latitude, char *lat, char *latHemi)
{
    int lat_d; // dd����
    int lat_temp0, lat_temp1;
    double lati; // γ��
    // ����� ddmm.mmmm ����
    sscanf(latitude, "%2d%2d.%d", &lat_d, &lat_temp0, &lat_temp1);
    // ������Чλ��
    lati = (double)lat_d + (double)lat_temp0 / 60 + (double)((double)lat_temp1 / 600000);
    lati = floor(lati * 10000000 + 0.5) / 10000000;
    sprintf(lat, "%f,%c", lati, latHemi); // ����γ��
}

/*****************************************************/
//  ��������Parse_longitude
//  ��  ������������
//  ��  �룺���ȡ��������ݡ����Ȱ���
//  ��  ������
/*****************************************************/
void Parse_longitude(char *longitude, char *lon, char *lonHemi)
{
    int lon_d; // dd����
    int lon_temp0, lon_temp1;
    double loni;
    // ����� ddmm.mmmm ����
    sscanf(longitude, "%3d%2d.%d", &lon_d, &lon_temp0, &lon_temp1);
    // ������Чλ��
    loni = (double)lon_d + (double)lon_temp0 / 60 + (double)((double)lon_temp1 / 600000);
    loni = floor(loni * 10000000 + 0.5) / 10000000;
    sprintf(lon, "%f,%c", loni, lonHemi); // ���澭��
}

/*****************************************************/
//  ��������extract_RTK_Buffer
//  ��  ������ȡRTK���� �ֱ�����Ӧ�Ļ����д�����
//  ��  �룺��
//  ��  ������
/*****************************************************/
void extract_RTK_Buffer(void)
{
    char *mark = NULL;                           // ���
    char sign1[] = "\r\n";                       // ��Ƿ���
    mark = strtok((char *)USART3_RX_BUF, sign1); // ��\r\nΪ�����ȡ�ַ���
    while (mark != NULL)
    {
        printf1(">--RTK-->> %s\r\n", mark);      // ��ӡÿ����ȡ���ַ���
        if (strstr(mark, "RMC") != NULL)
        {
            strcpy((char *)RMC_Buffer, mark);    // �����ַ���
        }
        else if (strstr(mark, "GGA") != NULL)
        {
            strcpy((char *)GGA_Buffer, mark);
        }
        else if (strstr(mark, "GSA") != NULL)
        {
            strcpy((char *)GSA_Buffer, mark);
        }
        else if (strstr(mark, "GSV") != NULL)
        {
            strcpy((char *)GSV_Buffer, mark);
        }
        else if (strstr(mark, "VTG") != NULL)
        {
            strcpy((char *)VTG_Buffer, mark);
        }
        else if (strstr(mark, "GLL") != NULL)
        {
            strcpy((char *)GLL_Buffer, mark);
        }
        else if (strstr(mark, "$WTRTK") != NULL)
        {
            strcpy((char *)WTZ_Buffer, mark);
        }
        mark = strtok(NULL, sign1); // ָ����һ�����ַ��� ʵ�ֱ���
    }
}

_RMC_Typedef RMC; // RMC���ݽṹ��

/*****************************************************/
//  ��������Parase_RMC
//  ��  ��: ����RMC����
//  ��  �룺��
//  ��  ������
/*****************************************************/
void Parase_RMC(void)
{
    // $ GPRMC��<1>��<2>��<3>��<4>��<5>��<6>��<7>��<8>��<9>��<10>��<11>��<12> * hh
    // <1> UTCʱ�䣬hhmmss��ʱ���룩��ʽ
    // <2>��λ״̬��A =��Ч��λ��V =��Ч��λ
    // <3>γ��ddmm.mmmm���ȷ֣���ʽ��ǰ���0Ҳ�������䣩
    // <4>γ�Ȱ���N�������򣩻�S���ϰ���
    // <5>����dddmm.mmmm���ȷ֣���ʽ��ǰ���0Ҳ�������䣩
    // <6>���Ȱ���E����������W��������
    // <7>�������ʣ�000.0�9�1999.9�ڣ�ǰ���0Ҳ�������䣩
    // <8>���溽��000.0�9�1359.9�ȣ����汱Ϊ�ο���׼��ǰ���0Ҳ�������䣩
    // <9> UTC���ڣ�ddmmyy�������꣩��ʽ
    // <10>��ƫ�ǣ�000.0�9�1180.0�ȣ�ǰ���0Ҳ�������䣩
    // <11>��ƫ�Ƿ���E��������W������
    // <12>ģʽָʾ����NMEA0183 3.00�汾�����A =������λ��D =��֣�E =Ͷ�ţ�N =������Ч��
    char *subString;                           // ���ַ���
    char *subStringNext;                       // ���ַ�����һ��
    char count = 0;                            // ����
    if (count_commas((char *)RMC_Buffer) < 12) // �ж϶��Ÿ����Ƿ�С��12
    {
        memset(RMC_Buffer, 0, sizeof(RMC_Buffer));
    }
    for (count = 0; count <= 12; count++) // ��ȡȫ������
    {
        if (count == 0)
        {
            subString = strstr((const char *)RMC_Buffer, ","); // ���Ҷ���
        }
        else
        {
            subString++;                                          // ƫ�Ƶ����ź���
            if ((subStringNext = strstr(subString, ",")) != NULL) // ������һ������
            {
                switch (count)
                {
                case 1:
                    memcpy(RMC.UTCtime, subString, subStringNext - subString); // ��ȡUTCʱ��
                    break;
                case 2:
                    memcpy(RMC.GPS_sta, subString, subStringNext - subString); // ��ȡ��λ״̬��A=��Ч��λ��V=��Ч��λ
                    break;
                case 3:
                    memcpy(RMC.latitude, subString, subStringNext - subString); // ��ȡγ����Ϣ(�ȷ�)
                    break;
                case 4:
                    memcpy(RMC.latitudeHemi, subString, subStringNext - subString); // ��ȡγ�Ȱ���(N/S)
                    break;
                case 5:
                    memcpy(RMC.longitude, subString, subStringNext - subString); // ��ȡ������Ϣ(�ȷ�)
                    break;
                case 6:
                    memcpy(RMC.longitudeHemi, subString, subStringNext - subString); // ��ȡ���Ȱ���(E/W)
                    break;
                case 7:
                    memcpy(RMC.speed, subString, subStringNext - subString); // ��ȡ��������
                    break;
                case 8:
                    memcpy(RMC.course, subString, subStringNext - subString); // ��ȡ���溽��
                    break;
                case 9:
                    memcpy(RMC.UTCdate, subString, subStringNext - subString); // ��ȡUTC����
                    break;
                case 10:
                    memcpy(RMC.magneticVariation, subString, subStringNext - subString); // ��ȡ��ƫ��
                    break;
                case 11:
                    memcpy(RMC.magneticVariationDirection, subString, subStringNext - subString); // ��ȡ��ƫ�Ƿ���
                    break;
                case 12:
                    memcpy(RMC.modeIndication, subString, subStringNext - subString); // ��ȡģʽָʾ
                    break;
                default:
                    break;
                }
                subString = subStringNext;
            }
        }
    }
    // ����UTCʱ��
    Parse_UTCtime(RMC.UTCtime, RMC.UTCtime);
    // ����UTC����
    Parase_UTCdate(RMC.UTCdate, RMC.UTCdate);
    // ����γ��
    Parse_latitude(RMC.latitude, RMC.latitude, RMC.latitudeHemi);
    // ��������
    Parse_longitude(RMC.longitude, RMC.longitude, RMC.longitudeHemi);
}

_GGA_Typedef GGA; // GGA���ݽṹ��

/*****************************************************/
//  ��������Parase_GGA
//  ��  ��: ����GGA����
//  ��  �룺��
//  ��  ������
/*****************************************************/
void Parase_GGA(void)
{
    // $ GPGGA��<1>��<2>��<3>��<4>��<5>��<6>��<7>��<8>��<9>��M��<10>��M��<11> ��<12> * hh
    // <1> UTCʱ�䣬hhmmss��ʱ���룩��ʽ
    // <2>γ��ddmm.mmmm���ȷ֣���ʽ��ǰ���0Ҳ�������䣩
    // <3>γ�Ȱ���N�������򣩻�S���ϰ���
    // <4>����dddmm.mmmm���ȷ֣���ʽ��ǰ���0Ҳ�������䣩
    // <5>���Ȱ���E����������W��������
    // <6> GPS״̬��0 =δ��λ��1 =�ǲ�ֶ�λ��2 =��ֶ�λ��6 =���ڲ���
    // <7>����ʹ�ý���λ�õ�����������00�9�112����ǰ���0Ҳ�������䣩
    // <8> HDOPˮƽ�������ӣ�0.5�9�199.9��
    // <9>���θ߶ȣ�-9999.9�9�199999.9��
    // <10>��������������Դ��ˮ׼��ĸ߶�
    // <11>���ʱ�䣨�����һ�ν��յ�����źſ�ʼ��������������ǲ�ֶ�λ��Ϊ�գ�
    // <12>���վID��0000�9�11023��ǰ���0Ҳ�������䣬������ǲ�ֶ�λ��Ϊ�գ�
    char *subString;                           // ���ַ���
    char *subStringNext;                       // ���ַ�����һ��
    char count = 0;                            // ����
    if (count_commas((char *)GGA_Buffer) < 12) // �ж϶��Ÿ����Ƿ�С��12
    {
        memset(GGA_Buffer, 0, sizeof(GGA_Buffer));
    }
    for (count = 0; count <= 12; count++) // ��ȡȫ������
    {
        if (count == 0)
        {
            subString = strstr((const char *)GGA_Buffer, ","); // ���Ҷ���
        }
        else
        {
            subString++;                                          // ƫ�Ƶ����ź���
            if ((subStringNext = strstr(subString, ",")) != NULL) // ������һ������
            {
                switch (count)
                {
                case 1:
                    memcpy(GGA.UTCtime, subString, subStringNext - subString); // ��ȡUTCʱ��
                    break;
                case 2:
                    memcpy(GGA.latitude, subString, subStringNext - subString); // ��ȡγ����Ϣ(�ȷ�)
                    break;
                case 3:
                    memcpy(GGA.latitudeHemi, subString, subStringNext - subString); // ��ȡγ�Ȱ���(N/S)
                    break;
                case 4:
                    memcpy(GGA.longitude, subString, subStringNext - subString); // ��ȡ������Ϣ(�ȷ�)
                    break;
                case 5:
                    memcpy(GGA.longitudeHemi, subString, subStringNext - subString); // ��ȡ���Ȱ���(E/W)
                    break;
                case 6:
                    memcpy(GGA.GPS_sta, subString, subStringNext - subString); // ��ȡGPS״̬
                    break;
                case 7:
                    memcpy(GGA.satellites, subString, subStringNext - subString); // ��ȡ����ʹ�ý���λ�õ���������
                    break;
                case 8:
                    memcpy(GGA.HDOP, subString, subStringNext - subString); // ��ȡHDOPˮƽ��������
                    break;
                case 9:
                    memcpy(GGA.altitude, subString, subStringNext - subString); // ��ȡ���θ߶�
                    break;
                case 10:
                    memcpy(GGA.geoidHeight, subString, subStringNext - subString); // ��ȡ��������������Դ��ˮ׼��ĸ߶�
                    break;
                case 11:
                    memcpy(GGA.DGPS_age, subString, subStringNext - subString); // ��ȡ���ʱ��
                    break;
                case 12:
                    memcpy(GGA.DGPS_ID, subString, subStringNext - subString); // ��ȡ���վID��
                    break;
                default:
                    break;
                }
                subString = subStringNext;
            }
        }
    }
    // ����UTCʱ��
    Parse_UTCtime(GGA.UTCtime, GGA.UTCtime);
    // ����γ��
    Parse_latitude(GGA.latitude, GGA.latitude, GGA.latitudeHemi);
    // ��������
    Parse_longitude(GGA.longitude, GGA.longitude, GGA.longitudeHemi);
}

_GSA_Typedef GSA; // GSA���ݽṹ��

/*****************************************************/
//  ��������Parase_GSA
//  ��  ��: ����GSA����
//  ��  �룺��
//  ��  ������
/*****************************************************/
void Parase_GSA(void)
{
    // $ GPGSA��<1>��<2>��<3>��<4> ���������� <12>��<13>��<14>��<15>��<16>��<17>��<18>
    // <1>ģʽ��M =�ֶ���A =�Զ���
    // <2>��λ��ʽ1 =δ��λ��2 =��ά��λ��3 =��ά��λ��
    // <3>��<14> PRN���֣�01��32�����ʹ���е����Ǳ�ţ����ɽ���12��������Ϣ
    // <15> PDOPλ�þ������ӣ�0.5�9�199.9��
    // <16> HDOPˮƽ�������ӣ�0.5�9�199.9��
    // <17> VDOP��ֱ�������ӣ�0.5�9�199.9��
    // <18>У���
    char *subString;                           // ���ַ���
    char *subStringNext;                       // ���ַ�����һ��
    char count = 0;                            // ����
    if (count_commas((char *)GSA_Buffer) < 18) // �ж϶��Ÿ����Ƿ�С��18
    {
        memset(GSA_Buffer, 0, sizeof(GSA_Buffer));
    }
    for (count = 0; count <= 18; count++) // ��ȡȫ������
    {
        if (count == 0)
        {
            subString = strstr((const char *)GSA_Buffer, ","); // ���Ҷ���
        }
        else
        {
            subString++;                                          // ƫ�Ƶ����ź���
            if ((subStringNext = strstr(subString, ",")) != NULL) // ������һ������
            {
                switch (count)
                {
                case 1:
                    memcpy(GSA.mode, subString, subStringNext - subString); // ��ȡģʽ
                    break;
                case 2:
                    memcpy(GSA.fixType, subString, subStringNext - subString); // ��ȡ��λ����
                    break;
                case 3:
                    memcpy(GSA.satellites, subString, subStringNext - subString); // ��ȡʹ����������
                    break;
                case 15:
                    memcpy(GSA.PDOP, subString, subStringNext - subString); // ��ȡPDOP
                    break;
                case 16:
                    memcpy(GSA.HDOP, subString, subStringNext - subString); // ��ȡHDOP
                    break;
                case 17:
                    memcpy(GSA.VDOP, subString, subStringNext - subString); // ��ȡVDOP
                    break;
                default:
                    break;
                }
                subString = subStringNext;
            }
        }
    }
}

_GSV_Typedef GSV; // GSV���ݽṹ��
/*****************************************************/
//  ��������Parase_GSV
//  ��  ��: ����GSV����
//  ��  �룺��
//  ��  ������
/*****************************************************/
void Parase_GSV(void)
{
    // $ GPGSV��<1>��<2>��<3>��<4>��<5>��<6>��<7>����<4>��<5>��<6>��<7>��<8 >
    // <1> GSV��������
    // <2>����GSV�ı��
    // <3>�ɼ����ǵ�������00��12��
    // <4>���Ǳ�ţ�01��32��
    // <5>�������ǣ�00��90�ȡ�
    // <6>���Ƿ�λ�ǣ�000��359�ȡ�ʵ��ֵ��
    // <7>Ѷ�������ȣ�C / No����00��99 dB���ޱ�δ���յ�Ѷ�š�
    // <8>У���
    char *subString;                          // ���ַ���
    char *subStringNext;                      // ���ַ�����һ��
    char count = 0;                           // ����
    if (count_commas((char *)GSV_Buffer) < 8) // �ж϶��Ÿ����Ƿ�С��8
    {
        memset(GSV_Buffer, 0, sizeof(GSV_Buffer));
    }
    for (count = 0; count <= 3; count++) // ��ȡȫ������
    {
        if (count == 0)
        {
            subString = strstr((const char *)GSV_Buffer, ","); // ���Ҷ���
        }
        else
        {
            subString++;                                          // ƫ�Ƶ����ź���
            if ((subStringNext = strstr(subString, ",")) != NULL) // ������һ������
            {
                switch (count)
                {
                case 1:
                    memcpy(GSV.totalMessages, subString, subStringNext - subString); // ��ȡ����Ϣ��
                    break;
                case 2:
                    memcpy(GSV.messageNumber, subString, subStringNext - subString); // ��ȡ��Ϣ���
                    break;
                case 3:
                    memcpy(GSV.satellitesInView, subString, subStringNext - subString); // ��ȡ�ɼ���������
                    break;
                default:
                    break;
                }
                subString = subStringNext;
            }
        }
    }
}

_VTG_Typedef VTG; // VTG���ݽṹ��

/*****************************************************/
//  ��������Parase_VTG
//  ��  ��: ����VTG����
//  ��  �룺��
//  ��  ������
/*****************************************************/
void Parase_VTG(void)
{
    // $ GPVTG��<1>��T��<2>��M��<3>��N��<4>��K��<5> * hh
    // �ֶ�0��$GPVTG�����ID�����������ΪTrack Made Good and Ground Speed��VTG�������ٶ���Ϣ
    // �ֶ�1���˶��Ƕȣ�000 - 359����ǰ��λ��������0��
    // �ֶ�2��T=�汱����ϵ
    // �ֶ�3���˶��Ƕȣ�000 - 359����ǰ��λ��������0��
    // �ֶ�4��M=�ű�����ϵ
    // �ֶ�5��ˮƽ�˶��ٶȣ�0.00����ǰ��λ��������0��
    // �ֶ�6��N=�ڣ�Knots
    // �ֶ�7��ˮƽ�˶��ٶȣ�0.00����ǰ��λ��������0��
    // �ֶ�8��K=����/ʱ��km/h
    // �ֶ�9��У��ֵ��$��*֮����������ֵ��
    char *subString;                          // ���ַ���
    char *subStringNext;                      // ���ַ�����һ��
    char count = 0;                           // ����
    if (count_commas((char *)VTG_Buffer) < 9) // �ж϶��Ÿ����Ƿ�С��9
    {
        memset(VTG_Buffer, 0, sizeof(VTG_Buffer));
    }
    for (count = 0; count <= 9; count++) // ��ȡȫ������
    {
        if (count == 0)
        {
            subString = strstr((const char *)VTG_Buffer, ","); // ���Ҷ���
        }
        else
        {
            subString++;                                          // ƫ�Ƶ����ź���
            if ((subStringNext = strstr(subString, ",")) != NULL) // ������һ������
            {
                switch (count)
                {
                case 1:
                    memcpy(VTG.course, subString, subStringNext - subString); // ��ȡ���溽��
                    break;
                case 2:
                    memcpy(VTG.reference, subString, subStringNext - subString); // ��ȡ�ο�
                    break;
                case 3:
                    memcpy(VTG.courseMagnetic, subString, subStringNext - subString); // ��ȡ���溽���ƫ��
                    break;
                case 4:
                    memcpy(VTG.referenceMagnetic, subString, subStringNext - subString); // ��ȡ�ο���ƫ��
                    break;
                case 5:
                    memcpy(VTG.speedKnots, subString, subStringNext - subString); // ��ȡ��������(��)
                    break;
                case 6:
                    memcpy(VTG.referenceKnots, subString, subStringNext - subString); // ��ȡ�ο�(��)
                    break;
                case 7:
                    memcpy(VTG.speedKm, subString, subStringNext - subString); // ��ȡ��������(����/Сʱ)
                    break;
                case 8:
                    memcpy(VTG.referenceKm, subString, subStringNext - subString); // ��ȡ�ο�(����/Сʱ)
                    break;
                default:
                    break;
                }
                subString = subStringNext;
            }
        }
    }
}

_GLL_Typedef GLL; // GLL���ݽṹ��

/*****************************************************/
//  ��������Parase_GLL
//  ��  ��: ����GLL����
//  ��  �룺��
//  ��  ������
/*****************************************************/
void Parase_GLL(void)
{
    // $GPGLL,<1>,<2>,<3>,<4>,<5>,<6>*2D
    // �ֶ�0��$GPGLL�����ID�����������ΪGeographic Position��GLL������λ��Ϣ
    // �ֶ�1��γ��ddmm.mmmm���ȷָ�ʽ��ǰ��λ��������0��
    // �ֶ�2��γ��N����γ����S����γ��
    // �ֶ�3������dddmm.mmmm���ȷָ�ʽ��ǰ��λ��������0��
    // �ֶ�4������E����������W��������
    // �ֶ�5��UTCʱ�䣬hhmmss.sss��ʽ
    // �ֶ�6��״̬��A=��λ��V=δ��λ
    // �ֶ�7��У��ֵ��$��*֮����������ֵ��
    char *subString;                          // ���ַ���
    char *subStringNext;                      // ���ַ�����һ��
    char count = 0;                           // ����
    if (count_commas((char *)GLL_Buffer) < 7) // �ж϶��Ÿ����Ƿ�С��7
    {
        memset(GLL_Buffer, 0, sizeof(GLL_Buffer));
    }
    for (count = 0; count <= 7; count++) // ��ȡȫ������
    {
        if (count == 0)
        {
            subString = strstr((const char *)GLL_Buffer, ","); // ���Ҷ���
        }
        else
        {
            subString++;                                          // ƫ�Ƶ����ź���
            if ((subStringNext = strstr(subString, ",")) != NULL) // ������һ������
            {
                switch (count)
                {
                case 1:
                    memcpy(GLL.latitude, subString, subStringNext - subString); // ��ȡγ����Ϣ(�ȷ�)
                    break;
                case 2:
                    memcpy(GLL.latitudeHemi, subString, subStringNext - subString); // ��ȡγ�Ȱ���(N/S)
                    break;
                case 3:
                    memcpy(GLL.longitude, subString, subStringNext - subString); // ��ȡ������Ϣ(�ȷ�)
                    break;
                case 4:
                    memcpy(GLL.longitudeHemi, subString, subStringNext - subString); // ��ȡ���Ȱ���(E/W)
                    break;
                case 5:
                    memcpy(GLL.UTCtime, subString, subStringNext - subString); // ��ȡUTCʱ��
                    break;
                case 6:
                    memcpy(GLL.status, subString, subStringNext - subString); // ��ȡ״̬
                    break;
                default:
                    break;
                }
                subString = subStringNext;
            }
        }
    }
    // ����UTCʱ��
    Parse_UTCtime(GLL.UTCtime, GLL.UTCtime);
    // ����γ��
    Parse_latitude(GLL.latitude, GLL.latitude, GLL.latitudeHemi);
    // ��������
    Parse_longitude(GLL.longitude, GLL.longitude, GLL.longitudeHemi);
}

_WTZ_Typedef WTZ; // WTZ���ݽṹ��

/*****************************************************/
//  ��������Parase_WTZ
//  ��  ��: ����RTK��̬����������
//  ��  �룺��
//  ��  ������
/*****************************************************/
void Parase_WTZ(void)
{
    // $WTRTK,0.00,0.00,0.00,0.00,-9.25,-1.28,0.00,0,19,0,5
    // ����        ˵��
    // $WTRTK      ��ͷ
    // 0.00        ���X���루��λ���ף�
    // 0.00        ���Y���루��λ���ף�
    // 0.00        ���Z���루��λ���ף�
    // 0.00        ���R���루��λ���ף�
    // -9.25       ������
    // -1.28       �����
    // 0.00        �����
    // 0           �ƶ�վRTK״̬    0��ʾ��ʼ����1��ʾ���㶨λ��2��ʾ���֣�4��ʾ�̶��⣬5��ʾ�����
    // 19          4G״̬           0~13�����ò��� 14~20��������   21���������
    // 0           �̶���վ״̬     -1��ʾѡ��̶���վʧ�� 0��ʾѡ���޹̶���վѡ�� 1��ʾѡ��̶���վ�ɹ�
    // CSQ         4G�ź�����
    // 5           Ntrip״̬        5:���ӳɹ�  ������������
    char *subString;                           // ���ַ���
    char *subStringNext;                       // ���ַ�����һ��
    char count = 0;                            // ����
    if (count_commas((char *)WTZ_Buffer) < 11) // �ж϶��Ÿ����Ƿ�С��11
    {
        memset(WTZ_Buffer, 0, sizeof(WTZ_Buffer));
    }
    for (count = 0; count <= 12; count++) // ��ȡȫ������
    {
        if (count == 0)
        {
            subString = strstr((const char *)GLL_Buffer, ","); // ���Ҷ���
        }
        else
        {
            subString++;                                          // ƫ�Ƶ����ź���
            if ((subStringNext = strstr(subString, ",")) != NULL) // ������һ������
            {
                switch (count)
                {
                case 5:
                    memcpy(WTZ.roll, subString, subStringNext - subString); // ��ȡ������
                    break;
                case 6:
                    memcpy(WTZ.pitch, subString, subStringNext - subString); // ��ȡ�����
                    break;
                case 7:
                    memcpy(WTZ.yaw, subString, subStringNext - subString); // ��ȡ�����
                    break;
                case 8:
                    memcpy(WTZ.RTK_status, subString, subStringNext - subString); // ��ȡ�ƶ�վRTK״̬
                    break;
                case 9:
                    memcpy(WTZ.status_4G, subString, subStringNext - subString); // ��ȡ4G״̬
                    break;
                case 10:
                    memcpy(WTZ.quality_4G, subString, subStringNext - subString); // ��ȡ�̶���վ״̬
                    break;
                default:
                    break;
                }
                subString = subStringNext;
            }
        }
    }
}


/*****************************************************/
//  ��������Parase_RTK  ������
//  ��  ��: ����RTK����
//  ��  �룺��
//  ��  ������
/*****************************************************/
void Parase_RTK(void)
{
    extract_RTK_Buffer(); // ��ȡRTK����
    Parase_RMC();         // ����RMC����
    Parase_GGA();         // ����GGA����
    Parase_GSA();         // ����GSA����
    Parase_GSV();         // ����GSV����
    Parase_VTG();         // ����VTG����
    Parase_GLL();         // ����GLL����
    Parase_WTZ();         // ����RTK��̬����������
}
