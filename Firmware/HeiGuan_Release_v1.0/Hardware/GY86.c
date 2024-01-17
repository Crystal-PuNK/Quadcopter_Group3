/* Includes --------------------------------------------------------------------*/
#include "GY86.h"
/* Defines -------------------------------------------------------------------*/

//Register Address
#define MPU6050_ADDRESS                0xD0
#define HMC5883L_ADDRESS               0x3C
#define MS5611_ADDRESS                 0xEC

#define	MPU6050_SMPLRT_DIV	      	   0x19
#define	MPU6050_CONFIG		           0x1A
#define	MPU6050_GYRO_CONFIG	           0x1B
#define	MPU6050_ACCEL_CONFIG	       0x1C

#define	MPU6050_ACCEL_XOUT_H	       0x3B
#define	MPU6050_ACCEL_XOUT_L	       0x3C
#define	MPU6050_ACCEL_YOUT_H	       0x3D
#define	MPU6050_ACCEL_YOUT_L	       0x3E
#define	MPU6050_ACCEL_ZOUT_H	       0x3F
#define	MPU6050_ACCEL_ZOUT_L	       0x40
#define	MPU6050_TEMP_OUT_H		       0x41
#define	MPU6050_TEMP_OUT_L		       0x42
#define	MPU6050_GYRO_XOUT_H		       0x43
#define	MPU6050_GYRO_XOUT_L		       0x44
#define	MPU6050_GYRO_YOUT_H		       0x45
#define	MPU6050_GYRO_YOUT_L		       0x46
#define	MPU6050_GYRO_ZOUT_H		       0x47
#define	MPU6050_GYRO_ZOUT_L		       0x48

#define	MPU6050_PWR_MGMT_1		       0x6B
#define	MPU6050_PWR_MGMT_2		       0x6C
#define	MPU6050_WHO_AM_I		       0x75

#define MPU6050_INT_PIN_CFG            0x37
#define MPU6050_USER_CTRL              0x6A


#define HMC5883L_CR_A                  0x00
#define HMC5883L_CR_B                  0x01
#define HMC5883L_MODE                  0x02

#define HMC5883L_GA_XOUT_H             0x03
#define HMC5883L_GA_XOUT_L             0x04
#define HMC5883L_GA_YOUT_H             0x05
#define HMC5883L_GA_YOUT_L             0x06
#define HMC5883L_GA_ZOUT_H             0x07
#define HMC5883L_GA_ZOUT_L             0x08
#define HMC5883L_STATUS                0x09
#define HMC5883L_IDENTIFY_A            0x0A
#define HMC5883L_IDENTIFY_B            0x0B
#define HMC5883L_IDENTIFY_C            0x0C

#define MS5611_RESET                   0x1E
#define MS5611_D1_OSR_256              0x40
#define MS5611_D1_OSR_512              0x42
#define MS5611_D1_OSR_1024             0x44
#define MS5611_D1_OSR_2048             0x46
#define MS5611_D1_OSR_4096             0x48
#define MS5611_D2_OSR_256              0x50
#define MS5611_D2_OSR_512              0x52
#define MS5611_D2_OSR_1024             0x54
#define MS5611_D2_OSR_2048             0x56
#define MS5611_D2_OSR_4096             0x58
#define MS5611_ADC_READ                0x00
#define MS5611_PROM_RESERVED           0xA0
#define MS5611_PROM_COEFFICIENT_1      0xA2
#define MS5611_PROM_COEFFICIENT_2      0xA4
#define MS5611_PROM_COEFFICIENT_3      0xA6
#define MS5611_PROM_COEFFICIENT_4      0xA8
#define MS5611_PROM_COEFFICIENT_5      0xAA
#define MS5611_PROM_COEFFICIENT_6      0xAC
#define MS5611_PROM_CRC                0xAE
/* Private Functions -----------------------------------------------------------*/

void     MPU6050_init(void);
void     HMC5883L_init(void);
void     MS5611_init(void);

void     GY86_WriteRegister(uint8_t GY86_DeviceAddress, uint8_t RegAddress, uint8_t Data);
uint8_t  GY86_ReadRegister(uint8_t GY86_DeviceAddress,uint8_t RegAddress);
void     MS5611_SendCommand(uint8_t Command);
uint32_t MS5611_ReadADC();
uint16_t MS5611_ReadC_x(uint8_t MS5611_PROM_Cx_ADDRESS);

