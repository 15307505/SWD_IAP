/**
  ******************************************************************************
  * @file    I2C/I2C_TwoBoards/I2C_DataExchangeDMA/main.h
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif 
  
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#if defined (USE_STM324xG_EVAL)
  #include "stm324xg_eval.h"

#elif defined (USE_STM324x7I_EVAL) 
  #include "stm324x7i_eval.h"
   
#else
 #error "Please select first the Evaluation board used in your application (in Project Options)"
#endif
  
/* Exported typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
  
/* Exported define ------------------------------------------------------------*/
/* Uncomment the line below if you will use the I2C peripheral as a Master */
/*  #define I2C_MASTER */
/* Uncomment the line below if you will use the I2C peripheral as a Slave */
#define I2C_SLAVE
  
/* To use the I2C at 400 KHz (in fast mode), the PCLK1 frequency (I2C peripheral
   input clock) must be a multiple of 10 MHz */
/* Uncomment the line below if you will use the I2C peripheral in Fast Mode */
#define FAST_I2C_MODE
  
/* Uncomment the line below if you will use the I2C peripheral in 10-bit addressing
   mode */
//#define I2C_10BITS_ADDRESS
  
/* Define I2C Speed ----------------------------------------------------------*/
#ifdef FAST_I2C_MODE
 #define I2C_SPEED 340000
 #define I2C_DUTYCYCLE I2C_DutyCycle_16_9  

#else /* STANDARD_I2C_MODE*/
 #define I2C_SPEED 100000
 #define I2C_DUTYCYCLE  I2C_DutyCycle_2
#endif /* FAST_I2C_MODE*/
  
/* Define Slave Address  -----------------------------------------------------*/
#ifdef I2C_10BITS_ADDRESS
 #define SLAVE_ADDRESS (uint16_t)0x0330

#else /* I2C_7BITS_ADDRESS */
 #define SLAVE_ADDRESS 0x30
#endif /* I2C_10BITS_ADDRESS */
 
/* USER_TIMEOUT value for waiting loops. This timeout is just a guarantee that the
   application will not remain stuck if the I2C communication is corrupted. 
   You may modify this timeout value depending on CPU frequency and application
   conditions (interrupts routines, number of data to transfer, speed, CPU
   frequency...). */ 
#define USER_TIMEOUT                  ((uint32_t)0x64) /* Waiting 1s */  
  
/* I2Cx Communication boards Interface */
#define I2Cx_DMA                      DMA1
#define I2Cx_DMA_CHANNEL              DMA_Channel_1
#define I2Cx_DR_ADDRESS               ((uint32_t)0x40005410)
#define I2Cx_DMA_STREAM_TX            DMA1_Stream6
#define I2Cx_DMA_STREAM_RX            DMA1_Stream0
#define I2Cx_TX_DMA_TCFLAG            DMA_FLAG_TCIF6
#define I2Cx_TX_DMA_FEIFLAG           DMA_FLAG_FEIF6
#define I2Cx_TX_DMA_DMEIFLAG          DMA_FLAG_DMEIF6
#define I2Cx_TX_DMA_TEIFLAG           DMA_FLAG_TEIF6
#define I2Cx_TX_DMA_HTIFLAG           DMA_FLAG_HTIF6
#define I2Cx_RX_DMA_TCFLAG            DMA_FLAG_TCIF0
#define I2Cx_RX_DMA_FEIFLAG           DMA_FLAG_FEIF0
#define I2Cx_RX_DMA_DMEIFLAG          DMA_FLAG_DMEIF0
#define I2Cx_RX_DMA_TEIFLAG           DMA_FLAG_TEIF0
#define I2Cx_RX_DMA_HTIFLAG           DMA_FLAG_HTIF0
#define DMAx_CLK                      RCC_AHB1Periph_DMA1    

#define I2Cx                          I2C1
#define I2Cx_CLK                      RCC_APB1Periph_I2C1
#define I2Cx_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define I2Cx_SDA_PIN                  GPIO_Pin_9                
#define I2Cx_SDA_GPIO_PORT            GPIOB                       
#define I2Cx_SDA_SOURCE               GPIO_PinSource9
#define I2Cx_SDA_AF                   GPIO_AF_I2C1
  
#define I2Cx_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define I2Cx_SCL_PIN                  GPIO_Pin_6                
#define I2Cx_SCL_GPIO_PORT            GPIOB                    
#define I2Cx_SCL_SOURCE               GPIO_PinSource6
#define I2Cx_SCL_AF                   GPIO_AF_I2C1

#define TXBUFFERSIZE   (countof(aTxBuffer) - 1)
#define RXBUFFERSIZE   TXBUFFERSIZE

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
  
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
