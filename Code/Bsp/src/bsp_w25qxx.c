#include "bsp.h"

#define TEST_SIZE 4096
/* 4Kbytes为一个Sector
16个扇区为1个Block */
static uint8_t w25qxx_buf[TEST_SIZE];

SFLASH_T g_tSF;

/*******************************************************************
*函数：uint8_t w25qxx_init(void)
*描述：w25qxx初始化
*输入：none
*输出：none
*返回：none
*版本：V1.0
********************************************************************/
uint8_t w25qxx_init(void)
{
	SPI1_SEND_BY_WAIT_Demo_Master_init();//hspi init
	SPIx_SPICR2_SPIEN_Setable(SPI1, ENABLE);//HSPI模块使能
	
	w25qxx_wake_up();
		
	if(w25qxx_read_id() != W25Q16)
	{
		return 1;
	}
	
//	w25qxx_power_down();
	
	return 0;
}
///*******************************************************************
//*函数：void w25qxx_disable(void)
//*描述：禁用w25qxx
//*输入：none
//*输出：none
//*返回：none
//*作者：朱长健
//*版本：V1.0
//********************************************************************/
void w25qxx_disable(void)
{
		SPI_SSN_Set_High(SPI1);//置高SSN信号
}
///*******************************************************************
//*函数：void w25qxx_read(uint8_t *cbuf, uint32_t addr, uint16_t csize)  
//*描述：读数据
//*输入：cbuf：缓冲区地址，addr：地址，csize：读取的长度
//*输出：none
//*返回：none
//*作者：朱长健
//*版本：V1.0
//********************************************************************/
void w25qxx_read(uint8_t *cbuf, uint32_t addr, uint16_t csize)   
{ 
 	uint16_t i;

	SPI_SSN_Set_Low(SPI1);//拉低SSN信号		
	
	SPI1_SEND_BY_WAIT_Demo_RW(READ_DATA);
	SPI1_SEND_BY_WAIT_Demo_RW((uint8_t)((addr) >> 16));
	SPI1_SEND_BY_WAIT_Demo_RW((uint8_t)((addr) >> 8));
	SPI1_SEND_BY_WAIT_Demo_RW((uint8_t)addr);
	
    for(i=0; i < csize; i++)
	{ 
        cbuf[i] = SPI1_SEND_BY_WAIT_Demo_RW(0xFF);		
    }
	
	SPI_SSN_Set_High(SPI1);//置高SSN信号 				    	      
}  
///***************************************************************************
//*函数：void w25qxx_write_page(uint8_t *cbuf, uint32_t addr, uint16_t csize)  
//*描述：在一页写入数据
//*输入：cbuf：缓冲区地址，addr：地址，csize：读取的长度
//*输出：none
//*返回：none
//*作者：朱长健
//*版本：V1.0
//*****************************************************************************/
void w25qxx_write_page(uint8_t *cbuf, uint32_t addr, uint16_t csize)   
{
	uint16_t i;
	
	w25qxx_write_enable(); 	

	SPI_SSN_Set_Low(SPI1);//拉低SSN信号		
	
	SPI1_SEND_BY_WAIT_Demo_RW(PAGE_PROGRAM);
	SPI1_SEND_BY_WAIT_Demo_RW((uint8_t)((addr) >> 16));
	SPI1_SEND_BY_WAIT_Demo_RW((uint8_t)((addr) >> 8));
	SPI1_SEND_BY_WAIT_Demo_RW((uint8_t)addr);
	for(i=0; i < csize; i++)
	{ 
        SPI1_SEND_BY_WAIT_Demo_RW(cbuf[i]);  
    }
	
	SPI_SSN_Set_High(SPI1);//置高SSN信号
	
	w25qxx_wait_busy();
}
///*******************************************************************************
//*函数：void w25qxx_write_no_check(uint8_t *cbuf, uint32_t addr, uint16_t csize)  
//*描述：不检查写入
//*输入：cbuf：缓冲区地址，addr：地址，csize：读取的长度
//*输出：none
//*返回：none
//*作者：朱长健
//*版本：V1.0
//********************************************************************************/
void w25qxx_write_no_check(uint8_t *cbuf, uint32_t addr, uint16_t csize)   
{ 			 		 
	uint16_t page_remain;	   
	page_remain = 256 - addr % 256;
	
	if(csize <= page_remain)
	{
		page_remain = csize;
	}
	while(1)
	{	   
		w25qxx_write_page(cbuf, addr, page_remain);
		if(csize == page_remain)
		{
			break;
		}
	 	else
		{
			cbuf += page_remain;
			addr += page_remain;	
			csize -= page_remain;			  
			if(csize > 256)
			{
				page_remain = 256;
			} 
			else 
			{
				page_remain = csize;
			} 
		}
	}    
} 
///*******************************************************************************
//*函数：void w25qxx_write(uint8_t *cbuf, uint32_t addr, uint16_t csize)  
//*描述：写入
//*输入：cbuf：缓冲区地址，addr：地址，csize：读取的长度
//*输出：none
//*返回：none
//*作者：朱长健
//*版本：V1.0
//********************************************************************************/
void w25qxx_write(uint8_t *cbuf, uint32_t addr, uint16_t csize)   
{ 
	uint32_t sec_pos;	 /* 扇区地址 */
	uint16_t sec_off;	 /* 扇区内偏移地址 */
	uint16_t sec_remain; /* 扇区内剩余地址 */	   
 	uint16_t i;    

	/* 地址非法 */
	if(addr >= 2 * 1024 * 1024)
	{
		return;
	}
	
	sec_pos = addr / 4096;	        /* 扇区地址 */
	sec_off = addr % 4096;          /* 在扇区内的偏移 */
	sec_remain = 4096 - sec_off;    /* 扇区剩余空间大小 */   
	if(csize <= sec_remain)
	{
		sec_remain = csize;
	}
	while(1)
	{
		w25qxx_read(w25qxx_buf, sec_pos * 4096, 4096);/* 读出整个扇区 */
		
		for(i = 0; i < sec_remain; i++)/* 检查是否需要擦除 */
		{
			if(w25qxx_buf[sec_off + i] != 0xFF)
			{
				break;
			}
		}

		if(i < sec_remain)/* 需要擦除 */
		{
			w25qxx_erase_sector(sec_pos);
			for(i = 0; i < sec_remain; i++)/* 复制数据 */
			{
				w25qxx_buf[i + sec_off] = cbuf[i];
			}
			w25qxx_write_no_check(w25qxx_buf, sec_pos * 4096, 4096);
		}
		else
		{
			w25qxx_write_no_check(cbuf, addr, sec_remain);
		}
		
		if(csize == sec_remain)/* 写入结束 */
		{
			break;
		}
		else
		{
			sec_pos++;				    /* 扇区地址增1 */
			sec_off = 0;			    /* 偏移位置为0 */ 	 
		   	cbuf += sec_remain;  	    /* 指针偏移 */
			addr += sec_remain;	        /* 写地址偏移 */	   
		   	csize -= sec_remain;	    /* 字节数递减 */
			if(csize > 4096)
			{
				sec_remain = 4096;/* 下一个扇区还是写不完 */
			}
			else 
			{
				sec_remain = csize;/* 下一个扇区可以写完 */
			}
		}
	} 
}
/*******************************************************************
*函数：uint16_t w25qxx_read_id(void)
*描述：读取ID
*输入：none
*输出：none
*返回：none
*作者：朱长健
*版本：V1.0
********************************************************************/
uint16_t w25qxx_read_id(void)
{
	uint16_t id = 0;

  SPI_SSN_Set_Low(SPI1);//拉低SSN信号
				    
	SPI1_SEND_BY_WAIT_Demo_RW(READ_DEVICE_ID);
	SPI1_SEND_BY_WAIT_Demo_RW(0); 
	SPI1_SEND_BY_WAIT_Demo_RW(0);
	SPI1_SEND_BY_WAIT_Demo_RW(0);	
	id |= SPI1_SEND_BY_WAIT_Demo_RW(0xFF) << 8;
	id |= SPI1_SEND_BY_WAIT_Demo_RW(0xFF);	
	
	SPI_SSN_Set_High(SPI1);//置高SSN信号
	
	return id;
}
///*******************************************************************
//*函数：uint16_t w25qxx_read_id(void)
//*描述：读取状态寄存器
//		BIT7  6   5:4   3   2   1   0
//		BPL  AAI  RES  BP1 BP0 WEL BUSY
//*输入：none
//*输出：none
//*返回：状态
//*作者：朱长健
//*版本：V1.0
//********************************************************************/
uint8_t w25qxx_read_sr(void)   
{  
	uint8_t byte = 0;  

	SPI_SSN_Set_Low(SPI1);//拉低SSN信号		
                            
	SPI1_SEND_BY_WAIT_Demo_RW(READ_STATUS_REG);
	byte = SPI1_SEND_BY_WAIT_Demo_RW(0xFF);
	
	SPI_SSN_Set_High(SPI1);//置高SSN信号  
	
	return byte;   
} 
///*******************************************************************
//*函数：void w25qxx_write_sr(uint8_t sr)
//*描述：写状态寄存器
//*输入：要写入的值
//*输出：none
//*返回：none
//*作者：朱长健
//*版本：V1.0
//********************************************************************/
void w25qxx_write_sr(uint8_t sr)   
{  
	
	SPI_SSN_Set_Low(SPI1);//拉低SSN信号	

	SPI1_SEND_BY_WAIT_Demo_RW(ENABLE_WRITE_STATUS_REG);             
	SPI1_SEND_BY_WAIT_Demo_RW(WRITE_STATUS_REG);
	SPI1_SEND_BY_WAIT_Demo_RW(sr);
	
	SPI_SSN_Set_High(SPI1);//置高SSN信号  
} 
///*******************************************************************
//*函数：void w25qxx_write_enable(void)  
//*描述：写使能
//*输入：none
//*输出：none
//*返回：none
//*作者：朱长健
//*版本：V1.0
//********************************************************************/
void w25qxx_write_enable(void)   
{  
	SPI_SSN_Set_Low(SPI1);//拉低SSN信号		
                            
	SPI1_SEND_BY_WAIT_Demo_RW(WRITE_ENABLE);
	
	SPI_SSN_Set_High(SPI1);//置高SSN信号
 
} 
///*******************************************************************
//*函数：void w25qxx_write_disable(void)  
//*描述：写禁止
//*输入：none
//*输出：none
//*返回：none
//*作者：朱长健
//*版本：V1.0
//********************************************************************/
void w25qxx_write_disable(void)   
{  
	SPI_SSN_Set_Low(SPI1);//拉低SSN信号		
                            
	SPI1_SEND_BY_WAIT_Demo_RW(WRITE_DISABLE);
	
	SPI_SSN_Set_High(SPI1);//置高SSN信号  
} 
///*******************************************************************
//*函数：void w25qxx_wait_busy(void)    
//*描述：等待空闲
//*输入：none
//*输出：none
//*返回：none
//*作者：朱长健
//*版本：V1.0
//********************************************************************/
void w25qxx_wait_busy(void)   
{   
	while((w25qxx_read_sr() & 0x01) == 0x01);
} 
///*******************************************************************
//*函数：void w25qxx_erase_chip(void) 
//*描述：擦除整个芯片
//*输入：none
//*输出：none
//*返回：none
//*作者：朱长健
//*版本：V1.0
//********************************************************************/
void w25qxx_erase_chip(void)   
{  
	w25qxx_write_enable(); 
	w25qxx_wait_busy();	
	
	SPI_SSN_Set_Low(SPI1);//拉低SSN信号		
                            
	SPI1_SEND_BY_WAIT_Demo_RW(CHIP_ERASE);
	
	SPI_SSN_Set_High(SPI1);//置高SSN信号  
	w25qxx_wait_busy();
} 
///*******************************************************************
//*函数：void w25qxx_erase_sector(uint32_t addr) 
//*描述：擦除一个扇区
//*输入：地址
//*输出：none
//*返回：none
//*作者：朱长健
//*版本：V1.0
//********************************************************************/
void w25qxx_erase_sector(uint32_t addr)    
{  
	addr *= 4096;
	
	w25qxx_write_enable(); 
	w25qxx_wait_busy();	

	SPI_SSN_Set_Low(SPI1);//拉低SSN信号		
                            
	SPI1_SEND_BY_WAIT_Demo_RW(SECTOR_ERASE_4KB);
	SPI1_SEND_BY_WAIT_Demo_RW((uint8_t)((addr) >> 16));
	SPI1_SEND_BY_WAIT_Demo_RW((uint8_t)((addr) >> 8));
	SPI1_SEND_BY_WAIT_Demo_RW((uint8_t)addr);
	
	SPI_SSN_Set_High(SPI1);//置高SSN信号  
	w25qxx_wait_busy();
}
///*******************************************************************
//*函数：void w25qxx_power_down(void)    
//*描述：进入掉电模式
//*输入：地址
//*输出：none
//*返回：none
//*作者：朱长健
//*版本：V1.0
//********************************************************************/
void w25qxx_power_down(void)    
{  
	SPI_SSN_Set_Low(SPI1);//拉低SSN信号	
                            
	SPI1_SEND_BY_WAIT_Demo_RW(W25X_POWER_DOWN);

	SPI_SSN_Set_High(SPI1);//置高SSN信号
//	bsp_DelayUS(1);
	delay_ms(1);//软件延时
}
/*******************************************************************
*函数：void w25qxx_wake_up(void)    
*描述：唤醒
*输入：地址
*输出：none
*返回：none
*版本：V1.0
********************************************************************/
void w25qxx_wake_up(void)    
{  
	SPI_SSN_Set_Low(SPI1);//拉低SSN信号
                            
	SPI1_SEND_BY_WAIT_Demo_RW(W25X_WAKE_UP);

	SPI_SSN_Set_High(SPI1);//置高SSN信号
	delay_ms(1);//软件延时
}