void MS5611_GetTemperature(uint8_t MS5611_D2_OSR_xxxx);
void MS5611_GetPressure(uint8_t MS5611_D1_OSR_xxxx);


/* Variables -------------------------------------------------------------------*/
// Public
struct GY86_Data GY86DataList={1,2,3,0,0,0,0,0,0,0,0};
// Private
struct GY86_Data* Original_Data_List = &GY86DataList;

//Pressure sensitivity
uint16_t SENS_T1 = 0;
//Pressure offset 
uint16_t OFF_T1  = 0;
//Temperature coefficient of pressure sensitivity 
double TCS     = 0;
//Temperature coefficient of pressure offset
double TCO     = 0;
//Reference temperature
uint16_t T_REF   = 0;
//Temperature coefficient of the temperature
double TEMPSENS  = 0;
uint32_t D1_Pressur     = 0;
uint32_t D2_Temperature = 0;
int32_t  dT      = 0;
int32_t  TEMP_100times  = 0;
int64_t  OFF     = 0;
int64_t  SENS    = 0;
int32_t  P_100times     = 0;

/* Functions -------------------------------------------------------------------*/

void GY86_init(void)
{
	MyIIC_Init();
	MPU6050_init();
	HMC5883L_init();
	MS5611_init();

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
	//���˯��,ѡ��ʱ�ӣ�������ʱ�ӣ�
	GY86_WriteRegister(MPU6050_ADDRESS,MPU6050_PWR_MGMT_1  ,0x01);
	GY86_WriteRegister(MPU6050_ADDRESS,MPU6050_PWR_MGMT_2  ,0x00);
	//ѡ���Ƶ��ԽСˢ��Խ��
	GY86_WriteRegister(MPU6050_ADDRESS,MPU6050_SMPLRT_DIV  ,0x09);
	//�ⲿͬ���Լ���ͨ�˲�
	GY86_WriteRegister(MPU6050_ADDRESS,MPU6050_CONFIG      ,0x06);
	//������ �Բⲻʹ�ܣ��������ѡ��
	GY86_WriteRegister(MPU6050_ADDRESS,MPU6050_GYRO_CONFIG ,0x18);
	//���ٶ� �Բⲻʹ�ܣ��������ѡ�񣬸�ͨ�˲���
	GY86_WriteRegister(MPU6050_ADDRESS,MPU6050_ACCEL_CONFIG,0x18);
	//ʹ��AUXIIC-BYPASS
	GY86_WriteRegister(MPU6050_ADDRESS,MPU6050_INT_PIN_CFG ,0x02);
	//�ر�IICMASTERmode
	GY86_WriteRegister(MPU6050_ADDRESS,MPU6050_USER_CTRL   ,0x00);
}	


/*********************************************************************************
 *HMC5883L�ĳ�ʼ��������
 * - PB8��PB9�ĳ�ʼ��
 *     - ��©���
 *     - PB8ģ��SCL��PB9ģ��SDA
 * - PB8��PB9��Ĭ������ߵ�ƽ
 *     - ʹ��SDA��SCL�����ͷ�״̬
 ********************************************************************************/
void HMC5883L_init(void)
{
	//���üĴ���AΪ 0111 0000
	GY86_WriteRegister(HMC5883L_ADDRESS,HMC5883L_CR_A, 0x70);
	//���üĴ���BΪ 0010 0000
	GY86_WriteRegister(HMC5883L_ADDRESS,HMC5883L_CR_B, 0x20);
	//ģʽ�Ĵ��� Ϊ 0000 0000
	GY86_WriteRegister(HMC5883L_ADDRESS,HMC5883L_MODE, 0x00);
}
	
void MS5611_init(void)
{
	//RESET��оƬ
	MS5611_SendCommand(MS5611_RESET);
	
	SENS_T1 = MS5611_ReadC_x(MS5611_PROM_COEFFICIENT_1) << 15;
	OFF_T1  = MS5611_ReadC_x(MS5611_PROM_COEFFICIENT_2) << 16;
	TCS     = MS5611_ReadC_x(MS5611_PROM_COEFFICIENT_3) / 256.0;
	TCO     = MS5611_ReadC_x(MS5611_PROM_COEFFICIENT_4) / 128.0 ;
	T_REF   = MS5611_ReadC_x(MS5611_PROM_COEFFICIENT_5) << 8 ;
	TEMPSENS= MS5611_ReadC_x(MS5611_PROM_COEFFICIENT_6) / 8388608.0;
}

