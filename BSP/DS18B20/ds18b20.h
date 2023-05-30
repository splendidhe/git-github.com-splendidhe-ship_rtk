#ifndef __DS18B20_H
#define __DS18B20_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//DS18B20驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/4/15
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//IO方向设置
#define DS18B20_IO_IN()  {GPIOE->MODER&=~(3<<(3*2));GPIOE->MODER|=0<<(3*2);}	//PE3输入模式
#define DS18B20_IO_OUT() {GPIOE->MODER&=~(3<<(3*2));GPIOE->MODER|=1<<(3*2);} 	//PE3输出模式
 
////IO操作函数											   
#define	DS18B20_DQ_OUT PEout(3) //数据端口	PE3
#define	DS18B20_DQ_IN  PEin(3)  //数据端口	PE3 
   	
u8 DS18B20_Init(void);			//初始化DS18B20
short DS18B20_Get_Temp(void);	//获取温度
void DS18B20_Start(void);		//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);		//读出一个字节
u8 DS18B20_Read_Bit(void);		//读出一个位
u8 DS18B20_Check(void);			//检测是否存在DS18B20
void DS18B20_Rst(void);			//复位DS18B20 
#endif
