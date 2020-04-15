#include "bsp.h"

#define TEST_SIZE 4096
/* 4KbytesΪһ��Sector
16������Ϊ1��Block */
static uint8_t w25qxx_buf[TEST_SIZE];

SFLASH_T g_tSF;

/*******************************************************************
*������uint8_t w25qxx_init(void)
*������w25qxx��ʼ��
*���룺none
*�����none
*���أ�none
*�汾��V1.0
********************************************************************/
uint8_t w25qxx_init(void)
{
	SPI1_SEND_BY_WAIT_Demo_Master_init();//hspi init
	SPIx_SPICR2_SPIEN_Setable(SPI1, ENABLE);//HSPIģ��ʹ��
	
	w25qxx_wake_up();
		
	if(w25qxx_read_id() != W25Q16)
	{
		return 1;
	}
	
//	w25qxx_power_down();
	
	return 0;
}
///*******************************************************************
//*������void w25qxx_disable(void)
//*����������w25qxx
//*���룺none
//*�����none
//*���أ�none
//*���ߣ��쳤��
//*�汾��V1.0
//********************************************************************/
void w25qxx_disable(void)
{
		SPI_SSN_Set_High(SPI1);//�ø�SSN�ź�
}
///*******************************************************************
//*������void w25qxx_read(uint8_t *cbuf, uint32_t addr, uint16_t csize)  
//*������������
//*���룺cbuf����������ַ��addr����ַ��csize����ȡ�ĳ���
//*�����none
//*���أ�none
//*���ߣ��쳤��
//*�汾��V1.0
//********************************************************************/
void w25qxx_read(uint8_t *cbuf, uint32_t addr, uint16_t csize)   
{ 
 	uint16_t i;

	SPI_SSN_Set_Low(SPI1);//����SSN�ź�		
	
	SPI1_SEND_BY_WAIT_Demo_RW(READ_DATA);
	SPI1_SEND_BY_WAIT_Demo_RW((uint8_t)((addr) >> 16));
	SPI1_SEND_BY_WAIT_Demo_RW((uint8_t)((addr) >> 8));
	SPI1_SEND_BY_WAIT_Demo_RW((uint8_t)addr);
	
    for(i=0; i < csize; i++)
	{ 
        cbuf[i] = SPI1_SEND_BY_WAIT_Demo_RW(0xFF);		
    }
	
	SPI_SSN_Set_High(SPI1);//�ø�SSN�ź� 				    	      
}  
///***************************************************************************
//*������void w25qxx_write_page(uint8_t *cbuf, uint32_t addr, uint16_t csize)  
//*��������һҳд������
//*���룺cbuf����������ַ��addr����ַ��csize����ȡ�ĳ���
//*�����none
//*���أ�none
//*���ߣ��쳤��
//*�汾��V1.0
//*****************************************************************************/
void w25qxx_write_page(uint8_t *cbuf, uint32_t addr, uint16_t csize)   
{
	uint16_t i;
	
	w25qxx_write_enable(); 	

	SPI_SSN_Set_Low(SPI1);//����SSN�ź�		
	
	SPI1_SEND_BY_WAIT_Demo_RW(PAGE_PROGRAM);
	SPI1_SEND_BY_WAIT_Demo_RW((uint8_t)((addr) >> 16));
	SPI1_SEND_BY_WAIT_Demo_RW((uint8_t)((addr) >> 8));
	SPI1_SEND_BY_WAIT_Demo_RW((uint8_t)addr);
	for(i=0; i < csize; i++)
	{ 
        SPI1_SEND_BY_WAIT_Demo_RW(cbuf[i]);  
    }
	
	SPI_SSN_Set_High(SPI1);//�ø�SSN�ź�
	
	w25qxx_wait_busy();
}
///*******************************************************************************
//*������void w25qxx_write_no_check(uint8_t *cbuf, uint32_t addr, uint16_t csize)  
//*�����������д��
//*���룺cbuf����������ַ��addr����ַ��csize����ȡ�ĳ���
//*�����none
//*���أ�none
//*���ߣ��쳤��
//*�汾��V1.0
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
//*������void w25qxx_write(uint8_t *cbuf, uint32_t addr, uint16_t csize)  
//*������д��
//*���룺cbuf����������ַ��addr����ַ��csize����ȡ�ĳ���
//*�����none
//*���أ�none
//*���ߣ��쳤��
//*�汾��V1.0
//********************************************************************************/
void w25qxx_write(uint8_t *cbuf, uint32_t addr, uint16_t csize)   
{ 
	uint32_t sec_pos;	 /* ������ַ */
	uint16_t sec_off;	 /* ������ƫ�Ƶ�ַ */
	uint16_t sec_remain; /* ������ʣ���ַ */	   
 	uint16_t i;    

	/* ��ַ�Ƿ� */
	if(addr >= 2 * 1024 * 1024)
	{
		return;
	}
	
	sec_pos = addr / 4096;	        /* ������ַ */
	sec_off = addr % 4096;          /* �������ڵ�ƫ�� */
	sec_remain = 4096 - sec_off;    /* ����ʣ��ռ��С */   
	if(csize <= sec_remain)
	{
		sec_remain = csize;
	}
	while(1)
	{
		w25qxx_read(w25qxx_buf, sec_pos * 4096, 4096);/* ������������ */
		
		for(i = 0; i < sec_remain; i++)/* ����Ƿ���Ҫ���� */
		{
			if(w25qxx_buf[sec_off + i] != 0xFF)
			{
				break;
			}
		}

		if(i < sec_remain)/* ��Ҫ���� */
		{
			w25qxx_erase_sector(sec_pos);
			for(i = 0; i < sec_remain; i++)/* �������� */
			{
				w25qxx_buf[i + sec_off] = cbuf[i];
			}
			w25qxx_write_no_check(w25qxx_buf, sec_pos * 4096, 4096);
		}
		else
		{
			w25qxx_write_no_check(cbuf, addr, sec_remain);
		}
		
		if(csize == sec_remain)/* д����� */
		{
			break;
		}
		else
		{
			sec_pos++;				    /* ������ַ��1 */
			sec_off = 0;			    /* ƫ��λ��Ϊ0 */ 	 
		   	cbuf += sec_remain;  	    /* ָ��ƫ�� */
			addr += sec_remain;	        /* д��ַƫ�� */	   
		   	csize -= sec_remain;	    /* �ֽ����ݼ� */
			if(csize > 4096)
			{
				sec_remain = 4096;/* ��һ����������д���� */
			}
			else 
			{
				sec_remain = csize;/* ��һ����������д�� */
			}
		}
	} 
}
/*******************************************************************
*������uint16_t w25qxx_read_id(void)
*��������ȡID
*���룺none
*�����none
*���أ�none
*���ߣ��쳤��
*�汾��V1.0
********************************************************************/
uint16_t w25qxx_read_id(void)
{
	uint16_t id = 0;

  SPI_SSN_Set_Low(SPI1);//����SSN�ź�
				    
	SPI1_SEND_BY_WAIT_Demo_RW(READ_DEVICE_ID);
	SPI1_SEND_BY_WAIT_Demo_RW(0); 
	SPI1_SEND_BY_WAIT_Demo_RW(0);
	SPI1_SEND_BY_WAIT_Demo_RW(0);	
	id |= SPI1_SEND_BY_WAIT_Demo_RW(0xFF) << 8;
	id |= SPI1_SEND_BY_WAIT_Demo_RW(0xFF);	
	
	SPI_SSN_Set_High(SPI1);//�ø�SSN�ź�
	
	return id;
}
///*******************************************************************
//*������uint16_t w25qxx_read_id(void)
//*��������ȡ״̬�Ĵ���
//		BIT7  6   5:4   3   2   1   0
//		BPL  AAI  RES  BP1 BP0 WEL BUSY
//*���룺none
//*�����none
//*���أ�״̬
//*���ߣ��쳤��
//*�汾��V1.0
//********************************************************************/
uint8_t w25qxx_read_sr(void)   
{  
	uint8_t byte = 0;  

	SPI_SSN_Set_Low(SPI1);//����SSN�ź�		
                            
	SPI1_SEND_BY_WAIT_Demo_RW(READ_STATUS_REG);
	byte = SPI1_SEND_BY_WAIT_Demo_RW(0xFF);
	
	SPI_SSN_Set_High(SPI1);//�ø�SSN�ź�  
	
	return byte;   
} 
///*******************************************************************
//*������void w25qxx_write_sr(uint8_t sr)
//*������д״̬�Ĵ���
//*���룺Ҫд���ֵ
//*�����none
//*���أ�none
//*���ߣ��쳤��
//*�汾��V1.0
//********************************************************************/
void w25qxx_write_sr(uint8_t sr)   
{  
	
	SPI_SSN_Set_Low(SPI1);//����SSN�ź�	

	SPI1_SEND_BY_WAIT_Demo_RW(ENABLE_WRITE_STATUS_REG);             
	SPI1_SEND_BY_WAIT_Demo_RW(WRITE_STATUS_REG);
	SPI1_SEND_BY_WAIT_Demo_RW(sr);
	
	SPI_SSN_Set_High(SPI1);//�ø�SSN�ź�  
} 
///*******************************************************************
//*������void w25qxx_write_enable(void)  
//*������дʹ��
//*���룺none
//*�����none
//*���أ�none
//*���ߣ��쳤��
//*�汾��V1.0
//********************************************************************/
void w25qxx_write_enable(void)   
{  
	SPI_SSN_Set_Low(SPI1);//����SSN�ź�		
                            
	SPI1_SEND_BY_WAIT_Demo_RW(WRITE_ENABLE);
	
	SPI_SSN_Set_High(SPI1);//�ø�SSN�ź�
 
} 
///*******************************************************************
//*������void w25qxx_write_disable(void)  
//*������д��ֹ
//*���룺none
//*�����none
//*���أ�none
//*���ߣ��쳤��
//*�汾��V1.0
//********************************************************************/
void w25qxx_write_disable(void)   
{  
	SPI_SSN_Set_Low(SPI1);//����SSN�ź�		
                            
	SPI1_SEND_BY_WAIT_Demo_RW(WRITE_DISABLE);
	
	SPI_SSN_Set_High(SPI1);//�ø�SSN�ź�  
} 
///*******************************************************************
//*������void w25qxx_wait_busy(void)    
//*�������ȴ�����
//*���룺none
//*�����none
//*���أ�none
//*���ߣ��쳤��
//*�汾��V1.0
//********************************************************************/
void w25qxx_wait_busy(void)   
{   
	while((w25qxx_read_sr() & 0x01) == 0x01);
} 
///*******************************************************************
//*������void w25qxx_erase_chip(void) 
//*��������������оƬ
//*���룺none
//*�����none
//*���أ�none
//*���ߣ��쳤��
//*�汾��V1.0
//********************************************************************/
void w25qxx_erase_chip(void)   
{  
	w25qxx_write_enable(); 
	w25qxx_wait_busy();	
	
	SPI_SSN_Set_Low(SPI1);//����SSN�ź�		
                            
	SPI1_SEND_BY_WAIT_Demo_RW(CHIP_ERASE);
	
	SPI_SSN_Set_High(SPI1);//�ø�SSN�ź�  
	w25qxx_wait_busy();
} 
///*******************************************************************
//*������void w25qxx_erase_sector(uint32_t addr) 
//*����������һ������
//*���룺��ַ
//*�����none
//*���أ�none
//*���ߣ��쳤��
//*�汾��V1.0
//********************************************************************/
void w25qxx_erase_sector(uint32_t addr)    
{  
	addr *= 4096;
	
	w25qxx_write_enable(); 
	w25qxx_wait_busy();	

	SPI_SSN_Set_Low(SPI1);//����SSN�ź�		
                            
	SPI1_SEND_BY_WAIT_Demo_RW(SECTOR_ERASE_4KB);
	SPI1_SEND_BY_WAIT_Demo_RW((uint8_t)((addr) >> 16));
	SPI1_SEND_BY_WAIT_Demo_RW((uint8_t)((addr) >> 8));
	SPI1_SEND_BY_WAIT_Demo_RW((uint8_t)addr);
	
	SPI_SSN_Set_High(SPI1);//�ø�SSN�ź�  
	w25qxx_wait_busy();
}
///*******************************************************************
//*������void w25qxx_power_down(void)    
//*�������������ģʽ
//*���룺��ַ
//*�����none
//*���أ�none
//*���ߣ��쳤��
//*�汾��V1.0
//********************************************************************/
void w25qxx_power_down(void)    
{  
	SPI_SSN_Set_Low(SPI1);//����SSN�ź�	
                            
	SPI1_SEND_BY_WAIT_Demo_RW(W25X_POWER_DOWN);

	SPI_SSN_Set_High(SPI1);//�ø�SSN�ź�
//	bsp_DelayUS(1);
	delay_ms(1);//�����ʱ
}
/*******************************************************************
*������void w25qxx_wake_up(void)    
*����������
*���룺��ַ
*�����none
*���أ�none
*�汾��V1.0
********************************************************************/
void w25qxx_wake_up(void)    
{  
	SPI_SSN_Set_Low(SPI1);//����SSN�ź�
                            
	SPI1_SEND_BY_WAIT_Demo_RW(W25X_WAKE_UP);

	SPI_SSN_Set_High(SPI1);//�ø�SSN�ź�
	delay_ms(1);//�����ʱ
}

