/**
  @page GPIO_IOToggle GPIO IO Toggle example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    GPIO/IO_Toggle/readme.txt 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Description of the GPIO IO Toggle example.
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

GPIO ports are connected on AHB bus, using BSRRH and BSRRL registers, one cycle 
is required to set a pin and another cycle to reset it. So GPIO pins can toggle 
at AHB clock divided by 2.

This example describes how to use BSRRH and BSRRL (Port Bit Set/Reset Register
High and Low) for maximum IO toggling.

PG6 and PG8 (configured in output pushpull mode) toggles in a forever loop:
- Set PG6 and PG8 by setting corresponding bits in BSRRL register
- Reset PG6 and PG8 by setting corresponding bits in BSRRH register

In this example, HCLK is configured at 168 MHz so PG6 and PG8 toggle at 84MHz.
To achieve the maximum IO toggling frequency, you have to configure your compiler
options for high speed optimization.


@par Directory contents 

  - GPIO/IOToggle/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - GPIO/IOToggle/stm32f4xx_conf.h     Library Configuration file
  - GPIO/IOToggle/stm32f4xx_it.c       Interrupt handlers
  - GPIO/IOToggle/stm32f4xx_it.h       Interrupt handlers header file
  - GPIO/IOToggle/main.c               Main program
  - GPIO/IOToggle/main.h               Main program header file
  

@par Hardware and Software environment

  - This example runs on STM32F405xx/407xx, STM32F415xx/417xx, STM32F427xx/437xx and 
    STM32F429xx/439xx devices.
    
  - This example has been tested with STMicroelectronics STM324xG-EVAL (STM32F40xx/
    STM32F41xx Devices), STM32437I-EVAL (STM32F427xx/STM32F437xx Devices) and 
    STM324x9I-EVAL RevB (STM32F429xx/STM32F439xx Devices) evaluation boards and 
    can be easily tailored to any other supported device and development board.     
    

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
          
   - If "STM32F429_439xx" is selected as default project Add the following files in the project source list:
     - Utilities\STM32_EVAL\STM324x9I_EVAL\stm324x9i_eval.c
                 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 