/**
  @page SDIO_uSDCard  How to use the SDIO to drive an SD card memory
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    SDIO/SDIO_uSDCard/readme.txt 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Description of the SDIO example.
  ******************************************************************************
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
  @endverbatim

@par Example Description 

This example shows how to use the SDIO firmware library and an associate driver 
to perform read/write operations on the SD Card memory  (SD Card V1.0, V1.1, 
V2.0 and SDHC (High Capacity) protocol) that could be mounted on the STM324xG_EVAL/
STM32437I_EVAL boards.

The example provides different SD Card transfer states and operations.
Below a description of each step:
  
- First the SDIO peripheral and SD Card are initialized using the "SD_Init()"
  function: 
  - Configure the SDIO according to the desired SDIO_CK clock frequency.
  - Reset the SD Card.
  - Identify the SD Card.
  - Initialize the SD Card.
  - Get the SD Card Information.
  - Select the SD Card.
  - Enable the Wide Bus mode (4-bit data).

- SD Card Erase Operation
  - Start an Erase operation: Erase the corresponding blocks using the "SD_Erase()"
    function. 
  - Read the Erased blocks using the "SD_ReadMultiBlocks()" function.
  - Test if the corresponding Blocks are well erased: check if the EraseStatus 
    variable is equal to PASSED. If the erase operation is passed LED1 turns ON.
            
- SD Card Single Block Operation
  - Start a Write operation: Write a single Block using the "SD_WriteBlock()"
    function. 
  - Read a single Block using the "SD_ReadBlock()" function
  - Compare the written Block and the read one: check if the TransferStatus1 
    variable is equal to PASSED. If the Single Block Read/write operation
    is passed LED2 turns ON.

- SD Card Multiple Block Operation
  - Start a Multiple Write operation: Write a multi Blocks using the 
    "SD_WriteMultiBlocks()" function. 
  - Read a multiple Blocks using the "SD_ReadMultiBlocks()" function
  - Compare the written Blocks and the read one: check if the TransferStatus2 
    variable is equal to PASSED. If the Multiple Blocks Read/Write operation
    is passed LED3 turns ON.

All data transfers are made by DMA.
At each operation, the SD Card presence and status is checked using the "SD_GetStatus()"
function and a global variable "Status" storing the results of the last operation.

SD Card Operations and Written/Read data correctness is signaled  by LED 
lightening and though as follows:
 - If the Erase operation is PASSED then LED1 ON else the LED4 is ON and LED1 
  is OFF 
 - If the Single Block Write/Read operation is PASSED then LED2 ON else the LED4 
   is ON and LED2 is OFF
 - If the Multi Blocks Write/Read operation is PASSED then LED3 ON else the LED4 
   is ON and LED3 is OFF
 - Any SD Card operation including the SD Initialization error is signaled by
   LED4 ON.
     

@par Directory contents 
  
  - SDIO/SDIO_uSDCard/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - SDIO/SDIO_uSDCard/stm32f4xx_conf.h     Library Configuration file
  - SDIO/SDIO_uSDCard/stm32f4xx_it.c       Interrupt handlers
  - SDIO/SDIO_uSDCard/stm32f4xx_it.h       Interrupt handlers header file
  - SDIO/SDIO_uSDCard/main.c               Main program
  - SDIO/SDIO_uSDCard/main.h               Main program header file
  
       
@par Hardware and Software environment 

  - This example runs on STM32F405xx/407xx, STM32F415xx/417xx, STM32F427xx/437xx devices.
    
  - This example has been tested with STMicroelectronics STM324xG-EVAL (STM32F40xx/
    STM32F41xx Devices) and STM32437I-EVAL (STM32F427xx/STM32F437xx Devices) 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM324xG-EVAL/STM32437I-EVAL Set-up
     - Make sure that Jumpers JP22 and JP16 are in position 1-2
         
    
@par How to use it ? 

In order to make the program work, you must do the following:
 - Copy all source files from this example folder to the template folder under
   Project\STM32F4xx_StdPeriph_Templates
 - Open your preferred toolchain 
 - Select the project workspace related to the used device 
   - If "STM32F40_41xxx" is selected as default project Add the following files in the project source list:
     - Utilities\STM32_EVAL\STM3240_41_G_EVAL\stm324xg_eval.c
     - Utilities\STM32_EVAL\STM3240_41_G_EVAL\stm324xg_eval_sdio_sd.c
      
   - If "STM32F427_437xx" is selected as default project Add the following files in the project source list:
     - Utilities\STM32_EVAL\STM324x7I_EVAL\stm324x7i_eval.c
     - Utilities\STM32_EVAL\STM324x7I_EVAL\stm324x7i_eval_sdio_sd.c
 - Rebuild all files and load your image into target memory
 - Run the example
    
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
