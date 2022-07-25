#include "core.h"
#ifndef __GPIO_H
#define __GPIO_H


void gpio_init(GPIO_TypeDef *Portx, uint16_t Pin);




void gpio_write(GPIO_TypeDef *Portx, uint16_t Pin,GPIO_PinState  state);


GPIO_PinState  gpio_read(GPIO_TypeDef *Portx, uint16_t Pin);


#endif