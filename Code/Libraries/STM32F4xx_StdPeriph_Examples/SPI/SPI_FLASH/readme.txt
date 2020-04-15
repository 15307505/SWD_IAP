/**
  @page SPI_FLASH SPI FLASH example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    SPI/SPI_FLASH/readme.txt 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Description of the SPI FLASH example.
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

This example shows how to use the SPI firmware library and an associated 
driver "spi_flash.c" to communicate with an M25P64 SPI FLASH.

The first step consist in reading the SPI Flash ID. A comparison between the ID 
read from SPI flash and the expected one is done and LED1 is turned on in case
of success otherwise LED2 is turned on.  

Using this driver the program performs an erase of the sector to be accessed, a 
write of a Tx_Buffer, defined in the main.c file, to the memory followed by a read
of the written data. Then data read from the memory stored in the Rx_Buffer are
compared with the expected values of the Tx_Buffer. The result of this comparison
is stored in the "TransferStatus1" variable.

A second erase of the same sector is done at the end, and a test is done to be
sure that all the data written there are erased further to the sector erase. All
the data location are read and checked with 0xFF value. The result of this test
is stored in "TransferStatus2" variable which is FAILED in case of error.

The FLASH_WRITE_ADDRESS and the FLASH_READ_ADDRESS where the program start the
write and the read operations are defined in the main.c file. 


@par Directory contents 
  
  - SPI/SPI_FLASH/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - SPI/SPI_FLASH/stm32f4xx_conf.h     Library Configuration file
  - SPI/SPI_FLASH/stm32f4xx_it.c       Interrupt handlers
  - SPI/SPI_FLASH/stm32f4xx_it.h       Interrupt handlers header file
  - SPI/SPI_FLASH/main.c               Main program
  - SPI/SPI_FLASH/spi_flash.c          SPI M25Pxxx driver
  - SPI/SPI_FLASH/spi_flash.h          SPI M25Pxxx driver header file

     
@par Hardware and Software environment 

  - This example runs on STM32F405xx/407xx, STM32F415xx/417xx and STM32F42xxx/43xxx devices.
  
  - This example can't be tested with evaluation boards (no SPI FLASH available).
	  
  - HW platform Set-up
    - Connect LED1 and LED2 respectively to PG.06 and PG.08 pins
    - Connect both SPI2 and SPI FLASH pins as following:
      - Connect SPI2_NSS (PI0) pin to SPI Flash chip select (pin1)
      - Connect SPI2_SCK (PI1) pin to SPI Flash serial clock (pin6)
      - Connect SPI2_MISO(PI2) pin to SPI Flash serial data output (pin2)
      - Connect SPI2_MOSI(PI3) pin to SPI Flash serial data input (pin5)
      - Connect SPI Flash Write Protect (pin3) to Vdd
      - Connect SPI Flash Hold (pin7) to Vdd
      - Connect SPI Flash Vcc (pin8) to Vdd
      - Connect SPI Flash Vss (pin4) to Vss      


@par How to use it ? 

In order to make the program work, you must do the following:
 - Copy all source files from this example folder to the template folder under
   Project\STM32F4xx_StdPeriph_Templates
 - Open your preferred toolchain 
 - Add the file "spi_flash.c" to the project source list
 - Rebuild all files and load your image into target memory
 - Run the example
   
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
