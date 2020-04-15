/**
  @page Lib_DEBUG_Example Lib DEBUG example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Lib_DEBUG/Lib_DEBUG_Example/readme.txt 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Description of the Lib DEBUG example.
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

This example shows how to declare a dynamic peripherals pointers used for Debug
mode.  

To use Debug mode you have to add the stm32f4xx_ip_dbg.c file to your application.
This creates a pointer to the peripheral structure in SRAM. Debugging consequently
becomes easier and all register settings can be obtained by dumping a peripheral 
variable.

When the "USE_FULL_ASSERT" label is uncommented (in stm32f4xx_conf.h file),
the assert_param macro is expanded and run-time checking is enabled in the firmware
library code. The run-time checking allows checking that all the library functions
input value lies within the parameter allowed values.

The associated program simulates wrong parameter passed to library function and the
source of the error is printed on Hyperterminal (through USART).

 @note The Debug mode increases the code size and reduces the code performance. 
       For this reason, it is recommended to used it only when debugging the 
       application and to remove it from the final application code.


@par Directory contents 
  
  - Lib_DEBUG/Lib_DEBUG_Example/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - Lib_DEBUG/Lib_DEBUG_Example/stm32f4xx_conf.h     Library Configuration file
  - Lib_DEBUG/Lib_DEBUG_Example/stm32f4xx_it.c       Interrupt handlers
  - Lib_DEBUG/Lib_DEBUG_Example/stm32f4xx_it.h       Interrupt handlers header file
  - Lib_DEBUG/Lib_DEBUG_Example/main.c               Main program
  - Lib_DEBUG/Lib_DEBUG_Example/main.h               Main program header file
  - Lib_DEBUG/Lib_DEBUG_Example/stm32f4xx_ip_dbg.c   Peripherals pointers initialization
  - Lib_DEBUG/Lib_DEBUG_Example/stm32f4xx_ip_dbg.h   Header for stm32f4xx_ip_dbg.c
  

@par Hardware and Software environment

  - This example runs on STM32F405xx/407xx, STM32F415xx/417xx and STM32F427xx/437xx devices.
    
  - This example has been tested with STMicroelectronics STM324xG-EVAL and STM32437I-EVAL 
    (STM32F427x/437x Devices) evaluation boards and can be easily tailored to any other  
    supported device and development board

  - STM324xG-EVAL/STM32437I-EVAL Set-up
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN16 and PC serial port.
      @note Make sure that JP22 is in position 1-2
 
  - Hyperterminal configuration:
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - BaudRate = 115200 baud
    - flow control: None

           
@par How to use it ? 

In order to make the program work, you must do the following:
 - Copy all source files from this example folder to the template folder under
   Project\STM32F4xx_StdPeriph_Templates
 - Open your preferred toolchain 
 - Add the required example files
   - stm32f4xx_ip_dbg.c
 - Rebuild all files and load your image into target memory
 - Run the example

 @note The free Lite version of TrueSTUDIO do not support printf() redirection.
       For more information, see "note.txt" under "STM32F4xx_StdPeriph_Templates
       \TrueSTUDIO".
       
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
