/**
  ******************************************************************************
  * @file           init_functions.h
  * @authors        Filip Kulik, Jacek Dobrowolski
  * @brief          Plik naglowkowy z funkcjami inicjalizującymi peryferia
  *                   Ten plik zawiera deklaracje funkcji inicjalizujących peryferia
  ******************************************************************************
  */

#ifndef INIT_FUNCTIONS_H
#define INIT_FUNCTIONS_H

#include "main.h"
#include "global_variables.h"

/**
  * @brief Inicjalizacja USART do komunikacji z modułem WiFi
  */
void USART6_Init(void);

/**
  * Inicjalizuje piny GPIO i ustawia przerwanie ::EXTI3_IRQHandler
  * na rosnące zbocze
  * @brief Inicjalizacja Fotokomórki
  */
void GPIO_IR_Init(void);

/**
  * Inicjalizuje piny GPIO i ustawia przerwanie ::EXTI15_10_IRQHandler
  * @brief Inicjalizacja przycisku użytkownika
  */
void GPIO_BUTTON_Init(void);

/**
  * @brief GPIO for onboard LED's Initialization Function
  */
void GPIO_LED_Init(void);

/**
  * @brief GPIO Initialization Function
  */
void MX_GPIO_Init(void);

/**
  * @brief USART3 Initialization Function
  */
void MX_USART3_UART_Init(void);

/**
  * @brief Inicjalizuje UART4 do komunikacji z modułem GPS
  */
void MX_UART4_Init(void);

/**
 *  Inicjalizuje przerwanie ::RTC_WKUP_IRQHandler() do wywoływania co sekundę
  * @brief Inicjalizuje RTC
  * Enables RTC_WKUP_IRQn_Handler every second
  */
void MX_RTC_Init(void);

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void);

#endif // INIT_FUNCTIONS_H
