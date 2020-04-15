/*
*********************************************************************************************************
*
*	模块名称 : 摄像头驱动BSP模块(For OV7670)
*	文件名称 : bsp_camera.c
*	版    本 : V1.0
*	说    明 : OV7670驱动程序。本程序适用于 guanfu_wang  的OV7670摄像头（不带FIFO,不带LDO，不带24M晶振)
*			  安富莱STM32-V5开发板集成了3.0V LDO给OV7670供电，主板集成了24M有源晶振提供给摄像头。
*
*			  本代码参考了 guanfu_wang 提供的例子。http://mcudiy.taobao.com/
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2013-03-01 armfly  正式发布
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/*
	安富莱STM32-V5开发板摄像头接口GPIO定义： 【DCIM设备： 摄像头  和 AD7606 模块不能同时使用】
	PA6/DCMI_PIXCLK
	PH8/DCMI_HSYNC
	PH9/DCMI_D0/AD7606_OS0
	PH10/DCMI_D1/AD7606_OS1
	PH11/DCMI_D2/AD7606_OS2
	PH12/DCMI_D3/AD7606_CONVST
	PH14/DCMI_D4/AD7606_RAGE
	PI4/DCMI_D5/AD7606_RESET
	PI5/DCMI_VSYNC
	PI6/DCMI_D6/AD7606_BUSY
	PI7/DCMI_D7/NRF905_CD

	--- I2C总线控制摄像头
	PH4/I2C2_SCL
	PH5/I2C2_SDA
*/

#define DCMI_DR_ADDRESS       	0x50050028
#define OV_REG_NUM  	116  //OV7670

/* 调分辨率

12 17 18 19 1A 03。。。

*/

/*
	以下为OV7670 QVGA RGB565参数  (by guanfu_wang)  http://mcudiy.taobao.com

	由于RA8875图形模式下，扫描方向为从左到右，从上到下。
	和wang_guanfu提供的缺省值不同。因此做了适当的调整。
*/
static const unsigned char  OV_reg[OV_REG_NUM][2] =
{
	{0x3a, 0x0c},
	{0x67, 0xc0},
	{0x68, 0x80},

	{0x40, 0xd0},
	{0x12, 0x14}, //COM7

	{0x32, 0x80},
	{0x17, 0x16},
	{0x18, 0x04},
	{0x19, 0x02},
	{0x1a, 0x7a},//0x7a,

	{0x03, 0x05},//0x0a,
	{0x0c, 0x00},
	{0x3e, 0x00},//

	{0x70, 0x00},
	{0x71, 0x01},
	{0x72, 0x11},
	{0x73, 0x00},//
	{0xa2, 0x02},
	{0x11, 0x01},

	{0x7a,  0x2C},
	{0x7b,  0x11},
	{0x7c,  0x1a},
	{0x7d,  0x2a},
	{0x7e,  0x42},
	{0x7f,  0x4c},
	{0x80,  0x56},
	{0x81,  0x5f},
	{0x82,  0x67},
	{0x83,  0x70},
	{0x84,  0x78},
	{0x85,  0x87},
	{0x86,  0x95},
	{0x87,  0xaf},
	{0x88,  0xc8},
	{0x89,  0xdf},

	////////////////
	{0x13, 0xe0},
	{0x00, 0x00},//AGC

	{0x10, 0x00},
	{0x0d, 0x00},
	{0x14, 0x10},//0x38, limit the max gain
	{0xa5, 0x05},
	{0xab, 0x07},

	{0x24, 0x75},//40
	{0x25, 0x63},
	{0x26, 0xA5},
	{0x9f, 0x78},
	{0xa0, 0x68},

	{0xa1, 0x03},//0x0b,
	{0xa6, 0xdf},//0xd8,
	{0xa7, 0xdf},//0xd8,
	{0xa8, 0xf0},
	{0xa9, 0x90},

	{0xaa, 0x94},//50
	{0x13, 0xe5},
	{0x0e, 0x61},
	{0x0f, 0x4b},
	{0x16, 0x02},

#if 1
	{0x1e, 0x37},//0x07, 0x17, 0x27, 0x37 选择1个，决定扫描方向. 需要和LCD的扫描方向匹配。
#else
	{0x1e, 0x27},//0x07,
#endif


	{0x21, 0x02},
	{0x22, 0x91},
	{0x29, 0x07},
	{0x33, 0x0b},

	{0x35, 0x0b},//60
	{0x37, 0x1d},
	{0x38, 0x71},
	{0x39, 0x2a},
	{0x3c, 0x78},

	{0x4d, 0x40},
	{0x4e, 0x20},
	{0x69, 0x5d},
	{0x6b, 0x0a},//PLL
	{0x74, 0x19},
	{0x8d, 0x4f},

	{0x8e, 0x00},//70
	{0x8f, 0x00},
	{0x90, 0x00},
	{0x91, 0x00},
	{0x92, 0x00},//0x19,//0x66

	{0x96, 0x00},
	{0x9a, 0x80},
	{0xb0, 0x84},
	{0xb1, 0x0c},
	{0xb2, 0x0e},

	{0xb3, 0x82},//80
	{0xb8, 0x0a},
	{0x43, 0x14},
	{0x44, 0xf0},
	{0x45, 0x34},

	{0x46, 0x58},
	{0x47, 0x28},
	{0x48, 0x3a},
	{0x59, 0x88},
	{0x5a, 0x88},

	{0x5b, 0x44},//90
	{0x5c, 0x67},
	{0x5d, 0x49},
	{0x5e, 0x0e},
	{0x64, 0x04},
	{0x65, 0x20},

	{0x66, 0x05},
	{0x94, 0x04},
	{0x95, 0x08},
	{0x6c, 0x0a},
	{0x6d, 0x55},


	{0x4f, 0x80},
	{0x50, 0x80},
	{0x51, 0x00},
	{0x52, 0x22},
	{0x53, 0x5e},
	{0x54, 0x80},

	{0x76, 0xe1},

	{0x6e, 0x11},//100
	{0x6f, 0x9f},//0x9e for advance AWB
	{0x55, 0x00},//亮度
	{0x56, 0x40},//对比度
	{0x57, 0x80},//0x40,
};

