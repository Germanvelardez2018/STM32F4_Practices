#include "core.h"
#ifndef __SPI_H
#define __SPI_H

/**
 * @brief init spi peripheral
 * 
 * @return ** void 
 */
void inline spi_init(void);


/**
 * @brief 
 * 
 * @param buffer data array to write
 * @param len   buffer size 
 * @param timeout 
 * @return ** void 
 */
void inline spi_write(uint8_t* buffer,uint32_t len,uint32_t timeout);


/**
 * @brief 
 * 
 * @param buffer data array where it will save the read data
 * @param len   buffer size 
 * @param timeout 
 * @return ** void 
 */
void inline spi_read(uint8_t* buffer,uint32_t len,uint32_t timeout);


#endif