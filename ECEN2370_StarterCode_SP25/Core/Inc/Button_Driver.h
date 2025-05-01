/*
 * Button_Driver.h
 *
 *  Created on: Apr 22, 2025
 *      Author: linhtran
 */

#ifndef INC_BUTTON_DRIVER_H_
#define INC_BUTTON_DRIVER_H_

#include <stdbool.h>
#include "stm32f4xx_hal.h"

#define BUTTON_PORT_NUMBER GPIOA
#define BUTTON_PIN_NUMBER 0
#define BUTTON_PRESSED 1
#define BUTTON_UNPRESSED 0

void Button_Init();
bool Button_IsPressed();
void Interrupt_Init();


#endif /* INC_BUTTON_DRIVER_H_ */