static void CAM_ConfigCPU(void);
static uint8_t OV_InitReg(void);
static void OV_WriteReg(uint8_t _ucRegAddr, uint8_t _ucRegValue);
static uint8_t OV_ReadReg(uint8_t _ucRegAddr);

CAM_T g_tCam;

/*
*********************************************************************************************************
*	函 数 名: bsp_InitCamera
*	功能说明: 配置摄像头GPIO和CAMERA设备.
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitCamera(void)
{
	CAM_ConfigCPU();

	#if 1	/* 下面的代码，验证读写寄存器是否正确 */
	{
		uint8_t read;

		read = OV_ReadReg(0x3A);

		OV_WriteReg(0x3A, read + 1);

		read = OV_ReadReg(0x3A);

		OV_WriteReg(0x3A, read + 1);

		read = OV_ReadReg(0x3A);
	}
	#endif

	OV_InitReg();
}

/*
*********************************************************************************************************
*	函 数 名: CAM_ConfigCPU
*	功能说明: 配置摄像头GPIO和CAMERA设备。0V7670的I2C接口配置在 bsp_gpio_i2c.c 文件实现。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void CAM_ConfigCPU(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	DCMI_InitTypeDef DCMI_InitStructure;

	/* 使能 DCMI 时钟 */
  	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);

	/* 使能 DMA2时钟 */
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

  	/* 使能 DCMI 接口 GPIO 时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI, ENABLE);

 	/* 安富莱STM32-V5开发板采用有源晶振提供24M时钟，因此不用PA8产生时钟 */
	#if 0	/* 如果用 PA8 输出24M时钟，可以使用如下代码. */
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_MCO);//MCO1:PA8
	  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	    RCC_MCO2Config(RCC_MCO1Source_PLLCLK, RCC_MCO1Div_3);
    #endif

	/*
		将DCMI相关的GPIO设置为复用模式
			PA6/DCMI_PIXCLK
			PH8/DCMI_HSYNC
			PH9/DCMI_D0/AD7606_OS0
			PH10/DCMI_D1/AD7606_OS1
			PH11/DCMI_D2/AD7606_OS2
			PH12/DCMI_D3/AD7606_CONVST
			PH14/DCMI_D4/AD7606_RAGE
			PI4/DCMI_D5/AD7606_RESET
			PI5/DCMI_VSYNC
			PI6/DCMI_D6/AD7606_BUSY
			PI7/DCMI_D7/NRF905_CD
	*/
    GPIO_PinAFConfig(GPIOH, GPIO_PinSource8, GPIO_AF_DCMI);//DCMI_HSYNC
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_DCMI);//DCMI_PIXCLK
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource4, GPIO_AF_DCMI);//DCMI_D5
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource5, GPIO_AF_DCMI);//DCMI_VSYNC
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource6, GPIO_AF_DCMI);//DCMI_D6
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource7, GPIO_AF_DCMI);//DCMI_D7
    GPIO_PinAFConfig(GPIOH, GPIO_PinSource9, GPIO_AF_DCMI);//DCMI_D0
    GPIO_PinAFConfig(GPIOH, GPIO_PinSource10, GPIO_AF_DCMI);//DCMI_D1
    GPIO_PinAFConfig(GPIOH, GPIO_PinSource11, GPIO_AF_DCMI);//DCMI_D2
    GPIO_PinAFConfig(GPIOH, GPIO_PinSource12, GPIO_AF_DCMI);//DCMI_D3
    GPIO_PinAFConfig(GPIOH, GPIO_PinSource14, GPIO_AF_DCMI);//DCMI_D4

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_14;
    GPIO_Init(GPIOH, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;
    GPIO_Init(GPIOI, &GPIO_InitStructure);

	/* 配置 DCMIC 参数 */
  	DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;
  	DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;
  	DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Rising ;
  	DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_High;
  	DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_Low;
  	DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;
  	DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b;
  	DCMI_Init(&DCMI_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: OV_InitReg
*	功能说明: 复位OV7670, 配置OV7670的寄存器，QVGA
*	形    参: 无
*	返 回 值: 0 表示正确，1表示失败
*********************************************************************************************************
*/
static uint8_t OV_InitReg(void)
{
  	uint8_t i;

  	CAM_ConfigCPU();
	//bsp_InitI2C();		/* 配置I2C总线, 在 bsp.c 文件执行了 */

	OV_WriteReg(0x12, 0x80); 	/* Reset SCCB */

	bsp_DelayMS(5);

	/* 设置 OV7670寄存器 */
  	for (i = 0; i < OV_REG_NUM; i++)
  	{
		OV_WriteReg(OV_reg[i][0], OV_reg[i][1]);
  	}
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: CAM_Start
*	功能说明: 启动DMA和DCMI，开始传送图像数据到LCD显存
*	形    参: _uiDispMemAddr 显存地址
*	返 回 值: 无
*********************************************************************************************************
*/
void CAM_Start(uint32_t _uiDispMemAddr)
{
  	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

  	/* 配置 DMA2， 由DCMI数据寄存器直接传送至LCD显存 */
  	DMA_DeInit(DMA2_Stream1);
  	DMA_InitStructure.DMA_Channel = DMA_Channel_1;
  	DMA_InitStructure.DMA_PeripheralBaseAddr = DCMI_DR_ADDRESS;		/* DCMI 数据寄存器地址 */
  	DMA_InitStructure.DMA_Memory0BaseAddr = _uiDispMemAddr;			/* LCD 显存数据地址 */
  	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
//  	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_BufferSize = 265*320*2;
  	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  	//DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  	//DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  	DMA_Init(DMA2_Stream1, &DMA_InitStructure);

	/* Enable DMA Stream Transfer Complete interrupt */
	DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, ENABLE);
 
  	DMA_Cmd(DMA2_Stream1, ENABLE);

	/* Enable the DMA Stream IRQ Channel */
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

  	DCMI_Cmd(ENABLE);
  	DCMI_CaptureCmd(ENABLE);
	
	g_tCam.CaptureOk = 0;		/* 全局标志 */
}

/*
*********************************************************************************************************
*	函 数 名: CAM_Stop
*	功能说明: 停止DMA和DCMI
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void CAM_Stop(void)
{
  	DMA_Cmd(DMA2_Stream1, DISABLE);
  	DCMI_Cmd(DISABLE);
  	DCMI_CaptureCmd(DISABLE);
}

/*
*********************************************************************************************************
*	函 数 名: DMA2_Stream1_IRQHandler
*	功能说明: DMA传输完成中断服务程序
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void DMA2_Stream1_IRQHandler(void)
{	
	/* Test on DMA Stream Transfer Complete interrupt */
	if (DMA_GetITStatus(DMA2_Stream1, DMA_IT_TCIF1))
	{
		/* Clear DMA Stream Transfer Complete interrupt pending bit */
		DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TCIF1);  
		
		/* 关闭摄像 */
		CAM_Stop();
		g_tCam.CaptureOk = 1;		/* 表示DMA传输结束 */
	}
}

