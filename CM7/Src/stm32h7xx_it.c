/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32h7xx_it.h"
#include "TM1637.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 #define GATE_ARMED 5 // Sekundy przed startem kiedy przeciecie fotoceli bedzie traktowane jako falstart
 #define NO_START_DELAY 2 // Tyle sekund po starcie jesli nie nastapilo przeciecie start uwazan jest za niewazny
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define LED_RED_ON() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
#define LED_RED_TOGGLE() HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
#define LED_RED_OFF() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
#define LED_GREEN_ON() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
#define LED_GREEN_TOGGLE() HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
#define LED_GREEN_OFF() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
#define LED_YELLOW_ON() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);
#define LED_YELLOW_TOGGLE() HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1);
#define LED_YELLOW_OFF() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_uart4_rx;
extern UART_HandleTypeDef huart4;
/* USER CODE BEGIN EV */

//uint8_t external_stop=1;
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */
	
  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 stream0 global interrupt.
  */
void DMA1_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream0_IRQn 0 */

  /* USER CODE END DMA1_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_uart4_rx);
  /* USER CODE BEGIN DMA1_Stream0_IRQn 1 */
  
  /* USER CODE END DMA1_Stream0_IRQn 1 */
}

/**
  * @brief This function handles UART4 global interrupt.
  */
void UART4_IRQHandler(void)
{
  /* USER CODE BEGIN UART4_IRQn 0 */
	__HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc);
	RTC->SHIFTR = RTC_SHIFTR_ADD1S | ((RTC->PRER & RTC_PRER_PREDIV_S_Msk) - (RTC->SSR & RTC_SSR_SS_Msk));
	gps_sync = IN_SYNC;
  /* USER CODE END UART4_IRQn 0 */
  HAL_UART_IRQHandler(&huart4);
  /* USER CODE BEGIN UART4_IRQn 1 */
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET); // Yellow LED
  /* USER CODE END UART4_IRQn 1 */
}

/* USER CODE BEGIN 1 */

void USART6_IRQHandler(void)
{
	uint8_t* pdata;
	HAL_UART_Receive_IT(&huart6, pdata, 1);
	HAL_UART_Transmit_IT(&huart3, pdata , 1);
	HAL_UART_IRQHandler(&huart6);
}

void RTC_WKUP_IRQHandler(void)
{	
// reseting wakeup flag must be done in software
	//HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1);
	
	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
	__HAL_RTC_EXTI_CLEAR_FLAG(RTC_EXTI_LINE_WAKEUPTIMER_EVENT);
	HAL_RTC_GetTime(&hrtc, (RTC_TimeTypeDef*) &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	if(starter_mode != SETUP && counter > 0)
	{
		counter--;
	}
	TM1637_WriteTime(&display_counter, 00, counter, TM1637_SEPARATOR_OFF);
	if(counter == GATE_ARMED)
	{
		start_state = GATE_CLOSED;
	}
	else if(counter == 0)
	{
		start_state = GATE_OPEN;
		LED_GREEN_ON();
		start_time = time;
		if(starter_mode != EXTERNAL)
		{
			counter = counter_reload;
		}
	}
	else if(counter == counter_reload - NO_START_DELAY)
	{
		start_state = NO_START;
		LED_RED_OFF();
		LED_GREEN_OFF();
	}
	TM1637_WriteTime(&display_clock, time.Hours, time.Minutes, TM1637_SEPARATOR_OFF);
	TM1637_WriteTime(&display_counter, 00, counter, TM1637_SEPARATOR_OFF);
}

void EXTI3_IRQHandler(void)
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
	LED_YELLOW_TOGGLE();
	if(start_state == GATE_OPEN)
	{
		// start potwierdzony
		LED_GREEN_OFF();
		send_time(&start_time, &date);
		start_state = NO_START;
	}	else if(start_state == GATE_CLOSED)	{
		
		if(starter_mode != EXTERNAL)
		{
			// falstart
			LED_RED_ON();
			send_time(&start_time, &date);
			start_state= NO_START;
		}
	} else {
	
	}
}

void EXTI15_10_IRQHandler(void)
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
	if(starter_mode == EXTERNAL) {
		start_state = GATE_CLOSED;
		counter = counter_reload;
		TM1637_WriteTime(&display_counter, 00, counter, TM1637_SEPARATOR_OFF);
	} 
	else if(starter_mode == SETUP)
	{
		time.Seconds = 0;
		HAL_RTC_SetTime(&hrtc, (RTC_TimeTypeDef*) &time, RTC_FORMAT_BIN);
		HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);
		
		if(counter == 30) {
			counter = 60;
		}else if(counter == 60) {
			counter = 3;
		} else if(counter == 3) {
			counter = 30;
		}
		TM1637_WriteTime(&display_counter, 00, counter, TM1637_SEPARATOR_ON);
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
