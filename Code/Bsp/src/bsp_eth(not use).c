/*
*********************************************************************************************************
*
*	模块名称 : STM32F4 内置ETH MAC 驱动模块
*	文件名称 : bsp_eth.c
*	版    本 : V2.4
*	说    明 : 安富莱STM32-F4开发板外扩的MAC为 DM9161
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2013-02-01 armfly  正式发布
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4x7_eth.h"
#include "bsp_eth.h"
#include "stdio.h"

#define eth_printf	printf
//#define eth_printf(...)

__IO uint32_t  EthInitStatus = 0;
__IO uint8_t EthLinkStatus = 0;

__IO uint8_t g_ucPhyAddress = 0;	/* PHY 地址 */

static void ETH_GPIO_Config_RMII(void);
static void ETH_MACDMA_Config(uint8_t ucPhyAddr);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  ETH_BSP_Config
  * @param  None
  * @retval None
  */
void ETH_BSP_Config(void)
{
//	RCC_ClocksTypeDef RCC_Clocks;
	uint32_t ulTemp;

	//bsp_DelayMS(500);
	/* Configure the GPIO ports for ethernet pins */
	ETH_GPIO_Config_RMII();
	
	ETH_MACDMA_Config(PHY_ADDRESS);
	
#if 0
	for (g_ucPhyAddress = 0; g_ucPhyAddress < 31; g_ucPhyAddress++)
	{
		ETH_MACDMA_Config(g_ucPhyAddress);
		
		ulTemp = ETH_ReadPHYRegister(g_ucPhyAddress, 0x02);
		if (ulTemp == 0x0181)
		{
			break;
		}
	}
#endif	
	
	/* Configure the Ethernet MAC/DMA */
	//ETH_MACDMA_Config();
	
	ulTemp = ETH_ReadPHYRegister(PHY_ADDRESS, PHY_BSR);
	if (ulTemp & PHY_Linked_Status)
	{
		eth_printf("%04X, Phy Link Ok\r\n", ulTemp);
	}
	else
	{
		eth_printf("%04X, Phy Link Faile\r\n",ulTemp);
	}
	
	/* 读取PHY ID, 识别DM9161 */
	{
		uint32_t uiID;
		
		uiID = ETH_ReadPHYRegister(PHY_ADDRESS, 0X02);
		
		if (uiID == 0x0181)
		{
			eth_printf("Phy Address = %d, Read ID = %04X, DM9161 Detected Ok!\r\n", PHY_ADDRESS, uiID);
		}
		else
		{
			eth_printf("Phy Address = %d, Read ID = %04X\r\n", PHY_ADDRESS, uiID);
		}
	}
	
	if (EthInitStatus == 0)
	{
		eth_printf("> Ethernet Init  Failed\r\n");
		return;
	}

	/* Configure the PHY to generate an interrupt on change of link status */
	Eth_Link_PHYITConfig(PHY_ADDRESS);

	/* Configure the EXTI for Ethernet link status. */
	Eth_Link_EXTIConfig();
}

/**
  * @brief  Configures the Ethernet Interface
  * @param  None
  * @retval None
  */
static void ETH_MACDMA_Config(uint8_t _ucPhyAdress)
{
	ETH_InitTypeDef ETH_InitStructure;

	/* Enable ETHERNET clock  */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_ETH_MAC | RCC_AHB1Periph_ETH_MAC_Tx |
	                RCC_AHB1Periph_ETH_MAC_Rx, ENABLE);

	/* Reset ETHERNET on AHB Bus */
	ETH_DeInit();

	/* Software reset */
	ETH_SoftwareReset();

	/* Wait for software reset */
	while (ETH_GetSoftwareResetStatus() == SET);

	/* ETHERNET Configuration --------------------------------------------------*/
	/* Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter */
	ETH_StructInit(&ETH_InitStructure);

	/* Fill ETH_InitStructure parametrs */
	/*------------------------   MAC   -----------------------------------*/
	ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;
	//ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Disable;
	//  ETH_InitStructure.ETH_Speed = ETH_Speed_10M;
	//  ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;

	ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
	ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
	ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
	ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;
	ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
	ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
	ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
	ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
#ifdef CHECKSUM_BY_HARDWARE
	ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
#endif

	/*------------------------   DMA   -----------------------------------*/

	/* When we use the Checksum offload feature, we need to enable the Store and Forward mode:
	the store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum,
	if the checksum is OK the DMA can handle the frame otherwise the frame is dropped */
	ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable;
	ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;
	ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;

	ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;
	ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;
	ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;
	ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;
	ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;
	ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;
	ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;
	ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;

	/* Configure Ethernet */
	EthInitStatus = ETH_Init(&ETH_InitStructure, _ucPhyAdress);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
static void ETH_GPIO_Config_RMII(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 安富莱STM32-F4开发板网卡接口采用RMII
		PA1/ETH_RMII_RX_CLK
		PA2/ETH_MDIO
		PA7/RMII_CRS_DV
		PC1/ETH_MDC
		PC4/ETH_RMII_RX_D0
		PC5/ETH_RMII_RX_D1
		PG11/ETH_RMII_TX_EN
		PG13/FSMC_A24/ETH_RMII_TXD0
		PG14/ETH_RMII_TXD1

		PH6/MII_INT
	*/
	/* Enable GPIOs clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOG |
	                     RCC_AHB1Periph_GPIOH, ENABLE);

	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_RMII);

	/* Configure PA1, PA2 and PA7 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_ETH);

	/* Configure PC1, PC4 and PC5 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_ETH);

	/* Configure PG11, PG14 and PG13 */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource13, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_ETH);
}

