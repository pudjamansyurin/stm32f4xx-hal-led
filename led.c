/*
 * led.c
 *
 *  Created on: Feb 12, 2022
 *      Author: pudja
 */
#include "./led.h"

/* Private macros */
#define GPIO_PIN_CNT            (16)
#define GPIO_PIN(__X__)         (1 << (__X__))
#define DELAY_MS(__X__)         (HAL_Delay(__X__))

/* Private function declarations */
static void _write(struct Led *led, GPIO_PinState state);
static void UTIL_PortEnableClock(GPIO_TypeDef *port);
static void UTIL_PortDisableClock(GPIO_TypeDef *port);

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
  if (pin_num >= GPIO_PIN_CNT)
    return HAL_ERROR;

  /* Check the parameters */
  assert_param(IS_GPIO_ALL_INSTANCE(port));
  assert_param(IS_GPIO_PIN(GPIO_PIN(pin_num)));

  /* Initialize properties */
  __HAL_UNLOCK(led);
  led->port = port;
  led->pin_num = pin_num;

  /* Enable the GPIO Clock */
  UTIL_PortEnableClock(port);

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
  _write(led, 0);
  /* DeInit the GPIO pin */
  HAL_GPIO_DeInit(led->port, GPIO_PIN(led->pin_num));

  __HAL_UNLOCK(led);
  return (HAL_OK);
}

/**
 * @brief Configure led suspend mode
 * @note The clock port is also disabled
 * @param led Pointer to Led handle
 * @param suspend Suspend state
 * @return HAL Status
 */
HAL_StatusTypeDef LED_Suspend(struct Led *led, FunctionalState suspend)
{
  __HAL_LOCK(led);

  /* Modify GPIO and clock state */
  if (suspend)
  {
    HAL_GPIO_DeInit(led->port, GPIO_PIN(led->pin_num));
    UTIL_PortDisableClock(led->port);
  }
  else
  {
    UTIL_PortEnableClock(led->port);
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
  _write(led, state);
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
 * @brief Blink selected LED
 * @param led Pointer to Led handle
 * @param on On duration
 * @param off Off duration
 * @return HAL Status
 */
HAL_StatusTypeDef LED_Blink(struct Led *led, uint32_t on, uint32_t off)
{
  __HAL_LOCK(led);
  _write(led, 1);
  DELAY_MS(on);
  _write(led, 0);
  DELAY_MS(off);

  __HAL_UNLOCK(led);
  return (HAL_OK);
}

/* Private function definitions */
static void _write(struct Led *led, GPIO_PinState state)
{
  /* Check active mode */
  if (led->active_mode == LED_ACTIVE_LOW)
    state = !state;

  /* Write the new state */
  HAL_GPIO_WritePin(led->port, GPIO_PIN(led->pin_num), state);
}

/**
 * @brief Enable GPIO clock
 * @param port Port to be enabled
 */
static void UTIL_PortEnableClock(GPIO_TypeDef *port)
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

/**
 * @brief Disable GPIO clock
 * @param port Port to be disabled
 */
static void UTIL_PortDisableClock(GPIO_TypeDef *port)
{
  assert_param(IS_GPIO_ALL_INSTANCE(port));

  /* Disable appropriate GPIO clock */
  if (port == GPIOH)
    __HAL_RCC_GPIOH_CLK_DISABLE();
  else if (port == GPIOG)
    __HAL_RCC_GPIOG_CLK_DISABLE();
  else if (port == GPIOF)
    __HAL_RCC_GPIOF_CLK_DISABLE();
  else if (port == GPIOE)
    __HAL_RCC_GPIOE_CLK_DISABLE();
  else if (port == GPIOD)
    __HAL_RCC_GPIOD_CLK_DISABLE();
  else if (port == GPIOC)
    __HAL_RCC_GPIOC_CLK_DISABLE();
  else if (port == GPIOB)
    __HAL_RCC_GPIOB_CLK_DISABLE();
  else if (port == GPIOA)
    __HAL_RCC_GPIOA_CLK_DISABLE();
}
