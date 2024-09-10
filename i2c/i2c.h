#ifndef __SYS_I2C_H__
#define __SYS_I2C_H__
/**********************
*       INCLUDE        
**********************/
#include "stm32f1xx_hal.h"
/**********************
*       DEFINE
**********************/
typedef enum{I2C_ACK=0,I2C_NACK}ACK_T;
/**********************
*   EXTERN VARIABLE
**********************/

/**********************
*  FUNCTIONS DECLARE
**********************/
void SYS_i2c_gpio_init(void);
void SYS_i2c_start(void);
void SYS_i2c_end(void);
void SYS_i2c_send_one_byte(uint8_t byte);
uint8_t SYS_i2c_rece_one_byte(void);
int SYS_i2c_write_one_select_reg(uint8_t deviceID,uint8_t regaddr,uint8_t data);
int SYS_i2c_read_one_select_reg(uint8_t deviceID,uint8_t regaddr,uint8_t*rece_buf);
#endif