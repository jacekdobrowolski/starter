#ifndef TM1637_TYPEDEF_H
#define TM1637_TYPEDEF_H

#include <stdint.h>
#include "stm32h7xx_hal.h"

#define TM1637_SEPARATOR_ON 1
#define TM1637_SEPARATOR_OFF 0
#define TM1637_MAX_BRIGHTNESS 0x8f
#define TM1637_FIXED_ADDRESS 0x44
#define TM1637_AUTO_INCREMENT_ADDRESS 0x40
#define TM1637_FIRST_SEGMENT_ADDRESS 0xc0
#define TM1637_SEPARATOR 0x80
#define TM1637_NO_SEPARATOR 0x00

typedef struct 
{
	uint16_t clk;
	uint16_t dio;
	GPIO_TypeDef* GPIO;
}TM1637_TypeDef;

void TM1637_DelayUs(unsigned int i); // nus delay 


void TM1637_PinsToIn(TM1637_TypeDef *display);

void TM1637_PinsToOut(TM1637_TypeDef *display);

void TM1637_Start (TM1637_TypeDef *display); // 1637 start 
 
void TM1637_Ask (TM1637_TypeDef *display); // 1637 Answer 
 
void TM1637_Stop (TM1637_TypeDef *display); // 1637 Stop

void TM1637_WriteByte(TM1637_TypeDef *display, unsigned char oneByte); // write a byte 
 
void TM1637_setBrightness(TM1637_TypeDef *display, unsigned char i);

void TM1637_Init(TM1637_TypeDef *display, uint16_t clk, uint16_t dio, GPIO_TypeDef* GPIO);


unsigned char TM1637_ConvertDecToSegment(uint16_t digit);

void TM1637_WriteDigits(TM1637_TypeDef *display, uint16_t digits[], uint8_t has_separator);


void TM1637_WriteTime(TM1637_TypeDef *display, uint8_t hour, uint8_t minute, uint8_t has_separator);

#endif // TM1637_TYPEDEF_H