//*********************************************************************//

/*
*********************************************************************************************************
*	�� �� ��: DemoSpiFlash
*	����˵��: ����EEPROM��д����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DemoSpiFlash(void)
{
//	uint32_t uiReadPageNo = 0;

	sf_ReadInfo();
	
	/* ��⴮��Flash OK */
	printf("��⵽����Flash, ID = %08X, �ͺ�: %s \r\n", g_tSF.ChipID , g_tSF.ChipName);
	printf("    ���� : %dM�ֽ�, ������С : %d�ֽ�\r\n", g_tSF.TotalSize/(1024*1024), g_tSF.PageSize);
	
	printf("\r\n��1 - ������Flash, ��ַ:0x%X,����:%d�ֽڡ�\r\n", TEST_ADDR, TEST_SIZE);
	sfReadTest();		/* ������Flash���ݣ�����ӡ������������ */
	
	printf("\r\n��2 - д����Flash, ��ַ:0x%X,����:%d�ֽڡ�\r\n", TEST_ADDR, TEST_SIZE);
	sfWriteTest();		/* д����Flash���ݣ�����ӡд���ٶ� */

	printf("\r\n��3 - ������������Flash��\r\n");
	sfErase();			/* ��������Flash���ݣ�ʵ���Ͼ���д��ȫ0xFF */

	printf("\r\n��4 - д��������Flash, ȫ0xAA��\r\n");
	sfWriteAll(0xAA);			/* д����Flash���ݣ�����ӡд���ٶ� */
	
	printf("\r\n��1 - ������Flash, ��ַ:0x%X,����:%d�ֽڡ�\r\n", TEST_ADDR, TEST_SIZE);
	sfReadTest();		/* ������Flash���ݣ�����ӡ������������ */
		
	printf("\r\n��5 - д��������Flash, ȫ0x66��\r\n");
	sfWriteAll(0x66);		

	printf("\r\n��6 - ����������Flash, %dM�ֽڡ�\r\n", g_tSF.TotalSize/(1024*1024));
	sfTestReadSpeed();		/* ����������Flash���ݣ������ٶ� */

