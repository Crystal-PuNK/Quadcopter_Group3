/* Includes ------------------------------------------------------------------*/
#include "MPU6050.h"



/* Functions -------------------------------------------------------------------*/


/*********************************************************************************
 * 写入/读取 指定寄存器的值
 *   - 模拟指定地址写和指定地址读两个IIC时序
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
 *MPU6050的初始化，包括
 * - PB8和PB9的初始化
 *     - 开漏输出
 *     - PB8模拟SCL，PB9模拟SDA
 * - PB8和PB9的默认输出高电平
 *     - 使得SDA和SCL处于释放状态
 ********************************************************************************/
void MPU6050_init(void)
{
	MyIIC_Init();
	
	//解除睡眠,选择时钟（陀螺仪时钟）
	MPU6050_WriteRegister(MPU6050_PWR_MGMT_1  ,0x01);
	MPU6050_WriteRegister(MPU6050_PWR_MGMT_2  ,0x00);
	//选择分频，越小刷新越快
	MPU6050_WriteRegister(MPU6050_SMPLRT_DIV  ,0x09);
	//外部同步以及低通滤波
	MPU6050_WriteRegister(MPU6050_CONFIG      ,0x06);
	//陀螺仪 自测不使能，最大量程选择
	MPU6050_WriteRegister(MPU6050_GYRO_CONFIG ,0x18);
	//加速度 自测不使能，最大量程选择，高通滤波器
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

 
 
 
 
 
 
 
 
 
 
 