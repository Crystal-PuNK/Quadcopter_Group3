#include "MyIIC.h"

/* Defines -------------------------------------------------------------------*/ 
#define SCL_PORT   GPIOB
#define SCL_PIN    GPIO_Pin_8

#define SDA_PORT   GPIOB
#define SDA_PIN    GPIO_Pin_9
#define IIC_DELAY  0


/* Functions -------------------------------------------------------------------*/ 

/*********************************************************************************
 *����ģ��IIC�ĳ�ʼ��������
 * - PB8��PB9�ĳ�ʼ��
 *     - ��©���
 *     - PB8ģ��SCL��PB9ģ��SDA
 * - PB8��PB9��Ĭ������ߵ�ƽ
 *     - ʹ��SDA��SCL�����ͷ�״̬
 ********************************************************************************/
void MyIIC_Init(void)
{
	//��ʼ��PB8��PB9��Ϊģ��SCL��SDA
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
 * SCL �� SDA ��д����
 * Delay��ʱ���д���ȶ
 ********************************************************************************/
void MyIIC_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(SCL_PORT,SCL_PIN,(BitAction)BitValue);
	Delay_us(IIC_DELAY);
}

void MyIIC_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(SDA_PORT,SDA_PIN,(BitAction)BitValue);
	Delay_us(IIC_DELAY);
}

/*********************************************************************************
 * SCL �� SDA �Ķ�����
 * Delay��ʱ���д���ȶ
 ********************************************************************************/
uint8_t MyIIC_R_SCL(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(SCL_PORT,SCL_PIN);
	Delay_us(IIC_DELAY);
	return BitValue;
}
uint8_t MyIIC_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(SDA_PORT,SDA_PIN);
	Delay_us(IIC_DELAY);
	return BitValue;
}


/*********************************************************************************
 * IIC ʱ�� ��ʾ/��ֹ��־
 ********************************************************************************/
void MyIIC_Start(void)
{
	//ע�⣬�˴���Ҫ���ͷ�SDA�����ǵ��ظ���ʼ����Sr��������ͷ�SCL���������壨��ΪSCL��ʼ�ǵ͵�ƽ�����ܱ�����Ϊ��ֹ������
	MyIIC_W_SDA(1);
	MyIIC_W_SCL(1);
	MyIIC_W_SDA(0);
	MyIIC_W_SCL(0);
}	

void MyIIC_Stop(void)
{
	//������SDA��֤Ϊ�͵�ƽ
	MyIIC_W_SDA(0);
	MyIIC_W_SCL(1);
	MyIIC_W_SDA(1);
}

/*********************************************************************************
 * IIC ʱ�� ����/�����ֽ�
 *  -������ֹ״̬������״̬��ʼǰ����Ҫ��֤SCL�Ե͵�ƽ�������������Ԫƴ��
 ********************************************************************************/
void MyIIC_SendByte(uint8_t Byte)
{
	//�Ӹߵ��ͷ���Byte��SDA������
	//ȡ�����λ������������һ�����֣�
	for(uint8_t i=0; i<8; i++)
	{
		MyIIC_W_SDA(Byte & (0x80 >> i)); //���������Ҳ����ʹ��Byte>>7
		MyIIC_W_SCL(1);
		MyIIC_W_SCL(0);
	}
	
}

uint8_t MyIIC_ReceiveByte(void)
{
	uint8_t Byte = 0x00;
	//�����ͷ�SDA�����ɴӻ�����
	MyIIC_W_SDA(1);
	//�ͷ�SDA����Լ�һ����ʱ����ֹSCL�ͷź�ӻ���û��������
	
	for(uint8_t i = 0; i <8; i++)
	{
		//����SCL����SCL�ߵ�ƽ�ڼ��д
		MyIIC_W_SCL(1);
		if(MyIIC_R_SDA()==1) {Byte|=(0x80 >> i);}
		MyIIC_W_SCL(0);
	}
	return Byte; 
}

/*********************************************************************************
 * IIC ʱ�� ����/����Ӧ��
 *  -������ֹ״̬������״̬��ʼǰ����Ҫ��֤SCL�Ե͵�ƽ�������������Ԫƴ��
 ********************************************************************************/

void MyIIC_SendACK(uint8_t AckBit)
{
	MyIIC_W_SDA(AckBit); //���������Ҳ����ʹ��Byte>>7
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