//				case 'Z': /* ��ȡǰ1K */
//					if (uiReadPageNo > 0)
//					{
//						uiReadPageNo--;
//					}
//					else
//					{
//						printf("�Ѿ�����ǰ\r\n");
//					}
//					sfViewData(uiReadPageNo * 1024);
//					break;

//				case 'X': /* ��ȡ��1K */
//					if (uiReadPageNo < g_tSF.TotalSize / 1024 - 1)
//					{
//						uiReadPageNo++;
//					}
//					else
//					{
//						printf("�Ѿ������\r\n");
//					}
//					sfViewData(uiReadPageNo * 1024);
//					break;
		
}
		
/*
*********************************************************************************************************
*	�� �� ��: sf_ReadBuffer
*	����˵��: ������ȡ�����ֽڡ��ֽڸ������ܳ���оƬ������
*	��    ��:  	_pBuf : ����Դ��������
*				_uiReadAddr ���׵�ַ
*				_usSize �����ݸ���, ���Դ���PAGE_SIZE,���ǲ��ܳ���оƬ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void sf_ReadBuffer(uint8_t * _pBuf, uint32_t _uiReadAddr, uint32_t _uiSize)
{
	/* �����ȡ�����ݳ���Ϊ0���߳�������Flash��ַ�ռ䣬��ֱ�ӷ��� */
	if ((_uiSize == 0) ||(_uiReadAddr + _uiSize) > g_tSF.TotalSize)
	{
		return;
	}

	/* ������������ */
  SPI_SSN_Set_Low(SPI1);//����SSN�ź�
	SPI1_SEND_BY_WAIT_Demo_RW(CMD_READ);							/* ���Ͷ����� */
	SPI1_SEND_BY_WAIT_Demo_RW((_uiReadAddr & 0xFF0000) >> 16);	/* ����������ַ�ĸ�8bit */
	SPI1_SEND_BY_WAIT_Demo_RW((_uiReadAddr & 0xFF00) >> 8);		/* ����������ַ�м�8bit */
	SPI1_SEND_BY_WAIT_Demo_RW(_uiReadAddr & 0xFF);				/* ����������ַ��8bit */
	while (_uiSize--)
	{
		*_pBuf++ = SPI1_SEND_BY_WAIT_Demo_RW(0);			/* ��һ���ֽڲ��洢��pBuf�������ָ���Լ�1 */
	}
	SPI_SSN_Set_High(SPI1);//�ø�SSN�ź�
}

