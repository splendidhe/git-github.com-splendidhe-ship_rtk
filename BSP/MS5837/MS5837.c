#include "MS5837.h"
#include "ms5837iic.h"
#include "usart.h"
#include "delay.h"
#include "EC20.h"

/*
C1 ѹ�������� SENS|T1
C2  ѹ������  OFF|T1
C3	�¶�ѹ��������ϵ�� TCS
C4	�¶�ϵ����ѹ������ TCO
C5	�ο��¶� T|REF
C6 	�¶�ϵ�����¶� TEMPSENS
*/
uint32_t Cal_C[7]; // ���ڴ��PROM�е�6������1-6

double OFF_;
float Aux;
/*
dT ʵ�ʺͲο��¶�֮��Ĳ���
TEMP ʵ���¶�
*/
uint64_t dT, TEMP;
/*
OFF ʵ���¶Ȳ���
SENS ʵ���¶�������
*/
uint64_t SENS;
uint32_t D1_Pres, D2_Temp;		 // ����ѹ��ֵ,�����¶�ֵ
uint32_t TEMP2, T2, OFF2, SENS2; // �¶�У��ֵ

uint32_t Pressure;			  // ��ѹ
uint32_t Atmdsphere_Pressure; // ����ѹ
uint32_t Depth;

/*******************************************************************************
 * @��������	MS583730BA_RESET
 * @����˵��   ��λMS5611
 * @�������   ��
 * @�������   ��
 * @���ز���   ��
 *******************************************************************************/
void MS583703BA_RESET(void)
{
	MS5837_IIC_Start();
	MS5837_IIC_Send_Byte(0xEC); // CSB�ӵأ�������ַ��0XEE������ 0X77
	MS5837_IIC_Wait_Ack();
	MS5837_IIC_Send_Byte(0x1E); // ���͸�λ����
	MS5837_IIC_Wait_Ack();
	MS5837_IIC_Stop();
}

/*******************************************************************************
 * @��������	MS5611_init
 * @����˵��   ��ʼ��5611
 * @�������  	��
 * @�������   ��
 * @���ز���   ��
 *******************************************************************************/
void MS5837_init(void)
{
	u8 inth, intl;
	u8 i;

	MS5837_IIC_Init();			// ��ʼ��IIC PC11 PC12����
	MS583703BA_RESET(); // Reset Device  ��λMS5837
	delay_ms(20);		// ��λ����ʱ��ע�������ʱ��һ����Ҫ�ģ��������̵��ƺ���������20ms��

	for (i = 1; i <= 6; i++)
	{
		MS5837_IIC_Start();
		MS5837_IIC_Send_Byte(0xEC);
		MS5837_IIC_Wait_Ack();
		MS5837_IIC_Send_Byte(0xA0 + (i * 2));
		MS5837_IIC_Wait_Ack();
		MS5837_IIC_Stop();
		delay_us(5);
		MS5837_IIC_Start();
		MS5837_IIC_Send_Byte(0xEC + 0x01); // �������ģʽ
		delay_us(1);
		MS5837_IIC_Wait_Ack();
		inth = MS5837_IIC_Read_Byte(1); // ��ACK�Ķ�����
		delay_us(1);
		intl = MS5837_IIC_Read_Byte(0); // ���һ���ֽ�NACK
		MS5837_IIC_Stop();
		Cal_C[i] = (((uint16_t)inth << 8) | intl);
	}

	for (i = 0; i < 5; i++)
	{
		delay_ms(1);
		MS5837_Getdata(); // ��ȡ����ѹ
		Atmdsphere_Pressure += Pressure;
		// printf1("��ѹԭʼ���ݣ�%d\r\n", Pressure); // �������ԭʼ����
	}
	Atmdsphere_Pressure = Atmdsphere_Pressure / 5;
	// printf1("Atmdsphere_Pressure:%d\r\n", Atmdsphere_Pressure); // �������ԭʼ����
}

/**************************ʵ�ֺ���********************************************
 *����ԭ��:unsigned long MS561101BA_getConversion(void)
 *��������:    ��ȡ MS5837 ��ת�����
 *******************************************************************************/
