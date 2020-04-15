#include <stdint.h>
#include <stdbool.h>

//#include "common.h"
#include "swd_pin.h"


/*
#define PAout(n)   BITBAND_REG(PTA->PDOR, n)
#define PAin(n)    BITBAND_REG(PTA->PDIR, n)
*/



void SW_PinInit(void)
{
    /* release trst pin */
    SWDIO_SET_OUTPUT();//设置PB3输出SWDIO_SET_OUTPUT

    SWCLK_SET_OUTPUT();/////
    RESET_SET_OUTPUT();////
    RESET_CLR();/////

    SWDIO_SET();//(swdio)PB3输出1
    SWCLK_CLR();//(swclk)PB2输出0
    RESET_SET();//(Trst)PB10输出1
}




void DELAY_US(uint32_t us)
{
    volatile uint32_t i, j;

    for(i = 0; i < us; i++)
    {
        for(j = 0; j < 30U; j++)
        {
            __NOP();
        }
    }

}

void PIN_DELAY(void)
{
    /*
      int32_t count;
      count = 15;
      while (--count);
     */
}

uint32_t SW_READ_BIT(void)
{
    uint32_t bit;
    SWCLK_CLR();
    ///PIN_DELAY();
    bit = SWDIO_IN();
    SWCLK_SET();
    ///PIN_DELAY();
    return bit;
}

void SW_CLOCK_CYCLE(void)
{
    SWCLK_CLR();
    ///PIN_DELAY();//此处以后增加频率关系
    SWCLK_SET();
    ///PIN_DELAY();
}

void SW_WRITE_BIT(uint32_t bit)
{
    SWDIO_OUT(bit);
    SW_CLOCK_CYCLE();
}

