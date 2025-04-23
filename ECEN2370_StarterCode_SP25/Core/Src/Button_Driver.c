/*
 * Button_Driver.c
 *
 *  Created on: Apr 22, 2025
 *      Author: linhtran
 */
#include "Button_Driver.h"

void Button_Init() {
	GPIO_InitTypeDef button_pin_config;
	__HAL_RCC_GPIOA_CLK_ENABLE();

	button_pin_config.Pin = GPIO_PIN_0;
	button_pin_config.Mode = GPIO_MODE_INPUT;
	button_pin_config.Speed = GPIO_SPEED_FREQ_MEDIUM;
	button_pin_config.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOA, &button_pin_config);
}

bool Button_IsPressed() {
	uint8_t val = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
	if (val) {
		return true;
	}
	return false;
}

void Interrupt_Init() {
	GPIO_InitTypeDef button_pin_config;
	button_pin_config.Pin = GPIO_PIN_0;
	button_pin_config.Mode = GPIO_MODE_IT_RISING;
	button_pin_config.Speed = GPIO_SPEED_FREQ_MEDIUM;
	button_pin_config.Pull = GPIO_NOPULL;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(GPIOA, &button_pin_config);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

