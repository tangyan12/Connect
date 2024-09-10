/**********************
*       INCLUDE        
**********************/
#include "uart.h"
/**********************
*       DEFINE
**********************/
#define MAXSIZE 100
/**********************
*   GLOBAL VARIABLE
**********************/
static UART_HandleTypeDef huart1;
static uint8_t uart1_rx_buffer[MAXSIZE];
static uint8_t uart1_tx_buffer[MAXSIZE];
static uint16_t uart1_rx_reg;
uint8_t uart1_rx_flag;
uint8_t uart1_tx_flag;
/**********************
*     FUNCTIONS
**********************/

int SYS_uart1_init(uint32_t baudrate)
{
    int res;
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();


    GPIO_InitTypeDef gpio_initstructure;
    gpio_initstructure.Mode = GPIO_MODE_AF_PP;
    gpio_initstructure.Pin = GPIO_PIN_9;
    gpio_initstructure.Pull = GPIO_PULLUP;
    gpio_initstructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA,&gpio_initstructure);
    gpio_initstructure.Mode = GPIO_MODE_AF_INPUT;
    gpio_initstructure.Pin = GPIO_PIN_10;
    HAL_GPIO_Init(GPIOA,&gpio_initstructure);
    
    huart1.Instance = USART1;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.BaudRate = baudrate;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;//default value
    res = HAL_UART_Init(&huart1);
    if(res != HAL_OK){
        HAL_UART_DeInit(&huart1);
        __HAL_RCC_USART1_CLK_DISABLE();
        return -1; 
    }
    
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    HAL_NVIC_SetPriority(USART1_IRQn,3,0);
    USART1->CR1 = USART1->CR1 | USART_FLAG_RXNE;//开启USART1的接受数据寄存器非空中断
    return 0;
}
int SYS_uart1_read_data(uint8_t *buffer,int length)
{
    uint16_t real_length;
    if(uart1_rx_flag){
        real_length = uart1_rx_reg & 0x3FFF;
        if(length > real_length){
            length = real_length; 
        }
        memcpy(buffer,uart1_rx_buffer,length);
        uart1_rx_flag = 0;
        uart1_rx_reg = 0;
        return length;
    }
    else
        return 0;
}
void SYS_uart1_send_data(const char *format,...)
{
    va_list arg;
    va_start(arg,format);
    vsnprintf((char *)uart1_tx_buffer,MAXSIZE,format,arg);
    strcat((char *)uart1_tx_buffer,"\r\n");
    va_end(arg);

    for (int i = 0; uart1_tx_buffer[i] != '\0'; i++)
    {
        USART1->DR = uart1_tx_buffer[i];
        while(!(USART1->SR & USART_FLAG_TXE));
    } 
}

void USART1_IRQHandler(void)
{
    if((USART1->SR & USART_FLAG_RXNE)){
        uint8_t ch = USART1->DR;
         if((uart1_rx_reg & 0x8000) == 0){
            if((uart1_rx_reg & 0x4000) != 0){
                if(ch == 0x0A){
                    uart1_rx_reg |= 0x8000;
                    uart1_rx_flag = 1;
                }
                else
                    uart1_rx_reg = 0;
            }
            else{
                if(ch == 0x0D){
                    uart1_rx_reg |= 0x4000;
                }
                else{
                    uart1_rx_buffer[uart1_rx_reg&0x3FFF] = ch;
                    uart1_rx_reg++;
                    if(uart1_rx_reg > MAXSIZE-1){
                        uart1_rx_reg = 0;
                    }
                }
            }
        }
        USART1->SR &= (~USART_FLAG_RXNE);
    }
}
