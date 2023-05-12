#include "stm32f10x.h"                  // Device header

uint32_t volatile msTicks = 0;

void SysTick_Handler(void) {
	msTicks++;
}

void DelayMs(uint32_t TICK) {
	while(msTicks < TICK);
	msTicks = 0;
}
