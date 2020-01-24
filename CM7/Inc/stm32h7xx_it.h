/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h7xx_it.h
  * @brief   This file contains the headers of the interrupt handlers.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32H7xx_IT_H
#define __STM32H7xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern volatile enum SyncState { IN_SYNC,
                                 WAITING_FOR_SYNC } gps_sync;
extern RTC_HandleTypeDef hrtc;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;
extern TM1637_TypeDef display_clock;
extern TM1637_TypeDef display_counter;
extern volatile RTC_TimeTypeDef time;
volatile RTC_TimeTypeDef start_time;
extern RTC_DateTypeDef date;
extern volatile enum StarterMode { AUTO_START_60 = 60,
                                   AUTO_START_30 = 30,
                                   EXTERNAL = 3,
                                   SETUP,
                                   INIT } starter_mode;
extern volatile enum StartState { GATE_OPEN,
                                  GATE_CLOSED,
                                  NO_START } start_state;
extern volatile uint8_t counter;
extern volatile uint8_t counter_reload;
extern uint8_t rx_data[64];
extern void send_time();
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
/* USER CODE BEGIN EFP */
/**
 * Przerwanie DMA pozostałość po CubeMx do usunięcia
 */
void DMA1_Stream0_IRQHandler(void);
/**
 * Obsługuje przerwanie UART połączonego z modulem GPS
 * Wykorzystane synchronizacji impulsu nowej sekundy
 */
void UART4_IRQHandler(void);
/**
 * Obsługuje przerwania UART połączonego z modułem WiFi
 */
void USART6_IRQHandler(void);
/**
 * Przerwanie wywoływane co sekunde
 * Odświeża wyświetlacze i otwiera bramkę
 */
void RTC_WKUP_IRQHandler(void);
/**
 * Przerwanie generowane przez przecięcie wiązki fotokomórki
 * Kluczowe w detekcji falstartu
 */
void EXTI3_IRQHandler(void);
/**
 * Przerwanie związane z przyciskiem użytownika
 */
void EXTI15_10_IRQHandler(void);
/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /* __STM32H7xx_IT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
