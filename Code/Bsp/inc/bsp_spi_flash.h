/*
*********************************************************************************************************
*
*	模块名称 : SPI接口串行FLASH 读写模块
*	文件名称 : bsp_spi_flash.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*********************************************************************************************************
*/

#ifndef _BSP_SPI_FLASH_H
#define _BSP_SPI_FLASH_H

/* 串行Flsh的片选GPIO端口  */
#define SF_RCC_CS 		RCC_AHB1Periph_GPIOA
#define SF_PORT_CS		GPIOA
#define SF_PIN_CS			GPIO_Pin_4
#define SF_CS_0()			SF_PORT_CS->BSRRH = SF_PIN_CS
#define SF_CS_1()			SF_PORT_CS->BSRRL = SF_PIN_CS


#define SF_MAX_PAGE_SIZE	(4 * 1024)

/* 定义串行Flash ID */
enum
{
	SST25VF016B_ID = 0xBF2541,
	MX25L1606E_ID  = 0xC22015,
	W25Q64BV_ID    = 0xEF4017,
	W25Q32BV_ID    = 0xEF4016,
	W25Q16BV_ID    = 0xEF4015
};

typedef struct
{
	uint32_t ChipID;		/* 芯片ID */
	char ChipName[16];		/* 芯片型号字符串，主要用于显示 */
	uint32_t TotalSize;		/* 总容量 */
	uint16_t PageSize;		/* 页面大小 */
}SFLASH_T;

void bsp_InitSFlash(void);
uint32_t sf_ReadID(void);
void sf_EraseChip(void);
void sf_EraseSector(uint32_t _uiSectorAddr);
void sf_PageWrite(uint8_t * _pBuf, uint32_t _uiWriteAddr, uint16_t _usSize);
uint8_t sf_WriteBuffer(uint8_t* _pBuf, uint32_t _uiWriteAddr, uint16_t _usWriteSize);
void sf_ReadBuffer(uint8_t * _pBuf, uint32_t _uiReadAddr, uint32_t _uiSize);
void sf_ReadInfo(void);

extern SFLASH_T g_tSF;

#endif
