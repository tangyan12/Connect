#ifndef __CAN_REFE_H__
#define __CAN_REFE_H__
/**********************
*       INCLUDE        
**********************/
#include <stm32f10x.h>
/**********************
*       DEFINE
**********************/

/**********************
*   EXTERN VARIABLE
**********************/

/**********************
*  FUNCTIONS DECLARE
**********************/
void SYS_can_1_init(void);
void SYS_can_trans_std_data(uint16_t ID,uint8_t length,uint8_t*databuf);
uint8_t SYS_can_rece_check_vaild(void);
void SYS_can_rece_std_data(uint16_t *ID,uint8_t*length,uint8_t* databuf);
#endif