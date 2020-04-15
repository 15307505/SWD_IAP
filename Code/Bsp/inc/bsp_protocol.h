#ifndef __BSP_PROTOCOL_H
#define __BSP_PROTOCOL_H


#define COM1_MAX_LEN       5
#define COM1_MAX_RECV_LEN  530
#define RBUF_MAX_LEN       530

extern unsigned char isEmpty(USART_TypeDef* USARTx);
extern unsigned char buffSize(USART_TypeDef* USARTx);
extern unsigned char isFull(USART_TypeDef* USARTx);
extern void pushBuff (USART_TypeDef* USARTx,u8 data);                                        //定义一个串口一接收数据的缓冲区
extern u8 popBuff(USART_TypeDef* USARTx);
extern void clearBuff (USART_TypeDef* USARTx);
	
typedef struct 
{      
    unsigned char rbuf[RBUF_MAX_LEN];
    unsigned short len;
}DataBuf;

typedef struct uartReceive                                                     
{			 			
    //unsigned char buff[rebufLen];		                                
    short readIndex;                                                
    short writeIndex;					 	       
}uartRec;

extern DataBuf Nb_Buf[COM1_MAX_LEN];
void COM1_Recieve_Callback(u8 Rxd_Data);
extern DataBuf COM1_Buf[COM1_MAX_LEN];
extern DataBuf COM1Temp;
extern uartRec COM1Receive;
extern u8 COM1_Rx_Sta;
#endif


