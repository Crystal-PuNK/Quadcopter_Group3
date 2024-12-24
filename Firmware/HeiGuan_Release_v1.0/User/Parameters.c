#include "Parameters.h"

// 定义参数列表（位于 Flash 的 .param_section 段中）
__attribute__((section(".param_section"))) const FlightParams flight_params = {
    .test = 114514,
    .accel_offset = {0.02f, -0.01f, 0.0f,},
    .accel_scale = {1.0f,1.003f,1.0f,},
    .gyro_offset = {0.05f, 0.01f, 0.017f,},
    .gyro_scale = {1.0f,1.0f,1.0f,},
    .mag_offset = {0.0f,0.0f,0.0f,},
    .mag_scale = {1.0f,1.0f,1.0f,},
    .kp = 1.0f,
    .ki = 0.5f,
    .kd = 1.0f,
    .rollOffset = {0.0f},
    .pitchOffset = {0.0f},
    
    // .checksum = 0xDEADBEEF  // 示例校验值
};

FlightParams params_ram;

void LoadParameters() {
    // 从 Flash 中读取参数到 RAM
    memcpy(&params_ram, (void *)&flight_params, sizeof(FlightParams));
}

void StoreParameters(){
    u32 addrx = FLASH_SECTOR_7_ORIGIN;
    u32 endaddr = FLASH_SECTOR_7_ORIGIN + sizeof(FlightParams);
    u32* pBuffer =  (u32 *)&params_ram;
    FLASH_Status status = FLASH_COMPLETE;
    FLASH_Unlock();//解锁
    FLASH_DataCacheCmd(DISABLE);//FLASH 擦除期间,必须禁止数据缓存

    status = FLASH_EraseSector(FLASH_Sector_7,VoltageRange_3);//VCC=2.7~3.6V 之间!!

    if(status==FLASH_COMPLETE){
        while(addrx<endaddr)//写数据
        {
        if(FLASH_ProgramWord(addrx,*pBuffer)!=FLASH_COMPLETE)//写入数据
            {
                break; //写入异常
            }
        addrx+=4; pBuffer++;
        }

    }

    FLASH_DataCacheCmd(ENABLE); //FLASH 擦除结束,开启数据缓存
    FLASH_Lock();//上锁
}