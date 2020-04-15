/*
*********************************************************************************************************
*
*	ģ������ : TSC2046���败��оƬ��������
*	�ļ����� : bsp_tsc2046.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2014-2015, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_TSC2046_H
#define _BSP_TSC2046_H

/*
��1��������STM32-V5������ + 3.5����ʾģ�飬 ��ʾģ���ϵĴ���оƬΪ TSC2046�������оƬ
	PI10   --> TP_CS
	PD3/LCD_BUSY   --> TP_BUSY
	PB3/SPI3_SCK   --> TP_SCK
	PB4/SPI3_MISO   --> TP_MISO
	PB5/SPI3_MOSI   --> TP_MOSI

	PI3/TP_INT   --> TP_PEN_INT
*/

/* TSC2046 ����оƬ */
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
