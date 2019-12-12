#include <stdint.h>
#include "stm32h7xx_hal.h"
#ifndef TM1637_HandleTypeDef_h
#define TM1637_h

typedef struct 
{
	uint16_t clk;
	uint16_t dio;
	GPIO_TypeDef* GPIO;
}TM1637_HandleTypeDef;

// deprecated
void init(TM1637_HandleTypeDef *display, uint16_t clk, uint16_t dio, GPIO_TypeDef* GPIO)
{
	display->clk = clk;
	display->dio = dio;
	display->GPIO = GPIO;
}
void pins_to_out(TM1637_HandleTypeDef *display)
{
	
	GPIO_InitTypeDef gpio;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pin =  display->clk|display->dio;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	
	HAL_GPIO_Init(GPIOE, &gpio);

}
void pins_to_in(TM1637_HandleTypeDef *display)
{
	
	GPIO_InitTypeDef gpio;
	gpio.Mode = GPIO_MODE_INPUT;
	gpio.Pin =  display->clk|display->dio;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	
	HAL_GPIO_Init(display->GPIO, &gpio);
}
void delay_us(unsigned int i) // nus delay 
{
	HAL_Delay(i/1000);
} 
void start (TM1637_HandleTypeDef *display) // 1637 start 
{
	HAL_GPIO_WritePin(display->GPIO, display->clk, GPIO_PIN_SET);
	HAL_GPIO_WritePin(display->GPIO, display->dio, GPIO_PIN_SET);
	delay_us (2);
	HAL_GPIO_WritePin(display->GPIO, display->dio, GPIO_PIN_RESET);
} 
void ask (TM1637_HandleTypeDef *display) // 1637 Answer 
{
	HAL_GPIO_WritePin(display->GPIO, display->clk, GPIO_PIN_RESET);
	delay_us (5); 
// Afterthe falling edge of the eighth clock delay 5us, ACK signals the beginning of judgment 
	pins_to_in(display);
	while (HAL_GPIO_ReadPin(display->GPIO, display->dio))
	{
	}
	pins_to_out(display);
	HAL_GPIO_WritePin(display->GPIO, display->clk, GPIO_PIN_SET);
	delay_us (2);
	HAL_GPIO_WritePin(display->GPIO, display->clk, GPIO_PIN_RESET);
} 
void stop (TM1637_HandleTypeDef *display) // 1637 Stop
{
	HAL_GPIO_WritePin(display->GPIO, display->clk, GPIO_PIN_RESET);
	delay_us (2);
	HAL_GPIO_WritePin(display->GPIO, display->dio, GPIO_PIN_RESET);
	delay_us (2);
	HAL_GPIO_WritePin(display->GPIO, display->clk, GPIO_PIN_SET);
	delay_us (2);
	HAL_GPIO_WritePin(display->GPIO, display->dio, GPIO_PIN_SET);

} 
void writeByte(TM1637_HandleTypeDef *display, unsigned char oneByte) // write a byte 
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
		delay_us (3); 
		oneByte = oneByte >> 1; 
		HAL_GPIO_WritePin(display->GPIO, display->clk, GPIO_PIN_SET);
		delay_us (3); 
	}
} 
unsigned char convertDecToSegment(uint16_t digit)
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
		};
		return digits[digit];

}
void writeDigits(TM1637_HandleTypeDef *display, uint16_t digits[])
{
	start(display); 
	writeByte(display, 0x40);
	//40H address is automatically incremented by 1 mode, 44H fixed address mode
	ask(display); 
	stop(display); 
	start(display); 
	writeByte(display, 0xc0); 
	//Set the first address
	ask(display); 
	unsigned char i;
	for(i=0;i<6;i++)  //Addresses from Canada, do not always write address
	{ 
	writeByte(display, convertDecToSegment(digits[i]));  //Send data
	ask(display); 
	} 
	
	stop(display); 
}
void setBrightness(TM1637_HandleTypeDef *display, unsigned char i)
{
	//0x8f maximum
	start(display);   
	writeByte(display, i); //Open display, maximum brightness
	ask(display);   
	stop(display); 
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
