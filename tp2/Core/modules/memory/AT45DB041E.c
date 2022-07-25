#include "AT45DB041E.h"
#include "gpio.h"
#define GPIO_PORT_CS                           (GPIO_TypeDef*) GPIOD
#define GPIO_PIN_CS                            (uint16_t)  GPIO_PIN_0
#define TIMEOUT_MEMORY                          (1000)
#define BYTES_P_PAGES                           (pg_256byte)






static flash_storage_t      __flash;       //! Estructura de memoria. Depende de capacidad y tamaño de pagina 





static inline uint8_t at45db_get_status(void);
static uint8_t at45db_busy();
static void at45db_resumen();
static uint32_t at45db_start();
static void at45db_set(mem_page_size page_size);



//--------------------------------




static void at45db_set(mem_page_size page_size){
    uint8_t offset = page_size - 8 ;     // ejemplo si las pag sno 1024, tenemos en total page_default ( 2 a la offset) 
    /* TODO: Add other densities, atm only AT45DB041E*/
    __flash.pg_shifts     = page_size;                     //Lo que hace es paginas de 1024 bytes potencia(2,10)
    __flash.pg_num        = AT45DB041E_PAGES_DEFAULT / (1 << offset) ;                    //512 PAGINAS DE 256 BYTES
    __flash.block_sz      = 1 << __flash.pg_shifts;  // Una pagina(1024) por block?
    __flash.erase_sz      = __flash.block_sz;
    __flash.n_eraseblocks = __flash.pg_num;
}

static inline uint8_t at45db_get_status(void){
    uint8_t cmd[2]={AT45DB_RDSR,0};
    uint8_t status[2]={0,0};
    //flanco descendente.En bajo el CS Pin
    gpio_write(GPIO_PORT_CS,GPIO_PIN_CS,0);
    spi_write_read(cmd,status,2,TIMEOUT_SPI);    //leo el primer byte del registro estado
    //flanco ascendente.En alto el CS Pin
    gpio_write(GPIO_PORT_CS,GPIO_PIN_CS,1);
    return  status[1];  // status[0] no tiene nada, se leyo antes de enviar comando
}


static uint8_t at45db_busy(){
    uint8_t ret = 0;
    uint32_t counter= 0;
    while(!((ret =__at45db_get_status()) & AT45DB_STATUS_READY) || !(counter > AT45DB_TIMEOUT))  //mientras este ocupado, espere
    {
        counter+=1;
    }
    return ret;  //es el primer byte del registro de estados
}








static void at45db_resumen(){
    gpio_write(GPIO_PORT_CS,GPIO_PIN_CS,1);
    gpio_write(GPIO_PORT_CS,GPIO_PIN_CS,0);
    spi_write((uint8_t*)RESUME_CMD,1,TIMEOUT_MEMORY);
    gpio_write(GPIO_PORT_CS,GPIO_PIN_CS,1);
    at45db_bsy();
}



static uint32_t at45db_start(){
    at45db_resumen();
    HAL_Delay(1);
    uint32_t ret = 1;
    uint8_t  cmd[5]= {AT45DB_RDDEVID,0,0,0,0};
    uint8_t  data[5]={0,0,0,0,0};
    gpio_write(GPIO_PORT_CS,GPIO_PIN_CS,0);
    spi_write_read(cmd,data,5,TIMEOUT_SPI); // Transaccion sin IT
    gpio_write(GPIO_PORT_CS,GPIO_PIN_CS,1);
    //checkeo respuesta correcta, este es solo para AT45DB041E, reformular
    if( data[1] != AT45DB_MANUFACT_ATMEL    
        || data[2] != DEVICE_ID_1        
        || data[3] != DEVICE_ID_2        
        || data[4] != EDI) {
        return 0;  //fallo checkeo de ID
    }
  
    if(!(__at45db_bsy() & AT45DB_STATUS_PGSIZE)) /* Configuro en modo 256 por pagina*/
    {   
        gpio_write(GPIO_PORT_CS,GPIO_PIN_CS,0);
        spi_write((uint8_t*)at45db_pgsize_cmd,AT45DB_PGSIZE_SIZE,TIMEOUT_SPI);
        gpio_write(GPIO_PORT_CS,GPIO_PIN_CS,1);
    }
      at45db_set(BYTES_P_PAGES);
    
    return ret; // Exito
//

}



void  AT45DB041E_erase_full(void){
    uint8_t cmd[4];
    cmd[0] = 0xc7;
    cmd[1] = 0x94;
    cmd[2] = 0x80;
    cmd[3] = 0x9a;
    gpio_write(GPIO_PORT_CS,GPIO_PIN_CS,0);
    spi_write((uint8_t*)cmd,4,TIMEOUT_SPI);
    gpio_write(GPIO_PORT_CS,GPIO_PIN_CS,1);
    __at45db_bsy();
}




void AT45DB041E_init(){
    gpio_init(GPIO_PORT_CS,GPIO_PIN_CS);
    spi_init();

}


uint32_t AT45DB041E_write_page( uint8_t buffer, uint32_t len,uint32_t page,uint8_t pos){

      // CONS 256 11 ADRRES | 8 BITES POSICION
    uint32_t ret = 0;
    if(page < __flash.pg_num){
     uint32_t offset = (page << __flash.pg_shifts) |((uint32_t)pos);
     uint8_t cmd[4] ;    
     cmd[0] = AT45DB_MNTHRUBF1;
     cmd[1] = (offset >> 11)  & 0xff;
     cmd[2] = (offset >> 3)   & 0xff;
     cmd[3] = (offset << 5)   & 0xff;
        //inicia (pulso en bajo)
     gpio_write(GPIO_PORT_CS,GPIO_PIN_CS, 0); 
     spi_write(cmd,4,TIMEOUT_SPI);   // envio comandos
     spi_write(buffer,(uint32_t)len,TIMEOUT_SPI); // escribo en memoria
      // finaliza (pulso en alto)
     gpio_write(GPIO_PORT_CS,GPIO_PIN_CS, 1); 
     at45db_busy();
     ret = 1;
     }    
    return ret;

}



uint32_t AT45DB041E_read_page( uint8_t buffer, uint32_t len,uint32_t page,uint8_t pos){

    int32_t ret= 0; //fallo por defecto
    if(page < __flash.pg_num ){
       uint8_t cmd[5] ;    
       uint32_t offset = (page << __flash.pg_shifts)|((uint32_t)pos); // 01 << 8 (256 bytes x pag) o 01 <<10 (1024 bytes x pag)
       cmd[0] = AT45DB_RDARRAYHF;
       cmd[1] = (offset >> 11) & 0xff;
       cmd[2] = (offset >> 3) & 0xff;
       cmd[3] = (offset << 5) & 0xff;
       cmd[4] = 0;
       //inicia (pulso en bajo)
       simo_gpio_write(GPIO_PORT_CS,GPIO_PIN_CS, 0); 
       simo_spi_write(cmd,5,TIMEOUT_SPI);    //  Envio secuencia de commandos
       simo_spi_read(buffer,len,TIMEOUT_SPI);    //data a escribir
       // finaliza (pulso en alto)
       simo_gpio_write(GPIO_PORT_CS,GPIO_PIN_CS, 1); 
       __at45db_bsy();
       ret = 1; // success
     }
    return ret;

}