/*
*********************************************************************************************************
*	�� �� ��: sfReadTest
*	����˵��: ������Flash����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void sfReadTest(void)
{
	uint16_t i;
	int32_t iTime1, iTime2;
	uint8_t buf[TEST_SIZE];

	/* ��ʼ��ַ = 0�� ���ݳ���Ϊ 256 */
	iTime1 = bsp_GetRunTime();	/* ���¿�ʼʱ�� */
	sf_ReadBuffer(buf, TEST_ADDR, TEST_SIZE);
	iTime2 = bsp_GetRunTime();	/* ���½���ʱ�� */
	printf("������Flash�ɹ����������£�\r\n");

	/* ��ӡ���� */
	for (i = 0; i < TEST_SIZE; i++)
	{
		printf(" %02X", buf[i]);

		if ((i & 31) == 31)
		{
			printf("\r\n");	/* ÿ����ʾ16�ֽ����� */
		}
		else if ((i & 31) == 15)
		{
			printf(" - ");
		}
	}

	/* ��ӡ���ٶ� */
	printf("���ݳ���: %d�ֽ�, ����ʱ: %dms, ���ٶ�: %d Bytes/s\r\n", TEST_SIZE, iTime2 - iTime1, (TEST_SIZE * 1000) / (iTime2 - iTime1));
}

