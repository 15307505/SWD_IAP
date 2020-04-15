/**
  @page NVIC_IRQMask NVIC IRQ Channels Mask example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    NVIC/NVIC_IRQMask/readme.txt 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Description of the NVIC IRQ Channels Mask example.
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

This example shows how to use of the Nested Vectored Interrupt Controller 
(NVIC) IRQ Channels configuration and how to mask/activate different IRQs: 

- Configuration of 3 timers (TIM2..TIM4) to generate an interrupt on each counter 
  update event.

- The three timers are linked to their correspondent Update IRQ channel.

- Assignment of an ascendant IRQ priority for each IRQ channel :
  - TIM2 has a preemption priority of 0
  - TIM3 has a preemption priority of 1
  - TIM4 has a preemption priority of 2
     
- In each interrupt routine: 
  - TIM2 toggles a LED1 each 1s 
  - TIM3 toggles a LED2 each 2s 
  - TIM4 toggles a LED3 each 3s 

- The KEY/WAKEUP (Wakeup for STM32F429x/STM32F439x Devices) and TAMPER buttons 
  are used to boost the execution priority as follows:

  - The KEY/WAKEUP button is used in GPIO mode and at each KEY/WAKEUP button 
    press, the execution priority is raised to 0 and turn LED4 ON. This prevents 
    all exceptions with configurable priority from activating, other than through 
    the Hard Fault fault escalation mechanism. As consequence, all LEDs stop toggling
    as TIM2, TIM3 and TIM4 IRQs are prevented from activation. 
    
    Pressing again the KEY/WAKEUP button will release the priority boosting, turn 
    LED4 OFF and will allow all exceptions with configurable priority to be activated 
    and TIM2, TIM3 and TIM4 can be generated again and the LEDs restart toggling.
    
    This execution priority is made using the CMSIS functions "__disable_irq()" 
    and "__enable_irq()". 
    These two functions are managing the Cortex-M4 PRIMASK special register.

  - The TAMPER button is used in EXTI mode and at each TAMPER button press, the
    execution priority is masked to 0x40 using the BASEPRI register. 
    A non-zero value will act as a priority mask, affecting the execution priority 
    when the priority defined by BASEPRI is the same or higher than the current 
    executing priority. 
    As consequence, LED2 and LED3 stop toggling as TIM3 and TIM4 IRQs are 
    prevented from activation. 
    Pressing again the TAMPER button will configure the BASEPRI register to 0, 
    thus it has no effect on the current priority and TIM3 and TIM4 can be 
    generated again and LED2 and LED3 restart toggling.
    
    This execution priority is made using the CMSIS functions "__set_BASEPRI()". 
    This function is managing the Cortex-M4 BASEPRI special register.
    Setting the BASEPRI register has no effect when the execution priority is
    raised to 0 using the "__disable_irq()" function.

 @note These mechanisms only affect the group priority. They have no effect on 
       the sub-priority. The sub-priority is only used to sort pending exception 
       priorities, and does not affect active exceptions.

   
@par Directory contents 
  
  - NVIC/NVIC_IRQMask/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - NVIC/NVIC_IRQMask/stm32f4xx_conf.h     Library Configuration file
  - NVIC/NVIC_IRQMask/stm32f4xx_it.c       Interrupt handlers
  - NVIC/NVIC_IRQMask/stm32f4xx_it.h       Interrupt handlers header file
  - NVIC/NVIC_IRQMask/main.c               Main program
  - NVIC/NVIC_IRQMask/main.h               Main program header file
  
          
@par Hardware and Software environment

  - This example runs on STM32F405xx/407xx, STM32F415xx/417xx, STM32F427xx/437xx and 
    STM32F429xx/439xx devices.
    
  - This example has been tested with STMicroelectronics STM324xG-EVAL (STM32F40xx/
    STM32F41xx Devices), STM32437I-EVAL (STM32F427xx/STM32F437xx Devices) and 
    STM324x9I-EVAL RevB (STM32F429xx/STM32F439xx Devices) evaluation boards and 
    can be easily tailored to any other supported device and development board.

  - STM324xG-EVAL/STM32437I-EVAL Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PG.06, PG.08, PI.09
      and PC.07 pins
    - Use the KEY push-button connected to pin PG15 (EXTI Line15)
    - Use the TAMPER push-button connected to pin PC13 (EXTI Line13)

  - STM324x9I-EVAL RevB Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PG.06, PG.07, PG.10
      and PG.12 pins 
    - Use the WAKEUP push-button connected to pin PA.0 (EXTI Line0)  
    - Use the TAMPER/KEY push-button connected to PC.13 pin (EXTI Line13).
              
              
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
