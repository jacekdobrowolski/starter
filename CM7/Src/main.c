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
#include "lib/TM1637.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include "DMA_CIRCULAR.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

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
TM1637_TypeDef display_clock;
TM1637_TypeDef display_counter;
RTC_TimeTypeDef time = {0};
RTC_DateTypeDef date = {0};
volatile enum SyncState{IN_SYNC, WAITING_FOR_SYNC}gps_sync;
volatile enum StarterMode{AUTO_START_60 = 60, AUTO_START_30 = 30, EXTERNAL = 1, SETUP, INIT}starter_mode;
volatile enum SetupMode{SETUP_60 = 60, SETUP_30 = 30, SETUP_EXTERNAL = 1}setup_mode;
uint8_t rx_data[64];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
void MX_RTC_Init(void);
void MX_UART4_Init(void);
void MX_USART3_UART_Init(void);


/* USER CODE BEGIN PFP */
void GPIO_LED_Init(void);
void GPIO_BUTTON_Init(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_DMA_Init();
	MX_RTC_Init();
	
	MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
	GPIO_LED_Init();
	GPIO_BUTTON_Init();
	LED_GREEN_ON();
	TM1637_Init(&display_clock, GPIO_PIN_0, GPIO_PIN_1, GPIOD); 
	TM1637_Init(&display_counter, GPIO_PIN_14, GPIO_PIN_15, GPIOF);
	TM1637_WriteTime(&display_clock, 88, 88, TM1637_SEPARATOR_ON);
	TM1637_WriteTime(&display_counter, 88, 88, TM1637_SEPARATOR_ON);
/*
	RTC_AlarmTypeDef alarm={0};
	alarm.Alarm = RTC_ALARM_A;
	alarm.AlarmMask = RTC_ALARMMASK_ALL;
	HAL_RTC_SetAlarm_IT(&hrtc, &alarm, RTC_FORMAT_BIN);
	HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 2);
	HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
	*/
	// Starter mode selection
	starter_mode = SETUP;
	setup_mode = SETUP_30;
	while(time.Seconds != 3)
	{
		HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
		TM1637_WriteTime(&display_counter, time.Seconds, setup_mode, TM1637_SEPARATOR_ON);
		HAL_Delay(100);
	}
	TM1637_WriteTime(&display_counter, 0, 0, TM1637_SEPARATOR_OFF);
	//starter_mode = (int) setup_mode;
	time.Seconds = 0;
	HAL_RTC_SetTime(&hrtc,  &time, RTC_FORMAT_BIN);
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
	}
	
	time.Hours += 1; // UTC -> polski czas zimowy
	HAL_RTC_SetTime(&hrtc,  &time, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);
	
	PRINT("SETUP TIME: %d:%d\r\n", time.Hours, time.Minutes);
	LED_RED_ON();
	gps_sync = WAITING_FOR_SYNC;
	__HAL_UART_ENABLE_IT(&huart4, UART_IT_RXNE);
	
	while(gps_sync == WAITING_FOR_SYNC) // czekamy na synchronizacje
	{
		__nop();
	}
	__HAL_UART_DISABLE_IT(&huart4, UART_IT_RXNE); // niepotrzebujemy juz tego przerwania po synchronizacji
	__HAL_RCC_UART4_CLK_DISABLE();
	__HAL_RTC_WRITEPROTECTION_ENABLE(&hrtc);
	
	
	
	
	__HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc);
	// disabling wakeup timer
	RTC->CR &= ~RTC_CR_WUTE;
	while(!(RTC->ISR & RTC_ISR_WUTWF_Msk)) {}
	// selecting 1s to 18h 
	RTC->CR |= RTC_CR_WUCKSEL_2;
	RTC->CR &= ~RTC_CR_WUCKSEL_1;
	// seting to wakeup every 1s
	RTC->WUTR &= 0x0000UL;
	// enabling wakeup timer interrupt
	RTC->CR	|= RTC_CR_WUTIE;
	RTC->CR |= RTC_CR_WUTE;
	__HAL_RTC_WRITEPROTECTION_ENABLE(&hrtc); 
		
	// unmasking line 19 in exti whiuch is RTC wakeup
	EXTI->IMR1 |= EXTI_IMR1_IM19;
	//EXTI->EMR1 |= EXTI_IMR1_IM19;
	// setting rising edge trigger and falling
	EXTI->RTSR1 |= EXTI_RTSR1_TR19;
	//__HAL_RTC_WAKEUPTIMER_EXTI_ENABLE_IT();
		
	HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 0,2);
	
	//Bez tego sie psuje
	TM1637_Init(&display_clock, GPIO_PIN_0, GPIO_PIN_1, GPIOD); 
	TM1637_Init(&display_counter, GPIO_PIN_14, GPIO_PIN_15, GPIOF);
	LED_GREEN_OFF();
	HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
  /* USER CODE END 2 */
	

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */		
		
		//HAL_UART_Transmit(&huart3, &rx_data[time_offset], 6, 10);
		//HAL_Delay(20);
		
		
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
                              |RCC_PERIPHCLK_UART4;
  PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */
  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 31;
  hrtc.Init.SynchPrediv = 1023;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
    
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date 
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */
	
  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
	
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart4, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart4, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
void GPIO_LED_Init()
{
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef gpio;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pin =  GPIO_PIN_1;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOE, &gpio);
	gpio.Pin =  GPIO_PIN_14 | GPIO_PIN_0;
	HAL_GPIO_Init(GPIOB, &gpio);
}

void GPIO_BUTTON_Init()
{
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef gpio;
	gpio.Mode = GPIO_MODE_INPUT;
	gpio.Pin =  GPIO_PIN_13;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOC, &gpio);
	LED_GREEN_OFF();
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0,1);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	RCC -> APB4ENR |= RCC_APB4ENR_SYSCFGEN;
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;
	EXTI->IMR1 |= EXTI_IMR1_IM13;
	EXTI->RTSR1 |= EXTI_RTSR1_TR13;
}

void RTC_Alarm_IRQHandler()
{
	LED_YELLOW_OFF();
	LED_RED_OFF();
}
/*
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	LED_RED_OFF();
	UNUSED(GPIO_Pin);
}
*/

void EXTI15_10_IRQHandler()
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
	if(starter_mode == SETUP)
	{
		time.Seconds = 0;
		HAL_RTC_SetTime(&hrtc,  &time, RTC_FORMAT_BIN);
		HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);
		
		if(setup_mode == SETUP_30) {
			setup_mode = SETUP_60;
		}else if(setup_mode == SETUP_60) {
			setup_mode = SETUP_EXTERNAL;
		} else if(setup_mode == SETUP_EXTERNAL) {
			setup_mode = SETUP_30;
		}
	}
}


void RTC_WKUP_IRQHandler()
{	
// reseting wakeup flag must be done in software
	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1);
	
	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
	__HAL_RTC_EXTI_CLEAR_FLAG(RTC_EXTI_LINE_WAKEUPTIMER_EVENT);
	
	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	TM1637_WriteTime(&display_clock, time.Hours, time.Minutes, TM1637_SEPARATOR_OFF);
	TM1637_WriteTime(&display_counter, time.Seconds, 00, TM1637_SEPARATOR_OFF);
}
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
