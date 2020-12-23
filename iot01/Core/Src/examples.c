/*
 * examples.c
 *
 *  Created on: Dec 20, 2020
 *      Author: kim
 */

#include "main.h"
#include "examples.h"

void LED1_Toggle(uint32_t interval)
{
	HAL_GPIO_TogglePin(URLED1_GPIO_Port, URLED1_Pin);
	HAL_Delay(interval);
}

void LED2_Toggle(uint32_t interval)
{
	HAL_GPIO_TogglePin(URLED2_GPIO_Port, URLED2_Pin);
	HAL_Delay(interval);
}

extern UART_HandleTypeDef huart1;

int __io_putchar(int ch) {
	while (HAL_OK != HAL_UART_Transmit(&huart1, (uint8_t *) &ch, 1, 30000)) {

	}
	return ch;
}

int __io_getchar(void) {
	uint8_t ch = 0;
	while(HAL_OK != HAL_UART_Receive(&huart1, (uint8_t *)&ch, 1, 30000)) {

	}
	return ch;
}