#if 0  /* 不知道干吗的，留着。。。 */
void OV_HW(unsigned int  hstart,unsigned int vstart,unsigned int hstop,unsigned int vstop)
{
	u8 v;
	OV_WriteReg(0x17,(hstart>>3)&0xff);//HSTART
	OV_WriteReg(0x18,(hstop>>3)&0xff);//HSTOP
	OV_ReadReg(0x32,&v);
	v=(v&0xc0)|((hstop&0x7)<<3)|(hstart&0x7);
	OV_WriteReg(0x32,v);//HREF

	OV_WriteReg(0x19,(vstart>>2)&0xff);//VSTART 开始高8位
	OV_WriteReg(0x1a,(vstop>>2)&0xff);//VSTOP	结束高8位
	OV_ReadReg(0x03,&v);
	v=(v&0xf0)|((vstop&0x3)<<2)|(vstart&0x3);
	OV_WriteReg(0x03,v);//VREF
	OV_WriteReg(0x11,0x00);
}
#endif

/*
*********************************************************************************************************
*	函 数 名: OV_WriteReg
*	功能说明: 写0V7670寄存器
*	形    参: _ucRegAddr  : 寄存器地址
*			  _ucRegValue : 寄存器值
*	返 回 值: 无
*********************************************************************************************************
*/
static void OV_WriteReg(uint8_t _ucRegAddr, uint8_t _ucRegValue)
{
    i2c_Start();							/* 总线开始信号 */

    i2c_SendByte(OV7670_SLAVE_ADDRESS);		/* 发送设备地址+写信号 */
	i2c_WaitAck();

    i2c_SendByte(_ucRegAddr);				/* 发送寄存器地址 */
	i2c_WaitAck();

    i2c_SendByte(_ucRegValue);				/* 发送寄存器数值 */
	i2c_WaitAck();

    i2c_Stop();                   			/* 总线停止信号 */
}

