#include "MyIIC.h"

/* Defines -------------------------------------------------------------------*/ 
#define SCL_PORT   GPIOB
#define SCL_PIN    GPIO_Pin_8

#define SDA_PORT   GPIOB
#define SDA_PIN    GPIO_Pin_9


/* Functions -------------------------------------------------------------------*/ 

/*********************************************************************************
 *软件模拟IIC的初始化，包括
 * - PB8和PB9的初始化
 *     - 开漏输出
 *     - PB8模拟SCL，PB9模拟SDA
 * - PB8和PB9的默认输出高电平
 *     - 使得SDA和SCL处于释放状态
 ********************************************************************************/
void MyIIC_Init(void)
{
	//初始化PB8和PB9作为模拟SCL与SDA
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOB,GPIO_Pin_8 | GPIO_Pin_9);
	
}

/*********************************************************************************
 * SCL 和 SDA 的写函数
 * Delay的时常有待商榷
 ********************************************************************************/
void MyIIC_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(SCL_PORT,SCL_PIN,(BitAction)BitValue);
	Delay_us(10);
}

void MyIIC_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(SDA_PORT,SDA_PIN,(BitAction)BitValue);
	Delay_us(10);
}

/*********************************************************************************
 * SCL 和 SDA 的读函数
 * Delay的时常有待商榷
 ********************************************************************************/
uint8_t MyIIC_R_SCL(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(SCL_PORT,SCL_PIN);
	Delay_us(10);
	return BitValue;
}
uint8_t MyIIC_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(SDA_PORT,SDA_PIN);
	Delay_us(10);
	return BitValue;
}


/*********************************************************************************
 * IIC 时序 启示/终止标志
 ********************************************************************************/
void MyIIC_Start(void)
{
	//注意，此处需要先释放SDA。考虑到重复起始条件Sr，如果先释放SCL会引起歧义（因为SCL起始是低电平，可能被理解为终止条件）
	MyIIC_W_SDA(1);
	MyIIC_W_SCL(1);
	MyIIC_W_SDA(0);
	MyIIC_W_SCL(0);
}	

void MyIIC_Stop(void)
{
	//先拉低SDA保证为低电平
	MyIIC_W_SDA(0);
	MyIIC_W_SCL(1);
	MyIIC_W_SDA(1);
}

/*********************************************************************************
 * IIC 时序 发送/接受字节
 *  -除了终止状态，所有状态开始前都需要保证SCL以低电平结束，方便个单元拼接
 ********************************************************************************/
void MyIIC_SendByte(uint8_t Byte)
{
	//从高到低发送Byte到SDA线缆上
	//取出最高位（从左到右数第一个数字）
	for(uint8_t i=0; i<8; i++)
	{
		MyIIC_W_SDA(Byte & (0x80 >> i)); //经典操作，也可以使用Byte>>7
		MyIIC_W_SCL(1);
		MyIIC_W_SCL(0);
	}
	
}

uint8_t MyIIC_ReceiveByte(void)
{
	uint8_t Byte = 0x00;
	//主机释放SDA，交由从机控制
	MyIIC_W_SDA(1);
	//释放SDA后可以加一个延时，防止SCL释放后从机还没发出数据
	
	for(uint8_t i = 0; i <8; i++)
	{
		//拉高SCL，在SCL高电平期间读写
		MyIIC_W_SCL(1);
		if(MyIIC_R_SDA()==1) {Byte|=(0x80 >> i);}
		MyIIC_W_SCL(0);
	}
	return Byte; 
}

/*********************************************************************************
 * IIC 时序 发送/接受应答
 *  -除了终止状态，所有状态开始前都需要保证SCL以低电平结束，方便个单元拼接
 ********************************************************************************/

void MyIIC_SendACK(uint8_t AckBit)
{
	MyIIC_W_SDA(AckBit); //经典操作，也可以使用Byte>>7
	MyIIC_W_SCL(1);
	MyIIC_W_SCL(0);
}

uint8_t MyIIC_ReceiveACK(void)
{
	uint8_t AckBit;
	MyIIC_W_SDA(1);
	MyIIC_W_SCL(1);
	AckBit = MyIIC_R_SDA();
	MyIIC_W_SCL(0);
		
	return AckBit;
}





