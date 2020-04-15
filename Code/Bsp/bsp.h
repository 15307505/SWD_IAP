/*
*********************************************************************************************************
*
*	ģ������ : �ײ�����ģ��
*	�ļ����� : bsp.h
*	��    �� : V1.0
*	˵    �� : ���ǵײ�����ģ�����е�h�ļ��Ļ����ļ���
*	 	       Ӧ�ó���ֻ�� #include bsp.h ���ɣ�����Ҫ#include ÿ��ģ��� h �ļ�
*********************************************************************************************************
*/

#ifndef _BSP_H_
#define _BSP_H_

/* ���� BSP �汾�� */
#define __STM32F4_BSP_VERSION		"1.0"

/* CPU����ʱִ�еĺ��� */
//#define CPU_IDLE()		bsp_Idle()

/* ����ȫ���жϵĺ� */
#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */

/* ���������ڵ��Խ׶��Ŵ� */
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
	EXTI9_5_IRQHandler ���жϷ�������ɢ�ڼ��������� bsp�ļ��С�
	��Ҫ���ϵ� stm32f4xx_it.c �С�

	���������б�ʾEXTI9_5_IRQHandler��ں������зŵ� stm32f4xx_it.c��
*/
#define EXTI9_5_ISR_MOVE_OUT

/* ͨ��ȡ��ע�ͻ������ע�͵ķ�ʽ�����Ƿ�����ײ�����ģ�� */
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
#include "usb_bsp_msc.h"		/* usb�ײ����� */
#include "HexToBin.h"       /* Hex�ļ�ת����Bin�ļ� */
#include "IAP.h"       
#include "ff.h"			/* FatFS�ļ�ϵͳģ��*/
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

