/**********************
*       INCLUDE        
**********************/
#include "can.h"
/**********************
*       DEFINE
**********************/

/**********************
*   GLOBAL VARIABLE
**********************/

/**********************
*     FUNCTIONS
**********************/
/** 
* @brief
* @param
* @return
*/
#define CAN_TX_PIN GPIO_Pin_12
#define CAN_RX_PIN GPIO_Pin_11
void SYS_can_gpio_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitTypeDef can_gpio_initstructure;
    can_gpio_initstructure.GPIO_Mode = GPIO_Mode_AF_PP;
    can_gpio_initstructure.GPIO_Pin = CAN_TX_PIN;
    can_gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&can_gpio_initstructure);
    can_gpio_initstructure.GPIO_Mode = GPIO_Mode_IPU;
    can_gpio_initstructure.GPIO_Pin = CAN_RX_PIN;
    GPIO_Init(GPIOA,&can_gpio_initstructure);
}
void SYS_can_1_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);

    CAN_InitTypeDef can_initstructure;
    can_initstructure.CAN_Mode =CAN_Mode_LoopBack ;//
    can_initstructure.CAN_Prescaler=48;
    can_initstructure.CAN_BS1=2;
    can_initstructure.CAN_BS2=3; 
    can_initstructure.CAN_SJW=2; 
    can_initstructure.CAN_NART=DISABLE;//禁止报文自动重传
    can_initstructure.CAN_TXFP=DISABLE;//发送FIFO优先级由标识符(0)/请求顺序(1)决定
    can_initstructure.CAN_ABOM=DISABLE;//CAN设备退出离线 增加软件开关(0)/不增加软件开关(1)
    can_initstructure.CAN_AWUM=DISABLE;//睡眠模式时由 软件唤醒(0)/硬件自动唤醒(1)
    can_initstructure.CAN_TTCM=DISABLE;//时间触发通信模式 禁止(0)/允许(1)
    can_initstructure.CAN_RFLM=DISABLE;//接收FIFO模式 覆盖原有报文(0)/丢弃新来报文(1)
    CAN_Init(CAN1,&can_initstructure);

    CAN_FilterInitTypeDef can_filter_initstructure;
    can_filter_initstructure.CAN_FilterActivation=ENABLE;
    can_filter_initstructure.CAN_FilterNumber =0;
    can_filter_initstructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
    can_filter_initstructure.CAN_FilterMode=CAN_FilterMode_IdMask;
    can_filter_initstructure.CAN_FilterScale=CAN_FilterScale_32bit;
    can_filter_initstructure.CAN_FilterIdLow=0;
    can_filter_initstructure.CAN_FilterMaskIdLow=0;
    can_filter_initstructure.CAN_FilterIdHigh=0;
    can_filter_initstructure.CAN_FilterMaskIdHigh=0;//报文无限制
    CAN_FilterInit(&can_filter_initstructure);

}

void SYS_can_trans_std_data(uint16_t ID,uint8_t length,uint8_t*databuf)
{
    CanTxMsg TxMessage;
    uint32_t time_out;
    uint8_t trans_mailbox_num;
    TxMessage.IDE=CAN_Id_Standard;
    TxMessage.RTR=CAN_RTR_Data;
    TxMessage.StdId=ID;
    TxMessage.ExtId=0;
    TxMessage.DLC=length;
    for (uint8_t i = 0; i < length; i++)
    {
        TxMessage.Data[i] = databuf[i];
    }
    trans_mailbox_num = CAN_Transmit(CAN1,&TxMessage);
    while (CAN_TransmitStatus(CAN1,trans_mailbox_num)!=CAN_TxStatus_Ok);
}
uint8_t SYS_can_rece_check_vaild(void)
{
    return CAN_MessagePending(CAN1,CAN_FIFO0);
}
void SYS_can_rece_std_data(uint16_t *ID,uint8_t*length,uint8_t* databuf)
{
    CanRxMsg RxMessage;
    CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);
    if(RxMessage.IDE != CAN_Id_Standard){
        return;
    }
    if(RxMessage.RTR != CAN_RTR_Data){
        return;
    }
    *ID = RxMessage.StdId;
    *length = RxMessage.DLC;
    for (uint8_t i = 0; i < *length; i++)
    {
        databuf[i] = RxMessage.Data[i];
    }
    
}