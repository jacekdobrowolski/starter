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
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define LED_RED_ON() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
#define LED_RED_OFF() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
#define LED_GREEN_ON() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
#define LED_GREEN_OFF() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
#define LED_YELLOW_ON() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);
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
extern volatile enum SyncState{IN_SYNC, WAITING_FOR_SYNC}gps_sync;
extern RTC_HandleTypeDef hrtc;

extern UART_HandleTypeDef huart6;
extern TM1637_TypeDef display_clock;
extern TM1637_TypeDef display_counter;
extern volatile RTC_TimeTypeDef time;
extern RTC_DateTypeDef date;
extern volatile enum StarterMode{AUTO_START_60 = 60, AUTO_START_30 = 30, EXTERNAL = 1, SETUP, INIT}starter_mode;
extern volatile enum SetupMode{SETUP_60 = 60, SETUP_30 = 30, SETUP_EXTERNAL = 1}setup_mode;
extern uint8_t rx_data[64];
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
	LED_GREEN_ON();
	HAL_UART_IRQHandler(&huart6);
}

void RTC_WKUP_IRQHandler()
{	
// reseting wakeup flag must be done in software
	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1);
	
	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
	__HAL_RTC_EXTI_CLEAR_FLAG(RTC_EXTI_LINE_WAKEUPTIMER_EVENT);
	
	HAL_RTC_GetTime(&hrtc, (RTC_TimeTypeDef*) &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	TM1637_WriteTime(&display_clock, time.Hours, time.Minutes, TM1637_SEPARATOR_OFF);
	TM1637_WriteTime(&display_counter, time.Seconds, setup_mode, TM1637_SEPARATOR_OFF);
}

void EXTI3_IRQHandler()
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
}

void EXTI15_10_IRQHandler()
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
	if(starter_mode == SETUP)
	{
		time.Seconds = 0;
		HAL_RTC_SetTime(&hrtc, (RTC_TimeTypeDef*) &time, RTC_FORMAT_BIN);
		HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);
		
		if(setup_mode == SETUP_30) {
			setup_mode = SETUP_60;
		}else if(setup_mode == SETUP_60) {
			setup_mode = SETUP_EXTERNAL;
		} else if(setup_mode == SETUP_EXTERNAL) {
			setup_mode = SETUP_30;
		}
		TM1637_WriteTime(&display_counter, time.Seconds, setup_mode, TM1637_SEPARATOR_ON);
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
