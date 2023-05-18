#include "stm32f10x.h"                  // Device header

uint32_t volatile State = 0;

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
		GPIO_TogglePIN(GPIOA, 6);
		
		for(int i = 0; i<3000000; i++);
		EXTI->PR |= EXTI_PR_PR9;
	}
}


