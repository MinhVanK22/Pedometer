#ifndef __MPU6050_H
#define __MPU6050_H

#include "stm32f10x.h"                  // Device header

#define MPU6050_ADDR 0xD0

#define SMPLRT_DIV_REG 0x19
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_ZOUT_H_REG 0x3F
#define PWR_MGMT_1_REG 0x6B
#define WHO_AM_I_REG 0x75

void MPU6050_Init(void);
void MPU_Write(uint8_t Address, uint8_t Reg, uint8_t Data);
void MPU_Read(uint8_t Address, uint8_t Reg, uint8_t *buffer, uint8_t size);
double MPU6050_Read_AccelZ(void);
int MPU6050_Counter(void);
#endif
