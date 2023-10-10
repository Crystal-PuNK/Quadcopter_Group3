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
	

}	
 
 
 
 
 
 
 
 
 
 
 