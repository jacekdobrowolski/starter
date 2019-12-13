#include <stdint.h>
#include "stm32h7xx_hal.h"
#ifndef TM1637_TYPEDEF_H
#define TM1637_TYPEDEF_H

typedef struct 
{
	uint16_t clk;
	uint16_t dio;
	GPIO_TypeDef* GPIO;
}TM1637_TypeDef;

void TM1637_PinsToIn(TM1637_TypeDef *display)
{
	
	GPIO_InitTypeDef gpio;
	gpio.Mode = GPIO_MODE_INPUT;
	gpio.Pin =  display->clk|display->dio;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	
	HAL_GPIO_Init(display->GPIO, &gpio);
}
void TM1637_PinsToOut(TM1637_TypeDef *display)
{
	
	GPIO_InitTypeDef gpio;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pin =  display->clk|display->dio;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	
	HAL_GPIO_Init(display->GPIO, &gpio);

}
void TM1637_Init(TM1637_TypeDef *display, uint16_t clk, uint16_t dio, GPIO_TypeDef* GPIO)
{
	display->clk = clk;
	display->dio = dio;
	display->GPIO = GPIO;
	TM1637_PinsToOut(display);
}


void TM1637_DelayUs(unsigned int i) // nus delay 
{
	//HAL_Delay(i);
} 
void TM1637_Start (TM1637_TypeDef *display) // 1637 start 
{
	HAL_GPIO_WritePin(display->GPIO, display->clk, GPIO_PIN_SET);
	HAL_GPIO_WritePin(display->GPIO, display->dio, GPIO_PIN_SET);
	TM1637_DelayUs (2);
	HAL_GPIO_WritePin(display->GPIO, display->dio, GPIO_PIN_RESET);
} 
void TM1637_Ask (TM1637_TypeDef *display) // 1637 Answer 
{
	HAL_GPIO_WritePin(display->GPIO, display->clk, GPIO_PIN_RESET);
	TM1637_DelayUs(5); 
// Afterthe falling edge of the eighth clock delay 5us, ACK signals the beginning of judgment 
	TM1637_PinsToIn(display);
	while (HAL_GPIO_ReadPin(display->GPIO, display->dio))
	{
	}
	TM1637_PinsToOut(display);
	HAL_GPIO_WritePin(display->GPIO, display->clk, GPIO_PIN_SET);
	TM1637_DelayUs(2);
	HAL_GPIO_WritePin(display->GPIO, display->clk, GPIO_PIN_RESET);
} 
void TM1637_Stop (TM1637_TypeDef *display) // 1637 Stop
{
	HAL_GPIO_WritePin(display->GPIO, display->clk, GPIO_PIN_RESET);
	TM1637_DelayUs(2);
	HAL_GPIO_WritePin(display->GPIO, display->dio, GPIO_PIN_RESET);
	TM1637_DelayUs(2);
	HAL_GPIO_WritePin(display->GPIO, display->clk, GPIO_PIN_SET);
	TM1637_DelayUs(2);
	HAL_GPIO_WritePin(display->GPIO, display->dio, GPIO_PIN_SET);

} 
void TM1637_WriteByte(TM1637_TypeDef *display, unsigned char oneByte) // write a byte 
{
	unsigned char i;
	for (i = 0; i <8; i++)
	{
		HAL_GPIO_WritePin(display->GPIO, display->clk, GPIO_PIN_RESET);
		if (oneByte & 0x01)
		{
			HAL_GPIO_WritePin(display->GPIO, display->dio, GPIO_PIN_SET);
		} 
		else 
		{
			HAL_GPIO_WritePin(display->GPIO, display->dio, GPIO_PIN_RESET);
		} 
		TM1637_DelayUs(3); 
		oneByte = oneByte >> 1; 
		HAL_GPIO_WritePin(display->GPIO, display->clk, GPIO_PIN_SET);
		TM1637_DelayUs(3); 
	}
} 
unsigned char TM1637_ConvertDecToSegment(uint16_t digit)
{
	/*
			0b00111111,    // 0
			0b00000110,    // 1
			0b01011011,    // 2
			0b01001111,    // 3
			0b01100110,    // 4
			0b01101101,    // 5
			0b01111101,    // 6
			0b00000111,    // 7
			0b01111111,    // 8
			0b01101111,    // 9
			0b01110111,    // A
			0b01111100,    // b
			0b00111001,    // C
			0b01011110,    // d
			0b01111001,    // E
			0b01110001     // F
	*/
		unsigned char digits[16] = 
		{ 0x3f,    // 0
			0x06,    // 1
			0x5b,    // 2
			0x4f,    // 3
			0x66,    // 4
			0x6d,    // 5
			0x7d,    // 6
			0x07,    // 7
			0x7f,    // 8
			0x6f,    // 9	
			0xff,   // 10 all
			0x00, //11 empty
		};
		return digits[digit];

}
void TM1637_WriteDigits(TM1637_TypeDef *display, uint16_t digits[], uint8_t isSeparator)
{
	TM1637_Start(display); 
	TM1637_WriteByte(display, 0x40);
	//40H address is automatically incremented by 1 mode, 44H fixed address mode
	TM1637_Ask(display); 
	TM1637_Stop(display); 
	TM1637_Start(display); 
	TM1637_WriteByte(display, 0xc0); 
	//Set the first address
	TM1637_Ask(display); 
	unsigned char i;
	unsigned char separator;
	for(i=0;i<4;i++)  
	{ 
		
		if(isSeparator)
		{
			separator = 0x80;
		}
		else
		{
			separator = 0x00;
		}
		
		TM1637_WriteByte(display, TM1637_ConvertDecToSegment(digits[i])|separator);  //Send data
		TM1637_Ask(display); 
	} 
	
	TM1637_Stop(display); 
}
void setBrightness(TM1637_TypeDef *display, unsigned char i)
{
	//0x8f maximum
	TM1637_Start(display);   
	TM1637_WriteByte(display, i); //Open display, maximum brightness
	TM1637_Ask(display);   
	TM1637_Stop(display); 
}
/*
void displayExample(uint16_t clk, uint16_t dio) //Write display register
{     
	unsigned char i;

	start(clk, dio); 
	writeByte(0x40, clk, dio);
	//40H address is automatically incremented by 1 mode, 44H fixed address mode
	ask(clk, dio); 
	stop(clk, dio); 
	start(clk, dio); 
	writeByte(0xc0, clk, dio); 
	//Set the first address
	ask(clk, dio); 
	for(i=0;i<6;i++)  //Addresses from Canada, do not always write address
	{ 
	writeByte(0xff, clk, dio);  //Send data
	ask(clk, dio); } 
	stop(clk, dio); 
	start(clk, dio);   
	writeByte(0x8f, clk, dio); //Open display, maximum brightness
	ask(clk, dio);   
	stop(clk, dio); 
}*/
#endif
