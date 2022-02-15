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
 * @param pin_num The GPIO pin number
 * @return HAL Status
 */
HAL_StatusTypeDef LED_Init(struct Led *led, GPIO_TypeDef *port, uint8_t pin_num)
{
  /* Check the structure handle allocation */
  if (led == NULL)
    return HAL_ERROR;
  if (pin_num >= GPIO_PIN_CNT) {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_GPIO_ALL_INSTANCE(port));
  assert_param(IS_GPIO_PIN(GPIO_PIN(pin_num)));

  /* Initialize properties */
  led->Lock = HAL_UNLOCKED;
  led->port = port;
  led->pin_num = pin_num;

  /* Enable the GPIO Clock */
  CMN_PortEnableClock(port);

  /* Configure the GPIO pin */
  led->init.Pin = GPIO_PIN(led->pin_num);
  led->init.Mode = GPIO_MODE_OUTPUT_PP;
  led->init.Pull = GPIO_NOPULL;
  led->init.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(led->port, &led->init);

  LED_SetActiveMode(led, LED_ACTIVE_HIGH);
  LED_Write(led, GPIO_PIN_RESET);
  return (HAL_OK);
}

/**
 * @brief DeInit LEDs.
 * @note The clock port is not disabled by default
 * @param led Pointer to Led handle
 * @return HAL Status
 */
HAL_StatusTypeDef LED_DeInit(struct Led *led)
{
  __HAL_LOCK(led);
  /* Turn off LED */
  LED_Write(led, GPIO_PIN_RESET);
  /* DeInit the GPIO pin */
  HAL_GPIO_DeInit(led->port, GPIO_PIN(led->pin_num));

  __HAL_UNLOCK(led);
  return (HAL_OK);
}

/**
 * @brief Configure led suspend mode
 * @note The clock port is not disabled by default
 * @param led Pointer to Led handle
 * @param suspend Suspend state
 * @return HAL Status
 */
HAL_StatusTypeDef LED_Suspend(struct Led *led, uint8_t suspend)
{
  __HAL_LOCK(led);
  /* Enable clock only when activation */
  if (!suspend) {
    CMN_PortEnableClock(led->port);
  }

  /* Modify GPIO state */
  if (suspend) {
    HAL_GPIO_DeInit(led->port, GPIO_PIN(led->pin_num));
  } else {
    HAL_GPIO_Init(led->port, &led->init);
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
  HAL_GPIO_WritePin(led->port, GPIO_PIN(led->pin_num), state);
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
  HAL_GPIO_TogglePin(led->port, GPIO_PIN(led->pin_num));
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
