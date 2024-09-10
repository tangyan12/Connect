#ifndef __MYSPI_H__
#define __MYSPI_H__
/**********************
*       INCLUDE        
**********************/
#include "stm32f1xx_hal.h"
/**********************
*       DEFINE
**********************/

#define SPI_MODE 0
#define SPI_MOSI_PIN GPIO_PIN_15
#define SPI_MISO_PIN GPIO_PIN_14
#define SPI_SCK_PIN  GPIO_PIN_13
#define SPI_NSS_PIN  GPIO_PIN_12
#define MO_W(x) HAL_GPIO_WritePin(GPIOB,SPI_MOSI_PIN,x)
#define SCK_W(x) HAL_GPIO_WritePin(GPIOB,SPI_SCK_PIN,x)
#define NSS_W(x) HAL_GPIO_WritePin(GPIOB,SPI_NSS_PIN,x)
#define MI_R() HAL_GPIO_ReadPin(GPIOB,SPI_MISO_PIN)
#if defined(SPI_MODE)
    #if (SPI_MODE == 0)
        #define SYS_spi_swap_byte(data) SYS_spi_mode_0_swap_byte(data) 
    #elif(SPI_MODE == 1)
        #define SYS_spi_swap_byte(data) SYS_spi_mode_1_swap_byte(data) 
    #elif(SPI_MODE == 2)
        #define SYS_spi_swap_byte(data) SYS_spi_mode_2_swap_byte(data)
    #elif(SPI_MODE == 3)
        #define SYS_spi_swap_byte(data) SYS_spi_mode_3_swap_byte(data)
    #endif
#endif
/**********************
*   EXTERN VARIABLE
**********************/

/**********************
*  FUNCTIONS DECLARE
**********************/
void SYS_spi_gpio_init(void);
void SYS_spi_start(void);
void SYS_spi_end(void);
uint8_t SYS_spi_mode_0_swap_byte(uint8_t data);
uint8_t SYS_spi_mode_1_swap_byte(uint8_t data);
uint8_t SYS_spi_mode_2_swap_byte(uint8_t data);
uint8_t SYS_spi_mode_3_swap_byte(uint8_t data);
#endif