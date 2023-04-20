#include "MKL46Z4.h"

void PORTC_PORTD_IRQHandler(void) {
	if((PTC->PDIR&(1u<<3)) == 0) {
		PTD->PTOR |= (1u<<5);
	}
	
	if((PTC->PDIR&(1u<<12)) == 0) {
		PTE->PTOR |= (1u<<29);
	}
	
	PORTC->PCR[3] |= PORT_PCR_ISF_MASK;
	PORTC->PCR[12] |= PORT_PCR_ISF_MASK;
}
