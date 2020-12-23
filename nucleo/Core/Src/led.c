#include "main.h"
#include "led.h"

void led_on(port, pin) {
	HAL_GPIO_WritePin(port, pin, SET);
}

void led_off(port, pin) {
	HAL_GPIO_WritePin(port, pin, RESET);
}