/*
*********************************************************************************************************
*	函 数 名: OV_ReadReg
*	功能说明: 读0V7670寄存器
*	形    参: _ucRegAddr  : 寄存器地址
*	返 回 值: 寄存器值
*********************************************************************************************************
*/
static uint8_t OV_ReadReg(uint8_t _ucRegAddr)
{
	uint16_t usRegValue;

	i2c_Start();                  			/* 总线开始信号 */
	i2c_SendByte(OV7670_SLAVE_ADDRESS);		/* 发送设备地址+写信号 */
	i2c_WaitAck();
	i2c_SendByte(_ucRegAddr);				/* 发送地址 */
	i2c_WaitAck();

	i2c_Stop();			/* 0V7670 需要插入 stop, 否则读取寄存器失败 */

	i2c_Start();                  			/* 总线开始信号 */
	i2c_SendByte(OV7670_SLAVE_ADDRESS + 1);/* 发送设备地址+读信号 */
	i2c_WaitAck();

	usRegValue = i2c_ReadByte();       		/* 读出高字节数据 */
	i2c_NAck();
	i2c_Stop();                  			/* 总线停止信号 */

	return usRegValue;
}

/*
*********************************************************************************************************
*	函 数 名: OV_ReadID
*	功能说明: 读0V7670的芯片ID
*	形    参: 无
*	返 回 值: 芯片ID. 正常应该返回 0x7673
*********************************************************************************************************
*/
uint16_t OV_ReadID(void)
{
	uint8_t idh,idl;

	idh = OV_ReadReg(0x0A);
	idl = OV_ReadReg(0x0B);
	return (idh << 8) + idl;
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
