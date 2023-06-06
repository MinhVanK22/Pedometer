#include "stm32f10x.h"                  // Device header
#include "MPU6050.h"

void TIM2_IRQHandler(void) {
    if(TIM2->SR & TIM_SR_UIF) {
			MPU6050_Counter(); //Dem buoc chan
			
			TIM2->SR &= ~TIM_SR_UIF; // Clear interrupt flag
    }
}
