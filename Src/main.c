#include "Pedometer.h"
#include "I2C.h"
#include "MPU6050.h"
#include "CLCD_I2C.h"
#include <stdio.h>

extern uint32_t volatile State;
extern uint32_t volatile LedTicks;
extern uint8_t volatile resetLCD;

unsigned int volatile numberSteps = 0;
CLCD_I2C_Name LCD1;

int main(void) {
	Init_IO();
	I2C_Init();
	MPU6050_Init();
	CLCD_I2C_Init(&LCD1,SLAVE_ADDRESS_LCD,16,2);
	char buf[4];
	
	while(1) {
    if(resetLCD == 1) {
			CLCD_I2C_Clear(&LCD1);
			CLCD_I2C_SetCursor(&LCD1,2,0);
			CLCD_I2C_WriteString(&LCD1, "So buoc chan");	
			CLCD_I2C_SetCursor(&LCD1,6,1);
			CLCD_I2C_WriteString(&LCD1, "0");
			
			resetLCD = 0;
		}
		/* Trang thai bo dem hoat dong */
		if(State % 2 == 0) {
			GPIOA->ODR &= ~(1u<<6); //Tat led do
			
			if(LedTicks >= 1000) {
				GPIO_TogglePIN(GPIOC, 13); //Nhay led xanh tan so 1Hz
				LedTicks = 0;
			}
		  
			numberSteps = MPU6050_Counter(); //Bat dau dem buoc chan
			
			CLCD_I2C_SetCursor(&LCD1,2,0);
			CLCD_I2C_WriteString(&LCD1, "So buoc chan");
			
			sprintf(buf, "%d", numberSteps);
			CLCD_I2C_SetCursor(&LCD1,6,1);
			CLCD_I2C_WriteString(&LCD1, buf);
			DelayMs(50);
		}
		/* Trang thai bo dem khong hoat dong */
		else {
			GPIOC->ODR |= (1u<<13); //Tat led xanh
			GPIOA->ODR |= (1u<<6);  //Bat led do
		}
	}
}
