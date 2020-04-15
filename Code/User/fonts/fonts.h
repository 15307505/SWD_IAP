/*										
*********************************************************************************************************
*	                                  
*	ģ������ : �ֿ�ģ��
*	�ļ����� : fonts.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*********************************************************************************************************
*/

#ifndef __FONTS_H_
#define __FONTS_H_

#define USE_SMALL_FONT			/* ������б�ʾʹ��С�ֿ⣬ �����ֻ��bsp_tft+lcd.c��ʹ�� */

///* 0x64000000 ��NOR Flash���׵�ַ */
//#define HZK12_ADDR (0x64000000 + 0x0)		/* 12�����ֿ��ַ */
//#define HZK16_ADDR (0x64000000 + 0x2C9D0)	/* 16�����ֿ��ַ */
//#define HZK24_ADDR (0x64000000 + 0x68190)	/* 24�����ֿ��ַ */
//#define HZK32_ADDR (0x64000000 + 0xEDF00)	/* 32�����ֿ��ַ */

extern unsigned char const g_Ascii12[];
extern unsigned char const g_Ascii16[];
extern unsigned char const g_Ascii24[];
extern unsigned char const g_Ascii32[];

extern unsigned char const g_Hz12[];
extern unsigned char const g_Hz16[];
extern unsigned char const g_Hz24[];
extern unsigned char const g_Hz32[];

extern unsigned char const g_RA8875_Ascii12_width[];
extern unsigned char const g_RA8875_Ascii16_width[];
extern unsigned char const g_RA8875_Ascii24_width[];
extern unsigned char const g_RA8875_Ascii32_width[];

#endif

