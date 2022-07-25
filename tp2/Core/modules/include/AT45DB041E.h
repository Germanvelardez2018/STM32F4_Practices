#include "spi.h"

#ifndef __AT45DB041E_H
#define __AT45DB041E_H




#define AT45DB_TIMEOUT          1000
#define TIMEOUT_SPI             100
#define MEMORY_SPI_PRESCALER    SIMO_SPI_PRESCALER_2


//static  SIMO_GPIO_PIN       __chip_select; //! Pin de Chip Select
//static  SIMO_SPI            __port ;       //! Port SPI Definido 
//static flash_storage_t      __flash;       //! Estructura de memoria. Depende de capacidad y tamaño de pagina 


#define RESUME_CMD                          0xAB       //salgo del deep sleep_us
#define PG_PER_BLOCK                        (8)
#define PG_PER_SECTOR                       (256)

#define AT45DB_RDDEVID                      0x9f /* Product ID, Manufacturer ID and density read */
#define AT45DB_RDSR                         0xd7 /* SR */
#define AT45DB_RESUME                       0xab /* Resume flash */

#define AT45DB_MANUFACT_ATMEL               0x1F /* Atmel manufacturer */
#define AT45DB_DENSITY_MSK                  0x1f /* Density Mask */
#define AT45DB_FAM_MSK                      0xe0 /* Family Mask */
#define AT45DB_FAM_DATAFLASH                0x20 /* DataFlash device */
#define AT45DB_DENSITY_64MBIT               0x08 /* AT45DB641 */
/* Read */
#define AT45DB_RDARRAYHF                    0x0b /* Continuous array read (high frequency) */
/* Erase */
#define AT45DB_PGERASE                      0x81 /* Page Erase */
#define AT45DB_MNTHRUBF1                    0x82 /* Main memory page program through buffer 1 */

/* PageSize reporgramming sequence */
#define AT45DB_PGSIZE_SIZE 4
static const uint8_t at45db_pgsize_cmd[] = {
    0x3D, 0x2A, 0x80, 0xA6
};

#define MANUFACTURER_ID                      0x1F    //id byte  1
#define DEVICE_ID_1                          0x24    //id byte  2
#define DEVICE_ID_2                          0x00    //id byte  3
#define EDI                                  0x01    // id byte 4


#define AT45DB_STATUS_PGSIZE                (1 << 0) /* PAGE SIZE */
#define AT45DB_STATUS_PROTECT               (1 << 1) /* PROTECT */
#define AT45DB_STATUS_COMP                  (1 << 6) /* COMP */
#define AT45DB_STATUS_READY                 (1 << 7) /* RDY/BUSY */

#define AT45DB_LOW_POWER                    (0x9B)  // INTO LOW POWER

#define AT45DB041E_PAGES_DEFAULT             2048   //! En paginas de 256


//! Estrutuca con formato de la memoria, copiada de una libreria de github
typedef struct __flash_storage
{
    int pg_num;
    int pg_shifts;
    uint32_t block_sz;
    uint32_t erase_sz;
    uint32_t n_eraseblocks;
} flash_storage_t;






/**
 * @brief Define el tamanio en bytes de la pagina a utilizar 
 * 
 */
typedef enum {
     pg_256byte     =  8           //! 2048 Paginas totales  
    ,pg_512bytes    =  9           //! 1024 Paginas totales
    ,pg_1024byte    =  10          //! 512  Paginas Totales
    ,pg_2048bytes   =  11          //! 256  Paginas Totales
}mem_page_size;








void AT45DB041E_init();


uint32_t AT45DB041E_write( uint8_t buffer, uint32_t len,uint32_t page,uint8_t pos);



uint32_t AT45DB041E_read( uint8_t buffer, uint32_t len,uint32_t page,uint8_t pos);



#endif

