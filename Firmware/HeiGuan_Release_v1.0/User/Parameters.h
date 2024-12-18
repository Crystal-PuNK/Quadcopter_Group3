#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stdint.h>

#define FLASH_SECTOR_7_ORIGIN 0x08060000
#define FLASH_SECTOR_7_END    0x0807FFFF

void LoadParameters(void);
void StoreParameters(void);


// 参数列表结构体定义
typedef struct {
    int32_t test;
    float accel_offset[3];   // 加速度计校准偏移
    float gyro_offset[3];    // 陀螺仪校准偏移
    float kp;                // PID 参数：比例
    float ki;                // PID 参数：积分
    float kd;                // PID 参数：微分
    // uint32_t checksum;       // 校验值，用于验证数据完整性
} FlightParams;

// 声明参数列表变量（定义在 .c 文件中）
extern const FlightParams flight_params;
extern FlightParams params_ram;  // RAM 中的参数副本


#endif /* PARAMETERS_H */