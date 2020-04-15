/**
  ******************************************************************************
  * @file    DMA/DMA_FLASHToRAM/main.h 
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#if defined (USE_STM324xG_EVAL)
  #include "stm324xg_eval.h"

#elif defined (USE_STM324x7I_EVAL) 
  #include "stm324x7i_eval.h"

#elif defined (USE_STM324x9I_EVAL) 
  #include "stm324x9i_eval.h"
#else
 #error "Please select first the Evaluation board used in your application (in Project Options)"
#endif
 
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* DMA Stream parameters definitions. You can modify these parameters to select
   a different DMA Stream and/or channel.
   But note that only DMA2 Streams are capable of Memory to Memory transfers */
#define DMA_STREAM               DMA2_Stream0
#define DMA_CHANNEL              DMA_Channel_0
#define DMA_STREAM_CLOCK         RCC_AHB1Periph_DMA2 
#define DMA_STREAM_IRQ           DMA2_Stream0_IRQn
#define DMA_IT_TCIF              DMA_IT_TCIF0
#define DMA_STREAM_IRQHANDLER    DMA2_Stream0_IRQHandler

#define BUFFER_SIZE              32
#define TIMEOUT_MAX              10000 /* Maximum timeout value */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
