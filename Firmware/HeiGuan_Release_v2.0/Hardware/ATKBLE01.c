#include "ATKBLE01.h"

/* Private Functions -------------------------------------------------------------------*/
void USART1_SendByte(uint8_t Byte);


//
/*********************************************************************************
 *USART的初始化，包括
 * -GPIO和USART时钟源
 * -GPIO初始化
 *    -PA9(TX) PC7(3.3v)
 *    -复用功能
 *    -复用推挽输出
 *    -默认上拉
 * -AFIO复用功能初始化
 * -USART初始化
 *   -波特率115200
 *   -数据位长度8bit
 *   -一个停止位
 *   -无校验位
 *   -无硬件流控制
 *   -发送模式
 * -使能USART
 ********************************************************************************/ 
void BLE_Init(void)
 {
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,  ENABLE);
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,  ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	 
	 GPIO_InitTypeDef GPIO_InitStructure;
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	 GPIO_Init(GPIOA,&GPIO_InitStructure);
	 GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	 GPIO_Init(GPIOC,&GPIO_InitStructure);
	 GPIO_SetBits(GPIOC,GPIO_Pin_7);

	 
	 USART_InitTypeDef USART_InitStructure;
	 USART_InitStructure.USART_BaudRate = 115200;
	 USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	 USART_InitStructure.USART_StopBits = USART_StopBits_1;
	 USART_InitStructure.USART_Parity = USART_Parity_No;
	 USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	 USART_InitStructure.USART_Mode = USART_Mode_Tx;
	 USART_Init(USART1,&USART_InitStructure);
	 USART_Cmd(USART1,ENABLE);
	 	 
 }


 
void USART1_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); //等待TDR中的数据完全移到移位寄存器
}

void BLE_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		USART1_SendByte(Array[i]);
	}
}

void BLE_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		USART1_SendByte(String[i]);
	}
}

uint32_t BLE_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void BLE_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		USART1_SendByte(Number / BLE_Pow(10, Length - i - 1) % 10 + '0');
	}
}

int fputc(int ch, FILE *f)
{
	USART1_SendByte(ch);
	return ch;
}

void BLE_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	BLE_SendString(String);
}
 
 
 
 
 