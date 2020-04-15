/*
*********************************************************************************************************
*
*	模块名称 : TFT液晶显示器驱动模块
*	文件名称 : OLED_tft_OLED.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*********************************************************************************************************
*/
#ifndef _BSP_OLED_H
#define _BSP_OLED_H

#include "bsp.h"

//汉字大小，英文数字大小
#define 	TYPE8X16		1
#define 	TYPE16X16		2
#define 	TYPE6X8			3

//-----------------OLED端口定义----------------  					   
#define RCC_OLED_PORT  (RCC_AHB1Periph_GPIOB)		/* GPIO端口时钟 */

#define OLED_CS_PORT	  GPIOB
#define OLED_CS_PIN		  GPIO_Pin_0

#define OLED_SCK_PORT	  GPIOB
#define OLED_SCK_PIN	  GPIO_Pin_13

#define OLED_SDIN_PORT	GPIOB
#define OLED_SDIN_PIN	  GPIO_Pin_15

#define OLED_DC_PORT	  GPIOB
#define OLED_DC_PIN	    GPIO_Pin_1

#define OLED_RST_PORT	  GPIOB
#define OLED_RST_PIN	  GPIO_Pin_2

/* 定义IO = 1和 0的代码 （不用更改） */
#define OLED_CS_1()		OLED_CS_PORT->BSRRL = OLED_CS_PIN
#define OLED_CS_0()		OLED_CS_PORT->BSRRH = OLED_CS_PIN

#define OLED_SCL_SET()		OLED_SCK_PORT->BSRRL = OLED_SCK_PIN
#define OLED_SCL_CLR()		OLED_SCK_PORT->BSRRH = OLED_SCK_PIN

#define OLED_SDA_SET()	OLED_SDIN_PORT->BSRRL = OLED_SDIN_PIN
#define OLED_SDA_CLR()	OLED_SDIN_PORT->BSRRH = OLED_SDIN_PIN

#define OLED_DC_SET()	OLED_SDIN_PORT->BSRRL = OLED_DC_PIN
#define OLED_DC_CLR()	OLED_SDIN_PORT->BSRRH = OLED_DC_PIN

#define OLED_RST_SET()	OLED_SDIN_PORT->BSRRL = OLED_RST_PIN
#define OLED_RST_CLR()	OLED_SDIN_PORT->BSRRH = OLED_RST_PIN

//CS管脚请接地

#if 1
	#include "bsp_tft_lcd.h"	/* 仅使用了其中的 FONT_T 结构体 */
#else
	typedef struct
	{
		uint16_t FontCode;	/* 字体代码 0 表示16点阵 */
		uint16_t FrontColor;/* 字体颜色 */
		uint16_t BackColor;	/* 文字背景颜色，透明 */
		uint16_t Space;		/* 文字间距，单位 = 像素 */
	}FONT_T;
#endif

/* 可供外部模块调用的函数 */
void OLED_InitHard(void);
void OLED_DispOn(void);
void OLED_DispOff(void);
void OLED_SetDir(uint8_t _ucDir);
void OLED_SetContrast(uint8_t ucValue);
void OLED_StartDraw(void);
void OLED_EndDraw(void);
void OLED_ClrScr(uint8_t _ucMode);
void OLED_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont);
void OLED_PutPixel(uint16_t _usX, uint16_t _usY, uint8_t _ucColor);
uint8_t OLED_GetPixel(uint16_t _usX, uint16_t _usY);
void OLED_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint8_t _ucColor);
void OLED_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint8_t _ucColor);
void OLED_DrawRect(uint16_t _usX, uint16_t _usY, uint8_t _usHeight, uint16_t _usWidth, uint8_t _ucColor);
void OLED_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint8_t _ucColor);
void OLED_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint8_t *_ptr);
void OLED_ClrOneScr(uint8_t x1, uint8_t y1,uint8_t x2, uint8_t y2,uint8_t _ucMode);
/*******************************************************************************************************************/


void OLED_Init(void);
void OLED_CLS(void);
void OLED_CLS_y(char y);
void OLED_CLS_line_area(u8 start_x,u8 start_y,u8 width);
void OLED_P6x8Str(u8 x,u8 y,u8 *ch,const u8 *F6x8);
void OLED_P8x16Str(u8 x,u8 y,u8 *ch,const u8 *F8x16);
void OLED_P14x16Str(u8 x,u8 y,u8 ch[],const u8 *F14x16_Idx,const u8 *F14x16);
void OLED_P16x16Str(u8 x,u8 y,u8 *ch,const u8 *F16x16_Idx,const u8 *F16x16);
//extern void OLED_Print(u8 x, u8 y, u8 *ch);
//void OLED_PutPixel(u8 x,u8 y);
void OLED_Print(u8 x, u8 y, u8 *ch,u8 char_size, u8 ascii_size);
void OLED_Rectangle(u8 x1,u8 y1,u8 x2,u8 y2,u8 gif);
void Draw_BMP(u8 x,u8 y,const u8 *bmp); 
void OLED_Fill(u8 dat);
void OLED_Set_Pos(u8 x, u8 y);
#endif


