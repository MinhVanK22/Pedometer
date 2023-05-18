#include "Pedometer.h"
#include "I2C.h"
#include "MPU6050.h"

extern uint32_t volatile State;
extern uint32_t volatile LedTicks;
double volatile Ax = 0, Ay = 0, Az = 0;
int volatile a = 0;

int main(void) {
	Init_IO();
	I2C_Init();
	MPU6050_Init();
	
	while(1) {
		/* Trang thai bo dem hoat dong */
		if(State % 2 == 0) {
			GPIOA->ODR &= ~(1u<<6); //Tat led do
			if(LedTicks >= 1000) {
				GPIO_TogglePIN(GPIOC, 13); //Nhay led xanh tan so 1Hz
				LedTicks = 0;
			}
			
			Ax = MPU6050_Read_AccelX();
			Ay = MPU6050_Read_AccelY();
			Az = MPU6050_Read_AccelZ();
			a = MPU6050_Counter();
			DelayMs(100);
		}
		/* Trang thai bo dem khong hoat dong */
		else {
			GPIOC->ODR |= (1u<<13); //Tat led xanh
			GPIOA->ODR |= (1u<<6);  //Bat led do
		}
	}
}