//*********************************************************************//

/*
*********************************************************************************************************
*	函 数 名: DemoSpiFlash
*	功能说明: 串行EEPROM读写例程
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void DemoSpiFlash(void)
{
//	uint32_t uiReadPageNo = 0;

	sf_ReadInfo();
	
	/* 检测串行Flash OK */
	printf("检测到串行Flash, ID = %08X, 型号: %s \r\n", g_tSF.ChipID , g_tSF.ChipName);
	printf("    容量 : %dM字节, 扇区大小 : %d字节\r\n", g_tSF.TotalSize/(1024*1024), g_tSF.PageSize);
	
	printf("\r\n【1 - 读串行Flash, 地址:0x%X,长度:%d字节】\r\n", TEST_ADDR, TEST_SIZE);
	sfReadTest();		/* 读串行Flash数据，并打印出来数据内容 */
	
	printf("\r\n【2 - 写串行Flash, 地址:0x%X,长度:%d字节】\r\n", TEST_ADDR, TEST_SIZE);
	sfWriteTest();		/* 写串行Flash数据，并打印写入速度 */

	printf("\r\n【3 - 擦除整个串行Flash】\r\n");
	sfErase();			/* 擦除串行Flash数据，实际上就是写入全0xFF */

	printf("\r\n【4 - 写整个串行Flash, 全0xAA】\r\n");
	sfWriteAll(0xAA);			/* 写串行Flash数据，并打印写入速度 */
	
	printf("\r\n【1 - 读串行Flash, 地址:0x%X,长度:%d字节】\r\n", TEST_ADDR, TEST_SIZE);
	sfReadTest();		/* 读串行Flash数据，并打印出来数据内容 */
		
	printf("\r\n【5 - 写整个串行Flash, 全0x66】\r\n");
	sfWriteAll(0x66);		

	printf("\r\n【6 - 读整个串行Flash, %dM字节】\r\n", g_tSF.TotalSize/(1024*1024));
	sfTestReadSpeed();		/* 读整个串行Flash数据，测试速度 */

