#include "I2C.h"

void I2C_Init(void) {
	/* Init I2C */
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; //Kich hoat clock I2C1
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN; //Kich hoat clock port B va AFIO
	
	//Chon port 6 va 7 la alternate funtion output open-drain (SCL, SDA)
	GPIOB->CRL |= GPIO_CRL_MODE6 | GPIO_CRL_MODE7;
	GPIOB->CRL &= ~(GPIO_CRL_CNF6 | GPIO_CRL_CNF7);
	GPIOB->CRL |= GPIO_CRL_CNF6 | GPIO_CRL_CNF7;
	
	//Thiet lap I2C1
	I2C1->CR1 |= I2C_CR1_SWRST;  // Reset I2C
	I2C1->CR1 &= ~I2C_CR1_SWRST;
	
	I2C1->CR2 |= 0x08; // Peripheral clock frequency 8MHz
	I2C1->CCR |= 0x28; // Generate 100 KHz SCL frequency
	I2C1->TRISE = 0x9; // Transition time between transmit and receive
	I2C1->CR1 |= I2C_CR1_PE; // Enable I2C
	
}

void I2C_Start(void) {
	I2C1->CR1 |= I2C_CR1_ACK;  // Enable the ACK
	I2C1->CR1 |= I2C_CR1_START; // Generate START
	while(!(I2C1->SR1 & I2C_SR1_SB)); //Wait for SB bit to set
}

void I2C_Write(uint8_t data) {
	while(!(I2C1->SR1 & (1u<<7))) {};  // Wait for TxE bit to set
  I2C1->DR = data;
  while(!(I2C1->SR1 & (1u<<2)));  // Wait for BTF bit to set
}

void I2C_Address(uint8_t address) {
	I2C1->DR = address;  // Send the address
  while(!(I2C1->SR1 & (1u<<1)));  // Wait for ADDR bit to set
  uint8_t temp = I2C1->SR1 | I2C1->SR2;  // Read SR1 and SR2 to clear the ADDR bit
}

void I2C_Read(uint8_t address, uint8_t *buffer, uint8_t size) {
	uint8_t remaining = size;
	
	/* STEP 1: Read only 1 byte */	
	if(size == 1)
	{
		/* STEP 1-a */	
		I2C1->DR = address;  // Send the address
		while (!(I2C1->SR1 & (1u<<1)));  // Wait for ADDR bit to set
		
		/* STEP 1-b */	
		I2C1->CR1 &= ~I2C_CR1_ACK;  // Clear the ACK bit 
		uint8_t temp = I2C1->SR1 | I2C1->SR2;  // Read SR1 and SR2 to clear the ADDR bit
		I2C1->CR1 |= I2C_CR1_STOP;  // Stop I2C

		/* STEP 1-c */	
		while (!(I2C1->SR1 & (1u<<6)));  // Wait for RxNE to set
		
		/* STEP 1-d */	
		buffer[size-remaining] = I2C1->DR;  // Read the data from the DATA REGISTER
		
	}

	/* STEP 2: Read multiple byte */		
	else 
	{
		/* STEP 2-a */
		I2C1->DR = address;  // Send the address
		while (!(I2C1->SR1 & (1u<<1)));  // Wait for ADDR bit to set
		
		/* STEP 2-b */
		uint8_t temp = I2C1->SR1 | I2C1->SR2;  // Read SR1 and SR2 to clear the ADDR bit
		
		while (remaining > 2)
		{
			/* STEP 2-c */
			while (!(I2C1->SR1 & (1u<<6)));  // Wait for RxNE to set
			
			/* STEP 2-d */
			buffer[size-remaining] = I2C1->DR;  // Copy the data into the buffer			
			
			/* STEP 2-e */
			I2C1->CR1 |= I2C_CR1_ACK;  // Set the ACK bit to Acknowledge the data received
			
			remaining--;
		}
		
		// Read the SECOND LAST BYTE
		while (!(I2C1->SR1 & (1u<<6)));  // Wait for RxNE to set
		buffer[size-remaining] = I2C1->DR;
		
		/* STEP 2-f */
		I2C1->CR1 &= ~I2C_CR1_ACK;  // Clear the ACK bit 
		
		/* STEP 2-g */
		I2C1->CR1 |= I2C_CR1_STOP;  // Stop I2C
		
		remaining--;
		
		// Read the Last BYTE
		while (!(I2C1->SR1 & (1u<<6)));  // Wait for RxNE to set
		buffer[size-remaining] = I2C1->DR;  // Copy the data into the buffer
	}
}

void I2C_Stop(void) {
	I2C1->CR1 |= I2C_CR1_STOP; // Generate STOP
}