/*
*********************************************************************************************************
*	�� �� ��: sf_ReadInfo
*	����˵��: ��ȡ����ID,�������������
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void sf_ReadInfo(void)
{
	/* �Զ�ʶ����Flash�ͺ� */
	{
		g_tSF.ChipID = sf_ReadID();	/* оƬID */

		switch (g_tSF.ChipID)
		{
			case SST25VF016B_ID:
				strcpy(g_tSF.ChipName, "SST25VF016B");
				g_tSF.TotalSize = 2 * 1024 * 1024;	/* ������ = 2M */
				g_tSF.PageSize = 4 * 1024;			/* ҳ���С = 4K */
				break;

			case MX25L1606E_ID:
				strcpy(g_tSF.ChipName, "MX25L1606E");
				g_tSF.TotalSize = 2 * 1024 * 1024;	/* ������ = 2M */
				g_tSF.PageSize = 4 * 1024;			/* ҳ���С = 4K */
				break;

			case W25Q64BV_ID:
				strcpy(g_tSF.ChipName, "W25Q64BV");
				g_tSF.TotalSize = 8 * 1024 * 1024;	/* ������ = 8M */
				g_tSF.PageSize = 4 * 1024;			/* ҳ���С = 4K */
				break;
			
			case W25Q32BV_ID:
				strcpy(g_tSF.ChipName, "W25Q32BV");
				g_tSF.TotalSize = 4 * 1024 * 1024;	/* ������ = 8M */
				g_tSF.PageSize = 4 * 1024;			/* ҳ���С = 4K */
				break;		
			
			case W25Q16BV_ID:
				strcpy(g_tSF.ChipName, "W25Q16BV");
				g_tSF.TotalSize = 2 * 1024 * 1024;	/* ������ = 8M */
				g_tSF.PageSize = 4 * 1024;			/* ҳ���С = 4K */
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
*	�� �� ��: sf_ReadID
*	����˵��: ��ȡ����ID
*	��    ��:  ��
*	�� �� ֵ: 32bit������ID (���8bit��0����ЧIDλ��Ϊ24bit��
*********************************************************************************************************
*/
uint32_t sf_ReadID(void)
{
	uint32_t uiID;
	uint8_t id1, id2, id3;

	SPI_SSN_Set_Low(SPI1);//����SSN�ź�
	SPI1_SEND_BY_WAIT_Demo_RW(CMD_RDID);								/* ���Ͷ�ID���� */
	id1 = SPI1_SEND_BY_WAIT_Demo_RW(0);					/* ��ID�ĵ�1���ֽ� */
	id2 = SPI1_SEND_BY_WAIT_Demo_RW(0);					/* ��ID�ĵ�2���ֽ� */
	id3 = SPI1_SEND_BY_WAIT_Demo_RW(0);					/* ��ID�ĵ�3���ֽ� */
	SPI_SSN_Set_High(SPI1);//�ø�SSN�ź�

	uiID = ((uint32_t)id1 << 16) | ((uint32_t)id2 << 8) | id3;

	return uiID;
}

/*
*********************************************************************************************************
*	�� �� ��: sfTestReadSpeed
*	����˵��: ���Դ���Flash���ٶȡ���ȡ��������Flash�����ݣ�����ӡ���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void sfTestReadSpeed(void)
{
	uint16_t i;
	int32_t iTime1, iTime2;
	uint8_t buf[TEST_SIZE];
	uint32_t uiAddr;

	/* ��ʼ��ַ = 0�� ���ݳ���Ϊ 256 */
	iTime1 = bsp_GetRunTime();	/* ���¿�ʼʱ�� */
	uiAddr = 0;
	for (i = 0; i < g_tSF.TotalSize / TEST_SIZE; i++, uiAddr += TEST_SIZE)
	{
		sf_ReadBuffer(buf, uiAddr, TEST_SIZE);
		printf(".");
		IWDT_Clr(); 
	}
	iTime2 = bsp_GetRunTime();	/* ���½���ʱ�� */

	/* ��ӡ���ٶ� */
	printf("���ݳ���: %d�ֽ�, ����ʱ: %dms, ���ٶ�: %d Bytes/s\r\n", g_tSF.TotalSize, iTime2 - iTime1, (g_tSF.TotalSize * 1000) / (iTime2 - iTime1));
}

/*
*********************************************************************************************************
*	�� �� ��: sfWriteAll
*	����˵��: д����EEPROMȫ������
*	��    �Σ�_ch : д�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void sfWriteAll(uint8_t _ch)
{
	uint16_t i;
	int32_t iTime1, iTime2;
	uint8_t buf[4 * 1024];

	/* �����Ի����� */
	for (i = 0; i < TEST_SIZE; i++)
	{
		buf[i] = _ch;
	}

	iTime1 = bsp_GetRunTime();	/* ���¿�ʼʱ�� */
	for (i = 0; i < g_tSF.TotalSize / g_tSF.PageSize; i++)
	{
		if (sf_WriteBuffer(buf, i * g_tSF.PageSize, g_tSF.PageSize) == 0)
		{
			printf("д����Flash����\r\n");
			return;
		}
		printf(".");
		IWDT_Clr(); 
	}
	iTime2 = bsp_GetRunTime();	/* ���½���ʱ�� */

	/* ��ӡ���ٶ� */
	printf("���ݳ���: %dK�ֽ�, д��ʱ: %dms, д�ٶ�: %dB/s\r\n", g_tSF.TotalSize / 1024, iTime2 - iTime1, (g_tSF.TotalSize * 1000) / (iTime2 - iTime1));
}

/*
*********************************************************************************************************
*	�� �� ��: sf_WriteBuffer
*	����˵��: д1��������У��,�������ȷ������д���Ρ��������Զ���ɲ���������
*	��    ��:  	_pBuf : ����Դ��������
*				_uiWrAddr ��Ŀ�������׵�ַ
*				_usSize �����ݸ��������ܳ���ҳ���С
*	�� �� ֵ: 1 : �ɹ��� 0 �� ʧ��
*********************************************************************************************************
*/
uint8_t sf_WriteBuffer(uint8_t* _pBuf, uint32_t _uiWriteAddr, uint16_t _usWriteSize)
{
	uint16_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

	Addr = _uiWriteAddr % g_tSF.PageSize;
	count = g_tSF.PageSize - Addr;
	NumOfPage =  _usWriteSize / g_tSF.PageSize;
	NumOfSingle = _usWriteSize % g_tSF.PageSize;

	if (Addr == 0) /* ��ʼ��ַ��ҳ���׵�ַ  */
	{
		if (NumOfPage == 0) /* ���ݳ���С��ҳ���С */
		{
			if (sf_AutoWritePage(_pBuf, _uiWriteAddr, _usWriteSize) == 0)
			{
				return 0;
			}
		}
		else 	/* ���ݳ��ȴ��ڵ���ҳ���С */
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
	else  /* ��ʼ��ַ����ҳ���׵�ַ  */
	{
		if (NumOfPage == 0) /* ���ݳ���С��ҳ���С */
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
		else	/* ���ݳ��ȴ��ڵ���ҳ���С */
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
	return 1;	/* �ɹ� */
}

/*
*********************************************************************************************************
*	�� �� ��: sf_AutoWritePage
*	����˵��: д1��PAGE��У��,�������ȷ������д���Ρ��������Զ���ɲ���������
*	��    ��:  	_pBuf : ����Դ��������
*				_uiWriteAddr ��Ŀ�������׵�ַ
*				_usSize �����ݸ��������ܳ���ҳ���С
*	�� �� ֵ: 0 : ���� 1 �� �ɹ�
*********************************************************************************************************
*/
static uint8_t sf_AutoWritePage(uint8_t *_ucpSrc, uint32_t _uiWrAddr, uint16_t _usWrLen)
{
	uint16_t i;
	uint16_t j;					/* ������ʱ */
	uint32_t uiFirstAddr;		/* ������ַ */
	uint8_t ucNeedErase;		/* 1��ʾ��Ҫ���� */
	uint8_t cRet;

	/* ����Ϊ0ʱ����������,ֱ����Ϊ�ɹ� */
	if (_usWrLen == 0)
	{
		return 1;
	}

	/* ���ƫ�Ƶ�ַ����оƬ�������˳� */
	if (_uiWrAddr >= g_tSF.TotalSize)
	{
		return 0;
	}

	/* ������ݳ��ȴ����������������˳� */
	if (_usWrLen > g_tSF.PageSize)
	{
		return 0;
	}

	/* ���FLASH�е�����û�б仯,��дFLASH */
	sf_ReadBuffer(w25qxx_buf, _uiWrAddr, _usWrLen);
	if (memcmp(w25qxx_buf, _ucpSrc, _usWrLen) == 0)
	{
		return 1;
	}

	/* �ж��Ƿ���Ҫ�Ȳ������� */
	/* ����������޸�Ϊ�����ݣ�����λ���� 1->0 ���� 0->0, ���������,���Flash���� */
	ucNeedErase = 0;
	if (sf_NeedErase(w25qxx_buf, _ucpSrc, _usWrLen))
	{
		ucNeedErase = 1;
	}

	uiFirstAddr = _uiWrAddr & (~(g_tSF.PageSize - 1));

	if (_usWrLen == g_tSF.PageSize)		/* ������������д */
	{
		for	(i = 0; i < g_tSF.PageSize; i++)
		{
			w25qxx_buf[i] = _ucpSrc[i];
		}
	}
	else						/* ��д�������� */
	{
		/* �Ƚ��������������ݶ��� */
		sf_ReadBuffer(w25qxx_buf, uiFirstAddr, g_tSF.PageSize);

		/* ���������ݸ��� */
		i = _uiWrAddr & (g_tSF.PageSize - 1);
		memcpy(&w25qxx_buf[i], _ucpSrc, _usWrLen);
	}

	/* д��֮�����У�飬�������ȷ����д�����3�� */
	cRet = 0;
	for (i = 0; i < 3; i++)
	{

		/* ����������޸�Ϊ�����ݣ�����λ���� 1->0 ���� 0->0, ���������,���Flash���� */
		if (ucNeedErase == 1)
		{
			sf_EraseSector(uiFirstAddr);		/* ����1������ */
		}

		/* ���һ��PAGE */
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

			/* ʧ�ܺ��ӳ�һ��ʱ�������� */
			for (j = 0; j < 10000; j++);
		}
	}

	return cRet;
}

/*
*********************************************************************************************************
*	�� �� ��: sf_NeedErase
*	����˵��: �ж�дPAGEǰ�Ƿ���Ҫ�Ȳ�����
*	��    ��:   _ucpOldBuf �� ������
*			   _ucpNewBuf �� ������
*			   _uiLen �����ݸ��������ܳ���ҳ���С
*	�� �� ֵ: 0 : ����Ҫ������ 1 ����Ҫ����
*********************************************************************************************************
*/
uint8_t sf_NeedErase(uint8_t * _ucpOldBuf, uint8_t *_ucpNewBuf, uint16_t _usLen)
{
	uint16_t i;
	uint8_t ucOld;

	/*
	�㷨��1����old ��, new ����
	      old    new
		  1101   0101
	~     1111
		= 0010   0101

	�㷨��2��: old �󷴵Ľ���� new λ��
		  0010   old
	&	  0101   new
		 =0000

	�㷨��3��: ���Ϊ0,���ʾ�������. �����ʾ��Ҫ����
	*/

	for (i = 0; i < _usLen; i++)
	{
		ucOld = *_ucpOldBuf++;
		ucOld = ~ucOld;

		/* ע������д��: if (ucOld & (*_ucpNewBuf++) != 0) */
		if ((ucOld & (*_ucpNewBuf++)) != 0)
		{
			return 1;
		}
	}
	return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: sf_EraseChip
*	����˵��: ��������оƬ
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void sf_EraseChip(void)
{
	sf_WriteEnable();								/* ����дʹ������ */

	/* ������������ */
	SPI_SSN_Set_Low(SPI1);//����SSN�ź�	
	SPI1_SEND_BY_WAIT_Demo_RW(CMD_BE);							/* ������Ƭ�������� */
	SPI_SSN_Set_High(SPI1);//�ø�SSN�ź�

	sf_WaitForWriteEnd();							/* �ȴ�����Flash�ڲ�д������� */
}


/*
*********************************************************************************************************
*	�� �� ��: sf_WriteEnable
*	����˵��: ����������дʹ������
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void sf_WriteEnable(void)
{
  SPI_SSN_Set_Low(SPI1);//����SSN�ź�	
	SPI1_SEND_BY_WAIT_Demo_RW(CMD_WREN);								/* �������� */
	SPI_SSN_Set_High(SPI1);//�ø�SSN�ź�
}

/*
*********************************************************************************************************
*	�� �� ��: sf_WaitForWriteEnd
*	����˵��: ����ѭ����ѯ�ķ�ʽ�ȴ������ڲ�д�������
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void sf_WaitForWriteEnd(void)
{
	SPI_SSN_Set_Low(SPI1);//����SSN�ź�	
	SPI1_SEND_BY_WAIT_Demo_RW(CMD_RDSR);							/* ������� ��״̬�Ĵ��� */
	while((SPI1_SEND_BY_WAIT_Demo_RW(0) & WIP_FLAG) == SET);	/* �ж�״̬�Ĵ�����æ��־λ */
	SPI_SSN_Set_High(SPI1);//�ø�SSN�ź�
}

/*
*********************************************************************************************************
*	�� �� ��: sf_EraseSector
*	����˵��: ����ָ��������
*	��    ��:  _uiSectorAddr : ������ַ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void sf_EraseSector(uint32_t _uiSectorAddr)
{
	sf_WriteEnable();								/* ����дʹ������ */

	/* ������������ */
	SPI_SSN_Set_Low(SPI1);//����SSN�ź�	
	SPI1_SEND_BY_WAIT_Demo_RW(CMD_SE);								/* ���Ͳ������� */
	SPI1_SEND_BY_WAIT_Demo_RW((_uiSectorAddr & 0xFF0000) >> 16);	/* ����������ַ�ĸ�8bit */
	SPI1_SEND_BY_WAIT_Demo_RW((_uiSectorAddr & 0xFF00) >> 8);		/* ����������ַ�м�8bit */
	SPI1_SEND_BY_WAIT_Demo_RW(_uiSectorAddr & 0xFF);				/* ����������ַ��8bit */
	SPI_SSN_Set_High(SPI1);//�ø�SSN�ź�

	sf_WaitForWriteEnd();							/* �ȴ�����Flash�ڲ�д������� */
}

