/*
*********************************************************************************************************
*
*	ģ������ : TIM������ʱ�жϺ�PWM����ģ��
*	�ļ����� : bsp_tim_pwm.c
*	��    �� : V1.1
*	˵    �� : ����STM32F4�ڲ�TIM���PWM�źţ� ��ʵ�ֻ����Ķ�ʱ�ж�
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2013-08-16 armfly  ��ʽ����
*		V1.1	2014-06-15 armfly  ���� bsp_SetTIMOutPWM����ռ�ձ�=0��100%ʱ���رն�ʱ����GPIO����Ϊ���
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"


/*
	���������GPIO��TIMͨ��:

	TIM1_CH1, PA8,	PE9,
	TIM1_CH2, PA9,	PE11
	TIM1_CH3, PA10,	PE13
	TIM1_CH4, PA11,	PE14

	TIM2_CH1, PA15 (����429��439) 407û�д˽�
	TIM2_CH2, PA1,	PB3
	TIM2_CH3, PA2,	PB10
	TIM2_CH4, PA3,	PB11

	TIM3_CH1, PA6,  PB4, PC6
	TIM3_CH2, PA7,	PB5, PC7
	TIM3_CH3, PB0,	PC8
	TIM3_CH4, PB1,	PC9

	TIM4_CH1, PB6,  PD12
	TIM4_CH2, PB7,	PD13
	TIM4_CH3, PB8,	PD14
	TIM4_CH4, PB9,	PD15

	TIM5_CH1, PA0,  PH10
	TIM5_CH2, PA1,	PH11
	TIM5_CH3, PA2,	PH12
	TIM5_CH4, PA3,	PI10

	TIM8_CH1, PC6,  PI5
	TIM8_CH2, PC7,	PI6
	TIM8_CH3, PC8,	PI7
	TIM8_CH4, PC9,	PI2

	TIM9_CH1, PA2,  PE5
	TIM9_CH2, PA3,	PE6

	TIM10_CH1, PB8,  PF6

	TIM11_CH1, PB9,  PF7

	TIM12_CH1, PB14,  PH6
	TIM12_CH2, PB15,  PH9

	TIM13_CH1, PA6,  PF8
	TIM14_CH1, PA7,  PF9

	APB1 ��ʱ���� TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13, TIM14
	APB2 ��ʱ���� TIM1, TIM8 ,TIM9, TIM10, TIM11
*/

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetRCCofGPIO
*	����˵��: ����GPIO �õ�RCC�Ĵ���
*	��    �Σ���
*	�� �� ֵ: GPIO����ʱ����
*********************************************************************************************************
*/
uint32_t bsp_GetRCCofGPIO(GPIO_TypeDef* GPIOx)
{
	uint32_t rcc = 0;

	if (GPIOx == GPIOA)
	{
		rcc = RCC_AHB1Periph_GPIOA;
	}
	else if (GPIOx == GPIOB)
	{
		rcc = RCC_AHB1Periph_GPIOB;
	}
	else if (GPIOx == GPIOC)
	{
		rcc = RCC_AHB1Periph_GPIOC;
	}
	else if (GPIOx == GPIOD)
	{
		rcc = RCC_AHB1Periph_GPIOD;
	}
	else if (GPIOx == GPIOE)
	{
		rcc = RCC_AHB1Periph_GPIOE;
	}
	else if (GPIOx == GPIOF)
	{
		rcc = RCC_AHB1Periph_GPIOF;
	}
	else if (GPIOx == GPIOG)
	{
		rcc = RCC_AHB1Periph_GPIOG;
	}
	else if (GPIOx == GPIOH)
	{
		rcc = RCC_AHB1Periph_GPIOH;
	}
	else if (GPIOx == GPIOI)
	{
		rcc = RCC_AHB1Periph_GPIOI;
	}

	return rcc;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetPinSource
*	����˵��: ���� GPIO_Pin_X �õ� GPIO_PinSource
*	��    �Σ�gpio_pin
*	�� �� ֵ: GPIO_PinSource
*********************************************************************************************************
*/
uint16_t bsp_GetPinSource(uint16_t gpio_pin)
{
	uint16_t ret = 0;

	if (gpio_pin == GPIO_Pin_0)
	{
		ret = GPIO_PinSource0;
	}
	else if (gpio_pin == GPIO_Pin_1)
	{
		ret = GPIO_PinSource1;
	}
	else if (gpio_pin == GPIO_Pin_2)
	{
		ret = GPIO_PinSource2;
	}
	else if (gpio_pin == GPIO_Pin_3)
	{
		ret = GPIO_PinSource3;
	}
	else if (gpio_pin == GPIO_Pin_4)
	{
		ret = GPIO_PinSource4;
	}
	else if (gpio_pin == GPIO_Pin_5)
	{
		ret = GPIO_PinSource5;
	}
	else if (gpio_pin == GPIO_Pin_6)
	{
		ret = GPIO_PinSource6;
	}
	else if (gpio_pin == GPIO_Pin_7)
	{
		ret = GPIO_PinSource7;
	}
	else if (gpio_pin == GPIO_Pin_8)
	{
		ret = GPIO_PinSource8;
	}
	else if (gpio_pin == GPIO_Pin_9)
	{
		ret = GPIO_PinSource9;
	}
	else if (gpio_pin == GPIO_Pin_10)
	{
		ret = GPIO_PinSource10;
	}
	else if (gpio_pin == GPIO_Pin_11)
	{
		ret = GPIO_PinSource11;
	}
	else if (gpio_pin == GPIO_Pin_12)
	{
		ret = GPIO_PinSource12;
	}
	else if (gpio_pin == GPIO_Pin_13)
	{
		ret = GPIO_PinSource13;
	}
	else if (gpio_pin == GPIO_Pin_14)
	{
		ret = GPIO_PinSource14;
	}
	else if (gpio_pin == GPIO_Pin_15)
	{
		ret = GPIO_PinSource15;
	}

	return ret;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetRCCofTIM
*	����˵��: ����TIM �õ�RCC�Ĵ���
*	��    �Σ���
*	�� �� ֵ: TIM����ʱ����
*********************************************************************************************************
*/
uint32_t bsp_GetRCCofTIM(TIM_TypeDef* TIMx)
{
	uint32_t rcc = 0;

	/*
		APB1 ��ʱ���� TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13, TIM14
		APB2 ��ʱ���� TIM1, TIM8 ,TIM9, TIM10, TIM11
	*/
	if (TIMx == TIM1)
	{
		rcc = RCC_APB2Periph_TIM1;
	}
	else if (TIMx == TIM8)
	{
		rcc = RCC_APB2Periph_TIM8;
	}
	else if (TIMx == TIM9)
	{
		rcc = RCC_APB2Periph_TIM9;
	}
	else if (TIMx == TIM10)
	{
		rcc = RCC_APB2Periph_TIM10;
	}
	else if (TIMx == TIM11)
	{
		rcc = RCC_APB2Periph_TIM11;
	}
	/* ������ APB1ʱ�� */
	else if (TIMx == TIM2)
	{
		rcc = RCC_APB1Periph_TIM2;
	}
	else if (TIMx == TIM3)
	{
		rcc = RCC_APB1Periph_TIM3;
	}
	else if (TIMx == TIM4)
	{
		rcc = RCC_APB1Periph_TIM4;
	}
	else if (TIMx == TIM5)
	{
		rcc = RCC_APB1Periph_TIM5;
	}
	else if (TIMx == TIM6)
	{
		rcc = RCC_APB1Periph_TIM6;
	}
	else if (TIMx == TIM7)
	{
		rcc = RCC_APB1Periph_TIM7;
	}
	else if (TIMx == TIM12)
	{
		rcc = RCC_APB1Periph_TIM12;
	}
	else if (TIMx == TIM13)
	{
		rcc = RCC_APB1Periph_TIM13;
	}
	else if (TIMx == TIM14)
	{
		rcc = RCC_APB1Periph_TIM14;
	}

	return rcc;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetAFofTIM
*	����˵��: ����TIM �õ�AF�Ĵ�������
*	��    ��: ��
*	�� �� ֵ: AF�Ĵ�������
*********************************************************************************************************
*/
uint8_t bsp_GetAFofTIM(TIM_TypeDef* TIMx)
{
	uint8_t ret = 0;

	/*
		APB1 ��ʱ���� TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13, TIM14
		APB2 ��ʱ���� TIM1, TIM8 ,TIM9, TIM10, TIM11
	*/
	if (TIMx == TIM1)
	{
		ret = GPIO_AF_TIM1;
	}
	else if (TIMx == TIM8)
	{
		ret = GPIO_AF_TIM8;
	}
	else if (TIMx == TIM9)
	{
		ret = GPIO_AF_TIM9;
	}
	else if (TIMx == TIM10)
	{
		ret = GPIO_AF_TIM10;
	}
	else if (TIMx == TIM11)
	{
		ret = GPIO_AF_TIM11;
	}
	/* ������ APB1ʱ�� */
	else if (TIMx == TIM2)
	{
		ret = GPIO_AF_TIM2;
	}
	else if (TIMx == TIM3)
	{
		ret = GPIO_AF_TIM3;
	}
	else if (TIMx == TIM4)
	{
		ret = GPIO_AF_TIM4;
	}
	else if (TIMx == TIM5)
	{
		ret = GPIO_AF_TIM5;
	}
	/* û�� TIM6 TIM7 */
	else if (TIMx == TIM8)
	{
		ret = GPIO_AF_TIM8;
	}
	else if (TIMx == TIM9)
	{
		ret = GPIO_AF_TIM9;
	}
	else if (TIMx == TIM12)
	{
		ret = GPIO_AF_TIM12;
	}
	else if (TIMx == TIM13)
	{
		ret = GPIO_AF_TIM13;
	}
	else if (TIMx == TIM14)
	{
		ret = GPIO_AF_TIM14;
	}

	return ret;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_ConfigTimGpio
*	����˵��: ����GPIO��TIMʱ�ӣ� GPIO���ӵ�TIM���ͨ��
*	��    ��: GPIOx
*			 GPIO_PinX
*			 TIMx
*			 _ucChannel
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_ConfigTimGpio(GPIO_TypeDef* GPIOx, uint16_t GPIO_PinX, TIM_TypeDef* TIMx, uint8_t _ucChannel)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* ʹ��GPIOʱ�� */
	RCC_AHB1PeriphClockCmd(bsp_GetRCCofGPIO(GPIOx), ENABLE);

  	/* ʹ��TIMʱ�� */
	if ((TIMx == TIM1) || (TIMx == TIM8) || (TIMx == TIM9) || (TIMx == TIM10) || (TIMx == TIM11))
	{
		RCC_APB2PeriphClockCmd(bsp_GetRCCofTIM(TIMx), ENABLE);
	}
	else
	{
		RCC_APB1PeriphClockCmd(bsp_GetRCCofTIM(TIMx), ENABLE);
	}

	/* ����GPIO */
	GPIO_InitStructure.GPIO_Pin = GPIO_PinX;	/* ������β� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOx, &GPIO_InitStructure);

	/* ���ӵ�AF���� */
	GPIO_PinAFConfig(GPIOx, bsp_GetPinSource(GPIO_PinX), bsp_GetAFofTIM(TIMx));
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_ConfigGpioOut
*	����˵��: ����GPIOΪ�����������Ҫ����PWM�����ռ�ձ�Ϊ0��100�������
*	��    ��: GPIOx
*			  GPIO_PinX
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_ConfigGpioOut(GPIO_TypeDef* GPIOx, uint16_t GPIO_PinX)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* ʹ��GPIOʱ�� */
	RCC_AHB1PeriphClockCmd(bsp_GetRCCofGPIO(GPIOx), ENABLE);

	/* ����GPIO */
	GPIO_InitStructure.GPIO_Pin = GPIO_PinX;		/* ������β� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	/* ��� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	/* ���� */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;	/* ������ */
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SetTIMOutPWM
*	����˵��: �������������PWM�źŵ�Ƶ�ʺ�ռ�ձ�.  ��Ƶ��Ϊ0������ռ��Ϊ0ʱ���رն�ʱ����GPIO���0��
*			  ��Ƶ��Ϊ0��ռ�ձ�Ϊ100%ʱ��GPIO���1.
*	��    ��: _ulFreq : PWM�ź�Ƶ�ʣ���λHz  (ʵ�ʲ��ԣ�������Ƶ��Ϊ 168M / 4 = 42M��. 0 ��ʾ��ֹ���
*			  _ulDutyCycle : PWM�ź�ռ�ձȣ���λ�����֮һ����5000����ʾ50.00%��ռ�ձ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_SetTIMOutPWM(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, TIM_TypeDef* TIMx, uint8_t _ucChannel,
	 uint32_t _ulFreq, uint32_t _ulDutyCycle)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	uint16_t usPeriod;
	uint16_t usPrescaler;
	uint32_t uiTIMxCLK;

	if (_ulDutyCycle == 0)
	{		
		TIM_Cmd(TIMx, DISABLE);		/* �ر�PWM��� */
		bsp_ConfigGpioOut(GPIOx, GPIO_Pin);	/* ����GPIOΪ������� */		
		GPIO_WriteBit(GPIOx, GPIO_Pin, Bit_RESET);	/* PWM = 0 */		
		return;
	}
	else if (_ulDutyCycle == 10000)
	{
		TIM_Cmd(TIMx, DISABLE);		/* �ر�PWM��� */

		bsp_ConfigGpioOut(GPIOx, GPIO_Pin);	/* ����GPIOΪ������� */		
		GPIO_WriteBit(GPIOx, GPIO_Pin, Bit_SET);	/* PWM = 1 */	
		return;
	}
	

	bsp_ConfigTimGpio(GPIOx, GPIO_Pin, TIMx, _ucChannel);	/* ʹ��GPIO��TIMʱ�ӣ�������TIMͨ����GPIO */
	
    /*-----------------------------------------------------------------------
		system_stm32f4xx.c �ļ��� void SetSysClock(void) ������ʱ�ӵ��������£�

		HCLK = SYSCLK / 1     (AHB1Periph)
		PCLK2 = HCLK / 2      (APB2Periph)
		PCLK1 = HCLK / 4      (APB1Periph)

		��ΪAPB1 prescaler != 1, ���� APB1�ϵ�TIMxCLK = PCLK1 x 2 = SystemCoreClock / 2;
		��ΪAPB2 prescaler != 1, ���� APB2�ϵ�TIMxCLK = PCLK2 x 2 = SystemCoreClock;

		APB1 ��ʱ���� TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM6, TIM12, TIM13,TIM14
		APB2 ��ʱ���� TIM1, TIM8 ,TIM9, TIM10, TIM11

	----------------------------------------------------------------------- */
	if ((TIMx == TIM1) || (TIMx == TIM8) || (TIMx == TIM9) || (TIMx == TIM10) || (TIMx == TIM11))
	{
		/* APB2 ��ʱ�� */
		uiTIMxCLK = SystemCoreClock;
	}
	else	/* APB1 ��ʱ�� */
	{
		uiTIMxCLK = SystemCoreClock / 2;
	}

	if (_ulFreq < 100)
	{
		usPrescaler = 10000 - 1;					/* ��Ƶ�� = 10000 */
		usPeriod =  (uiTIMxCLK / 10000) / _ulFreq  - 1;		/* �Զ���װ��ֵ */
	}
	else if (_ulFreq < 3000)
	{
		usPrescaler = 100 - 1;					/* ��Ƶ�� = 100 */
		usPeriod =  (uiTIMxCLK / 100) / _ulFreq  - 1;		/* �Զ���װ��ֵ */
	}
	else	/* ����4K��Ƶ�ʣ������Ƶ */
	{
		usPrescaler = 0;					/* ��Ƶ�� = 1 */
		usPeriod = uiTIMxCLK / _ulFreq - 1;	/* �Զ���װ��ֵ */
	}

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = usPeriod;
	TIM_TimeBaseStructure.TIM_Prescaler = usPrescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = (_ulDutyCycle * usPeriod) / 10000;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	if (_ucChannel == 1)
	{
		TIM_OC1Init(TIMx, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
	}
	else if (_ucChannel == 2)
	{
		TIM_OC2Init(TIMx, &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);
	}
	else if (_ucChannel == 3)
	{
		TIM_OC3Init(TIMx, &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);
	}
	else if (_ucChannel == 4)
	{
		TIM_OC4Init(TIMx, &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);
	}

	TIM_ARRPreloadConfig(TIMx, ENABLE);

	/* TIMx enable counter */
	TIM_Cmd(TIMx, ENABLE);

	/* ������仰����TIM1��TIM8�Ǳ���ģ�����TIM2-TIM6�򲻱�Ҫ */
	if ((TIMx == TIM1) || (TIMx == TIM8))
	{
		TIM_CtrlPWMOutputs(TIM1, ENABLE);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SetTIMforInt
*	����˵��: ����TIM��NVIC�����ڼ򵥵Ķ�ʱ�ж�. ������ʱ�жϡ� �жϷ��������Ӧ�ó���ʵ�֡�
*	��    ��: TIMx : ��ʱ��
*			  _ulFreq : ��ʱƵ�� ��Hz���� 0 ��ʾ�رա�
*			  _PreemptionPriority : �ж����ȼ�����
*			  _SubPriority : �����ȼ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_SetTIMforInt(TIM_TypeDef* TIMx, uint32_t _ulFreq, uint8_t _PreemptionPriority, uint8_t _SubPriority)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	uint16_t usPeriod;
	uint16_t usPrescaler;
	uint32_t uiTIMxCLK;

  	/* ʹ��TIMʱ�� */
	if ((TIMx == TIM1) || (TIMx == TIM8) || (TIMx == TIM9) || (TIMx == TIM10) || (TIMx == TIM11))
	{
		RCC_APB2PeriphClockCmd(bsp_GetRCCofTIM(TIMx), ENABLE);
	}
	else
	{
		RCC_APB1PeriphClockCmd(bsp_GetRCCofTIM(TIMx), ENABLE);
	}

	if (_ulFreq == 0)
	{
		TIM_Cmd(TIMx, DISABLE);		/* �رն�ʱ��� */
		return;
	}

    /*-----------------------------------------------------------------------
		system_stm32f4xx.c �ļ��� void SetSysClock(void) ������ʱ�ӵ��������£�

		HCLK = SYSCLK / 1     (AHB1Periph)
		PCLK2 = HCLK / 2      (APB2Periph)
		PCLK1 = HCLK / 4      (APB1Periph)

		��ΪAPB1 prescaler != 1, ���� APB1�ϵ�TIMxCLK = PCLK1 x 2 = SystemCoreClock / 2;
		��ΪAPB2 prescaler != 1, ���� APB2�ϵ�TIMxCLK = PCLK2 x 2 = SystemCoreClock;

		APB1 ��ʱ���� TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13,TIM14
		APB2 ��ʱ���� TIM1, TIM8 ,TIM9, TIM10, TIM11

	----------------------------------------------------------------------- */
	if ((TIMx == TIM1) || (TIMx == TIM8) || (TIMx == TIM9) || (TIMx == TIM10) || (TIMx == TIM11))
	{
		/* APB2 ��ʱ�� */
		uiTIMxCLK = SystemCoreClock;
	}
	else	/* APB1 ��ʱ�� */
	{
		uiTIMxCLK = SystemCoreClock / 2;
	}

	if (_ulFreq < 100)
	{
		usPrescaler = 10000 - 1;					/* ��Ƶ�� = 1000 */
		usPeriod =  (uiTIMxCLK / 10000) / _ulFreq  - 1;		/* �Զ���װ��ֵ */
	}
	else if (_ulFreq < 3000)
	{
		usPrescaler = 100 - 1;					/* ��Ƶ�� = 100 */
		usPeriod =  (uiTIMxCLK / 100) / _ulFreq  - 1;		/* �Զ���װ��ֵ */
	}
	else	/* ����4K��Ƶ�ʣ������Ƶ */
	{
		usPrescaler = 0;					/* ��Ƶ�� = 1 */
		usPeriod = uiTIMxCLK / _ulFreq - 1;	/* �Զ���װ��ֵ */
	}

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = usPeriod;
	TIM_TimeBaseStructure.TIM_Prescaler = usPrescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);

	TIM_ARRPreloadConfig(TIMx, ENABLE);

	/* TIM Interrupts enable */
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);

	/* TIMx enable counter */
	TIM_Cmd(TIMx, ENABLE);

	/* ����TIM��ʱ�����ж� (Update) */
	{
		NVIC_InitTypeDef NVIC_InitStructure;	/* �жϽṹ���� misc.h �ж��� */
		uint8_t irq = 0;	/* �жϺ�, ������ stm32f4xx.h */

		if ((TIMx == TIM1) || (TIMx == TIM10))
			irq = TIM1_UP_TIM10_IRQn;
		else if (TIMx == TIM2)
			irq = TIM2_IRQn;
		else if (TIMx == TIM3)
			irq = TIM3_IRQn;
		else if (TIMx == TIM4)
			irq = TIM4_IRQn;
		else if (TIMx == TIM5)
			irq = TIM5_IRQn;
		else if (TIMx == TIM9)
			irq = TIM1_BRK_TIM9_IRQn;
		else if (TIMx == TIM11)
			irq = TIM1_TRG_COM_TIM11_IRQn;

		NVIC_InitStructure.NVIC_IRQChannel = irq;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = _PreemptionPriority;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = _SubPriority;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