void GY86_GetData(void)
{
	int16_t DataH,DataL;
	
	DataH = GY86_ReadRegister(MPU6050_ADDRESS,MPU6050_ACCEL_XOUT_H);
	DataL = GY86_ReadRegister(MPU6050_ADDRESS,MPU6050_ACCEL_XOUT_L);
	Original_Data_List->AX = ((DataH << 8) | DataL)/16;
	
	DataH = GY86_ReadRegister(MPU6050_ADDRESS,MPU6050_ACCEL_YOUT_H);
	DataL = GY86_ReadRegister(MPU6050_ADDRESS,MPU6050_ACCEL_YOUT_L);
	Original_Data_List->AY = ((DataH << 8) | DataL)/16;
	
	DataH = GY86_ReadRegister(MPU6050_ADDRESS,MPU6050_ACCEL_ZOUT_H);
	DataL = GY86_ReadRegister(MPU6050_ADDRESS,MPU6050_ACCEL_ZOUT_L);
	Original_Data_List->AZ = ((DataH << 8) | DataL)/16;
	
	DataH = GY86_ReadRegister(MPU6050_ADDRESS,MPU6050_TEMP_OUT_H);
	DataL = GY86_ReadRegister(MPU6050_ADDRESS,MPU6050_TEMP_OUT_L);
	Original_Data_List->CORE_Temperature = (DataH << 8) | DataL;
	
	DataH = GY86_ReadRegister(MPU6050_ADDRESS,MPU6050_GYRO_XOUT_H);
	DataL = GY86_ReadRegister(MPU6050_ADDRESS,MPU6050_GYRO_XOUT_L);
	Original_Data_List->GX = ((DataH << 8) | DataL)/2000;
	
	DataH = GY86_ReadRegister(MPU6050_ADDRESS,MPU6050_GYRO_YOUT_H);
	DataL = GY86_ReadRegister(MPU6050_ADDRESS,MPU6050_GYRO_YOUT_L);
	Original_Data_List->GY = ((DataH << 8) | DataL)/2000;
	
	DataH = GY86_ReadRegister(MPU6050_ADDRESS,MPU6050_GYRO_ZOUT_H);
	DataL = GY86_ReadRegister(MPU6050_ADDRESS,MPU6050_GYRO_ZOUT_L);
	Original_Data_List->GZ = ((DataH << 8) | DataL)/2000;
	
	DataH = GY86_ReadRegister(HMC5883L_ADDRESS,HMC5883L_GA_XOUT_H);
	DataL = GY86_ReadRegister(HMC5883L_ADDRESS,HMC5883L_GA_XOUT_L);
	Original_Data_List->GaX = (DataH << 8) | DataL;
	
	DataH = GY86_ReadRegister(HMC5883L_ADDRESS,HMC5883L_GA_YOUT_H);
	DataL = GY86_ReadRegister(HMC5883L_ADDRESS,HMC5883L_GA_YOUT_L);
	Original_Data_List->GaY = (DataH << 8) | DataL;
	
	DataH = GY86_ReadRegister(HMC5883L_ADDRESS,HMC5883L_GA_ZOUT_H);
	DataL = GY86_ReadRegister(HMC5883L_ADDRESS,HMC5883L_GA_ZOUT_L);
	Original_Data_List->GaZ = (DataH << 8) | DataL;
	
	MS5611_GetPressure(MS5611_D2_OSR_4096);
	Original_Data_List->Height = P_100times;
	

}

 

 





/*********************************************************************************
 * д��/��ȡ ָ���Ĵ�����ֵ
 *   - ģ��ָ����ַд��ָ����ַ������IICʱ��
 ********************************************************************************/
void GY86_WriteRegister(uint8_t GY86_DeviceAddress, uint8_t RegAddress, uint8_t Data)
{
	MyIIC_Start();
	MyIIC_SendByte(GY86_DeviceAddress | 0x00);
	MyIIC_ReceiveACK();
	MyIIC_SendByte(RegAddress);
	MyIIC_ReceiveACK();
	MyIIC_SendByte(Data);
	MyIIC_ReceiveACK();
	MyIIC_Stop();
}

