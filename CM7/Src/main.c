/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "TM1637.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "led_functions.h"
#include "global_variables.h"
#include "init_functions.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */\

/// HW semaphore 0
#define HSEM_ID_0 (0U)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/**
 * Maksymalna liczba znaków obsługiwana przez ::PRINT
 */
#define PRINT_SIZE 30
/**
 * Makro o składni printf transmitujące po UART do STLINK liczba znaków ograniczona przez ::PRINT_SIZE
 * \param FORMAT formatowanie jak w funkcjach standardowych
 * \param ... lista wartości do formatowanego ciągu
 */
#define PRINT(FORMAT, ...) \
	char str[PRINT_SIZE]; \
	sprintf( str, FORMAT, __VA_ARGS__); \
	HAL_UART_Transmit(&huart3, (unsigned char *)str, strlen(str), 10);

/* USER CODE END PM */
/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;
UART_HandleTypeDef huart4;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;

TM1637_TypeDef display_clock;
TM1637_TypeDef display_counter;

volatile RTC_TimeTypeDef time = {0};
volatile RTC_TimeTypeDef start_time = {0};
volatile RTC_DateTypeDef date = {0};

volatile enum SyncState gps_sync = WAITING_FOR_SYNC;
volatile enum StarterMode starter_mode = INIT;
volatile enum StartState start_state = NO_START;

volatile uint8_t counter = 30;
volatile uint8_t counter_reload;

uint8_t rx_data[64];


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* USER CODE BEGIN Boot_Mode_Sequence_0 */
    int32_t timeout;
  /* USER CODE END Boot_Mode_Sequence_0 */

/* USER CODE BEGIN Boot_Mode_Sequence_1 */
  /* Wait until CPU2 boots and enters in stop mode or timeout*/
  timeout = 0xFFFF;
  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
  if ( timeout < 0 )
  {
  Error_Handler();
  }

/* USER CODE END Boot_Mode_Sequence_1 */
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
/* USER CODE BEGIN Boot_Mode_Sequence_2 */
/* When system initialization is finished, Cortex-M7 will release Cortex-M4 by means of
HSEM notification */
/*HW semaphore Clock enable*/
__HAL_RCC_HSEM_CLK_ENABLE();
/*Take HSEM */
HAL_HSEM_FastTake(HSEM_ID_0);
/*Release HSEM in order to notify the CPU2(CM4)*/
HAL_HSEM_Release(HSEM_ID_0,0);
/* wait until CPU2 wakes up from stop mode */
timeout = 0xFFFF;
while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) == RESET) && (timeout-- > 0));
if ( timeout < 0 )
{
Error_Handler();
}
/* USER CODE END Boot_Mode_Sequence_2 */

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();

	MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
	GPIO_LED_Init();
	GPIO_IR_Init();
	USART6_Init();
	GPIO_BUTTON_Init();

	TM1637_Init(&display_clock, GPIO_PIN_0, GPIO_PIN_1, GPIOD);
	TM1637_Init(&display_counter, GPIO_PIN_14, GPIO_PIN_15, GPIOF);



	MX_RTC_Init();
	LED_GREEN_ON();
	// Starter mode selection
	starter_mode = SETUP;
	counter = 30;

	TM1637_WriteTime(&display_clock, 88, 88, TM1637_SEPARATOR_ON);
	TM1637_WriteTime(&display_counter, 00, counter, TM1637_SEPARATOR_ON);

	HAL_NVIC_DisableIRQ(USART6_IRQn);
	HAL_UART_Transmit(&huart6, (uint8_t *) "AT+S.RESET\n\r", 12, 100);
	while(time.Seconds != 15)
	{
		__nop();
	}
	switch(counter)
	{
		case 30:
			starter_mode = AUTO_START;
			counter_reload = 30;
			break;
		case 60:
			starter_mode = AUTO_START;
			counter_reload = 60;
			break;
		case 3:
			starter_mode = EXTERNAL;
			counter_reload = 4;
			break;
		default:
			Error_Handler();
	}
	LED_GREEN_OFF();

	// open 8888 tcp socket
	HAL_UART_Transmit(&huart6, (uint8_t *) "AT+S.SOCKDON=8888,t\n\r", 21, 100);
	TM1637_WriteTime(&display_counter, 0, 0, TM1637_SEPARATOR_OFF);
	//starter_mode = (int) setup_mode;
	time.Seconds = 0;
	HAL_RTC_SetTime(&hrtc, (RTC_TimeTypeDef*) &time, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, (RTC_DateTypeDef*) &date, RTC_FORMAT_BIN);

	// Setting up RTC with GPS data
	unsigned int checksum = 0x0;
	MX_UART4_Init();
	while(checksum != 6)
	{
		HAL_UART_Receive(&huart4, rx_data, 64, 1000);
		checksum = sscanf((const char*)rx_data, "$GPZDA,%2u%2d%2d.00,%2d,%2d,%4d,00,00*%2X",
			(unsigned int *)&time.Hours, (unsigned int *)&time.Minutes, (unsigned int *)&time.Seconds,
				(unsigned int *)&date.WeekDay, (unsigned int *)&date.Month, (unsigned int *)&date.Year, &checksum);
	}
	PRINT("%.2d:%.2d:%.2d\t%d-%d-%d", time.Hours, time.Minutes, time.Seconds, date.Date, date.Month, date.Year);
	time.Hours = (time.Hours+1)%24; // UTC -> polski czas zimowy
	HAL_RTC_SetTime(&hrtc, (RTC_TimeTypeDef*) &time, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, (RTC_DateTypeDef*) &date, RTC_FORMAT_BIN);

	gps_sync = WAITING_FOR_SYNC;
	__HAL_UART_ENABLE_IT(&huart4, UART_IT_RXNE);

	while(gps_sync == WAITING_FOR_SYNC) // czekamy na synchronizacje
	{
		__nop();
	}

	__HAL_UART_DISABLE_IT(&huart4, UART_IT_RXNE); // niepotrzebujemy juz tego przerwania po synchronizacji
	__HAL_RCC_UART4_CLK_DISABLE();
	__HAL_RTC_WRITEPROTECTION_ENABLE(&hrtc);

	//Bez tego sie psuje
	TM1637_Init(&display_clock, GPIO_PIN_0, GPIO_PIN_1, GPIOD);
	TM1637_Init(&display_counter, GPIO_PIN_14, GPIO_PIN_15, GPIOF);

  while (1)
  {
		__nop();
  }
  /* USER CODE END 3 */
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
