/**
  @page I2C_EEPROM  How to use the I2C to drive an EEPROM memory
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    I2C/I2C_EEPROM/readme.txt 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Description of the I2C and M24CXX EEPROM communication example.
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

This example shows how to use the I2C firmware library and an associate 
I2C EEPROM driver to communicate with an I2C EEPROM device (here the example is 
interfacing with M24C64 EEPROM)

I2C peripheral is configured in Master transmitter during write operation and in
Master receiver during read operation from I2C EEPROM. 

The peripheral used is I2C1 but can be configured by modifying the defines values
in stm324xg_eval.h (or stm324x7i_eval.h) file. The speed is set to 100kHz and can 
be configured by modifying the relative define in stm324xg_eval_i2c_ee.h 
(or stm324x7i_eval_i2c_ee.h) file.

For M24C64 devices all the memory is accessible through the two-bytes 
addressing mode and needs to define block addresses. In this case, only the physical 
address has to be defined (according to the address pins (E0,E1 and E2) connection).
This address is defined in stm324xg_eval_i2c_ee.h (or stm324x7i_eval_i2c_ee.h) 
(default is 0xA0: E0, E1 and E2 tied to ground). 
The EEPROM addresses where the program starts the write and the read operations 
is defined in the main.c file. 

First, the content of Tx1_Buffer is written to the EEPROM_WriteAddress1 and the
written data are read. The written and the read buffers data are then compared.
Following the read operation, the program waits that the EEPROM reverts to its 
Standby state. A second write operation is, then, performed and this time, Tx2_Buffer
is written to EEPROM_WriteAddress2, which represents the address just after the last 
written one in the first write. After completion of the second write operation, the 
written data are read. The contents of the written and the read buffers are compared.

All transfers are managed in DMA mode (except when 1-byte read/write operation is
required). Once sEE_ReadBuffer() or sEE_WriteBuffer() function is called, the 
use application may perform other tasks in parallel while Read/Write operation is
managed by DMA.

This example provides the possibility to use the STM324xG-EVAL (or STM32437I-EVAL) 
LCD screen for messages display (transfer status: Ongoing, PASSED, FAILED).
To enable this option uncomment the define USE_LCD in the main.h file.  


@par Directory contents 

  - I2C/I2C_EEPROM/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - I2C/I2C_EEPROM/stm32f4xx_conf.h     Library Configuration file
  - I2C/I2C_EEPROM/stm32f4xx_it.c       Interrupt handlers
  - I2C/I2C_EEPROM/stm32f4xx_it.h       Interrupt handlers header file
  - I2C/I2C_EEPROM/main.c               Main program
  - I2C/I2C_EEPROM/main.h               Main program header file

      
@par Hardware and Software environment
  
  - This example runs on STM32F40xx/STM32F41xx, STM32F427x/STM32F437x Devices.
    
  - This example has been tested with STMicroelectronics STM324xG-EVAL (STM32F40xx/
    STM32F41xx Devices) and STM32437I-EVAL (STM32F427x/STM32F437x Devices) evaluation 
    boards and can be easily tailored to any other supported device and development 
    board.

  - STM324xG-EVAL and STM32437I-EVAL Set-up 
    - Make sure the jumper JP24 "WP" is fitted on the board.
    - You may modify the timeout values "sEE_LONG_TIMEOUT" present in "stm324x7i_eval_i2c_ee.h" 
      depending on CPU frequency and application conditions.


@par How to use it ? 

In order to make the program work, you must do the following:
 - Copy all source files from this example folder to the template folder under
   Project\STM32F4xx_StdPeriph_Templates
 - Open your preferred toolchain 
 - Select the project workspace related to the used device 
   - If "STM32F40_41xxx" is selected as default project Add the following files in the project source list:
     - Utilities\STM32_EVAL\STM3240_41_G_EVAL\stm324xg_eval.c
     - Utilities\STM32_EVAL\STM3240_41_G_EVAL\stm324xg_eval_lcd.c 
     - Utilities\STM32_EVAL\STM3240_41_G_EVAL\stm324xg_eval_i2c_ee.c
      
   - If "STM32F427_437xx" is selected as default project Add the following files in the project source list:
     - Utilities\STM32_EVAL\STM324x7I_EVAL\stm324x7i_eval.c
     - Utilities\STM32_EVAL\STM324x7I_EVAL\stm324x7i_eval_lcd.c 
     - Utilities\STM32_EVAL\STM324x7I_EVAL\stm324x7i_eval_i2c_ee.c
     
 - Rebuild all files and load your image into target memory
 - Run the example
   
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
