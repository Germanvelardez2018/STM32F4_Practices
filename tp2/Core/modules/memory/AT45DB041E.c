#include "AT45DB041E.h"
#define SPI_CS_PIN                      (10)




void AT45DB041E_init(){
    spi_init();
    gpio_init();

}


void AT45DB041E_write( uint8_t buffer, uint32_t len){

}



void AT45DB041E_read( uint8_t buffer, uint32_t len){

}