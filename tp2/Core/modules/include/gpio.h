#include "core.h"
#ifndef __GPIO_H
#define __GPIO_H


void gpio_init(GPIO_TypeDef *Portx, uint16_t Pin);




void gpio_write(uint8_t* buffer,uint32_t len);


void gpio_read(uint8_t* buffer,uint32_t len);


#endif