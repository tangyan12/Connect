/**********************
*       INCLUDE        
**********************/
#include "spi.h"
/**********************
*       DEFINE
**********************/

/**********************
*   GLOBAL VARIABLE
**********************/

/**********************
*     FUNCTIONS
**********************/


void SYS_spi_gpio_init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef spi_gpio_initstructure;
    spi_gpio_initstructure.Mode = GPIO_MODE_OUTPUT_PP;
    spi_gpio_initstructure.Pin = SPI_MOSI_PIN | SPI_NSS_PIN | SPI_SCK_PIN;
    spi_gpio_initstructure.Pull = GPIO_PULLUP;
    spi_gpio_initstructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB,&spi_gpio_initstructure);
    spi_gpio_initstructure.Mode = GPIO_MODE_INPUT;
    spi_gpio_initstructure.Pin = SPI_MISO_PIN;
    HAL_GPIO_Init(GPIOB,&spi_gpio_initstructure);

    NSS_W(1);
    if(SPI_MODE == 0 || SPI_MODE == 1){
        SCK_W(0);
    }
    else
        SCK_W(1);
}

void SYS_spi_start(void)
{
    NSS_W(0);
}
void SYS_spi_end(void)
{
    NSS_W(1);
}


uint8_t SYS_spi_mode_0_swap_byte(uint8_t data)
{
    uint8_t byte = 0;
    for (int i = 0; i < 8; i++)
    {
        if(data & (0x80>>i)){
            MO_W(1);
        }
        else
            MO_W(0);

        SCK_W(1);

        if(MI_R()){
            byte |= (0x80>>i);
        }

        SCK_W(0);
    }
    return byte;
}

uint8_t SYS_spi_mode_1_swap_byte(uint8_t data)
{
    uint8_t byte = 0;
    for (int i = 0; i < 8; i++)
    {
        SCK_W(1);

        if(data & (0x80>>i)){
            MO_W(1);
        }
        else
            MO_W(0);

        SCK_W(0);

        if(MI_R()){
            byte |= (0x80>>i);
        }

    }
    return byte;
}

uint8_t SYS_spi_mode_2_swap_byte(uint8_t data)
{
    uint8_t byte = 0;
    for (int i = 0; i < 8; i++)
    {
        if(data & (0x80>>i)){
            MO_W(1);
        }
        else
            MO_W(0);
            
        SCK_W(0);

        if(MI_R()){
            byte |= (0x80>>i);
        }

        SCK_W(1);
    }
    return byte;
}

uint8_t SYS_spi_mode_3_swap_byte(uint8_t data)
{
    uint8_t byte = 0;
    for (int i = 0; i < 8; i++)
    {
        SCK_W(0);

        if(data & (0x80>>i)){
            MO_W(1);
        }
        else
            MO_W(0);
      
        SCK_W(1);

        if(MI_R()){
            byte |= (0x80>>i);
        }
    }
    return byte;
}
