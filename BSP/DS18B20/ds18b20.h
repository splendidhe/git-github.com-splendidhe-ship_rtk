#ifndef __DS18B20_H
#define __DS18B20_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//DS18B20��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/4/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//IO��������
#define DS18B20_IO_IN()  {GPIOE->MODER&=~(3<<(3*2));GPIOE->MODER|=0<<(3*2);}	//PE3����ģʽ
#define DS18B20_IO_OUT() {GPIOE->MODER&=~(3<<(3*2));GPIOE->MODER|=1<<(3*2);} 	//PE3���ģʽ
 
////IO��������											   
#define	DS18B20_DQ_OUT PEout(3) //���ݶ˿�	PE3
#define	DS18B20_DQ_IN  PEin(3)  //���ݶ˿�	PE3 
   	
u8 DS18B20_Init(void);			//��ʼ��DS18B20
short DS18B20_Get_Temp(void);	//��ȡ�¶�
void DS18B20_Start(void);		//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);		//����һ���ֽ�
u8 DS18B20_Read_Bit(void);		//����һ��λ
u8 DS18B20_Check(void);			//����Ƿ����DS18B20
void DS18B20_Rst(void);			//��λDS18B20 
#endif
