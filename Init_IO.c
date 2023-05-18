#include "stm32f10x.h"                  // Device header

void Init_IO(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN; //Kich hoat clock port A, port C va AFIO
	
	/* Init LED and switch */
	GPIOC->CRH |= GPIO_CRH_MODE13; //Chon chan C13 la output
	
	//Chon chan A6 la output
	GPIOA->CRL |= GPIO_CRL_MODE6;
	GPIOA->CRL &= ~GPIO_CRL_CNF6; //GPIO push-pull
	
	//Chon chan A8 la input
	GPIOA->CRH &= ~GPIO_CRH_CNF8;
	GPIOA->CRH |= GPIO_CRH_CNF8_1;
	GPIOA->ODR |= (1u<<8); //pull up
	
	//Chon chan A9 la input
	GPIOA->CRH &= ~GPIO_CRH_CNF9;
	GPIOA->CRH |= GPIO_CRH_CNF9_1;
	GPIOA->ODR |= (1u<<9); //pull up
	
	/* Init interrupt line 8 and line 9 */
	EXTI->IMR |= EXTI_IMR_MR8 | EXTI_IMR_MR9; //Ngat Enable line 8 va 9
	EXTI->FTSR |= EXTI_FTSR_TR8 | EXTI_FTSR_TR9; //Chon ngat suon giam o line 8 va 9
	NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	//NVIC_SetPriority(EXTI9_5_IRQn,4);
	
	/* Init SysTick timer */
	SysTick->LOAD = SystemCoreClock / 1000;
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
	
	/* Serial Wire Debug */  
  AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE; //JTAG-DP Disabled and SW-DP Enabled
  //DBGMCU->CR |= DBGMCU_CR_DBG_STANDBY | DBGMCU_CR_TRACE_MODE; // Kích hoat che do go loi SWD
}
