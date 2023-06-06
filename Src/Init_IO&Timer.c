#include "stm32f10x.h"                  // Device header

void Init_IO(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN; //Kich hoat clock port A, port C va AFIO
	
	/* Init LED and switch */
	GPIOC->CRH |= GPIO_CRH_MODE13; //Chon chan C13 la output
	
	GPIOA->CRL |= GPIO_CRL_MODE6; //Chon chan A6 la output
	GPIOA->CRL &= ~GPIO_CRL_CNF6; //GPIO push-pull
	
	//Chon chan A8 la input
	GPIOA->CRH &= ~GPIO_CRH_CNF8;
	GPIOA->CRH |= GPIO_CRH_CNF8_1; //Input with pull up or pull down
	GPIOA->ODR |= (1u<<8); //Select pull up
	
	//Chon chan A9 la input
	GPIOA->CRH &= ~GPIO_CRH_CNF9;
	GPIOA->CRH |= GPIO_CRH_CNF9_1; //Input with pull up or pull down
	GPIOA->ODR |= (1u<<9); //Select pull up
	
	/* Init interrupt line 8 and line 9 */
	EXTI->IMR |= EXTI_IMR_MR8 | EXTI_IMR_MR9; //Ngat enable o line 8 va 9
	EXTI->FTSR |= EXTI_FTSR_TR8 | EXTI_FTSR_TR9; //Chon ngat suon giam o line 8 va 9
	NVIC_SetPriority(EXTI9_5_IRQn,8);
	NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	
	/* Serial Wire Debug */
  AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE; //JTAG-DP Disabled and SW-DP Enabled
}

void Init_Timer(void) {
	/* Init SysTick timer */
	SysTick->LOAD = SystemCoreClock / 1000; //Dem trong 1ms
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk; //0x7
	NVIC_SetPriority(SysTick_IRQn,6);
	
	/* Init TIM2 */
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;  //Enable clock TIM2
  TIM2->CR1 |= TIM_CR1_CEN | TIM_CR1_ARPE; 	//Enable counter, auto reload
	TIM2->DIER |= TIM_DIER_UIE;  //Enable TIM2 interrupt
	TIM2->PSC = 6000 - 1; // tDelay = (PSC+1)*(ARR+1)/SrcClock 
  TIM2->ARR = 300 - 1;
	NVIC_SetPriority(TIM2_IRQn,7);
	NVIC_EnableIRQ(TIM2_IRQn);
}
