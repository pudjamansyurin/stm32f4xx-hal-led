# STM32F4xx HAL Led Module

## How to use the module

#### **`main.c`**

```c
#include "stm32f4xx-hal-led/led.h"

int main(void)
{
  struct LedStruct hled;

  /* Initialize the LED for port A5 */
  LED_Init(&hled, GPIOA, 5);

  /* Set default to Active High */
  LED_SetActiveMode(&hled, LED_ACTIVE_HIGH);

  /* Super loop */
  while(1) {
    /* Turn on the LED */
    LED_Write(&hled, 1);

    /* Toggle the LED */
    LED_Toggle(&hled);

    /* Blink the LED */
    LED_Blink(&hled, 1000, 500);
  }

  /* DeInit the LED */
  LED_DeInit(&hled);
}
```
