#include "GuassNewton.h"
#include <math.h>
#include <stdio.h>
#define M 300 // 数据测量组数
#define N 6 // 元数
static const float eps = 1e-6;
float LM_lamda = 0.1f;


float acc [M][3];
float gyro[M][3];
float mag [M][3];

// 校准前收集数据
void prepareData() {
    OLED_ShowString(2,1,"Collecting  Data");
    OLED_ShowString(3,1,"   Begin in 3s  ");
    OLED_ShowString(4,1,"**            **");
    Delay_s(1); OLED_ShowString(4,1,"**----        **");
    Delay_s(1); OLED_ShowString(4,1,"**--------    **");
    Delay_s(1); OLED_ShowString(4,1,"**------------**");
    OLED_ShowString(2,1,"    Acc Mag     ");
    OLED_ShowString(3,1,"   End in 7s    ");
    OLED_ShowString(4,1,"*              *");
    for (int i = 0; i < 3; i++)
    {
        params_ram.accel_offset[i] = 0.0f;
        params_ram.gyro_offset [i] = 0.0f;
        params_ram.mag_offset  [i] = 0.0f;
        params_ram.accel_scale [i] = 1.0f;
        params_ram.gyro_scale  [i] = 1.0f;
        params_ram.mag_scale   [i] = 1.0f;
    }
    for (int i = 0; i < M; ++i) {
        GY86_GetData();
        acc[i][0]= GY86DataList.AX;
        acc[i][1]= GY86DataList.AY;
        acc[i][2]= GY86DataList.AZ;
        mag[i][0]= GY86DataList.GaX;
        mag[i][1]= GY86DataList.GaY;
        mag[i][2]= GY86DataList.GaZ;
        Delay_ms(5);
        if      (i == 50-1 ){OLED_ShowString(4,1,"**--          **");Delay_ms(1500);}
        else if (i == 100-1){OLED_ShowString(4,1,"**----        **");Delay_ms(1500);}
        else if (i == 150-1){OLED_ShowString(4,1,"**------      **");Delay_ms(1500);}
        else if (i == 200-1){OLED_ShowString(4,1,"**--------    **");Delay_ms(1500);}
        else if (i == 250-1){OLED_ShowString(4,1,"**----------  **");Delay_ms(1500);}
    }
    OLED_ShowString(4,1,"**------------**");
    Delay_ms(500);
    OLED_ShowString(2,1,"      Gyro      ");
    OLED_ShowString(3,1,"   End in 3s    ");
    OLED_ShowString(4,1,"*              *");
    for (int i = 0; i < M; ++i) {
        gyro[i][0] = GY86DataList.GX;
        gyro[i][1] = GY86DataList.GY;
        gyro[i][2] = GY86DataList.GZ;
        Delay_ms(10);
        if      (i == 50-1 ){OLED_ShowString(4,1,"**--          **");}
        else if (i == 100-1){OLED_ShowString(4,1,"**----        **");}
        else if (i == 150-1){OLED_ShowString(4,1,"**------      **");}
        else if (i == 200-1){OLED_ShowString(4,1,"**--------    **");}
        else if (i == 250-1){OLED_ShowString(4,1,"**----------  **");}
    }
    OLED_ShowString(4,1,"**------------**");
    float gyroSum[3] = {0, 0, 0};
    for (int i = 0; i < M; ++i) {
        gyroSum[0] += gyro[i][0];
        gyroSum[1] += gyro[i][1];
        gyroSum[2] += gyro[i][2];
    }
    params_ram.gyro_offset[0] = gyroSum[0] / M;
    params_ram.gyro_offset[1] = gyroSum[1] / M;
    params_ram.gyro_offset[2] = gyroSum[2] / M;
    OLED_ShowString(2,1,"Collecting  Data");
    OLED_ShowString(3,1,"    Finished    ");
    OLED_ShowString(4,1,"****************");
}

float constrainFloat(float x, float low, float high) {
    if (isnan(x)) {
        return (low + high) * 0.5f;
    }
    return ((x) < (low) ? (low) : ((x) > (high) ? (high) : (x)));
}

