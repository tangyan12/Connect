#ifndef __USART_H__
#define __USART_H__
/**********************
*       INCLUDE        
**********************/
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
/**********************
*       DEFINE
**********************/

/**********************
*   EXTERN VARIABLE
**********************/
extern uint8_t uart1_rx_flag;
extern uint8_t uart1_tx_flag;
/**********************
*  FUNCTIONS DECLARE
**********************/
int SYS_uart1_init(uint32_t baudrate);
int SYS_uart1_read_data(uint8_t *buffer,int length);
void SYS_uart1_send_data(const char *format,...);
#endif