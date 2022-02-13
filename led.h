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

struct LedStruct {
  GPIO_TypeDef *port;
  uint16_t pin;
  enum LedActiveMode active_mode;
  HAL_LockTypeDef Lock;
};

/* Public function definitions */
HAL_StatusTypeDef LED_Init(struct LedStruct *led,
                           GPIO_TypeDef *port,
                           uint16_t pin);
HAL_StatusTypeDef LED_DeInit(struct LedStruct *led);
HAL_StatusTypeDef LED_SetActiveMode(struct LedStruct *led,
                                    enum LedActiveMode mode);
HAL_StatusTypeDef LED_Write(struct LedStruct *led, GPIO_PinState state);
HAL_StatusTypeDef LED_Toggle(struct LedStruct *led);
HAL_StatusTypeDef LED_Blink(struct LedStruct *led, uint32_t timeout);

#endif /* INC_MODULES_LED_H_ */
