/*
 * led.c
 *
 *  Created on: Feb 12, 2022
 *      Author: pudja
 */
#include "./led.h"

/* Private function declarations */
static inline void PortEnableClock(GPIO_TypeDef *port);

/* Public function definitions */
/**
 * @brief Configures LED GPIO.
 * @param led Pointer to LedStruct handle
 * @param port The GPIO port used
 * @param pin The GPIO pin used
 * @return HAL Status
 */
HAL_StatusTypeDef LED_Init(struct LedStruct *led,
                           GPIO_TypeDef *port,
                           uint16_t pin)
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
  PortEnableClock(port);

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
 * @note Led DeInit does not disable the GPIO clock
 * @param led Pointer to LedStruct handle
 * @return HAL Status
 */
HAL_StatusTypeDef LED_DeInit(struct LedStruct *led)
{
  __HAL_LOCK(led);
  /* Turn off LED */
  LED_Write(led, GPIO_PIN_RESET);
  /* DeInit the GPIO pin */
  HAL_GPIO_DeInit(led->port, led->pin);

  __HAL_UNLOCK(led);
  return (HAL_OK);
}

/**
 * @brief Change default LED active mode
 * @param led Pointer to LedStruct handle
 * @param state The new mode
 * @return HAL Status
 */
HAL_StatusTypeDef LED_SetActiveMode(struct LedStruct *led,
                                    enum LedActiveMode mode)
{
  __HAL_LOCK(led);
  led->active_mode = mode;
  __HAL_UNLOCK(led);

  return (HAL_OK);
}

/**
 * @brief Change selected LED state
 * @param led Pointer to LedStruct handle
 * @param state The new led state
 * @return HAL Status
 */
HAL_StatusTypeDef LED_Write(struct LedStruct *led, GPIO_PinState state)
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
 * @param led Pointer to LedStruct handle
 * @return HAL Status
 */
HAL_StatusTypeDef LED_Toggle(struct LedStruct *led)
{
  __HAL_LOCK(led);
  HAL_GPIO_TogglePin(led->port, led->pin);
  __HAL_UNLOCK(led);

  return (HAL_OK);
}

/**
 * @brief Blink selected LED state
 * @param led Pointer to LedStruct handle
 * @param timeout Timeout duration
 * @return HAL Status
 */
HAL_StatusTypeDef LED_Blink(struct LedStruct *led, uint32_t timeout)
{
  __HAL_LOCK(led);
  LED_Write(led, GPIO_PIN_SET);
  DELAY_MS(timeout);
  LED_Write(led, GPIO_PIN_RESET);
  __HAL_UNLOCK(led);

  return (HAL_OK);
}

/* Private function definitions */
/**
 * @brief Enable GPIO clock
 * @param port Port to be enabled
 */
static inline void PortEnableClock(GPIO_TypeDef *port)
{
  assert_param(IS_GPIO_ALL_INSTANCE(port));

  /* Enable appropriate GPIO clock */
  if (port == GPIOH)
    __HAL_RCC_GPIOH_CLK_ENABLE();
  else if (port == GPIOG)
    __HAL_RCC_GPIOG_CLK_ENABLE();
  else if (port == GPIOF)
    __HAL_RCC_GPIOF_CLK_ENABLE();
  else if (port == GPIOE)
    __HAL_RCC_GPIOE_CLK_ENABLE();
  else if (port == GPIOD)
    __HAL_RCC_GPIOD_CLK_ENABLE();
  else if (port == GPIOC)
    __HAL_RCC_GPIOC_CLK_ENABLE();
  else if (port == GPIOB)
    __HAL_RCC_GPIOB_CLK_ENABLE();
  else if (port == GPIOA)
    __HAL_RCC_GPIOA_CLK_ENABLE();
}