/*
*********************************************************************************************************
*	�� �� ��: sf_CmpData
*	����˵��: �Ƚ�Flash������.
*	��    ��:  	_ucpTar : ���ݻ�����
*				_uiSrcAddr ��Flash��ַ
*				_uiSize �����ݸ���, ���Դ���PAGE_SIZE,���ǲ��ܳ���оƬ������
*	�� �� ֵ: 0 = ���, 1 = ����
*********************************************************************************************************
*/
uint8_t sf_CmpData(uint32_t _uiSrcAddr, uint8_t *_ucpTar, uint32_t _uiSize)
{
	uint8_t ucValue;

	/* �����ȡ�����ݳ���Ϊ0���߳�������Flash��ַ�ռ䣬��ֱ�ӷ��� */
	if ((_uiSrcAddr + _uiSize) > g_tSF.TotalSize)
	{
		return 1;
	}

	if (_uiSize == 0)
	{
		return 0;
	}

	SPI_SSN_Set_Low(SPI1);//����SSN�ź�	
	SPI1_SEND_BY_WAIT_Demo_RW(CMD_READ);							/* ���Ͷ����� */
	SPI1_SEND_BY_WAIT_Demo_RW((_uiSrcAddr & 0xFF0000) >> 16);		/* ����������ַ�ĸ�8bit */
	SPI1_SEND_BY_WAIT_Demo_RW((_uiSrcAddr & 0xFF00) >> 8);		/* ����������ַ�м�8bit */
	SPI1_SEND_BY_WAIT_Demo_RW(_uiSrcAddr & 0xFF);					/* ����������ַ��8bit */
	while (_uiSize--)
	{
		/* ��һ���ֽ� */
		ucValue = SPI1_SEND_BY_WAIT_Demo_RW(0);
		if (*_ucpTar++ != ucValue)
		{
			SPI_SSN_Set_High(SPI1);//�ø�SSN�ź�
			return 1;
		}
	}
	SPI_SSN_Set_High(SPI1);//�ø�SSN�ź�
	return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: sf_PageWrite
*	����˵��: ��һ��page��д�������ֽڡ��ֽڸ������ܳ���ҳ���С��4K)
*	��    ��:  	_pBuf : ����Դ��������
*				_uiWriteAddr ��Ŀ�������׵�ַ
*				_usSize �����ݸ��������ܳ���ҳ���С
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void sf_PageWrite(uint8_t * _pBuf, uint32_t _uiWriteAddr, uint16_t _usSize)
{
	  uint32_t i, j;
		for (j = 0; j < _usSize / 256; j++)
		{
			sf_WriteEnable();								/* ����дʹ������ */

			SPI_SSN_Set_Low(SPI1);//����SSN�ź�	
			SPI1_SEND_BY_WAIT_Demo_RW(0x02);								/* ����AAI����(��ַ�Զ����ӱ��) */
			SPI1_SEND_BY_WAIT_Demo_RW((_uiWriteAddr & 0xFF0000) >> 16);	/* ����������ַ�ĸ�8bit */
			SPI1_SEND_BY_WAIT_Demo_RW((_uiWriteAddr & 0xFF00) >> 8);		/* ����������ַ�м�8bit */
			SPI1_SEND_BY_WAIT_Demo_RW(_uiWriteAddr & 0xFF);				/* ����������ַ��8bit */

			for (i = 0; i < 256; i++)
			{
				SPI1_SEND_BY_WAIT_Demo_RW(*_pBuf++);					/* �������� */
			}

			SPI_SSN_Set_High(SPI1);//�ø�SSN�ź�

			sf_WaitForWriteEnd();						/* �ȴ�����Flash�ڲ�д������� */

			_uiWriteAddr += 256;
		}

		/* ����д����״̬ */
		SPI_SSN_Set_Low(SPI1);//����SSN�ź�	
		SPI1_SEND_BY_WAIT_Demo_RW(CMD_DISWR);
		SPI_SSN_Set_High(SPI1);//�ø�SSN�ź�

		sf_WaitForWriteEnd();							/* �ȴ�����Flash�ڲ�д������� */
}

