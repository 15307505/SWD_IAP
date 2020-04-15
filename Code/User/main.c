#include "bsp.h"			/* 底层硬件驱动 */


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"SWD DownLoader"
#define EXAMPLE_DATE	"2020-03-15"
#define DEMO_VER		   "1.0"

/* 仅允许本文件内调用的函数声明 */
static void PrintfLogo(void);
extern u8 nonside[];

FONT_T tFont12, tFont16, tFont24, tFont32;          
u8 CRCTest[] = {0x68,0x00,0x06,0x68,0x15,0xff,0,0,0,0};   
u8 Maintask_Status = State_Init;

/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: c程序入口
*	形    参：无
*	返 回 值: 错误代码(无需处理)
*********************************************************************************************************
*/
int main(void)
{
	{
		/* 设置字体参数 */
		tFont16.FontCode = FC_ST_16;	/* 字体代码 16点阵 */
		tFont16.FrontColor = 1;		/* 字体颜色 0 或 1 */
		tFont16.BackColor = 0;		/* 文字背景颜色 0 或 1 */
		tFont16.Space = 0;			/* 文字间距，单位 = 像素 */

		tFont12.FontCode = FC_ST_12;	/* 字体代码 12点阵 */
		tFont12.FrontColor = 1;		/* 字体颜色 0 或 1 */
		tFont12.BackColor = 0;		/* 文字背景颜色 0 或 1 */
		tFont12.Space = 1;			/* 文字间距，单位 = 像素 */
		
		tFont24.FontCode = FC_ST_24;	/* 字体代码 24点阵 */
		tFont24.FrontColor = 1;		/* 字体颜色 0 或 1 */
		tFont24.BackColor = 0;		/* 文字背景颜色 0 或 1 */
		tFont24.Space = 1;			/* 文字间距，单位 = 像素 */
		
		tFont32.FontCode = FC_ST_32;	/* 字体代码 32点阵 */
		tFont32.FrontColor = 1;		/* 字体颜色 0 或 1 */
		tFont32.BackColor = 0;		/* 文字背景颜色 0 或 1 */
		tFont32.Space = 1;			/* 文字间距，单位 = 像素 */	
	}
	/*
		ST固件库中的启动文件已经执行了 SystemInit() 函数，该函数在 system_stm32f4xx.c 文件，主要功能是
	配置CPU系统的时钟，内部Flash访问时序，配置FSMC用于外部SRAM
	*/
	bsp_Init();		/* 硬件初始化 */
	PrintfLogo();	/* 打印例程信息到串口1 */
	
	OLED_ClrScr(0);

	OLED_DispStr(30,0,"SWD工具" ,&tFont12);
	bsp_DelayMS(1200);
  
 BEEP_Stop();
	
	ViewRootDir();		      /* 显示SD卡根目录下的文件名 */
	usbd_OpenMassStorage();	/* 初始化USB Device，配置为Mass Storage */
//	
	printf("【2 - 使能U盘】\r\n");
	
	/* 进入主程序循环体 */
	
	while (1)
	{
		bsp_Idle();		/* 这个函数在bsp.c文件。用户可以修改这个函数实现CPU休眠和喂狗 */
		
//		switch(Maintask_Status) //IAP在线升级的程序
//		{
//			case State_Init: 
//			        if( IsKeyDown1() == 1 ) 
//										 {
//											// Test_Hex2Bin();
//											 
//											 BEEP_KeyTone();
//											 OpenFile(); //打开文件系统
//											 Maintask_Status = State_IAP;
//										 }
//				             break;
//			case State_KEY:
//			               Maintask_Status = State_IAP;
//				             break;
//			case State_IAP:
//				             IAPData();
//				             break;
//			case State_Sleep:
//				        break;
//      default :
//                break;						
//		}

		switch(Maintask_Status)  //SWD下载的程序
		{
			case State_Init: 
			        if( IsKeyDown1() == 1 ) 
										 {
												bsp_DelayMS(100);
											 BEEP_KeyTone();
											 OpenFile(); //打开文件系统
											 Maintask_Status = State_IAP;
										 }
				       break;
			case State_KEY:
			               Maintask_Status = State_IAP;
				             break;
			case State_IAP:
																	bsp_DelayMS(100);
				             SWD_UpData(); //开始执行SWD下载
                 //test();     //读写DP AP寄存器测试
				             break;
			case State_Sleep:
				        break;
      default :
                break;						
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: PrintfLogo
*	功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的超级终端软件可以观察结果
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	/* 检测CPU ID */
	{
		/* 参考手册：
			32.6.1 MCU device ID code
			33.1 Unique device ID register (96 bits)
		*/
		uint32_t CPU_Sn0, CPU_Sn1, CPU_Sn2;

		CPU_Sn0 = *(__IO uint32_t*)(0x1FFF7A10);
		CPU_Sn1 = *(__IO uint32_t*)(0x1FFF7A10 + 4);
		CPU_Sn2 = *(__IO uint32_t*)(0x1FFF7A10 + 8);

		printf("\r\nCPU : STM32F401RCT6, LQFP64, UID = %08X %08X %08X\r\n"
			, CPU_Sn2, CPU_Sn1, CPU_Sn0);
	}

	printf("\n\r");
	printf("*************************************************************\r\n");
	printf("* 例程名称   : %s\r\n", EXAMPLE_NAME);	/* 打印例程名称 */
	printf("* 例程版本   : %s\r\n", DEMO_VER);		/* 打印例程版本 */
	//printf("* 发布日期   : %s\r\n", EXAMPLE_DATE);	/* 打印例程日期 */

	/* 打印ST固件库版本，这3个定义宏在stm32f10x.h文件中 */
	printf("* 固件库版本 : V%d.%d.%d (STM32F4xx_StdPeriph_Driver)\r\n", __STM32F4XX_STDPERIPH_VERSION_MAIN,
			__STM32F4XX_STDPERIPH_VERSION_SUB1,__STM32F4XX_STDPERIPH_VERSION_SUB2);
	printf("* \r\n");	/* 打印一行空格 */

	printf("* Copyright 	南京信息工程大学无锡研究生院 Code Fan\r\n");
	printf("*************************************************************\r\n");
}
