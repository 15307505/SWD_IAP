/**
  ******************************************************************************
  * @file    USART/USART_TwoBoards/USART_DataExchangeInterrupt/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"

/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_DataExchangeInterrupt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint8_t aTxBuffer[];
extern uint8_t aRxBuffer[];
extern uint8_t aCmdBuffer[];
extern uint8_t aAckBuffer[];
extern __IO uint8_t ubRxIndex;
extern __IO uint8_t ubTxIndex;

extern __IO uint8_t ubUsartTransactionType;
extern __IO uint8_t ubUsartMode;

__IO uint8_t ubCounter = 0x00;
extern __IO uint32_t TimeOut;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  /* Decrement the timeout value */
  if (TimeOut != 0x0)
  {
    TimeOut--;
  }
  
  if (ubCounter < 10)
  {
    ubCounter++;
  }
  else
  {
    ubCounter = 0x00;
    STM_EVAL_LEDToggle(LED1);
  }
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f40xx.s/startup_stm32f427x.s).                         */
/******************************************************************************/

/**
* @brief  This function handles USRAT interrupt request.
* @param  None
* @retval None
*/
void USARTx_IRQHandler(void)
{
  /* USART in Receiver mode */
  if (USART_GetITStatus(USARTx, USART_IT_RXNE) == SET)
  {
    if (ubRxIndex < BUFFERSIZE)
    {
      /* Receive Transaction data */
      aRxBuffer[ubRxIndex++] = USART_ReceiveData(USARTx);
    }
    else
    {
      /* Disable the Rx buffer not empty interrupt */
      USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE);
    }
  }
  /* USART in Transmitter mode */
  if (USART_GetITStatus(USARTx, USART_IT_TXE) == SET)
  {
    if (ubTxIndex < BUFFERSIZE)
    {
      /* Send Transaction data */
      USART_SendData(USARTx, aTxBuffer[ubTxIndex++]);
    }
    else
    {
      /* Disable the Tx buffer empty interrupt */
      USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
    }
  }
}


/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
