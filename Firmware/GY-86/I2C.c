#include "GY_Reg.h"
#include "I2C.h"

#define MPU6050 (uint8_t)0b1101000
#define HMC5883L (uint8_t)0b00011110
#define I2C1 *((unsigned volatile int*)0x40005400)
#define I2C2 *((unsigned volatile int*)0x40005800)
#define I2C3 *((unsigned volatile int*)0x40005C00)
#define GPIOA *((unsigned volatile int*)0x40020000)
#define GPIOB *((unsigned volatile int*)0x40020400)
#define GPIOC *((unsigned volatile int*)0x40020800)
	
#define RCC_AHB1ENR *((unsigned volatile int*)0x40023830)
#define RCC_APB1ENR *((unsigned volatile int*)0x40023840)
	
#define GPIOB_MODER *((unsigned volatile int*)0x40020400)
#define GPIOB_OTYPER *((unsigned volatile int*)0x40020404)
#define GPIOB_OSPEEDR *((unsigned volatile int*)0x40020408)
	
#define I2C_CR1 *((unsigned volatile int*)0x40005400)
#define I2C_CR2 *((unsigned volatile int*)0x40005404)
#define I2C_SR1 *((unsigned volatile int*)0x40005414)
#define I2C_SR2 *((unsigned volatile int*)0x40005418)
#define I2C_OAR1 *((unsigned volatile int*)0x40005408)	
#define I2C_OAR2 *((unsigned volatile int*)0x4000540C)
#define I2C_CCR *((unsigned volatile int*)0x4000541C)
#define I2C_DR *((unsigned volatile int*)0x40005410)	
#define I2C_TRISE *((unsigned volatile int*)0x40005420)	

#define Mask 0x00ffffff

void GY_WriteReg(uint8_t Address,uint8_t regist,uint8_t data){
	//起始条件
	I2C_CR1 |= 0x1<<8;
	while((((I2C_SR1 | (I2C_SR2<<16))& Mask)& 0x30001)!= 0x30001);
	
	//发送从机地址
	I2C_DR = (Address<<1)|1;
	while((((I2C_SR1 | (I2C_SR2<<16))& Mask)& 0x00070082) != 0x00070082);
	
	//发送寄存器地址
	I2C_DR = regist;
	while((((I2C_SR1 | (I2C_SR2<<16))& Mask)& 0x00070080) != 0x00070080);
	
	//发送数据
	I2C_DR = data;
	while((((I2C_SR1 | (I2C_SR2<<16))& Mask)& 0x00070084) != 0x00070084);
	
	//结束条件
	I2C_CR1 |= 1<<9;
}


uint8_t GY_ReadReg(uint8_t Address,uint8_t regist){
	uint8_t data;
	//起始条件
	I2C_CR1 |= 0x1<<8;
	while((((I2C_SR1 | (I2C_SR2<<16))& Mask)& 0x30001) != 0x30001);
	
	//发送从机地址
	I2C_DR = (Address<<1)|1;
	while((((I2C_SR1 | (I2C_SR2<<16))& Mask)& 0x00070082) != 0x00070082);
	
	//发送数据
	I2C_DR = regist;
	while((((I2C_SR1 | (I2C_SR2<<16))& Mask)& 0x00070084) != 0x00070084);
	
	//重复起始条件
	I2C_CR1 |= 0x1<<8;
	while((((I2C_SR1 | (I2C_SR2<<16))& Mask)& 0x30001) != 0x30001);
	
	//读数据模式
	I2C_DR = Address<<1;
	while((((I2C_SR1 | (I2C_SR2<<16))& Mask)& 0x00030002) != 0x00030002);
	
	//主机不应答
	I2C_CR1 &= 0xFBFF;
	
	//产生结束条件
	I2C_CR1 |= 1<<9;
	
	while((((I2C_SR1 | (I2C_SR2<<16))& Mask)& 0x00030040) != 0x00030040);
	data = I2C_DR;
	
	I2C_CR1 |= 1<<10;
	return data;
}

void GY_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ,
                        int16_t *HmX, int16_t *HmY, int16_t *HmZ)
{
	uint8_t DataH, DataL;
	
	DataH = GY_ReadReg(MPU6050,MPU6050_ACCEL_XOUT_H);
	DataL = GY_ReadReg(MPU6050,MPU6050_ACCEL_XOUT_L);
	*AccX = (DataH << 8) | DataL;
	
	DataH = GY_ReadReg(MPU6050,MPU6050_ACCEL_YOUT_H);
	DataL = GY_ReadReg(MPU6050,MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH << 8) | DataL;
	
	DataH = GY_ReadReg(MPU6050,MPU6050_ACCEL_ZOUT_H);
	DataL = GY_ReadReg(MPU6050,MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH << 8) | DataL;
	
	DataH = GY_ReadReg(MPU6050,MPU6050_GYRO_XOUT_H);
	DataL = GY_ReadReg(MPU6050,MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH << 8) | DataL;
	
	DataH = GY_ReadReg(MPU6050,MPU6050_GYRO_YOUT_H);
	DataL = GY_ReadReg(MPU6050,MPU6050_GYRO_YOUT_L);
	*GyroY = (DataH << 8) | DataL;
	
	DataH = GY_ReadReg(MPU6050,MPU6050_GYRO_ZOUT_H);
	DataL = GY_ReadReg(MPU6050,MPU6050_GYRO_ZOUT_L);
	*GyroZ = (DataH << 8) | DataL;
	
	DataH = GY_ReadReg(HMC5883L,HM_XMSB);
	DataL = GY_ReadReg(MPU6050,HM_XLSB);
	*HmX = (DataH << 8) | DataL;
	
	DataH = GY_ReadReg(HMC5883L,HM_YMSB);
	DataL = GY_ReadReg(MPU6050,HM_YLSB);
	*HmY = (DataH << 8) | DataL;
	
	DataH = GY_ReadReg(HMC5883L,HM_ZMSB);
	DataL = GY_ReadReg(MPU6050,HM_ZLSB);
	*HmZ = (DataH << 8) | DataL;
}

void GY_Init(void){
	RCC_AHB1ENR|= 0x2;                //打开GPIOB时钟
	RCC_APB1ENR = 1<<21;              //打开I2C时钟

	//PB8 PB9
	GPIOB_MODER |= 0xA0000;          
	GPIOB_OTYPER |= 0x300;
	GPIOB_OSPEEDR |= 0xF0000;
	
	I2C_CR1 &= 0xFFFE;
	I2C_CR1 |= 0x0400;         //Ack置1
	I2C_CR2 |= 0b00010110;            //clock frequency
	I2C_CCR |= 0b000010100000;          //调整占空比 100kHz
	I2C_TRISE |= 0x11;
	I2C_CR1 |= 0x1;            //使能I2C
	
	 
	GY_WriteReg(MPU6050,MPU6050_PWR_MGMT_1,0x01);
	GY_WriteReg(MPU6050,MPU6050_PWR_MGMT_2, 0x00);
	GY_WriteReg(MPU6050,MPU6050_SMPLRT_DIV, 0x09);
	GY_WriteReg(MPU6050,MPU6050_CONFIG, 0x06);
	GY_WriteReg(MPU6050,MPU6050_GYRO_CONFIG, 0x18);
	GY_WriteReg(MPU6050,MPU6050_ACCEL_CONFIG, 0x18);
	
	GY_WriteReg(HMC5883L,HM_CONFIGA, 0b01110000);
	GY_WriteReg(HMC5883L,HM_CONFIGB, 0b00100000);
	GY_WriteReg(HMC5883L,HM_MODE, 0b00000001);
}
