#include "stm32f10x.h"                  // Device header

uint32_t volatile msTicks = 0;
uint32_t volatile LedTicks = 0;

//Ham xu ly ngat
void SysTick_Handler(void) {
	msTicks++;
	LedTicks++;
}

//Ham tao tre ms
void DelayMs(uint32_t TICK) {
	uint32_t temp = msTicks;
	while(msTicks < TICK + temp);
	msTicks = 0;
}
