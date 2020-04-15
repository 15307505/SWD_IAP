#ifndef __SWD_PIN_H__
#define __SWD_PIN_H__

#include <stdint.h>
#include "gpio.h"


#define SWCLK_PORT             GPIOB
#define SWCLK_GPIO             GPIOB
#define SWCLK_PIN              0
#define SWDIO_PORT             GPIOB
#define SWDIO_GPIO             GPIOB
#define SWDIO_PIN              1
#define RESET_PORT             GPIOA
#define RESET_GPIO             GPIOA
#define RESET_PIN              4




#define SWDIO_SET_OUTPUT()     SWDIO_GPIO ->PDDR |=  (1 << SWDIO_PIN)  //DDIR(1)
#define SWDIO_SET_INPUT()      SWDIO_GPIO ->PDDR &= ~(1 << SWDIO_PIN);\
                               SWDIO_GPIO ->PIDR  &= ~(1 << SWDIO_PIN)



#define SWCLK_SET_OUTPUT()     SWDIO_GPIO ->PDDR |=  (1 << SWCLK_PIN)  //DDIR(1)
#define RESET_SET_OUTPUT()     SWDIO_GPIO ->PDDR |=  (1 << RESET_PIN)  //DDIR(1)

#define SWCLK_SET()            SWCLK_GPIO ->PDOR |=  (1 << SWCLK_PIN)  //TCK(1)
#define SWCLK_CLR()            SWCLK_GPIO ->PDOR &= ~(1 << SWCLK_PIN)  //TCK(0)
#define RESET_SET()            RESET_GPIO ->PDOR |=  (1 << RESET_PIN)  //TRST(1)
#define RESET_CLR()            RESET_GPIO ->PDOR &= ~(1 << RESET_PIN)  //TRST(0) 
#define SWDIO_SET()            SWDIO_GPIO ->PDOR |=  (1 << SWDIO_PIN)   //SWDIO_OUT(1)
#define SWDIO_CLR()            SWDIO_GPIO ->PDOR &= ~(1 << SWDIO_PIN)  //SWDIO_OUT(0)



#define SWDIO_IN()    (((SWDIO_GPIO ->PDIR) >> SWDIO_PIN) & 0x01U)//BITBAND_REG(PTB->PDIR, n)
#define SWDIO_OUT(n)    {if ( n ) SWDIO_SET(); else SWDIO_CLR();}


/* pin interface */
void SW_PinInit(void);
uint32_t SW_READ_BIT(void);
void SW_CLOCK_CYCLE(void);
void SW_WRITE_BIT(uint32_t bit);

void PIN_DELAY(void);
void DELAY_US(uint32_t us);

#endif


