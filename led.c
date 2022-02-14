/*
 * led.c
 *
 *  Created on: Feb 12, 2022
 *      Author: pudja
 */
#include "./led.h"
#include "stm32f4xx-hal-common/common.h"

/* Public function definitions */
/**
 * @brief Configures LED GPIO.
 * @param led Pointer to Led handle
 * @param port The GPIO port used
 * @param pin The GPIO pin used
 * @return HAL Status
 */
HAL_StatusTypeDef LED_Init(struct Led *led, GPIO_TypeDef *port, uint16_t pin)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Check the structure handle allocation */
  if (led == NULL)
    return HAL_ERROR;

  /* Check the parameters */
  assert_param(IS_GPIO_ALL_INSTANCE(port));
  assert_param(IS_GPIO_PIN(pin));

  /* Initialize properties */
  led->Lock = HAL_UNLOCKED;
  led->port = port;
  led->pin = pin;

  /* Enable the GPIO Clock */
  CMN_PortEnableClock(port);

  /* Configure the GPIO pin */
  GPIO_InitStruct.Pin = led->pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(led->port, &GPIO_InitStruct);

  LED_SetActiveMode(led, LED_ACTIVE_HIGH);
  LED_Write(led, GPIO_PIN_RESET);
  return (HAL_OK);
}

/**
 * @brief DeInit LEDs.
 * @note It is optional for disable the GPIO clock
 * @param led Pointer to Led handle
 * @param disable Disable the port clock
 * @return HAL Status
 */
HAL_StatusTypeDef LED_DeInit(struct Led *led, uint8_t disable)
{
  __HAL_LOCK(led);
  /* Turn off LED */
  LED_Write(led, GPIO_PIN_RESET);
  /* DeInit the GPIO pin */
  HAL_GPIO_DeInit(led->port, led->pin);

  /* Disable clock */
  if (disable) {
    CMN_PortDisableClock(led->port);
  }

  __HAL_UNLOCK(led);
  return (HAL_OK);
}

/**
 * @brief Configure led suspend mode
 * @param led Pointer to Led handle
 * @param on Suspend state
 * @return HAL Status
 */
HAL_StatusTypeDef LED_Suspend(struct Led *led, uint8_t on)
{
  __HAL_LOCK(led);
  /* Turn off LED */
  LED_Write(led, GPIO_PIN_RESET);

  /* Modify clock */
  if (on) {
    CMN_PortDisableClock(led->port);
  } else {
    CMN_PortEnableClock(led->port);
  }

  __HAL_UNLOCK(led);
  return (HAL_OK);
}

/**
 * @brief Change default LED active mode
 * @param led Pointer to Led handle
 * @param state The new mode
 * @return HAL Status
 */
HAL_StatusTypeDef LED_SetActiveMode(struct Led *led, enum LedActiveMode mode)
{
  __HAL_LOCK(led);
  led->active_mode = mode;
  __HAL_UNLOCK(led);

  return (HAL_OK);
}

/**
 * @brief Change selected LED state
 * @param led Pointer to Led handle
 * @param state The new led state
 * @return HAL Status
 */
HAL_StatusTypeDef LED_Write(struct Led *led, GPIO_PinState state)
{
  __HAL_LOCK(led);
  /* Check active mode */
  if (led->active_mode == LED_ACTIVE_LOW) {
    state = !state;
  }
  /* Write the new state */
  HAL_GPIO_WritePin(led->port, led->pin, state);
  __HAL_UNLOCK(led);

  return (HAL_OK);
}

/**
 * @brief Toggle selected LED state
 * @param led Pointer to Led handle
 * @return HAL Status
 */
HAL_StatusTypeDef LED_Toggle(struct Led *led)
{
  __HAL_LOCK(led);
  HAL_GPIO_TogglePin(led->port, led->pin);
  __HAL_UNLOCK(led);

  return (HAL_OK);
}

/**
 * @brief Blink selected LED state
 * @param led Pointer to Led handle
 * @param timeout Timeout duration
 * @return HAL Status
 */
HAL_StatusTypeDef LED_Blink(struct Led *led, uint32_t timeout)
{
  __HAL_LOCK(led);
  LED_Write(led, GPIO_PIN_SET);
  DELAY_MS(timeout);
  LED_Write(led, GPIO_PIN_RESET);
  __HAL_UNLOCK(led);

  return (HAL_OK);
}
