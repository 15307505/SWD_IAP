/**
  @page PWR_PVD PWR Programmable Voltage Detector (PVD) example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    PWR/PWR_PVD/readme.txt 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Description of the PWR Programmable Voltage Detector (PVD) example.
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

This example shows how to configure the programmable voltage detector using
an external interrupt line. In this example, EXTI line 16 is configured to generate 
an interrupt on each rising or falling edge of the PVD output signal (which 
indicates that the Vdd voltage is below the PVD threshold).
In the interrupt routine a LED connected to a specific GPIO pin is toggled every 
time the voltage drops below or above the target threshold.

A LED connected to a specific GPIO pin is toggling to indicate that the system 
is in RUN.


@par Directory contents 
  
  - PWR/PWR_PVD/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - PWR/PWR_PVD/stm32f4xx_conf.h     Library Configuration file
  - PWR/PWR_PVD/stm32f4xx_it.c       Interrupt handlers
  - PWR/PWR_PVD/stm32f4xx_it.h       Interrupt handlers header file
  - PWR/PWR_PVD/main.c               Main program
  - PWR/PWR_PVD/main.h               Main program header file


@par Hardware and Software environment

  - This example runs on STM32F405xx/407xx, STM32F415xx/417xx and STM32F427xx/437xx 
    devices.
    
  - This example has been tested with STMicroelectronics STM324xG-EVAL (STM32F40xx/
    STM32F41xx Devices) and STM32437I-EVAL (STM32F427xx/STM32F437xx Devices) evaluation 
    boards and can be easily tailored to any other supported device and development board.

  - HW Set-up
    - Connect a LED to PG.06 pin (will be referred to by LED1 in the example code)
    - Connect a LED to PG.07 pin (will be referred to by LED2 in the example code)
    
 @note This example can't run on the evaluation boards.
  

@par How to use it ? 

In order to make the program work, you must do the following:
 - Copy all source files from this example folder to the template folder under
   Project\STM32F4xx_StdPeriph_Templates
 - Open your preferred toolchain 
 - Select the project workspace related to the used device 
   - If "STM32F40_41xxx" is selected as default project Add the following files in the project source list:
     - Utilities\STM32_EVAL\STM3240_41_G_EVAL\stm324xg_eval.c
        
   - If "STM32F427_437xx" is selected as default project Add the following files in the project source list:
     - Utilities\STM32_EVAL\STM324x7I_EVAL\stm324x7i_eval.c
             
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */