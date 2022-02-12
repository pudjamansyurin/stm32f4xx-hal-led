/*
 * led.h
 *
 *  Created on: Feb 12, 2022
 *      Author: pudja
 */

#ifndef INC_MODULES_LED_H_
#define INC_MODULES_LED_H_

#include "stm32f4xx_hal.h"

/* Public types */
struct LedStruct {
  GPIO_TypeDef *port;
  uint16_t pin;
  uint8_t active_low;
  HAL_LockTypeDef Lock;
};

/* Public function definitions */
HAL_StatusTypeDef LED_Init(struct LedStruct *led,
                           GPIO_TypeDef *port,
                           uint16_t pin);
HAL_StatusTypeDef LED_DeInit(struct LedStruct *led);
HAL_StatusTypeDef LED_Write(struct LedStruct *led, GPIO_PinState state);
HAL_StatusTypeDef LED_Toggle(struct LedStruct *led);
HAL_StatusTypeDef LED_SetActiveLow(struct LedStruct *led, uint8_t state);

#endif /* INC_MODULES_LED_H_ */