void gaussElimination(float (*a)[7]) {
    for (int i = 0; i < 6; ++i) {
		int r = i;
		for (int j = i + 1; j < 6; ++j)
		    if(fabsf(a[j][i]) > fabsf(a[r][i])) r = j;
        if (fabsf(a[r][i]) < eps) {
			printf("Error: gaussElimination no solution!\r\n");
			return;
		}
		if(r != i) {
            for (int j = 0; j < 6 + 1; ++j) {
                float temp = a[i][j];
                a[i][j] = a[r][j];
                a[r][j] = temp;
            }
        }
		for (int j = i + 1; j < 6; ++j){
			const float div = a[j][i] / a[i][i];
            for (int k = i; k < 6 + 1; ++k)
			    a[j][k] -= div * a[i][k];
		}
	}
	for(int i = 6 - 1; i >= 0; --i){
		for (int j = i + 1; j < 6; ++j)
		    a[i][6] -= a[j][6] * a[i][j];
		a[i][6] /= a[i][i];
	}
} 

void gaussNewton(float *caliVal, float (*data)[3]) {
    float Delta = 100.0f, DeltaNew = 0.0f; 
    static float Jr[M][N], JrT[N][M]; // Jacobi 矩阵以及 Jacobi 矩阵的转置
    static float r[M]; // 残差函数 r(beta)
    static float delta[N][N + 1]; // 待求的迭代增量 delta，也是高斯消元的系数矩阵
    float sum = 0.0f;

    // 初始化    
    static float beta_offset[3],beta_scale[3];
    beta_offset[0] = beta_offset[1] = beta_offset[2] = 0.0f;
    beta_scale [0] = beta_scale [1] = beta_scale [2] = 1.0f;
    LM_lamda = 0.1f;

    int cnt = 0;
    // 迭代
    while (Delta > eps && cnt < 100) {
        // 计算 Jacobi 矩阵
        for (int i = 0; i < M; ++i) {
            Jr[i][0] = 2.0f * beta_scale[0] * beta_scale[0] * (beta_offset[0] - data[i][0]);
            Jr[i][1] = 2.0f * (data[i][0] - beta_offset[0]) * (data[i][0] - beta_offset[0]) * beta_scale[0];
            Jr[i][2] = 2.0f * beta_scale[1] * beta_scale[1] * (beta_offset[1] - data[i][1]);
            Jr[i][3] = 2.0f * (data[i][1] - beta_offset[1]) * (data[i][1] - beta_offset[1]) * beta_scale[1];
            Jr[i][4] = 2.0f * beta_scale[2] * beta_scale[2] * (beta_offset[2] - data[i][2]);
            Jr[i][5] = 2.0f * (data[i][2] - beta_offset[2]) * (data[i][2] - beta_offset[2]) * beta_scale[2];
        }
        // 计算 Jacobi 矩阵的转置
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                JrT[j][i] = Jr[i][j];
            }
        }
        sum = 0.0;
        // 计算残差函数 r(beta)
        for (int i = 0; i < M; ++i) {
            r[i] = beta_scale[0] * beta_scale[0] * (beta_offset[0] - data[i][0]) * (beta_offset[0] - data[i][0]) + 
                   beta_scale[1] * beta_scale[1] * (beta_offset[1] - data[i][1]) * (beta_offset[1] - data[i][1]) + 
                   beta_scale[2] * beta_scale[2] * (beta_offset[2] - data[i][2]) * (beta_offset[2] - data[i][2]) - 1.0f;
            sum += r[i] * r[i];
        }
        // 计算 JrT * Jr 并作为系数矩阵放入 delta
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                delta[i][j] = 0;
                for (int k = 0; k < M; ++k) {
                    delta[i][j] += JrT[i][k] * Jr[k][j];
                }
            }
        }
        // 计算 JrT * r 并作为增广列放入 delta
        for (int i = 0; i < N; ++i) {
            delta[i][N] = 0;
            for (int j = 0; j < M; ++j) {
                delta[i][N] += JrT[i][j] * r[j];
            }
        }
        // 加入 LM 因子
        for (int i = 0; i < N; ++i) {
            delta[i][i] += LM_lamda;
        }
        // 高斯消元
        gaussElimination(delta);
        // 计算 DeltaNew
        DeltaNew = 0;
        for (int i = 0; i < N; ++i) {
            DeltaNew += delta[i][N] * delta[i][N];
        }
        if (DeltaNew < Delta) {
            // LM 因子减小
            LM_lamda /= 3.0;

            // 更新 beta
            beta_offset[0] -= delta[0][N];
            beta_scale[0] -= delta[1][N];
            beta_offset[1] -= delta[2][N];
            beta_scale[1] -= delta[3][N];
            beta_offset[2] -= delta[4][N];
            beta_scale[2] -= delta[5][N];

            Delta = DeltaNew;
        } else {
            // LM 因子增大
            LM_lamda *= 3.0;
            LM_lamda = constrainFloat(LM_lamda, 0, 1e10f);
        }
        
        printf("After %d iterations: Ox = %f, Sx = %f, Oy = %f, Sy = %f, Oz = %f, Sz = %f lastSum = %f\r\n", 
            ++cnt, beta_offset[0], beta_scale[0], beta_offset[1], beta_scale[1], beta_offset[2], beta_scale[2], sum);
    }
    // 将校准结果写入 caliVal
    caliVal[0] = beta_offset[0];
    caliVal[1] = beta_offset[1];
    caliVal[2] = beta_offset[2];
    caliVal[3] = beta_scale[0];
    caliVal[4] = beta_scale[1];
    caliVal[5] = beta_scale[2];
}