/*
*********************************************************************************************************
*	�� �� ��: sfErase
*	����˵��: ��������Flash
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void sfErase(void)
{
	int32_t iTime1, iTime2;

	iTime1 = bsp_GetRunTime();	/* ���¿�ʼʱ�� */
	sf_EraseChip();
	iTime2 = bsp_GetRunTime();	/* ���½���ʱ�� */

	/* ��ӡ���ٶ� */
	printf("��������Flash��ɣ�, ��ʱ: %dms\r\n", iTime2 - iTime1);
	return;
}

/*
*********************************************************************************************************
*	�� �� ��: sfWriteTest
*	����˵��: д����Flash����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void sfWriteTest(void)
{
	uint16_t i;
	int32_t iTime1, iTime2;
	uint8_t buf[TEST_SIZE];

	/* �����Ի����� */
	for (i = 0; i < TEST_SIZE; i++)
	{
		buf[i] = i;
	}

	/* дEEPROM, ��ʼ��ַ = 0�����ݳ���Ϊ 256 */
	iTime1 = bsp_GetRunTime();	/* ���¿�ʼʱ�� */
	if (sf_WriteBuffer(buf, TEST_ADDR, TEST_SIZE) == 0)
	{
		printf("д����Flash����\r\n");
		return;
	}
	else
	{
		iTime2 = bsp_GetRunTime();	/* ���½���ʱ�� */
		printf("д����Flash�ɹ���\r\n");
	}


	/* ��ӡ���ٶ� */
	printf("���ݳ���: %d�ֽ�, д��ʱ: %dms, д�ٶ�: %dB/s\r\n", TEST_SIZE, iTime2 - iTime1, (TEST_SIZE * 1000) / (iTime2 - iTime1));
}

