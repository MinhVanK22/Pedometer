#include "stm32f10x.h"                  // Device header

void Init(void) {
	//Init LED and switch
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN;
	
	GPIOC->CRH |= GPIO_CRH_MODE13;
	
	GPIOA->CRL |= GPIO_CRL_MODE6;
	GPIOA->CRL &= ~GPIO_CRL_CNF6; //Noi VDD
	
	GPIOA->CRH &= ~GPIO_CRH_CNF8;
	GPIOA->CRH |= GPIO_CRH_CNF8_1;
	GPIOA->ODR |= (1u<<8); //pull up
	
	GPIOA->CRH &= ~GPIO_CRH_CNF9;
	GPIOA->CRH |= GPIO_CRH_CNF9_1;
	GPIOA->ODR |= (1u<<9); //pull up
	
	//Init interrupt line 8 and line 9
	EXTI->IMR |= EXTI_IMR_MR8 | EXTI_IMR_MR9; //Ngat Enable line 8 va 9
	EXTI->FTSR |= EXTI_FTSR_TR8 | EXTI_FTSR_TR9; //Chon ngat suon giam o line 8 va 9
	NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	//NVIC_SetPriority(EXTI9_5_IRQn,4);
	
	//Init SysTick timer
	SysTick->LOAD = SystemCoreClock / 1000;
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
}
