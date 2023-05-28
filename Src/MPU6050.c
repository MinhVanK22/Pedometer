#include "MPU6050.h"
#include "I2C.h"
#include "CLCD_I2C.h"
#include <stdio.h>

extern CLCD_I2C_Name LCD1;

int16_t Accel_X_RAW = 0;
int16_t Accel_Y_RAW = 0;
int16_t Accel_Z_RAW = 0;
int16_t Gyro_X_RAW = 0;
int16_t Gyro_Y_RAW = 0;
int16_t Gyro_Z_RAW = 0;

#define WINDOW_SIZE 5
#define threshold 1.06
uint16_t stepCount = 0;
uint8_t thresholdCount = 0;
double curAccelZ = 0, preAccelZ = 0;
double dataAccelZ[WINDOW_SIZE] = {};
uint8_t curIndex = 0, preIndex = WINDOW_SIZE - 1;

void MPU_Write (uint8_t Address, uint8_t Reg, uint8_t Data)
{
	I2C_Start ();
	I2C_Address (Address);
	I2C_Write (Reg);
	I2C_Write (Data);
	I2C_Stop ();
}

void MPU_Read (uint8_t Address, uint8_t Reg, uint8_t *buffer, uint8_t size)
{
	I2C_Start ();
	I2C_Address (Address);
	I2C_Write (Reg);
	I2C_Start ();  // Repeated start
	I2C_Read (Address+0x01, buffer, size);
	I2C_Stop ();
}

void MPU6050_Init (void)
{
	uint8_t check;
	uint8_t Data;

	// Check device
	MPU_Read (MPU6050_ADDR, WHO_AM_I_REG, &check, 1);

	if (check == 104)  // 0x68 will be returned by the sensor if everything goes well
	{
		// Power management register 0X6B we should write all 0's to wake the sensor up
		Data = 0;
		MPU_Write (MPU6050_ADDR, PWR_MGMT_1_REG, Data);

		// Set DATA RATE of 1KHz by writing SMPLRT_DIV register
		Data = 0x07;
		MPU_Write(MPU6050_ADDR, SMPLRT_DIV_REG, Data);

		// Set accelerometer configuration in ACCEL_CONFIG Register
		// XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> ± 2g
		Data = 0x00;
		MPU_Write(MPU6050_ADDR, ACCEL_CONFIG_REG, Data);

		// Set Gyroscopic configuration in GYRO_CONFIG Register
		// XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> ± 250 °/s
		Data = 0x00;
		MPU_Write(MPU6050_ADDR, GYRO_CONFIG_REG, Data);
	}

}

double MPU6050_Read_AccelX(void)
{
	uint8_t Rec_Data[2];

	// Read 2 BYTES of data starting from ACCEL_XOUT_H register
	MPU_Read (MPU6050_ADDR, ACCEL_XOUT_H_REG, Rec_Data, 2);

	Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);

	return Accel_X_RAW/16384.0; // Chuyen doi tu RAW value thanh gia tri gia toc(g): (Accel_X_RAW / Sensitivity)
}

double MPU6050_Read_AccelY(void)
{
	uint8_t Rec_Data[2];

	// Read 2 BYTES of data starting from ACCEL_YOUT_H register
	MPU_Read (MPU6050_ADDR, ACCEL_YOUT_H_REG, Rec_Data, 2);

	Accel_Y_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);

	return Accel_Y_RAW/16384.0; // Chuyen doi tu RAW value thanh gia tri gia toc(g): (Accel_Y_RAW / Sensitivity)
}

double MPU6050_Read_AccelZ(void)
{
	uint8_t Rec_Data[2];

	// Read 2 BYTES of data starting from ACCEL_ZOUT_H register
	MPU_Read (MPU6050_ADDR, ACCEL_ZOUT_H_REG, Rec_Data, 2);

	Accel_Z_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);

	return Accel_Z_RAW/16384.0; // Chuyen doi tu RAW value thanh gia tri gia toc(g): (Accel_Z_RAW / Sensitivity)
}

double MPU6050_Read_GyroX(void)
{
	uint8_t Rec_Data[2];

	// Read 2 BYTES of data starting from GYRO_XOUT_H register
	MPU_Read (MPU6050_ADDR, GYRO_XOUT_H_REG, Rec_Data, 2);

	Gyro_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);

	return Gyro_X_RAW/131.0; // Chuyen doi tu RAW value thanh gia tri dsp (°/s): (Gyro_X_RAW / Sensitivity)
}

double MPU6050_Read_GyroY(void)
{
	uint8_t Rec_Data[2];

	// Read 2 BYTES of data starting from GYRO_YOUT_H register
	MPU_Read (MPU6050_ADDR, GYRO_YOUT_H_REG, Rec_Data, 2);

	Gyro_Y_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);

	return Gyro_Y_RAW/131.0; // Chuyen doi tu RAW value thanh gia tri dsp (°/s): (Gyro_Y_RAW / Sensitivity)
}

double MPU6050_Read_GyroZ(void)
{
	uint8_t Rec_Data[2];

	// Read 2 BYTES of data starting from GYRO_ZOUT_H register
	MPU_Read (MPU6050_ADDR, GYRO_ZOUT_H_REG, Rec_Data, 2);

	Gyro_Z_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);

	return Gyro_Z_RAW/131.0; // Chuyen doi tu RAW value thanh gia tri dsp (°/s): (Gyro_Z_RAW / Sensitivity)
}

// Ham dem buoc chan
int MPU6050_Counter(void) 
{
	char buf[4];
	curAccelZ = MPU6050_Read_AccelZ();
	
	dataAccelZ[curIndex] = curAccelZ;
	curIndex = (curIndex + 1) % WINDOW_SIZE;
	
	double filterAccelZ = 0;
  for(int i = 0; i < WINDOW_SIZE; i++) {
     filterAccelZ += dataAccelZ[i];
  }
  filterAccelZ /= WINDOW_SIZE;
	
	preAccelZ = dataAccelZ[preIndex];
	if(filterAccelZ > threshold && curAccelZ > preAccelZ) {
		thresholdCount++;
	}
	
	if(thresholdCount > 2) {
		stepCount++; //Dem buoc chan
		
		//In so buoc chan ra man hinh
		CLCD_I2C_SetCursor(&LCD1,2,0);
		CLCD_I2C_WriteString(&LCD1, "So buoc chan");
		sprintf(buf, "%d", stepCount);
		CLCD_I2C_SetCursor(&LCD1,6,1);
		CLCD_I2C_WriteString(&LCD1, buf);
		
		thresholdCount = 0; //Dat lai so lan vuot nguong
	}
	
	if(curIndex == 0) {
		preIndex = WINDOW_SIZE - 1;
	} else {
		preIndex = curIndex - 1;
	}
	
	return stepCount;
}
