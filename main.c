#include "Pedometer.h"

int main(void) {
	Init();
	
	while(1) {
		GPIOC->ODR |= (1u<<13);
		GPIOA->ODR &= ~(1u<<6);
		DelayMs(1000);
		GPIOC->ODR &= ~(1u<<13);
		GPIOA->ODR |= (1u<<6);
		DelayMs(1000);
	}
}
