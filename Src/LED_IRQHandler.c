#include "stm32f10x.h"                  // Device header

uint32_t volatile State = 0;
uint8_t volatile resetLCD = 0;
extern unsigned int stepCount;

//Ham Toggle pin output 
void GPIO_TogglePIN(GPIO_TypeDef *GPIOx, uint16_t PIN) {
		GPIOx->ODR ^= (1u << PIN);
}

//Ham xu ly ngat
void EXTI9_5_IRQHandler(void) {
	if((GPIOA->IDR & (1u<<8)) == 0) {
		State++;
		
		for(int i = 0; i < 3000000; i++);
		EXTI->PR |= EXTI_PR_PR8;
	}
	
	if((GPIOA->IDR & (1u<<9)) == 0) {
		stepCount = 0; //Reset value counter
		resetLCD = 1;
		
		for(int i = 0; i<3000000; i++);
		EXTI->PR |= EXTI_PR_PR9;
	}
}
