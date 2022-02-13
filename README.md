# STM32F4xx HAL Led Module

## How to use the module

```c
#include "stm32f4xx-hal-led-module/led.h"

int main(void)
{
  struct LedStruct hled;
  
  /* Initialize the LED for port A5 */
  LED_Init(&hled, GPIOA, GPIO_PIN_5);
  
  /* Set default to Active High */
  LED_SetActiveMode(&hled, LED_ACTIVE_HIGH);
  
  /* Super loop */
  while(1) {
    /* Turn on the LED */
    LED_Write(&hled, GPIO_PIN_SET);
    
    /* Toggle the LED */
    LED_Toggle(&hled);
    
    /* Blink the LED for 500 ms */
    LED_Blink(&hled, 500);
  }
  
  /* DeInit the LED */
  LED_DeInit(&hled);
}
```