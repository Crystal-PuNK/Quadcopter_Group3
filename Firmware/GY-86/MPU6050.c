/* Includes ------------------------------------------------------------------*/
#include "MPU6050.h"



/* Functions -------------------------------------------------------------------*/


/*********************************************************************************
 * д��/��ȡ ָ���Ĵ�����ֵ
 *   - ģ��ָ����ַд��ָ����ַ������IICʱ��
 ********************************************************************************/
void MPU6050_WriteRegister(uint8_t RegAddress, uint8_t Data)
{
	MyIIC_Start();
	MyIIC_SendByte(MPU6050_ADDRESS);
	MyIIC_ReceiveACK();
	MyIIC_SendByte(RegAddress);
	MyIIC_ReceiveACK();
	MyIIC_SendByte(Data);
	MyIIC_ReceiveACK();
	MyIIC_Stop();
}

uint8_t MPU6050_ReadRegister(uint8_t RegAddress)
{
	uint8_t Data;
	MyIIC_Start();
	MyIIC_SendByte(MPU6050_ADDRESS);
	MyIIC_ReceiveACK();
	MyIIC_SendByte(RegAddress);
	MyIIC_ReceiveACK();
	
	MyIIC_Start();
	MyIIC_SendByte(MPU6050_ADDRESS | 0x01);
	MyIIC_ReceiveACK();
	Data = MyIIC_ReceiveByte();
	MyIIC_SendACK(1);
	MyIIC_Stop();
	
	return Data;
}










/*********************************************************************************
 *MPU6050�ĳ�ʼ��������
 * - PB8��PB9�ĳ�ʼ��
 *     - ��©���
 *     - PB8ģ��SCL��PB9ģ��SDA
 * - PB8��PB9��Ĭ������ߵ�ƽ
 *     - ʹ��SDA��SCL�����ͷ�״̬
 ********************************************************************************/
void MPU6050_init(void)
{
	MyIIC_Init();
	
	//���˯��,ѡ��ʱ�ӣ�������ʱ�ӣ�
	MPU6050_WriteRegister(MPU6050_PWR_MGMT_1  ,0x01);
	MPU6050_WriteRegister(MPU6050_PWR_MGMT_2  ,0x00);
	//ѡ���Ƶ��ԽСˢ��Խ��
	MPU6050_WriteRegister(MPU6050_SMPLRT_DIV  ,0x09);
	//�ⲿͬ���Լ���ͨ�˲�
	MPU6050_WriteRegister(MPU6050_CONFIG      ,0x06);
	//������ �Բⲻʹ�ܣ��������ѡ��
	MPU6050_WriteRegister(MPU6050_GYRO_CONFIG ,0x18);
	//���ٶ� �Բⲻʹ�ܣ��������ѡ�񣬸�ͨ�˲���
	MPU6050_WriteRegister(MPU6050_ACCEL_CONFIG,0x18);
}	


void MPU6050_GetData(int16_t *AccX,int16_t *AccY,int16_t *AccZ,int16_t *GyroX,int16_t *GyroY,int16_t *GyroZ)
{
	int16_t DataH,DataL;
	
	DataH = MPU6050_ReadRegister(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_ReadRegister(MPU6050_ACCEL_XOUT_L);
	*AccX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadRegister(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadRegister(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadRegister(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadRegister(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadRegister(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadRegister(MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadRegister(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_ReadRegister(MPU6050_GYRO_YOUT_L);
	*GyroY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadRegister(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_ReadRegister(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (DataH << 8) | DataL;
}

 
 
 
 
 
 
 
 
 
 
 