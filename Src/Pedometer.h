#ifndef __PEDOMETER_H
#define __PEDOMETER_H

#include "stm32f10x.h"                  // Device header

void Init_IO(void);
void Init_Timer(void);
void DelayMs(uint32_t TICK);
void GPIO_TogglePIN(GPIO_TypeDef *GPIOx, uint16_t PIN);

#endif