#if 0
void ETH_GPIO_Config_MII(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIOs clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB |
	                     RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOI |
	                     RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOH |
	                     RCC_AHB1Periph_GPIOF, ENABLE);

	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure MCO (PA8) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	#ifdef PHY_CLOCK_MCO

		/* Output HSE clock (25MHz) on MCO pin (PA8) to clock the PHY */
		RCC_MCO1Config(RCC_MCO1Source_HSE, RCC_MCO1Div_1);
	#endif /* PHY_CLOCK_MCO */

	SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_MII);

	/* Ethernet pins configuration ************************************************/
   /*
        ETH_MDIO -------------------------> PA2
        ETH_MDC --------------------------> PC1
        ETH_PPS_OUT ----------------------> PB5
        ETH_MII_CRS ----------------------> PH2
        ETH_MII_COL ----------------------> PH3
        ETH_MII_RX_ER --------------------> PI10
        ETH_MII_RXD2 ---------------------> PH6
        ETH_MII_RXD3 ---------------------> PH7
        ETH_MII_TX_CLK -------------------> PC3
        ETH_MII_TXD2 ---------------------> PC2
        ETH_MII_TXD3 ---------------------> PB8
        ETH_MII_RX_CLK/ETH_RMII_REF_CLK---> PA1
        ETH_MII_RX_DV/ETH_RMII_CRS_DV ----> PA7
        ETH_MII_RXD0/ETH_RMII_RXD0 -------> PC4
        ETH_MII_RXD1/ETH_RMII_RXD1 -------> PC5
        ETH_MII_TX_EN/ETH_RMII_TX_EN -----> PG11
        ETH_MII_TXD0/ETH_RMII_TXD0 -------> PG13
        ETH_MII_TXD1/ETH_RMII_TXD1 -------> PG14
                                                  */

	/* Configure PA1, PA2 and PA7 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_ETH);

	/* Configure PB5 and PB8 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_8;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_ETH);

	/* Configure PC1, PC2, PC3, PC4 and PC5 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_ETH);

	/* Configure PG11, PG14 and PG13 */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource13, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_ETH);

	/* Configure PH2, PH3, PH6, PH7 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOH, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource2, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource3, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource6, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource7, GPIO_AF_ETH);

	/* Configure PI10 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOI, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource10, GPIO_AF_ETH);
}
#endif

/**
  * @brief  Configure the PHY to generate an interrupt on change of link status.
  * @param PHYAddress: external PHY address
  * @retval None
  */
uint32_t Eth_Link_PHYITConfig(uint16_t PHYAddress)
{
//	uint32_t tmpreg = 0;

	/* 
		DM9161 中断控制寄存器 21 		;
		使能中断输出
		全双工模式变化、速度模式变化和Link状态改变时产生中断
	*/
	if (!(ETH_WritePHYRegister(PHYAddress, 21, 0)))
	{
		/* Return ERROR in case of write timeout */
		return ETH_ERROR;
	}

	return ETH_SUCCESS;
}

/**
  * @brief  EXTI configuration for Ethernet link status.
  * @param PHYAddress: external PHY address
  * @retval None
  */
void Eth_Link_EXTIConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 安富莱STM32-F4开发板使用PH6作为中断输入口, 下降沿表示中断信号 */
	/* Enable the INT (PH6) Clock */
	RCC_AHB1PeriphClockCmd(ETH_LINK_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure INT pin as input */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = ETH_LINK_PIN;
	GPIO_Init(ETH_LINK_GPIO_PORT, &GPIO_InitStructure);

	/* Connect EXTI Line to INT Pin */
	SYSCFG_EXTILineConfig(ETH_LINK_EXTI_PORT_SOURCE, ETH_LINK_EXTI_PIN_SOURCE);

	/* Configure EXTI line */
	EXTI_InitStructure.EXTI_Line = ETH_LINK_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set the EXTI interrupt to the highest priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = ETH_LINK_EXTI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  This function handles Ethernet link status.
  * @param  None
  * @retval None
  */
void Eth_Link_ITHandler(uint16_t PHYAddress)
{
	/* 检查连接状态 */
	if (((ETH_ReadPHYRegister(PHYAddress, 0x01)) & (1 << 2)) != 0)
	{
		EthLinkStatus = 1;

		eth_printf("Network Cable is now connected\r\n");
	}
    else
    {
    	EthLinkStatus = 0;
    	eth_printf("Network Cable is unplugged\r\n");
    }
}

void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(ETH_LINK_EXTI_LINE) != RESET)
	{
		Eth_Link_ITHandler(PHY_ADDRESS);
		/* Clear interrupt pending bit */
		EXTI_ClearITPendingBit(ETH_LINK_EXTI_LINE);
	}
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
