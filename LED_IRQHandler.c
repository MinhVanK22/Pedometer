#include "stm32f10x.h"                  // Device header

void EXTI9_5_IRQHandler(void) {
	if((GPIOA->IDR & (1u<<9)) == 0) {
		GPIOC->ODR &= ~(1u<<13);
		for(int i=0; i<3000000; i++);
		EXTI->PR |= EXTI_PR_PR9;
	}
	
	if((GPIOA->IDR & (1u<<8)) == 0) {
		GPIOA->ODR |= (1u<<6);
		for(int i=0; i<3000000; i++);
		EXTI->PR |= EXTI_PR_PR8;
	}
}
