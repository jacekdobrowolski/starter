#include <stdint.h>
#include "stm32h7xx_hal.h"
#ifndef TM1637_h
#define TM1637_h
void pins_to_out(GPIO_TypeDef* GPIO)
{
	
	GPIO_InitTypeDef hgpiob;
	hgpiob.Mode = GPIO_MODE_OUTPUT_PP;
	hgpiob.Pin =  GPIO_PIN_1|GPIO_PIN_2;
	hgpiob.Pull = GPIO_NOPULL;
	hgpiob.Speed = GPIO_SPEED_FREQ_MEDIUM;
	
	HAL_GPIO_Init(GPIOE, &hgpiob);

}
void pins_to_in(GPIO_TypeDef* GPIO)
{
	
	GPIO_InitTypeDef hgpiob;
	hgpiob.Mode = GPIO_MODE_INPUT;
	hgpiob.Pin =  GPIO_PIN_1|GPIO_PIN_2;
	hgpiob.Pull = GPIO_NOPULL;
	hgpiob.Speed = GPIO_SPEED_FREQ_MEDIUM;
	
	HAL_GPIO_Init(GPIO, &hgpiob);
}
void delay_us(unsigned int i) // nus delay 
{
	HAL_Delay(i);	
} 
void start (uint16_t clk, uint16_t dio) // 1637 start 
{
	HAL_GPIO_WritePin(GPIOE, clk, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, dio, GPIO_PIN_SET);
	delay_us (2);
	HAL_GPIO_WritePin(GPIOE, dio, GPIO_PIN_RESET);
} 
void ask (uint16_t clk, uint16_t dio) // 1637 Answer 
{
	HAL_GPIO_WritePin(GPIOE, clk, GPIO_PIN_RESET);
	delay_us (5); 
// Afterthe falling edge of the eighth clock delay 5us, ACK signals the beginning of judgment 
	pins_to_in(GPIOE);
	while (HAL_GPIO_ReadPin(GPIOE, dio))
	{
	}
	pins_to_out(GPIOE);
	HAL_GPIO_WritePin(GPIOE, clk, GPIO_PIN_SET);
	delay_us (2);
	HAL_GPIO_WritePin(GPIOE, clk, GPIO_PIN_RESET);
} 
void stop (uint16_t clk, uint16_t dio) // 1637 Stop
{
	HAL_GPIO_WritePin(GPIOE, clk, GPIO_PIN_RESET);
	delay_us (2);
	HAL_GPIO_WritePin(GPIOE, dio, GPIO_PIN_RESET);
	delay_us (2);
	HAL_GPIO_WritePin(GPIOE, clk, GPIO_PIN_SET);
	delay_us (2);
	HAL_GPIO_WritePin(GPIOE, dio, GPIO_PIN_SET);
} 
void writeByte(unsigned char oneByte, uint16_t clk, uint16_t dio) // write a byte 
{
	unsigned char i;
	for (i = 0; i <8; i++)
	{
		HAL_GPIO_WritePin(GPIOE, clk, GPIO_PIN_RESET);
		if (oneByte & 0x01)
		{
			HAL_GPIO_WritePin(GPIOE, dio, GPIO_PIN_SET);
		} 
		else 
		{
			HAL_GPIO_WritePin(GPIOE, dio, GPIO_PIN_RESET);
		} 
		delay_us (3); 
		oneByte = oneByte >> 1; 
		HAL_GPIO_WritePin(GPIOE, clk, GPIO_PIN_SET);
		delay_us (3); 
	}
} 
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
}
#endif
