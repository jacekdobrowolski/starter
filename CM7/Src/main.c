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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */
#define MODE_SELECT_DELAY 25
/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define PRINT_SIZE 30
#define PRINT(FORMAT, ...) \
	char str[PRINT_SIZE]; \
	sprintf( str, FORMAT, __VA_ARGS__); \
	HAL_UART_Transmit(&huart3, (unsigned char *)str, strlen(str), 10);
#define LED_RED_ON() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
#define LED_RED_OFF() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
#define LED_GREEN_ON() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
#define LED_GREEN_TOGGLE() HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
#define LED_GREEN_OFF() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
#define LED_YELLOW_ON() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);
#define LED_YELLOW_OFF() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
/* USER CODE END PM */
/* Private variables ---------------------------------------------------------*/

RTC_HandleTypeDef hrtc;
UART_HandleTypeDef huart4;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_uart4_rx;

/* USER CODE BEGIN PV */
UART_HandleTypeDef huart6;
TM1637_TypeDef display_clock;
TM1637_TypeDef display_counter;
volatile RTC_TimeTypeDef time = {0};
volatile RTC_DateTypeDef date = {0};
volatile enum SyncState{IN_SYNC, WAITING_FOR_SYNC}gps_sync;
volatile enum StarterMode{AUTO_START, EXTERNAL, SETUP, INIT}starter_mode;
volatile uint8_t counter_reload;
volatile enum StartState{GATE_OPEN, GATE_CLOSED, GATE_READY,  NO_START, FALSTART}start_state;
volatile uint8_t counter;
uint8_t rx_data[64];
/* USER CODE END PV */

#include "init_functions.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
//static void MX_DMA_Init(void);
void MX_RTC_Init(void);
void MX_UART4_Init(void);
void MX_USART3_UART_Init(void);


/* USER CODE BEGIN PFP */
void GPIO_IR_Init(void);
void GPIO_LED_Init(void);
void GPIO_BUTTON_Init(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void USART6_Init(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void send_time(volatile RTC_TimeTypeDef* time, volatile RTC_DateTypeDef* date)
	{
		char time_string[14];
		uint8_t millis = (float) 1000*(1023-time->SubSeconds)/1024.0f;
		sprintf(time_string, "%.2d:%.2d:%.2d.%.3d,\n", time->Hours, time->Minutes, time->Seconds, millis);
		char data[30];
		sprintf(data, "AT+S.SOCKDW=0,0,%d\r%s", strlen(time_string), time_string);
		HAL_UART_Transmit(&huart6, (uint8_t*) data , strlen(data), 100);
		char data1[30];
		
		char date_string[10];
 
		sprintf(date_string, "%d.%d.%d", date->Date, (uint8_t)date->Month, date->Year);
		sprintf(data1, "AT+S.FSC=0:/%s,%d\r%s", date_string, strlen(time_string), time_string);
		HAL_UART_Transmit(&huart6, (uint8_t*) data1 , strlen(data1), 100);
	}
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
	
	HAL_UART_Transmit(&huart6, (uint8_t *) "AT+S.RESET\n\r", 12, 100);
	while(time.Seconds != MODE_SELECT_DELAY)
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
	HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);

	// Setting up RTC with GPS data
	uint8_t checksum = 0;
	MX_UART4_Init();
	while(checksum != 6)
	{
		HAL_UART_Receive(&huart4, rx_data, 64, 1000);
		checksum = sscanf((const char*)rx_data, "$GPZDA,%2u%2d%2d.00,%2d,%2d,%4d,00,00*%*X",
			(unsigned int *)&time.Hours, (unsigned int *)&time.Minutes, (unsigned int *)&time.Seconds,
				(unsigned int *)&date.WeekDay, (unsigned int *)&date.Month, (unsigned int *)&date.Year);
		//LED_GREEN_TOGGLE();
	}
	
	time.Hours = (time.Hours+1)%24; // UTC -> polski czas zimowy
	HAL_RTC_SetTime(&hrtc, (RTC_TimeTypeDef*) &time, RTC_FORMAT_BIN);
	while(gps_sync == WAITING_FOR_SYNC) // czekamy na synchronizacje
	{
		__nop();
	}
	HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);
	
	gps_sync = WAITING_FOR_SYNC;
	__HAL_UART_ENABLE_IT(&huart4, UART_IT_RXNE);
	
	__HAL_UART_DISABLE_IT(&huart4, UART_IT_RXNE); // niepotrzebujemy juz tego przerwania po synchronizacji
	__HAL_RCC_UART4_CLK_DISABLE();
	__HAL_RTC_WRITEPROTECTION_ENABLE(&hrtc);
	
	//Bez tego sie psuje
	TM1637_Init(&display_clock, GPIO_PIN_0, GPIO_PIN_1, GPIOD); 
	TM1637_Init(&display_counter, GPIO_PIN_14, GPIO_PIN_15, GPIOF);
	HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
	//HAL_UART_Transmit(&huart6, (uint8_t *) "AT+S.SOCKDW=0,0,15\rHello, World!\n\r", 34, 100);
	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
		__nop();	
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Supply configuration update enable 
  */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);
  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 120;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 20;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART3
                              |RCC_PERIPHCLK_UART4|RCC_PERIPHCLK_USART6;
  PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}










/** 
  * Enable DMA controller clock
  */
/*
static void MX_DMA_Init(void) 
{
*/
  /* DMA controller clock enable */
/*
  __HAL_RCC_DMA1_CLK_ENABLE();
*/
  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
/*
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

}
*/




/* USER CODE BEGIN 4 */










/* USER CODE END 4 */

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
