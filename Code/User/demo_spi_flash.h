/*
*********************************************************************************************************
*
*	ģ������ : ����Flash��д��ʾģ�顣
*	�ļ����� : sfDemo.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2011-05-13 armfly  ST�̼���V3.5.0�汾��
*
*	Copyright (C), 2010-2011, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _SFDEMO_H
#define _SFDEMO_H

/* ���ⲿ���õĺ������� */
void DemoSpiFlash(void);
void sfDispMenu(void);
void sfReadTest(void);
void sfWriteTest(void);
void sfErase(void);
void sfViewData(uint32_t _uiAddr);
void sfWriteAll(uint8_t _ch);
void sfTestReadSpeed(void);
#endif


