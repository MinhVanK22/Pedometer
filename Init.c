#include "MKL46Z4.h"

void Init(void) {
	/* Init green led and red led */
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	PORTD->PCR[5] |= PORT_PCR_MUX(1u);
	PTD->PDDR |= (1u<<5);
	
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	PORTE->PCR[29] |= PORT_PCR_MUX(1u);
	PTE->PDDR |= (1u<<29);
	
	/* Init SW1 and SW3 */
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	PORTC->PCR[3] = PORT_PCR_MUX(1u) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PORTC->PCR[12] = PORT_PCR_MUX(1u) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PTC->PDDR &= ~(1u<<3);
	PTC->PDDR &= ~(1u<<12);
	
	/* Set interrupt for switch*/
	PORTC->PCR[3] |= PORT_PCR_IRQC(0xA); //ngat o suon giam
	PORTC->PCR[12] |= PORT_PCR_IRQC(0xA);
	NVIC_ClearPendingIRQ(31);
	NVIC_EnableIRQ(31);
}
