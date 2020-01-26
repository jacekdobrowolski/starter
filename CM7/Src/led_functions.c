#include "led_functions.h"

void LED_RED_ON(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
}

void LED_RED_TOGGLE(void)
{
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
}

void LED_RED_OFF(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
}

void LED_GREEN_ON(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
}

void LED_GREEN_TOGGLE(void)
{
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
}

void LED_GREEN_OFF(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
}

void LED_YELLOW_ON(void)
{
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);
}

void LED_YELLOW_TOGGLE(void)
{
    HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1);
}

void LED_YELLOW_OFF(void)
{
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
}