unsigned long MS583703BA_getConversion(uint8_t command)
{
	unsigned long conversion = 0;
	u8 temp[3];

	MS5837_IIC_Start();
	MS5837_IIC_Send_Byte(0xEC); // д��ַ
	MS5837_IIC_Wait_Ack();
	MS5837_IIC_Send_Byte(command); // дת������
	MS5837_IIC_Wait_Ack();
	MS5837_IIC_Stop();

	delay_ms(10);
	MS5837_IIC_Start();
	MS5837_IIC_Send_Byte(0xEC); // д��ַ
	MS5837_IIC_Wait_Ack();
	MS5837_IIC_Send_Byte(0); // start read sequence
	MS5837_IIC_Wait_Ack();
	MS5837_IIC_Stop();

	MS5837_IIC_Start();
	MS5837_IIC_Send_Byte(0xEC + 0x01); // �������ģʽ
	MS5837_IIC_Wait_Ack();
	temp[0] = MS5837_IIC_Read_Byte(1); // ��ACK�Ķ�����  bit 23-16
	temp[1] = MS5837_IIC_Read_Byte(1); // ��ACK�Ķ�����  bit 8-15
	temp[2] = MS5837_IIC_Read_Byte(0); // ��NACK�Ķ����� bit 0-7
	MS5837_IIC_Stop();

	conversion = (unsigned long)temp[0] * 65536 + (unsigned long)temp[1] * 256 + (unsigned long)temp[2];
	return conversion;
}

///***********************************************
//  * @brief  ��ȡ��ѹ
//  * @param  None
//  * @retval None
//************************************************/
void MS5837_Getdata(void)
{
	D1_Pres = MS583703BA_getConversion(0x48);
	D2_Temp = MS583703BA_getConversion(0x58);

	if (D2_Temp > (((uint32_t)Cal_C[5]) * 256))
	{
		dT = D2_Temp - (((uint32_t)Cal_C[5]) * 256);
		TEMP = 2000 + dT * ((uint32_t)Cal_C[6]) / 8388608;
		OFF_ = (uint32_t)Cal_C[2] * 65536 + ((uint32_t)Cal_C[4] * dT) / 128;
		SENS = (uint32_t)Cal_C[1] * 32768 + ((uint32_t)Cal_C[3] * dT) / 256;
	}
	else
	{
		dT = (((uint32_t)Cal_C[5]) * 256) - D2_Temp;
		TEMP = 2000 - dT * ((uint32_t)Cal_C[6]) / 8388608;
		OFF_ = (uint32_t)Cal_C[2] * 65536 - ((uint32_t)Cal_C[4] * dT) / 128;
		SENS = (uint32_t)Cal_C[1] * 32768 - ((uint32_t)Cal_C[3] * dT) / 256;
	}
	//	printf("1:%d\t%d\t%llu\t%llu\t%f\t%llu\r\n",D1_Pres,D2_Temp,dT,TEMP,OFF_,SENS);
	// printf("%d\t",Cal_C[5]);
	if (TEMP < 2000) // low temp
	{
		Aux = (2000 - TEMP) * (2000 - TEMP);
		T2 = 3 * (dT * dT) / 8589934592;
		OFF2 = 3 * Aux / 2;
		SENS2 = 5 * Aux / 8;
		//		printf("2:%f\t%d\t%d\t%d\r\n",Aux,T2,OFF2,SENS2);
	}
	else
	{
		//		printf("!!");

		Aux = (TEMP - 2000) * (TEMP - 2000);
		T2 = 2 * (dT * dT) / 137438953472;
		OFF2 = 1 * Aux / 16;
		SENS2 = 0;
	}
	OFF_ = OFF_ - OFF2;
	SENS = SENS - SENS2;
	TEMP = (TEMP - T2) / 100;
	Pressure = ((D1_Pres * SENS / 2097152 - OFF_) / 8192) / 10;
	// Depth = 0.983615 * (Pressure - Atmdsphere_Pressure);
	printf1("	Temp : %llu\r\n",TEMP);       //��������¶�
	printf1("	Pressure : %u\r\n",Pressure); //���������ѹ
    pressure = Pressure;
}
