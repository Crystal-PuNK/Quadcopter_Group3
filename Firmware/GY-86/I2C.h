#ifndef __I2C_H
#define __I2C_H

typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
typedef signed short     int int16_t;

void GY_WriteReg(uint8_t Address,uint8_t regist,uint8_t data);
uint8_t GY_RReadReg(uint8_t Address,uint8_t regist);
void GY_Init(void);
void GY_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ,
                        int16_t *HmX, int16_t *HmY, int16_t *HmZ);




#endif
