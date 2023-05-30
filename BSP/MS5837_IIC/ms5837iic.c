#include "ms5837iic.h"
#include "delay.h"

//��ʼ��IIC
void MS5837_IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOF_CLK_ENABLE();   //ʹ��GPIOBʱ��
    
    //PF0 PF1��ʼ������
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //����
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
 
	MS5837_IIC_SCL=1;
	MS5837_IIC_SDA=1;
	delay_ms(1);	
}
//����IIC��ʼ�ź�
void MS5837_IIC_Start(void)
{
	MS5837_SDA_OUT();     //sda�����
	MS5837_IIC_SDA=1;	  	  
	MS5837_IIC_SCL=1;
	delay_us(4);
 	MS5837_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	MS5837_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void MS5837_IIC_Stop(void)
{
	MS5837_SDA_OUT();//sda�����
	MS5837_IIC_SCL=0;
	MS5837_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	MS5837_IIC_SCL=1; 
	MS5837_IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 MS5837_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MS5837_SDA_IN();      //SDA����Ϊ����  
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
	MS5837_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void MS5837_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	MS5837_SDA_OUT(); 	    
    MS5837_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        MS5837_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		MS5837_IIC_SCL=1;
		delay_us(2); 
		MS5837_IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 MS5837_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MS5837_SDA_IN();//SDA����Ϊ����
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
        MS5837_IIC_NAck();//����nACK
    else
        MS5837_IIC_Ack(); //����ACK   
    return receive;
}