uint8_t GY86_ReadRegister(uint8_t GY86_DeviceAddress,uint8_t RegAddress)
{
	uint8_t Data;
	MyIIC_Start();
	MyIIC_SendByte(GY86_DeviceAddress | 0x00);
	MyIIC_ReceiveACK();
	MyIIC_SendByte(RegAddress);
	MyIIC_ReceiveACK();
	
	MyIIC_Start();
	MyIIC_SendByte(GY86_DeviceAddress | 0x01);
	MyIIC_ReceiveACK();
	Data = MyIIC_ReceiveByte();
	MyIIC_SendACK(1);
	MyIIC_Stop();
	
	return Data;
}

void MS5611_SendCommand(uint8_t Command)
{
	MyIIC_Start();
	MyIIC_SendByte(MS5611_ADDRESS | 0x00);
	MyIIC_ReceiveACK();
	MyIIC_SendByte(Command);
	MyIIC_ReceiveACK();
	MyIIC_Stop();
}
 
uint32_t MS5611_ReadADC()
{
	uint32_t Data;
	MyIIC_Start();
	MyIIC_SendByte(MS5611_ADDRESS | 0x00);
	MyIIC_ReceiveACK();
	MyIIC_SendByte(MS5611_ADC_READ);
	MyIIC_ReceiveACK();
	
	MyIIC_Start();
	MyIIC_SendByte(MS5611_ADDRESS | 0x01);
	MyIIC_ReceiveACK();
	Data = MyIIC_ReceiveByte();
	Data = Data << 8;
	MyIIC_SendACK(1);
	Data = MyIIC_ReceiveByte();
	Data = Data << 8;
	MyIIC_SendACK(1);
	Data = MyIIC_ReceiveByte();
	Data = Data << 8;
	MyIIC_SendACK(0);
	MyIIC_Stop();
	
	return Data;
}


uint16_t MS5611_ReadC_x(uint8_t MS5611_PROM_Cx_ADDRESS)
{
	uint32_t Data;
	MyIIC_Start();
	MyIIC_SendByte(MS5611_ADDRESS | 0x00);
	MyIIC_ReceiveACK();
	MyIIC_SendByte(MS5611_PROM_Cx_ADDRESS);
	MyIIC_ReceiveACK();
	
	MyIIC_Start();
	MyIIC_SendByte(MS5611_ADDRESS | 0x01);
	MyIIC_ReceiveACK();
	Data = MyIIC_ReceiveByte();
	Data = Data << 8;
	MyIIC_SendACK(1);
	Data = MyIIC_ReceiveByte();
	Data = Data << 8;
	MyIIC_SendACK(0);
	MyIIC_Stop();
	
	return Data;
}
 
void MS5611_GetTemperature(uint8_t MS5611_D2_OSR_xxxx)
{
	MS5611_SendCommand(MS5611_D2_OSR_xxxx);
	Delay_ms(10);
	D2_Temperature = MS5611_ReadADC();
	
	dT = D2_Temperature - T_REF;
	TEMP_100times = 2000 + dT * TEMPSENS;
}

void MS5611_GetPressure(uint8_t MS5611_D1_OSR_xxxx)
{
	MS5611_GetTemperature(MS5611_D1_OSR_4096);
	double T2 = 0;
	double OFF2 = 0;
	double SENS2 = 0;
	MS5611_SendCommand(MS5611_D1_OSR_xxxx);
	Delay_ms(10);
	D1_Pressur = MS5611_ReadADC();
	OFF  = OFF_T1  + TCO * dT;
	SENS = SENS_T1 + TCS * dT;
	
	
	if(TEMP_100times < 2000)
	{
		T2 = (dT*dT)/2147483648.0;
		OFF2 = 5*(TEMP_100times-2000)*(TEMP_100times-2000)/2;
		SENS2 = 5*(TEMP_100times-2000)*(TEMP_100times-2000)/4;
		
		if(TEMP_100times < -1500)
		{
			OFF2  = OFF2  + 7 *(TEMP_100times+1500)*(TEMP_100times+1500);
			SENS2 = SENS2 + 11*(TEMP_100times+1500)*(TEMP_100times+1500)/2;
		}
		 
	}
	
	
	TEMP_100times = TEMP_100times - T2;
	OFF =  OFF - OFF2;
	SENS = SENS -SENS2;
	
	
	P_100times = (D1_Pressur * SENS / 2097152.0 - OFF)/32768.0;
}
 
 
 