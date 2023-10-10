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
	

}	
 
 
 
 
 
 
 
 
 
 
 