#include "stm32f10x.h"                  // Device header
#include "CLCD_I2C.h"

extern uint16_t stepCount;
extern CLCD_I2C_Name LCD1;

uint32_t volatile State = 0; //Bien trang thai cua may dem buoc chan

//Ham xu ly ngat switch
void EXTI9_5_IRQHandler(void) {
	if((GPIOA->IDR & (1u<<8)) == 0) {
		State++;
		
		for(int i = 0; i < 3000000; i++);
		EXTI->PR |= EXTI_PR_PR8;
	}
	
	if((GPIOA->IDR & (1u<<9)) == 0) {
		stepCount = 0; //Reset value counter
		
		//Reset man hinh LCD ve trang thai ban dau
		CLCD_I2C_Clear(&LCD1);
		CLCD_I2C_SetCursor(&LCD1,1,0);
		CLCD_I2C_WriteString(&LCD1, "_So buoc chan_");	
		CLCD_I2C_SetCursor(&LCD1,6,1);
		CLCD_I2C_WriteString(&LCD1, "0");
		
		for(int i = 0; i < 3000000; i++);
		EXTI->PR |= EXTI_PR_PR9;
	}
}

//Ham toggle pin output 
void GPIO_TogglePIN(GPIO_TypeDef *GPIOx, uint16_t PIN) {
		GPIOx->ODR ^= (1u << PIN);
}
