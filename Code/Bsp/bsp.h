/*
*********************************************************************************************************
*
*	模块名称 : 底层驱动模块
*	文件名称 : bsp.h
*	版    本 : V1.0
*	说    明 : 这是底层驱动模块所有的h文件的汇总文件。
*	 	       应用程序只需 #include bsp.h 即可，不需要#include 每个模块的 h 文件
*********************************************************************************************************
*/

#ifndef _BSP_H_
#define _BSP_H_

/* 定义 BSP 版本号 */
#define __STM32F4_BSP_VERSION		"1.0"

/* CPU空闲时执行的函数 */
//#define CPU_IDLE()		bsp_Idle()

/* 开关全局中断的宏 */
#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */

/* 这个宏仅用于调试阶段排错 */
//#define BSP_Printf		printf
//#define BSP_Printf(...)

//my printf
#define Debug_Printf		printf
//#define Debug_Printf(...)

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>  

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

enum _Main_Status
{
	State_Init   = 0,
	State_KEY    = 1,
	State_IAP    = 2,
	State_Sleep  = 3
};

/*
	EXTI9_5_IRQHandler 的中断服务程序分散在几个独立的 bsp文件中。
	需要整合到 stm32f4xx_it.c 中。

	定义下面行表示EXTI9_5_IRQHandler入口函数集中放到 stm32f4xx_it.c。
*/
#define EXTI9_5_ISR_MOVE_OUT

/* 通过取消注释或者添加注释的方式控制是否包含底层驱动模块 */
#include "bsp_uart_fifo.h"
#include "bsp_led.h"
#include "bsp_timer.h"
#include "bsp_key.h"
#include "bsp_user_lib.h"
#include "bsp_spi_bus.h"
#include "bsp_spi_flash.h"
//#include "bsp_cpu_flash.h"
#include "bsp_sdio_sd.h"
#include "bsp_oled.h"
#include "bsp_beep.h"
#include "bsp_tim_pwm.h"
#include "bsp_protocol.h"

#include "demo_sdio_fatfs.h"
#include "demo_spi_flash.h"
#include "usb_bsp_msc.h"		/* usb底层驱动 */
#include "HexToBin.h"       /* Hex文件转换成Bin文件 */
#include "IAP.h"       
#include "ff.h"			/* FatFS文件系统模块*/
#include "fonts.h"


#include "debug_cm.h"
#include "algo/flash_blob.h"
#include "bsp_swd.h"
#include "bsp_swd_flash.h"


void bsp_Idle(void);
void bsp_Init(void);
static void NVIC_Configuration(void);

extern	FONT_T tFont12, tFont16, tFont24, tFont32;
extern  unsigned char File_buf[1024];
#endif

