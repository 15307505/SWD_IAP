#include "bsp.h"

u8 COM1_Rx_Sta=0;

DataBuf COM1_Buf[COM1_MAX_LEN];

DataBuf COM1Temp;

uartRec COM1Receive = {0,0};
/**************************************************************************************************
 * 缓冲区当前所有数据长度
 *************************************************************************************************/
unsigned char buffSize(USART_TypeDef* USARTx)
{
	if(USARTx == USART1)
	{
    short  wi = COM1Receive.writeIndex;
    short  ri = COM1Receive.readIndex;
    return (wi >= ri) ? (wi - ri) : (COM1_MAX_LEN - ri + wi);
	}
//	else if(USARTx == UART3)
//	{
//    short  wi = Rs232Receive.writeIndex;
//    short  ri = Rs232Receive.readIndex;
////	SendData1(wi);
////	SendData1(ri);
//    return (wi >= ri) ? (wi - ri) : (RS232_MAX_LEN - ri + wi);
//	}
//	else if(USARTx == UART5)
//	{
//    short  wi = Rs485Receive.writeIndex;
//    short  ri = Rs485Receive.readIndex;
//    return (wi >= ri) ? (wi - ri) : (RS485_MAX_LEN - ri + wi);
//	}
	return 0;
}

/**************************************************************************************************
 * 缓冲区是否空了 
返回值 :1 空  0: 非空
 *************************************************************************************************/
unsigned char isEmpty(USART_TypeDef* USARTx)
{	
	if(USARTx == USART1)
	{
    short  wi = COM1Receive.writeIndex;
    short  ri = COM1Receive.readIndex;
    return (wi == ri);
	}
//	else if(USARTx == UART3)
//	{
//    short  wi = Rs232Receive.writeIndex;
//    short  ri = Rs232Receive.readIndex;
//    return (wi == ri);
//	}
//	else if(USARTx == UART5)
//	{
//    short  wi = Rs485Receive.writeIndex;
//    short  ri = Rs485Receive.readIndex;
//    return (wi == ri);
//	}
	return 1;
}

/**************************************************************************************************
 * 缓冲区是否满了
返回值  1: 满 0: 非满
 *************************************************************************************************/
unsigned char isFull(USART_TypeDef* USARTx)
{

	if(USARTx == USART1)
	{
    short  wi = COM1Receive.writeIndex;
    short  ri = COM1Receive.readIndex;
    return ((wi + 1) % COM1_MAX_LEN == ri);
	}
//	else if(USARTx == UART3)
//	{
//    short  wi = Rs232Receive.writeIndex;
//    short  ri = Rs232Receive.readIndex;
//    return ((wi + 1) % RS232_MAX_LEN == ri);
//	}
//	else if(USARTx == UART5)
//	{
//    short  wi = Rs485Receive.writeIndex;
//    short  ri = Rs485Receive.readIndex;
//    return ((wi + 1) % RS485_MAX_LEN == ri);
//	}
	return 1;
}

/**************************************************************************************************
 * 将串口接收到的 数据 从顶部压入堆栈
 *************************************************************************************************/

void pushBuff (USART_TypeDef* USARTx,u8 data)
{
//	GPSBuf[GSMReceive.writeIndex].rbuf[UART1_Rx_Sta]  = data;
//  GPSBuf[GSMReceive.writeIndex].len   = UART1_Rx_Sta;
//		
//    //memcpy(uart1Buf[_uartReceive.writeIndex].rbuf,data.rbuf,data.len);          //rbuf 数值传递
//    //GPSBuf.len = data.len;                           //len数值传递
//    //writeIndex = (writeIndex + 1) % UART1_MAX_SEND_LEN;

	if(USARTx == USART1)
	{
		COM1_Buf[COM1Receive.writeIndex].rbuf[COM1_Rx_Sta]  = data;
		COM1_Buf[COM1Receive.writeIndex].len   = COM1_Rx_Sta+1;
	}
//	else if(USARTx == UART3)
//	{
//		Rs232_Buf[Rs232Receive.writeIndex].rbuf[Rs232_Rx_Sta]  = data;
//		Rs232_Buf[Rs232Receive.writeIndex].len   = Rs232_Rx_Sta+1;
//	}	
//	else if(USARTx == UART5)
//	{
//		Rs485_Buf[Rs485Receive.writeIndex].rbuf[g_tModH.RxCount]  = data;
//		Rs485_Buf[Rs485Receive.writeIndex].len   = g_tModH.RxCount+1;
//	}	
	
}  

/**************************************************************************************************
 * 将堆栈里的数据从底部弹出
 *************************************************************************************************/

u8 popBuff(USART_TypeDef* USARTx)
{
	if(USARTx == USART1)
	{
		if(isEmpty(USART1) == 1)
	  {
			 return 0;
	  }
		memcpy(COM1Temp.rbuf,COM1_Buf[COM1Receive.readIndex].rbuf,COM1_Buf[COM1Receive.readIndex].len);
		COM1Temp.len = COM1_Buf[COM1Receive.readIndex].len;
		COM1Receive.readIndex = (COM1Receive.readIndex + 1) % COM1_MAX_LEN;
		return 1;
	}
//	else if(USARTx == UART3)
//	{
//		if(isEmpty(UART3) == 1)
//	  {
//			 return 0;
//	  }
//		memcpy(Rs232Temp.rbuf,Rs232_Buf[Rs232Receive.readIndex].rbuf,Rs232_Buf[Rs232Receive.readIndex].len);
//		Rs232Temp.len = Rs232_Buf[Rs232Receive.readIndex].len;
//		Rs232Receive.readIndex = (Rs232Receive.readIndex + 1) % RS232_MAX_LEN;
//    return 3;    
//	}
//	else if(USARTx == UART5)
//	{
//		if(isEmpty(UART5) == 1)
//	  {
//			 return 0;
//	  }
//		memcpy(g_tModH.RxBuf,Rs485_Buf[Rs485Receive.readIndex].rbuf,Rs485_Buf[Rs485Receive.readIndex].len);
//		g_tModH.len = Rs485_Buf[Rs485Receive.readIndex].len;
//		Rs485Receive.readIndex = (Rs485Receive.readIndex + 1) % RS485_MAX_LEN;
//    return 5;    
//	}
	
	return 0;
}

void clearBuff (USART_TypeDef* USARTx)
{
	if(USARTx == USART1)
	{
	  COM1Receive.readIndex = COM1Receive.writeIndex;
	}
//	else if(USARTx == UART3)
//	{
//		Rs232Receive.readIndex = Rs232Receive.writeIndex;
//	}
//	else if(USARTx == UART5)
//	{
//		Rs485Receive.readIndex = Rs485Receive.writeIndex;
//	}
	
}

/*******************   接收中断回调  *********************/
void COM1_Recieve_Callback(u8 Rxd_Data)
{
  IAP_t.Rxd_TimeOut = 50;  //10MS超时服务
	if (isFull(USART1))
	{
		COM1_Rx_Sta = 0;
		printf("COM1 Data over\r\n");
		return;
	}
	pushBuff (USART1,Rxd_Data);              //记录接收到的值
	COM1_Rx_Sta ++;
	
	if(COM1_Rx_Sta > COM1_MAX_RECV_LEN)
	{
		COM1_Rx_Sta = 0;
		COM1Receive.writeIndex = (COM1Receive.writeIndex + 1) % COM1_MAX_LEN;
	}	
}
