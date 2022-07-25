#include "gpio.h"







void gpio_init(GPIO_TypeDef *Portx, uint16_t Pin)
{

     GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Portx, Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED2_Pin LED1_Pin */
  GPIO_InitStruct.Pin = Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Portx, &GPIO_InitStruct);
}

void gpio_write(uint8_t *buffer, uint32_t len)
{
}

void gpio_read(uint8_t *buffer, uint32_t len)
{
}