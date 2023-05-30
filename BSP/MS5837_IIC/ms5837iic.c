#include "ms5837iic.h"
#include "delay.h"

//初始化IIC
void MS5837_IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOF_CLK_ENABLE();   //使能GPIOB时钟
    
    //PF0 PF1初始化设置
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //快速
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
 
	MS5837_IIC_SCL=1;
	MS5837_IIC_SDA=1;
	delay_ms(1);	
}
//产生IIC起始信号
void MS5837_IIC_Start(void)
{
	MS5837_SDA_OUT();     //sda线输出
	MS5837_IIC_SDA=1;	  	  
	MS5837_IIC_SCL=1;
	delay_us(4);
 	MS5837_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	MS5837_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void MS5837_IIC_Stop(void)
{
	MS5837_SDA_OUT();//sda线输出
	MS5837_IIC_SCL=0;
	MS5837_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	MS5837_IIC_SCL=1; 
	MS5837_IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 MS5837_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MS5837_SDA_IN();      //SDA设置为输入  
	MS5837_IIC_SDA=1;delay_us(1);	   
	MS5837_IIC_SCL=1;delay_us(1);	 
	while(MS5837_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			MS5837_IIC_Stop();
			return 1;
		}
	}
	MS5837_IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void MS5837_IIC_Ack(void)
{
	MS5837_IIC_SCL=0;
	MS5837_SDA_OUT();
	MS5837_IIC_SDA=0;
	delay_us(2);
	MS5837_IIC_SCL=1;
	delay_us(2);
	MS5837_IIC_SCL=0;
}
//不产生ACK应答		    
void MS5837_IIC_NAck(void)
{
	MS5837_IIC_SCL=0;
	MS5837_SDA_OUT();
	MS5837_IIC_SDA=1;
	delay_us(2);
	MS5837_IIC_SCL=1;
	delay_us(2);
	MS5837_IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void MS5837_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	MS5837_SDA_OUT(); 	    
    MS5837_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        MS5837_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		MS5837_IIC_SCL=1;
		delay_us(2); 
		MS5837_IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 MS5837_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MS5837_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        MS5837_IIC_SCL=0; 
        delay_us(2);
		MS5837_IIC_SCL=1;
        receive<<=1;
        if(MS5837_READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        MS5837_IIC_NAck();//发送nACK
    else
        MS5837_IIC_Ack(); //发送ACK   
    return receive;
}

