/*
*********************************************************************************************************
*
*	模块名称 : TSC2046电阻触摸芯片驱动程序
*	文件名称 : bsp_tsc2046.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2014-2015, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_TSC2046_H
#define _BSP_TSC2046_H

/*
【1】安富莱STM32-V5开发板 + 3.5寸显示模块， 显示模块上的触摸芯片为 TSC2046或其兼容芯片
	PI10   --> TP_CS
	PD3/LCD_BUSY   --> TP_BUSY
	PB3/SPI3_SCK   --> TP_SCK
	PB4/SPI3_MISO   --> TP_MISO
	PB5/SPI3_MOSI   --> TP_MOSI

	PI3/TP_INT   --> TP_PEN_INT
*/

/* TSC2046 触摸芯片 */
#define TSC2046_RCC_CS 		RCC_AHB1Periph_GPIOI
#define TSC2046_PORT_CS		GPIOI
#define TSC2046_PIN_CS		GPIO_Pin_10
#define TSC2046_CS_1()		TSC2046_PORT_CS->BSRRL = TSC2046_PIN_CS
#define TSC2046_CS_0()		TSC2046_PORT_CS->BSRRH = TSC2046_PIN_CS

#define TSC2046_RCC_BUSY 	RCC_AHB1Periph_GPIOD
#define TSC2046_PORT_BUSY	GPIOD
#define TSC2046_PIN_BUSY	GPIO_Pin_3

#define TSC2046_RCC_INT 	RCC_AHB1Periph_GPIOI
#define TSC2046_PORT_INT	GPIOI
#define TSC2046_PIN_INT		GPIO_Pin_3

void TSC2046_InitHard(void);
uint16_t TSC2046_ReadAdc(uint8_t _ucCh);
uint8_t TSC2046_PenInt(void);

#endif
