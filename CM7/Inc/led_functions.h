/**
  ******************************************************************************
  * @file           led_functions.h
  * @authors        Filip Kulik, Jacek Dobrowolski
  * @brief          Plik naglowkowy z funkcjami do sterowania LED na płytce nucleo
  ******************************************************************************
  */

#ifndef LED_FUNCTIONS_H
#define LED_FUNCTIONS_H
#include "stm32h755xx.h"
#include "stm32h7xx_hal_gpio.h"
/**
 * Włącza czerwoną diodę
 * \param Brak
 * \retval Brak
 */
void LED_RED_ON(void);
/**
 * Przełącza czerwoną diodę
 * \param Brak
 * \retval Brak
 */
void LED_RED_TOGGLE(void);
/**
 * Wyłącza czerwoną diodę
 * \param Brak
 * \retval Brak
 */
void LED_RED_OFF(void);
/**
 * Włącza zieloną diodę
 * \param Brak
 * \retval Brak
 */
void LED_GREEN_ON(void);
/**
 * Przełącza zieloną diodę
 * \param Brak
 * \retval Brak
 */
void LED_GREEN_TOGGLE(void);
/**
 * Wyłącza zieloną diodę
 * \param Brak
 * \retval Brak
 */
void LED_GREEN_OFF(void);
/**
 * Włącza żółtą diodę
 * \param Brak
 * \retval Brak
 */
void LED_YELLOW_ON(void);
/**
 * Przełącza żółtą diodę
 * \param Brak
 * \retval Brak
 */
void LED_YELLOW_TOGGLE(void);
/**
 * Wyłącza żółtą diodę
 * \param Brak
 * \retval Brak
 */
void LED_YELLOW_OFF(void);

#endif // LED_FUNCTIONS_H