//				case 'Z': /* 读取前1K */
//					if (uiReadPageNo > 0)
//					{
//						uiReadPageNo--;
//					}
//					else
//					{
//						printf("已经是最前\r\n");
//					}
//					sfViewData(uiReadPageNo * 1024);
//					break;

//				case 'X': /* 读取后1K */
//					if (uiReadPageNo < g_tSF.TotalSize / 1024 - 1)
//					{
//						uiReadPageNo++;
//					}
//					else
//					{
//						printf("已经是最后\r\n");
//					}
//					sfViewData(uiReadPageNo * 1024);
//					break;
		
}
		
/*
*********************************************************************************************************
*	函 数 名: sf_ReadBuffer
*	功能说明: 连续读取若干字节。字节个数不能超出芯片容量。
*	形    参:  	_pBuf : 数据源缓冲区；
*				_uiReadAddr ：首地址
*				_usSize ：数据个数, 可以大于PAGE_SIZE,但是不能超出芯片总容量
*	返 回 值: 无
*********************************************************************************************************
*/
void sf_ReadBuffer(uint8_t * _pBuf, uint32_t _uiReadAddr, uint32_t _uiSize)
{
	/* 如果读取的数据长度为0或者超出串行Flash地址空间，则直接返回 */
	if ((_uiSize == 0) ||(_uiReadAddr + _uiSize) > g_tSF.TotalSize)
	{
		return;
	}

	/* 擦除扇区操作 */
  SPI_SSN_Set_Low(SPI1);//拉低SSN信号
	SPI1_SEND_BY_WAIT_Demo_RW(CMD_READ);							/* 发送读命令 */
	SPI1_SEND_BY_WAIT_Demo_RW((_uiReadAddr & 0xFF0000) >> 16);	/* 发送扇区地址的高8bit */
	SPI1_SEND_BY_WAIT_Demo_RW((_uiReadAddr & 0xFF00) >> 8);		/* 发送扇区地址中间8bit */
	SPI1_SEND_BY_WAIT_Demo_RW(_uiReadAddr & 0xFF);				/* 发送扇区地址低8bit */
	while (_uiSize--)
	{
		*_pBuf++ = SPI1_SEND_BY_WAIT_Demo_RW(0);			/* 读一个字节并存储到pBuf，读完后指针自加1 */
	}
	SPI_SSN_Set_High(SPI1);//置高SSN信号
}

