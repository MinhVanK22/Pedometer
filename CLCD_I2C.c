#include "CLCD_I2C.h"
#include "Pedometer.h"
#include "I2C.h"

void LCD_WriteI2C(uint8_t Address, uint8_t *Data, int size)
{
	I2C_Start ();
	I2C_Address (Address);
	for (int i=0; i<size; i++) {
		I2C_Write (*Data++);
	}
	I2C_Stop ();
}

static void CLCD_Write(CLCD_I2C_Name* LCD, uint8_t Data, uint8_t Mode)
{
	char Data_H;
	char Data_L;
	uint8_t Data_I2C[4];
	Data_H = Data & 0xF0;
	Data_L = (Data<<4) & 0xF0;
	
	if(LCD->BACKLIGHT)
	{
		Data_H |= LCD_BACKLIGHT; 
		Data_L |= LCD_BACKLIGHT; 
	}
	
	if(Mode == CLCD_DATA)
	{
		Data_H |= LCD_RS;
		Data_L |= LCD_RS;
	}
	else if(Mode == CLCD_COMMAND)
	{
		Data_H &= ~LCD_RS;
		Data_L &= ~LCD_RS;
	}
	
	Data_I2C[0] = Data_H | LCD_EN;
	DelayMs(1);
	Data_I2C[1] = Data_H;
	Data_I2C[2] = Data_L | LCD_EN;
	DelayMs(1);
	Data_I2C[3] = Data_L;
	LCD_WriteI2C (SLAVE_ADDRESS_LCD, (uint8_t *) Data_I2C, 4);
}


void CLCD_I2C_Init(CLCD_I2C_Name* LCD, uint8_t Address, uint8_t Colums, uint8_t Rows)
{
	LCD->ADDRESS = Address;
	LCD->COLUMS = Colums;
	LCD->ROWS = Rows;
	
	LCD->FUNCTIONSET = LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS; //Chon che do 4 bit, 2 dong và 5x8 dots
	//LCD->ENTRYMODE = LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT; //Man hinh dich phai
	LCD->DISPLAYCTRL = LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF; //Bat man hinh, tat hien thi con tro va tat blink
	LCD->CURSORSHIFT = LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVERIGHT; //Dat con tro o che do dich sang phai
	LCD->BACKLIGHT = LCD_BACKLIGHT;

	DelayMs(30);
	CLCD_Write(LCD, 0x30, CLCD_COMMAND);
  DelayMs(5);
	CLCD_Write(LCD, 0x30, CLCD_COMMAND);
	DelayMs(1);
	CLCD_Write(LCD, 0x30, CLCD_COMMAND);
	DelayMs(1);
	CLCD_Write(LCD, 0x20, CLCD_COMMAND);
	DelayMs(1);
	
	CLCD_Write(LCD, LCD->FUNCTIONSET, CLCD_COMMAND);
	//CLCD_Write(LCD, LCD->ENTRYMODE, CLCD_COMMAND);
	CLCD_Write(LCD, LCD->DISPLAYCTRL, CLCD_COMMAND);
	CLCD_Write(LCD, LCD->CURSORSHIFT, CLCD_COMMAND);
	
	
	CLCD_Write(LCD, LCD_CLEARDISPLAY, CLCD_COMMAND);
	CLCD_Write(LCD, LCD_RETURNHOME, CLCD_COMMAND);
}

void CLCD_I2C_SetCursor(CLCD_I2C_Name* LCD, uint8_t Xpos, uint8_t Ypos)
{
	uint8_t DRAM_ADDRESS = 0x00;
	if(Xpos >= LCD->COLUMS)
	{
		Xpos = LCD->COLUMS - 1;
	}
	if(Ypos >= LCD->ROWS)
	{
		Ypos = LCD->ROWS - 1;
	}
	if(Ypos == 0)
	{
		DRAM_ADDRESS = 0x00 + Xpos;
	}
	else if(Ypos == 1)
	{
		DRAM_ADDRESS = 0x40 + Xpos;
	}
	else if(Ypos == 2)
	{
		DRAM_ADDRESS = 0x14 + Xpos;
	}
	else if(Ypos == 3)
	{
		DRAM_ADDRESS = 0x54 + Xpos;
	}
	CLCD_Write(LCD, LCD_SETDDRAMADDR | DRAM_ADDRESS, CLCD_COMMAND);
}

void CLCD_I2C_WriteChar(CLCD_I2C_Name* LCD, char character)
{
	CLCD_Write(LCD, character, CLCD_DATA);
}

void CLCD_I2C_WriteString(CLCD_I2C_Name* LCD, char *String)
{
	while(*String)CLCD_I2C_WriteChar(LCD, *String++);
}

void CLCD_I2C_Clear(CLCD_I2C_Name* LCD)
{
	CLCD_Write(LCD, LCD_CLEARDISPLAY, CLCD_COMMAND);
	DelayMs(5);
}
