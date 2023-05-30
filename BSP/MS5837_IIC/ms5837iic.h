#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
#include "main.h"

#define MS5837_SDA_IN()  {GPIOF->MODER&=~(3<<(0*2));GPIOF->MODER|=0<<0*2;}	//PF0����ģʽ
#define MS5837_SDA_OUT() {GPIOF->MODER&=~(3<<(0*2));GPIOF->MODER|=1<<0*2;} //PF1���ģʽ

#define MS5837_IIC_SCL    PFout(1) //SCL
#define MS5837_IIC_SDA    PFout(0) //SDA	 
#define MS5837_READ_SDA   PFin(0)  //����SDA 
 
//IIC���в�������
void MS5837_IIC_Init(void);                //��ʼ��IIC��IO��				 
void MS5837_IIC_Start(void);			//����IIC��ʼ�ź�
void MS5837_IIC_Stop(void);	  						//����IICֹͣ�ź�
void MS5837_IIC_Send_Byte(u8 txd);					//IIC����һ���ֽ�
u8 MS5837_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 MS5837_IIC_Wait_Ack(void); 							//IIC�ȴ�ACK�ź�
void MS5837_IIC_Ack(void);									//IIC����ACK�ź�
void MS5837_IIC_NAck(void);								//IIC������ACK�ź�

// void MS5837_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
// u8 MS5837_IIC_Read_One_Byte(u8 daddr,u8 addr);	  

#endif

