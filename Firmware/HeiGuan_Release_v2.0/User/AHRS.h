#ifndef AHRS_H
#define AHRS_H

#include "stm32f4xx.h"
//----------------------------------------------------------------------------------------------------
// Variable declaration

typedef struct {
	float yaw, pitch, roll;
} Angle;
extern volatile float twoKp;			// 2 * proportional gain (Kp)
extern volatile float twoKi;			// 2 * integral gain (Ki)
extern volatile float q0, q1, q2, q3;	// quaternion of sensor frame relative to auxiliary frame
extern const float PI;
extern const float RAD_TO_DEGREE;
extern const float DEGREE_TO_RAD;
//---------------------------------------------------------------------------------------------------
// Function declarations

void MahonyAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
void MahonyAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);
void Attitude_Update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);


#endif