/*
*********************************************************************************************************
*	函 数 名: sfReadTest
*	功能说明: 读串行Flash测试
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void sfReadTest(void)
{
	uint16_t i;
	int32_t iTime1, iTime2;
	uint8_t buf[TEST_SIZE];

	/* 起始地址 = 0， 数据长度为 256 */
	iTime1 = bsp_GetRunTime();	/* 记下开始时间 */
	sf_ReadBuffer(buf, TEST_ADDR, TEST_SIZE);
	iTime2 = bsp_GetRunTime();	/* 记下结束时间 */
	printf("读串行Flash成功，数据如下：\r\n");

	/* 打印数据 */
	for (i = 0; i < TEST_SIZE; i++)
	{
		printf(" %02X", buf[i]);

		if ((i & 31) == 31)
		{
			printf("\r\n");	/* 每行显示16字节数据 */
		}
		else if ((i & 31) == 15)
		{
			printf(" - ");
		}
	}

	/* 打印读速度 */
	printf("数据长度: %d字节, 读耗时: %dms, 读速度: %d Bytes/s\r\n", TEST_SIZE, iTime2 - iTime1, (TEST_SIZE * 1000) / (iTime2 - iTime1));
}

/*
*********************************************************************************************************
*	函 数 名: sf_ReadInfo
*	功能说明: 读取器件ID,并填充器件参数
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void sf_ReadInfo(void)
{
	/* 自动识别串行Flash型号 */
	{
		g_tSF.ChipID = sf_ReadID();	/* 芯片ID */

		switch (g_tSF.ChipID)
		{
			case SST25VF016B_ID:
				strcpy(g_tSF.ChipName, "SST25VF016B");
				g_tSF.TotalSize = 2 * 1024 * 1024;	/* 总容量 = 2M */
				g_tSF.PageSize = 4 * 1024;			/* 页面大小 = 4K */
				break;

			case MX25L1606E_ID:
				strcpy(g_tSF.ChipName, "MX25L1606E");
				g_tSF.TotalSize = 2 * 1024 * 1024;	/* 总容量 = 2M */
				g_tSF.PageSize = 4 * 1024;			/* 页面大小 = 4K */
				break;

			case W25Q64BV_ID:
				strcpy(g_tSF.ChipName, "W25Q64BV");
				g_tSF.TotalSize = 8 * 1024 * 1024;	/* 总容量 = 8M */
				g_tSF.PageSize = 4 * 1024;			/* 页面大小 = 4K */
				break;
			
			case W25Q32BV_ID:
				strcpy(g_tSF.ChipName, "W25Q32BV");
				g_tSF.TotalSize = 4 * 1024 * 1024;	/* 总容量 = 8M */
				g_tSF.PageSize = 4 * 1024;			/* 页面大小 = 4K */
				break;		
			
			case W25Q16BV_ID:
				strcpy(g_tSF.ChipName, "W25Q16BV");
				g_tSF.TotalSize = 2 * 1024 * 1024;	/* 总容量 = 8M */
				g_tSF.PageSize = 4 * 1024;			/* 页面大小 = 4K */
				break;
			default:
				strcpy(g_tSF.ChipName, "Unknow Flash");
				g_tSF.TotalSize = 2 * 1024 * 1024;
				g_tSF.PageSize = 4 * 1024;
				break;
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: sf_ReadID
*	功能说明: 读取器件ID
*	形    参:  无
*	返 回 值: 32bit的器件ID (最高8bit填0，有效ID位数为24bit）
*********************************************************************************************************
*/
uint32_t sf_ReadID(void)
{
	uint32_t uiID;
	uint8_t id1, id2, id3;

	SPI_SSN_Set_Low(SPI1);//拉低SSN信号
	SPI1_SEND_BY_WAIT_Demo_RW(CMD_RDID);								/* 发送读ID命令 */
	id1 = SPI1_SEND_BY_WAIT_Demo_RW(0);					/* 读ID的第1个字节 */
	id2 = SPI1_SEND_BY_WAIT_Demo_RW(0);					/* 读ID的第2个字节 */
	id3 = SPI1_SEND_BY_WAIT_Demo_RW(0);					/* 读ID的第3个字节 */
	SPI_SSN_Set_High(SPI1);//置高SSN信号

	uiID = ((uint32_t)id1 << 16) | ((uint32_t)id2 << 8) | id3;

	return uiID;
}

/*
*********************************************************************************************************
*	函 数 名: sfTestReadSpeed
*	功能说明: 测试串行Flash读速度。读取整个串行Flash的数据，最后打印结果
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void sfTestReadSpeed(void)
{
	uint16_t i;
	int32_t iTime1, iTime2;
	uint8_t buf[TEST_SIZE];
	uint32_t uiAddr;

	/* 起始地址 = 0， 数据长度为 256 */
	iTime1 = bsp_GetRunTime();	/* 记下开始时间 */
	uiAddr = 0;
	for (i = 0; i < g_tSF.TotalSize / TEST_SIZE; i++, uiAddr += TEST_SIZE)
	{
		sf_ReadBuffer(buf, uiAddr, TEST_SIZE);
		printf(".");
		IWDT_Clr(); 
	}
	iTime2 = bsp_GetRunTime();	/* 记下结束时间 */

	/* 打印读速度 */
	printf("数据长度: %d字节, 读耗时: %dms, 读速度: %d Bytes/s\r\n", g_tSF.TotalSize, iTime2 - iTime1, (g_tSF.TotalSize * 1000) / (iTime2 - iTime1));
}

/*
*********************************************************************************************************
*	函 数 名: sfWriteAll
*	功能说明: 写串行EEPROM全部数据
*	形    参：_ch : 写入的数据
*	返 回 值: 无
*********************************************************************************************************
*/
void sfWriteAll(uint8_t _ch)
{
	uint16_t i;
	int32_t iTime1, iTime2;
	uint8_t buf[4 * 1024];

	/* 填充测试缓冲区 */
	for (i = 0; i < TEST_SIZE; i++)
	{
		buf[i] = _ch;
	}

	iTime1 = bsp_GetRunTime();	/* 记下开始时间 */
	for (i = 0; i < g_tSF.TotalSize / g_tSF.PageSize; i++)
	{
		if (sf_WriteBuffer(buf, i * g_tSF.PageSize, g_tSF.PageSize) == 0)
		{
			printf("写串行Flash出错！\r\n");
			return;
		}
		printf(".");
		IWDT_Clr(); 
	}
	iTime2 = bsp_GetRunTime();	/* 记下结束时间 */

	/* 打印读速度 */
	printf("数据长度: %dK字节, 写耗时: %dms, 写速度: %dB/s\r\n", g_tSF.TotalSize / 1024, iTime2 - iTime1, (g_tSF.TotalSize * 1000) / (iTime2 - iTime1));
}

/*
*********************************************************************************************************
*	函 数 名: sf_WriteBuffer
*	功能说明: 写1个扇区并校验,如果不正确则再重写两次。本函数自动完成擦除操作。
*	形    参:  	_pBuf : 数据源缓冲区；
*				_uiWrAddr ：目标区域首地址
*				_usSize ：数据个数，不能超过页面大小
*	返 回 值: 1 : 成功， 0 ： 失败
*********************************************************************************************************
*/
uint8_t sf_WriteBuffer(uint8_t* _pBuf, uint32_t _uiWriteAddr, uint16_t _usWriteSize)
{
	uint16_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

	Addr = _uiWriteAddr % g_tSF.PageSize;
	count = g_tSF.PageSize - Addr;
	NumOfPage =  _usWriteSize / g_tSF.PageSize;
	NumOfSingle = _usWriteSize % g_tSF.PageSize;

	if (Addr == 0) /* 起始地址是页面首地址  */
	{
		if (NumOfPage == 0) /* 数据长度小于页面大小 */
		{
			if (sf_AutoWritePage(_pBuf, _uiWriteAddr, _usWriteSize) == 0)
			{
				return 0;
			}
		}
		else 	/* 数据长度大于等于页面大小 */
		{
			while (NumOfPage--)
			{
				if (sf_AutoWritePage(_pBuf, _uiWriteAddr, g_tSF.PageSize) == 0)
				{
					return 0;
				}
				_uiWriteAddr +=  g_tSF.PageSize;
				_pBuf += g_tSF.PageSize;
			}
			if (sf_AutoWritePage(_pBuf, _uiWriteAddr, NumOfSingle) == 0)
			{
				return 0;
			}
		}
	}
	else  /* 起始地址不是页面首地址  */
	{
		if (NumOfPage == 0) /* 数据长度小于页面大小 */
		{
			if (NumOfSingle > count) /* (_usWriteSize + _uiWriteAddr) > SPI_FLASH_PAGESIZE */
			{
				temp = NumOfSingle - count;

				if (sf_AutoWritePage(_pBuf, _uiWriteAddr, count) == 0)
				{
					return 0;
				}

				_uiWriteAddr +=  count;
				_pBuf += count;

				if (sf_AutoWritePage(_pBuf, _uiWriteAddr, temp) == 0)
				{
					return 0;
				}
			}
			else
			{
				if (sf_AutoWritePage(_pBuf, _uiWriteAddr, _usWriteSize) == 0)
				{
					return 0;
				}
			}
		}
		else	/* 数据长度大于等于页面大小 */
		{
			_usWriteSize -= count;
			NumOfPage =  _usWriteSize / g_tSF.PageSize;
			NumOfSingle = _usWriteSize % g_tSF.PageSize;

			if (sf_AutoWritePage(_pBuf, _uiWriteAddr, count) == 0)
			{
				return 0;
			}

			_uiWriteAddr +=  count;
			_pBuf += count;

			while (NumOfPage--)
			{
				if (sf_AutoWritePage(_pBuf, _uiWriteAddr, g_tSF.PageSize) == 0)
				{
					return 0;
				}
				_uiWriteAddr +=  g_tSF.PageSize;
				_pBuf += g_tSF.PageSize;
			}

			if (NumOfSingle != 0)
			{
				if (sf_AutoWritePage(_pBuf, _uiWriteAddr, NumOfSingle) == 0)
				{
					return 0;
				}
			}
		}
	}
	return 1;	/* 成功 */
}

/*
*********************************************************************************************************
*	函 数 名: sf_AutoWritePage
*	功能说明: 写1个PAGE并校验,如果不正确则再重写两次。本函数自动完成擦除操作。
*	形    参:  	_pBuf : 数据源缓冲区；
*				_uiWriteAddr ：目标区域首地址
*				_usSize ：数据个数，不能超过页面大小
*	返 回 值: 0 : 错误， 1 ： 成功
*********************************************************************************************************
*/
static uint8_t sf_AutoWritePage(uint8_t *_ucpSrc, uint32_t _uiWrAddr, uint16_t _usWrLen)
{
	uint16_t i;
	uint16_t j;					/* 用于延时 */
	uint32_t uiFirstAddr;		/* 扇区首址 */
	uint8_t ucNeedErase;		/* 1表示需要擦除 */
	uint8_t cRet;

	/* 长度为0时不继续操作,直接认为成功 */
	if (_usWrLen == 0)
	{
		return 1;
	}

	/* 如果偏移地址超过芯片容量则退出 */
	if (_uiWrAddr >= g_tSF.TotalSize)
	{
		return 0;
	}

	/* 如果数据长度大于扇区容量，则退出 */
	if (_usWrLen > g_tSF.PageSize)
	{
		return 0;
	}

	/* 如果FLASH中的数据没有变化,则不写FLASH */
	sf_ReadBuffer(w25qxx_buf, _uiWrAddr, _usWrLen);
	if (memcmp(w25qxx_buf, _ucpSrc, _usWrLen) == 0)
	{
		return 1;
	}

	/* 判断是否需要先擦除扇区 */
	/* 如果旧数据修改为新数据，所有位均是 1->0 或者 0->0, 则无需擦除,提高Flash寿命 */
	ucNeedErase = 0;
	if (sf_NeedErase(w25qxx_buf, _ucpSrc, _usWrLen))
	{
		ucNeedErase = 1;
	}

	uiFirstAddr = _uiWrAddr & (~(g_tSF.PageSize - 1));

	if (_usWrLen == g_tSF.PageSize)		/* 整个扇区都改写 */
	{
		for	(i = 0; i < g_tSF.PageSize; i++)
		{
			w25qxx_buf[i] = _ucpSrc[i];
		}
	}
	else						/* 改写部分数据 */
	{
		/* 先将整个扇区的数据读出 */
		sf_ReadBuffer(w25qxx_buf, uiFirstAddr, g_tSF.PageSize);

		/* 再用新数据覆盖 */
		i = _uiWrAddr & (g_tSF.PageSize - 1);
		memcpy(&w25qxx_buf[i], _ucpSrc, _usWrLen);
	}

	/* 写完之后进行校验，如果不正确则重写，最多3次 */
	cRet = 0;
	for (i = 0; i < 3; i++)
	{

		/* 如果旧数据修改为新数据，所有位均是 1->0 或者 0->0, 则无需擦除,提高Flash寿命 */
		if (ucNeedErase == 1)
		{
			sf_EraseSector(uiFirstAddr);		/* 擦除1个扇区 */
		}

		/* 编程一个PAGE */
		sf_PageWrite(w25qxx_buf, uiFirstAddr, g_tSF.PageSize);

		if (sf_CmpData(_uiWrAddr, _ucpSrc, _usWrLen) == 0)
		{
			cRet = 1;
			break;
		}
		else
		{
			if (sf_CmpData(_uiWrAddr, _ucpSrc, _usWrLen) == 0)
			{
				cRet = 1;
				break;
			}

			/* 失败后延迟一段时间再重试 */
			for (j = 0; j < 10000; j++);
		}
	}

	return cRet;
}

/*
*********************************************************************************************************
*	函 数 名: sf_NeedErase
*	功能说明: 判断写PAGE前是否需要先擦除。
*	形    参:   _ucpOldBuf ： 旧数据
*			   _ucpNewBuf ： 新数据
*			   _uiLen ：数据个数，不能超过页面大小
*	返 回 值: 0 : 不需要擦除， 1 ：需要擦除
*********************************************************************************************************
*/
uint8_t sf_NeedErase(uint8_t * _ucpOldBuf, uint8_t *_ucpNewBuf, uint16_t _usLen)
{
	uint16_t i;
	uint8_t ucOld;

	/*
	算法第1步：old 求反, new 不变
	      old    new
		  1101   0101
	~     1111
		= 0010   0101

	算法第2步: old 求反的结果与 new 位与
		  0010   old
	&	  0101   new
		 =0000

	算法第3步: 结果为0,则表示无需擦除. 否则表示需要擦除
	*/

	for (i = 0; i < _usLen; i++)
	{
		ucOld = *_ucpOldBuf++;
		ucOld = ~ucOld;

		/* 注意错误的写法: if (ucOld & (*_ucpNewBuf++) != 0) */
		if ((ucOld & (*_ucpNewBuf++)) != 0)
		{
			return 1;
		}
	}
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: sf_EraseChip
*	功能说明: 擦除整个芯片
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void sf_EraseChip(void)
{
	sf_WriteEnable();								/* 发送写使能命令 */

	/* 擦除扇区操作 */
	SPI_SSN_Set_Low(SPI1);//拉低SSN信号	
	SPI1_SEND_BY_WAIT_Demo_RW(CMD_BE);							/* 发送整片擦除命令 */
	SPI_SSN_Set_High(SPI1);//置高SSN信号

	sf_WaitForWriteEnd();							/* 等待串行Flash内部写操作完成 */
}


/*
*********************************************************************************************************
*	函 数 名: sf_WriteEnable
*	功能说明: 向器件发送写使能命令
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void sf_WriteEnable(void)
{
  SPI_SSN_Set_Low(SPI1);//拉低SSN信号	
	SPI1_SEND_BY_WAIT_Demo_RW(CMD_WREN);								/* 发送命令 */
	SPI_SSN_Set_High(SPI1);//置高SSN信号
}

/*
*********************************************************************************************************
*	函 数 名: sf_WaitForWriteEnd
*	功能说明: 采用循环查询的方式等待器件内部写操作完成
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void sf_WaitForWriteEnd(void)
{
	SPI_SSN_Set_Low(SPI1);//拉低SSN信号	
	SPI1_SEND_BY_WAIT_Demo_RW(CMD_RDSR);							/* 发送命令， 读状态寄存器 */
	while((SPI1_SEND_BY_WAIT_Demo_RW(0) & WIP_FLAG) == SET);	/* 判断状态寄存器的忙标志位 */
	SPI_SSN_Set_High(SPI1);//置高SSN信号
}

/*
*********************************************************************************************************
*	函 数 名: sf_EraseSector
*	功能说明: 擦除指定的扇区
*	形    参:  _uiSectorAddr : 扇区地址
*	返 回 值: 无
*********************************************************************************************************
*/
void sf_EraseSector(uint32_t _uiSectorAddr)
{
	sf_WriteEnable();								/* 发送写使能命令 */

	/* 擦除扇区操作 */
	SPI_SSN_Set_Low(SPI1);//拉低SSN信号	
	SPI1_SEND_BY_WAIT_Demo_RW(CMD_SE);								/* 发送擦除命令 */
	SPI1_SEND_BY_WAIT_Demo_RW((_uiSectorAddr & 0xFF0000) >> 16);	/* 发送扇区地址的高8bit */
	SPI1_SEND_BY_WAIT_Demo_RW((_uiSectorAddr & 0xFF00) >> 8);		/* 发送扇区地址中间8bit */
	SPI1_SEND_BY_WAIT_Demo_RW(_uiSectorAddr & 0xFF);				/* 发送扇区地址低8bit */
	SPI_SSN_Set_High(SPI1);//置高SSN信号

	sf_WaitForWriteEnd();							/* 等待串行Flash内部写操作完成 */
}

/*
*********************************************************************************************************
*	函 数 名: sf_CmpData
*	功能说明: 比较Flash的数据.
*	形    参:  	_ucpTar : 数据缓冲区
*				_uiSrcAddr ：Flash地址
*				_uiSize ：数据个数, 可以大于PAGE_SIZE,但是不能超出芯片总容量
*	返 回 值: 0 = 相等, 1 = 不等
*********************************************************************************************************
*/
uint8_t sf_CmpData(uint32_t _uiSrcAddr, uint8_t *_ucpTar, uint32_t _uiSize)
{
	uint8_t ucValue;

	/* 如果读取的数据长度为0或者超出串行Flash地址空间，则直接返回 */
	if ((_uiSrcAddr + _uiSize) > g_tSF.TotalSize)
	{
		return 1;
	}

	if (_uiSize == 0)
	{
		return 0;
	}

	SPI_SSN_Set_Low(SPI1);//拉低SSN信号	
	SPI1_SEND_BY_WAIT_Demo_RW(CMD_READ);							/* 发送读命令 */
	SPI1_SEND_BY_WAIT_Demo_RW((_uiSrcAddr & 0xFF0000) >> 16);		/* 发送扇区地址的高8bit */
	SPI1_SEND_BY_WAIT_Demo_RW((_uiSrcAddr & 0xFF00) >> 8);		/* 发送扇区地址中间8bit */
	SPI1_SEND_BY_WAIT_Demo_RW(_uiSrcAddr & 0xFF);					/* 发送扇区地址低8bit */
	while (_uiSize--)
	{
		/* 读一个字节 */
		ucValue = SPI1_SEND_BY_WAIT_Demo_RW(0);
		if (*_ucpTar++ != ucValue)
		{
			SPI_SSN_Set_High(SPI1);//置高SSN信号
			return 1;
		}
	}
	SPI_SSN_Set_High(SPI1);//置高SSN信号
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: sf_PageWrite
*	功能说明: 向一个page内写入若干字节。字节个数不能超出页面大小（4K)
*	形    参:  	_pBuf : 数据源缓冲区；
*				_uiWriteAddr ：目标区域首地址
*				_usSize ：数据个数，不能超过页面大小
*	返 回 值: 无
*********************************************************************************************************
*/
void sf_PageWrite(uint8_t * _pBuf, uint32_t _uiWriteAddr, uint16_t _usSize)
{
	  uint32_t i, j;
		for (j = 0; j < _usSize / 256; j++)
		{
			sf_WriteEnable();								/* 发送写使能命令 */

			SPI_SSN_Set_Low(SPI1);//拉低SSN信号	
			SPI1_SEND_BY_WAIT_Demo_RW(0x02);								/* 发送AAI命令(地址自动增加编程) */
			SPI1_SEND_BY_WAIT_Demo_RW((_uiWriteAddr & 0xFF0000) >> 16);	/* 发送扇区地址的高8bit */
			SPI1_SEND_BY_WAIT_Demo_RW((_uiWriteAddr & 0xFF00) >> 8);		/* 发送扇区地址中间8bit */
			SPI1_SEND_BY_WAIT_Demo_RW(_uiWriteAddr & 0xFF);				/* 发送扇区地址低8bit */

			for (i = 0; i < 256; i++)
			{
				SPI1_SEND_BY_WAIT_Demo_RW(*_pBuf++);					/* 发送数据 */
			}

			SPI_SSN_Set_High(SPI1);//置高SSN信号

			sf_WaitForWriteEnd();						/* 等待串行Flash内部写操作完成 */

			_uiWriteAddr += 256;
		}

		/* 进入写保护状态 */
		SPI_SSN_Set_Low(SPI1);//拉低SSN信号	
		SPI1_SEND_BY_WAIT_Demo_RW(CMD_DISWR);
		SPI_SSN_Set_High(SPI1);//置高SSN信号

		sf_WaitForWriteEnd();							/* 等待串行Flash内部写操作完成 */
}

/*
*********************************************************************************************************
*	函 数 名: sfErase
*	功能说明: 擦除串行Flash
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void sfErase(void)
{
	int32_t iTime1, iTime2;

	iTime1 = bsp_GetRunTime();	/* 记下开始时间 */
	sf_EraseChip();
	iTime2 = bsp_GetRunTime();	/* 记下结束时间 */

	/* 打印读速度 */
	printf("擦除串行Flash完成！, 耗时: %dms\r\n", iTime2 - iTime1);
	return;
}

/*
*********************************************************************************************************
*	函 数 名: sfWriteTest
*	功能说明: 写串行Flash测试
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void sfWriteTest(void)
{
	uint16_t i;
	int32_t iTime1, iTime2;
	uint8_t buf[TEST_SIZE];

	/* 填充测试缓冲区 */
	for (i = 0; i < TEST_SIZE; i++)
	{
		buf[i] = i;
	}

	/* 写EEPROM, 起始地址 = 0，数据长度为 256 */
	iTime1 = bsp_GetRunTime();	/* 记下开始时间 */
	if (sf_WriteBuffer(buf, TEST_ADDR, TEST_SIZE) == 0)
	{
		printf("写串行Flash出错！\r\n");
		return;
	}
	else
	{
		iTime2 = bsp_GetRunTime();	/* 记下结束时间 */
		printf("写串行Flash成功！\r\n");
	}


	/* 打印读速度 */
	printf("数据长度: %d字节, 写耗时: %dms, 写速度: %dB/s\r\n", TEST_SIZE, iTime2 - iTime1, (TEST_SIZE * 1000) / (iTime2 - iTime1));
}

