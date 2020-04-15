/*
*********************************************************************************************************
*	                                  
*	模块名称 : 独立看门狗驱动
*	文件名称 : bsp_iwdg.c
*	版    本 : V1.0
*	说    明 : IWDG驱动程序
*	修改记录 :
*		版本号   日期        作者     说明
*		V1.0    2013-12-02  armfly   正式发布
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "bsp.h"
#include "stdio.h"
#include "stdlib.h"

__IO uint32_t LsiFreq = 0;
__IO uint32_t CaptureNumber = 0, PeriodValue = 0;
uint32_t GetLSIFrequency(void);
/*
*********************************************************************************************************
*	函 数 名: bsp_InitIwdg
*	功能说明: 独立看门狗时间配置函数
*	形    参：IWDGTime: 0 ---- 0x0FFF
*			  独立看门狗时间设置,单位为ms,IWDGTime = 1000 大约就是一秒的
*             时间，这里没有结合TIM5测得实际LSI频率，只是为了操作方便取了
*             一个估计值超过IWDGTime不进行喂狗的话系统将会复位。
*			  LSI = 34000左右
*	返 回 值: 无		        
*********************************************************************************************************
*/
void bsp_InitIwdg(uint32_t _ulIWDGTime)
{
		
	/* 检测系统是否从独立看门狗复位中恢复 */
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{		
		/* 清除复位标志 */
		RCC_ClearFlag();
	}
	else
	{
		/* 标志没有设置 */

	}
	
#if 1
	/* 通过TIM5输入捕获得到LSI频率 */
	LsiFreq = GetLSIFrequency();
#else
	/* 使能LSI */
	RCC_LSICmd(ENABLE);
	
	/* 等待直到LSI就绪 */
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{}
	
    /* */		
	LsiFreq = 32000;
#endif
	
	/* 写入0x5555表示允许访问IWDG_PR 和IWDG_RLR寄存器 */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	/*  LSI/32 分频*/
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	
	/*特别注意，由于这里_ulIWDGTime的最小单位是ms, 所以这里重装计数的
	  计数时 需要除以1000
	 Counter Reload Value = (_ulIWDGTime / 1000) /(1 / IWDG counter clock period)
	                      = (_ulIWDGTime / 1000) / (32/LSI)
	                      = (_ulIWDGTime / 1000) / (32/LsiFreq)
	                      = LsiFreq * _ulIWDGTime / 32000
	 实际测试LsiFreq = 34000，所以这里取1的时候 大概就是1ms 
	*/
	IWDG_SetReload(_ulIWDGTime);
	
	/* 重载IWDG计数 */
	IWDG_ReloadCounter();
	
	/* 使能 IWDG (LSI oscillator 由硬件使能) */
	IWDG_Enable();		
}

/*
*********************************************************************************************************
*	函 数 名: IWDG_Feed
*	功能说明: 喂狗函数
*	形    参：无
*	返 回 值: 无		        
*********************************************************************************************************
*/
void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}

/*
*********************************************************************************************************
*	函 数 名: GetLSIFrequency
*	功能说明: 配置TIM5的通道4测试LSI频率
*	形    参：无
*	返 回 值: LSI Frequency	 实际测试LSI = 34000左右,手册上面给的范围是	        
*********************************************************************************************************
*/
uint32_t GetLSIFrequency(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	RCC_ClocksTypeDef  RCC_ClockFreq;
	
	/* 使能外部低速晶振 */
	RCC_LSICmd(ENABLE);
	
	/* 等待直到LSI就绪 */
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{}
	
	/* TIM5 配置 */ 
	/* 使能TIM5时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	/* 将TIM5_CH4输入捕获通道连接到LSI时钟输出 */
	TIM_RemapConfig(TIM5, TIM5_LSI);
	
	/* 配置TIM5分频 */
	TIM_PrescalerConfig(TIM5, 0, TIM_PSCReloadMode_Immediate);
	
	/* 
	 TIM5 配置: 输入捕获模式 
	 1. LSI 振荡器连接到 TIM5 CH4
	 2. 上升沿做为激活的边沿
	 3. TIM5 CCR4用于计算频率值 
	 4. 这里主要分频设置TIM_ICPSC_DIV8，它的意思不是TIM的8分频
		它的意思是每8个时间捕获一次，这里就表示8个上升沿以后是
		一次捕获进入中断，然后再过8个上升沿捕获一次进入中断，这两
		次中断以后得到时间间隔，也就是8个周期，所以最后得到的结果
		是放大了8倍。
    */
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV8;
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInit(TIM5, &TIM_ICInitStructure);
	
	/* 使能TIM5中断通道 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* 使能TIM5计数 */
	TIM_Cmd(TIM5, ENABLE);
	
	/* 清除标志 */
	TIM5->SR = 0;
	
	/* 使能 CC4 中断请求 */  
	TIM_ITConfig(TIM5, TIM_IT_CC4, ENABLE);
	
	
	/* 等待直到 TIM5 得到 2 个LSI 边沿 */
	while(CaptureNumber != 2)
	{
		
	}
	
	/* 重定义TIM5到复位值 */
	TIM_DeInit(TIM5);
		
	/* 计算 LSI 频率, 根据TIM5输入时钟频率做计算(PCLK1)*/
	/* 得到 SYSCLK, HCLK 和 PCLKx  */
	RCC_GetClocksFreq(&RCC_ClockFreq);
	
	/*
	  HCLK = SYSCLK / 1     (AHB1Periph)     = 168MHz
      PCLK2 = HCLK / 2      (APB2Periph)      = 84MHz
      PCLK1 = HCLK / 4      (APB1Periph)      = 42MHz
    
      因为APB1 prescaler != 1, 所以 APB1上的TIMxCLK = PCLK1 x 2 = SystemCoreClock / 2;
      因为APB2 prescaler != 1, 所以 APB2上的TIMxCLK = PCLK2 x 2 = SystemCoreClock;

      APB1 定时器有 TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM6, TIM12, TIM13,TIM14
      APB2 定时器有 TIM1, TIM8 ,TIM9, TIM10, TIM11
	*/
	if ((RCC->CFGR & RCC_CFGR_PPRE1) == 0)
	{   
		/* 如果PCLK1 prescaler == 1 那么 TIMCLK = PCLK1 */
		return ((RCC_ClockFreq.PCLK1_Frequency / PeriodValue)*8);
		
	}
	else
	{   
		/* 
		  如果PCLK1 prescaler 不等于 1 那么 TIMCLK = 2 * PCLK1
		  这里设置乘以8是因为前面设置每8个事件产生一次捕获。
		*/
		return (((2 * RCC_ClockFreq.PCLK1_Frequency) / PeriodValue)*8) ;
		
	}
}




