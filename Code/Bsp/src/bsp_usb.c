/*
*********************************************************************************************************
*
*	ģ������ : USB�ײ�GPIO���ж�����
*	�ļ����� : bsp_usb.c
*	��    �� : V1.0
*	˵    �� : �ṩUSB�ײ�ӿ�. ���� usb_bsp.c ��V2.1.0�� 19-March-2012���޸ġ�
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2013-02-01 armfly  ��ʽ����
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

/*
	usbd_usr.h �ļ��ӿ����Ƴ����ŵ��û�����
	Libraries\STM32_USB_Device_Library\Core\inc\usbd_usr.h
*/

#include "bsp.h"
#include "bsp_usb.h"

extern USB_OTG_CORE_HANDLE  USB_OTG_dev;
extern USB_OTG_CORE_HANDLE  USB_OTG_Core;

/**
  * @brief  USB_OTG_BSP_Init
  *         Initilizes BSP configurations
  * @param  None
  * @retval None
  */
void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��ص�GPIOȫ���л�������ģʽ */
	{
		RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}

	/* USB Host */
	if (pdev == &USB_OTG_Core)
	{
		/* ������STM32-V5������ֻʹ��2���� PB14/OTG_HS_DM �� PB15/OTG_HS_DP ��U�� */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;

		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_OTG2_FS) ;
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_OTG2_FS) ;

		RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_OTG_HS, ENABLE) ;
	}

	/* USB Device */
	if (pdev == &USB_OTG_dev)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;

		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_OTG1_FS);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_OTG1_FS);

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
		RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS, ENABLE);
	}
}

/* �ر����к�USB��ص��ж�  */
void USB_OTG_BSP_DisableInterrupt(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* �ȹر����к�USB��ص��ж� */

	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;

	NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = OTG_HS_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = OTG_HS_EP1_OUT_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = OTG_HS_EP1_IN_IRQn;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  USB_OTG_BSP_EnableInterrupt
  *         Configures USB Global interrupt
  * @param  None
  * @retval None
  */
void USB_OTG_BSP_EnableInterrupt(USB_OTG_CORE_HANDLE *pdev)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* �ȹر����к�USB��ص��ж� */
	USB_OTG_BSP_DisableInterrupt();

	/* USB Host */
	if (pdev == &USB_OTG_Core)
	{
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = OTG_HS_IRQn;	/* USE_USB_OTG_HS */
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		/* ʹ�� OTG_HS_IRQn */
		NVIC_Init(&NVIC_InitStructure);
	}

	/* USB Device */
	if (pdev == &USB_OTG_dev)
	{
		NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
			NVIC_InitStructure.NVIC_IRQChannel = OTG_HS_EP1_OUT_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

			NVIC_InitStructure.NVIC_IRQChannel = OTG_HS_EP1_IN_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
		#endif
	}
}

/**
  * @brief  BSP_Drive_VBUS
  *         Drives the Vbus signal through IO
  * @param  state : VBUS states
  * @retval None
  */

void USB_OTG_BSP_DriveVBUS(USB_OTG_CORE_HANDLE *pdev, uint8_t state)
{
	/* ������STM32-V5�����岻֧�ֶ��⹩��Ŀ��� */
}

/**
  * @brief  USB_OTG_BSP_ConfigVBUS
  *         Configures the IO for the Vbus and OverCurrent
  * @param  None
  * @retval None
  */

void  USB_OTG_BSP_ConfigVBUS(USB_OTG_CORE_HANDLE *pdev)
{
	/* ������STM32-V5�����岻֧�ֶ��⹩��Ŀ��� */
}

/**
  * @brief  USB_OTG_BSP_uDelay
  *         This function provides delay time in micro sec
  * @param  usec : Value of delay required in micro sec
  * @retval None
  */
void USB_OTG_BSP_uDelay (const uint32_t usec)
{
	bsp_DelayUS(usec);
}


/**
  * @brief  USB_OTG_BSP_mDelay
  *          This function provides delay time in milli sec
  * @param  msec : Value of delay required in milli sec
  * @retval None
  */
void USB_OTG_BSP_mDelay (const uint32_t msec)
{
	bsp_DelayMS(msec);
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
