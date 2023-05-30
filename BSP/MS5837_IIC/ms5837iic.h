#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
#include "main.h"

#define MS5837_SDA_IN()  {GPIOF->MODER&=~(3<<(0*2));GPIOF->MODER|=0<<0*2;}	//PF0输入模式
#define MS5837_SDA_OUT() {GPIOF->MODER&=~(3<<(0*2));GPIOF->MODER|=1<<0*2;} //PF1输出模式

#define MS5837_IIC_SCL    PFout(1) //SCL
#define MS5837_IIC_SDA    PFout(0) //SDA	 
#define MS5837_READ_SDA   PFin(0)  //输入SDA 
 
//IIC所有操作函数
void MS5837_IIC_Init(void);                //初始化IIC的IO口				 
void MS5837_IIC_Start(void);			//发送IIC开始信号
void MS5837_IIC_Stop(void);	  						//发送IIC停止信号
void MS5837_IIC_Send_Byte(u8 txd);					//IIC发送一个字节
u8 MS5837_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 MS5837_IIC_Wait_Ack(void); 							//IIC等待ACK信号
void MS5837_IIC_Ack(void);									//IIC发送ACK信号
void MS5837_IIC_NAck(void);								//IIC不发送ACK信号

// void MS5837_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
// u8 MS5837_IIC_Read_One_Byte(u8 daddr,u8 addr);	  

#endif

