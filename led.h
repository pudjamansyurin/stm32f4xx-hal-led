/*
 * led.h
 *
 *  Created on: Feb 12, 2022
 *      Author: pudja
 */

#ifndef INC_MODULES_LED_H_
#define INC_MODULES_LED_H_

#include "stm32f4xx_hal.h"

/* Public macros */
#define DELAY_MS(__X__)        HAL_Delay(__X__)

/* Public types */
enum LedActiveMode {
  LED_ACTIVE_LOW,
  LED_ACTIVE_HIGH
};

struct Led {
  HAL_LockTypeDef Lock;
  GPIO_TypeDef *port;
  GPIO_InitTypeDef init;
  uint8_t pin_num;
  enum LedActiveMode active_mode;
};

/* Public function definitions */
HAL_StatusTypeDef LED_Init(struct Led *led, GPIO_TypeDef *port, uint8_t pin_num);
HAL_StatusTypeDef LED_DeInit(struct Led *led);
HAL_StatusTypeDef LED_Suspend(struct Led *led, uint8_t suspend);
HAL_StatusTypeDef LED_SetActiveMode(struct Led *led, enum LedActiveMode mode);
HAL_StatusTypeDef LED_Write(struct Led *led, GPIO_PinState state);
HAL_StatusTypeDef LED_Toggle(struct Led *led);
HAL_StatusTypeDef LED_Blink(struct Led *led, uint32_t timeout);

#endif /* INC_MODULES_LED_H_ */
