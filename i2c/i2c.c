/**********************
*       INCLUDE        
**********************/
#include "i2c.h"
/**********************
*       DEFINE
**********************/
#define I2C1_SCL_PIN GPIO_PIN_6
#define I2C1_SDA_PIN GPIO_PIN_7
#define SCL(x) HAL_GPIO_WritePin(GPIOB,I2C1_SCL_PIN,x)
#define SDA(x) HAL_GPIO_WritePin(GPIOB,I2C1_SDA_PIN,x)
/**********************
*   GLOBAL VARIABLE
**********************/

/**********************
*     FUNCTIONS
**********************/

void SYS_i2c_gpio_init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef i2c_gpio_initstructure;
    i2c_gpio_initstructure.Mode = GPIO_MODE_OUTPUT_OD;
    i2c_gpio_initstructure.Pin  = GPIO_PIN_6;
    i2c_gpio_initstructure.Pull = GPIO_NOPULL;
    i2c_gpio_initstructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB,&i2c_gpio_initstructure);
    i2c_gpio_initstructure.Pin = GPIO_PIN_7;
    HAL_GPIO_Init(GPIOB,&i2c_gpio_initstructure);

    SCL(1);
    SDA(1);
}


void SYS_i2c_start(void)
{
    SDA(1);
    SCL(1);

    SDA(0);
    SCL(0);
}
void SYS_i2c_end(void)
{
    SDA(0);

    SCL(1);
    SDA(1);
}
void SYS_i2c_send_one_byte(uint8_t byte)
{
    for (int i = 0; i < 8; i++)
    {
        if(byte & (0x80>>i)){
            SDA(1);
        }
        else
            SDA(0);
        SCL(1);
        SCL(0);
    }
}
uint8_t SYS_i2c_rece_one_byte(void)
{
    SDA(1);

    uint8_t byte = 0;
    for (int i = 0; i < 8; i++)
    {
        SCL(1);
        if(HAL_GPIO_ReadPin(GPIOB,I2C1_SDA_PIN)){
            byte |= (0x80>>i);
       }
        SCL(0);
    }
    return byte;
}
ACK_T SYS_i2c_check_ack(void)
{
    SDA(1);
    ACK_T res;

    SCL(1);
    if(HAL_GPIO_ReadPin(GPIOB,I2C1_SDA_PIN)){
        res = I2C_NACK;
    }
    else
        res = I2C_ACK;

    SCL(0);
    return res;
} 
void SYS_i2c_send_ack(uint8_t ack)
{   
    int s_ack;
    if(ack != 0){
        s_ack = 1;
    }
    else
        s_ack = ack;
    SDA(s_ack);
    SCL(1);
    SCL(0);

    SDA(1);
} 
int SYS_i2c_read_one_select_reg(uint8_t deviceID,uint8_t regaddr,uint8_t*rece_buf)
{
    SYS_i2c_start();
    SYS_i2c_send_one_byte(deviceID<<1);
    if(SYS_i2c_check_ack()){
        return 1;
    }
    SYS_i2c_send_one_byte(regaddr);
    if(SYS_i2c_check_ack()){
        return 1;
    }
    SYS_i2c_start();
    SYS_i2c_send_one_byte((deviceID<<1)|0x01);
    if(SYS_i2c_check_ack()){
        return 1;
    }
    *rece_buf = SYS_i2c_rece_one_byte();
    SYS_i2c_send_ack(1);
    SYS_i2c_end();
    return 0;
}
int SYS_i2c_write_one_select_reg(uint8_t deviceID,uint8_t regaddr,uint8_t data)
{
    SYS_i2c_start();
    SYS_i2c_send_one_byte(deviceID<<1);
    if(SYS_i2c_check_ack()){
        return 1;
    }
    SYS_i2c_send_one_byte(regaddr);
    if(SYS_i2c_check_ack()){
        return 1;
    }
    SYS_i2c_send_one_byte(data);
    if(SYS_i2c_check_ack()){
        return 1;
    }
    SYS_i2c_end();
    return 0;
}