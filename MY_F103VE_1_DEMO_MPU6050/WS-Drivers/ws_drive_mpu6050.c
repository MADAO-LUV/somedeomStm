//****************************************
// Update to MPU6050 by shinetop
// MCU: STC89C52
// 2012.3.1
// ����: ��ʾ���ٶȼƺ������ǵ�10λԭʼ����
//****************************************
// GY-52 MPU3050 IIC���Գ���
// ʹ�õ�Ƭ��STC89C51 
// ����11.0592M
// ��ʾ��LCD1602
// ���뻷�� Keil uVision2
// �ο��꾧��վ24c04ͨ�ų���
// ʱ�䣺2011��9��1��
// QQ��531389319
//****************************************
#include "ws_drive_mpu6050.h"

extern  I2C_HandleTypeDef hi2c2;


typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;


//****************************************
// ����MPU6050�ڲ���ַ
//****************************************
#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG			0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)
#define	WHO_AM_I			0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
#define	SlaveAddress	0xD0	//IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ
//****************************************
//�������ͼ�����
//****************************************


//MPU6050��������
void  WS_Init_MPU6050(void);													//��ʼ��MPU6050

    
//**************************************
//��I2C�豸д��һ���ֽ�����
//**************************************
void WS_IIC_Write_Reg(uint8_t REG_Address,uint8_t REG_data)
{
	 HAL_StatusTypeDef stu =   HAL_I2C_Mem_Write(&hi2c2,SlaveAddress,REG_Address,I2C_MEMADD_SIZE_8BIT,&REG_data,1,1) ;
   if(stu != HAL_OK)
	 {
	   WS_Debug("IIC write err %d\r\n",stu);
	 }
}
//**************************************
//��I2C�豸��ȡһ���ֽ�����
//**************************************
uint8_t WS_IIC_Read_Reg(uint8_t REG_Address)
{
	 uint8_t REG_data;
	 HAL_I2C_Mem_Read(&hi2c2,SlaveAddress,REG_Address,I2C_MEMADD_SIZE_8BIT,&REG_data,1,1);
	 return REG_data;
}
//**************************************
//��ʼ��MPU6050
//**************************************
void WS_Init_MPU6050(void)
{
	WS_IIC_Write_Reg(PWR_MGMT_1, 0x00);	//�������״̬
	WS_IIC_Write_Reg(SMPLRT_DIV, 0x07);
	WS_IIC_Write_Reg(CONFIG, 0x06);
	WS_IIC_Write_Reg(GYRO_CONFIG, 0x18);
	WS_IIC_Write_Reg(ACCEL_CONFIG, 0x01);
}
//**************************************
//���ݶ�ȡ���ϳ�����
//**************************************
int16_t WS_MPU6050_GetData(uint8_t REG_Address)
{
	uint8_t buf[2];
	HAL_I2C_Mem_Read(&hi2c2,SlaveAddress,REG_Address,I2C_MEMADD_SIZE_8BIT,buf,2,1);
	return (buf[0]<<8)+buf[1];   //�ϳ�����
}

//  ������ 
int WS_OS_Task_MPU6050(void * date)
{
	  WS_Debug ("ACCEL:x,y,x,GYRO:%6d,%6d,%6d,%6d,%6d,%6d\r\n",WS_MPU6050_GetData(ACCEL_XOUT_H),WS_MPU6050_GetData(ACCEL_YOUT_H),WS_MPU6050_GetData(ACCEL_ZOUT_H),
	                                                           WS_MPU6050_GetData(GYRO_XOUT_H), WS_MPU6050_GetData(GYRO_YOUT_H) ,WS_MPU6050_GetData(GYRO_ZOUT_H));
	  return 200;
}