void IMU_Calibrate(){
    float cali_buffer[6] = {0,0,0,1,1,1};
    prepareData();
    OLED_ShowString(2,1,"Calibrating Acc ");
    OLED_ShowString(3,1,"*              *");
    OLED_ShowString(4,1,"****************");
    gaussNewton(cali_buffer,acc);
    OLED_ShowFNum(2,1,cali_buffer[0],5,4);OLED_ShowFNum(2,9,cali_buffer[3],5,4);
    OLED_ShowFNum(3,1,cali_buffer[1],5,4);OLED_ShowFNum(3,9,cali_buffer[4],5,4);
    OLED_ShowFNum(4,1,cali_buffer[2],5,4);OLED_ShowFNum(4,9,cali_buffer[5],5,4);
    Delay_s(3);
    for (int i = 0; i < 3; i++)
    {
        params_ram.accel_offset[i] = cali_buffer[i];
        params_ram.accel_scale[i]  = cali_buffer[i+3];
    }
    OLED_ShowString(2,1,"Calibrating Gyro");
    OLED_ShowString(3,1,"*              *");
    OLED_ShowString(4,1,"****************");
    gaussNewton(cali_buffer,gyro);
    OLED_ShowFNum(2,1,cali_buffer[0],5,4);OLED_ShowFNum(2,9,cali_buffer[3],5,4);
    OLED_ShowFNum(3,1,cali_buffer[1],5,4);OLED_ShowFNum(3,9,cali_buffer[4],5,4);
    OLED_ShowFNum(4,1,cali_buffer[2],5,4);OLED_ShowFNum(4,9,cali_buffer[5],5,4);
    Delay_s(3);
    for (int i = 0; i < 3; i++)
    {
        params_ram.gyro_offset[i] = cali_buffer[i];
        params_ram.gyro_scale[i]  = cali_buffer[i+3];
    }
    OLED_ShowString(2,1,"Calibrating Mag ");
    OLED_ShowString(3,1,"*              *");
    OLED_ShowString(4,1,"****************");
    gaussNewton(cali_buffer,mag);
    OLED_ShowFNum(2,1,cali_buffer[0],5,4);OLED_ShowFNum(2,9,cali_buffer[3],5,4);
    OLED_ShowFNum(3,1,cali_buffer[1],5,4);OLED_ShowFNum(3,9,cali_buffer[4],5,4);
    OLED_ShowFNum(4,1,cali_buffer[2],5,4);OLED_ShowFNum(4,9,cali_buffer[5],5,4);
    Delay_s(3);
    for (int i = 0; i < 3; i++)
    {
        params_ram.mag_offset[i] = cali_buffer[i];
        params_ram.mag_scale[i]  = cali_buffer[i+3];
    }
}