// #include "jy61p.h"
// #include "string.h"

// SAcc 	ACC;
// SAngle 	Angle;
// SGyro 	Gyro;

// float angle_x;
// float angle_y;
// float angle_z;

// float acc_x;
// float acc_y;
// float acc_z;

// float gyro_x;
// float gyro_y;
// float gyro_z;

// void CopeSerial2Data(unsigned char ucData)
// {
// 	static unsigned char ucRxBuffer[250];
// 	static unsigned char ucRxCnt = 0;	
	
// 	ucRxBuffer[ucRxCnt++]=ucData;	// ���յ������ݴ��뻺������
// 	if(ucRxBuffer[0]!=0x55)         // ����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
// 	{
// 		ucRxCnt=0;
// 		return;
// 	}
// 	if(ucRxCnt<11) 
// 	{
// 		return;                     // ���ݲ���11�����򷵻�
// 	}
// 	else
// 	{
// 		switch(ucRxBuffer[1])       // �ж��������������ݣ�Ȼ���俽������Ӧ�Ľṹ���У���Щ���ݰ���Ҫͨ����λ���򿪶�Ӧ������󣬲��ܽ��յ�������ݰ�������
// 		{                           // memcpyΪ�������Դ����ڴ濽������,�����ջ��������ַ����������ݽṹ�����棬�Ӷ�ʵ�����ݵĽ�����            
// 			case 0x51:	memcpy(&ACC,&ucRxBuffer[2],8);break;
// 			case 0x52:	memcpy(&Gyro,&ucRxBuffer[2],8);break;
// 			case 0x53:	memcpy(&Angle,&ucRxBuffer[2],8);break;
// 		}
// 		ucRxCnt=0;                  // ��ջ�����
// 	}
// }

// void get_jy61p_data()
// {
// 	// ��̬��
// 	acc_x = (float)ACC.a[0]/32768*16;
// 	acc_y = (float)ACC.a[1]/32768*16;
// 	acc_z = (float)ACC.a[2]/32768*16;
	
// 	// ���ٶ�
// 	gyro_x = (float)Gyro.w[0]/32768*2000;
// 	gyro_y = (float)Gyro.w[1]/32768*2000;
// 	gyro_z = (float)Gyro.w[2]/32768*2000;
	
// 	// ���ٶ�
// 	angle_x = (float)Angle.Angle[0]/32768*180;
// 	angle_y = (float)Angle.Angle[1]/32768*180;
// 	angle_z = (float)Angle.Angle[2]/32768*180;

// }

