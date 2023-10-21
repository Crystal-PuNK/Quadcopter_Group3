#ifndef __GY86_H
#define __GY86_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "Delay.h"
#include "MyIIC.h"

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






/* Public Variables -------------------------------------------------------------------*/
struct GY86_Data{
	int16_t AX;
	int16_t AY;
	int16_t AZ;
	int16_t CORE_Temperature;
	int16_t GX;
	int16_t GY;
	int16_t GZ;
	int16_t GaX;
	int16_t GaY;
	int16_t GaZ;
	int32_t Height;
};

/* Public Functions -------------------------------------------------------------------*/

void GY86_init(void);

void GY86_GetData(void);

#endif
