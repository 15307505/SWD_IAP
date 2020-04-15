#ifndef BSP_W25QXX_H
#define BSP_W25QXX_H

#define TEST_ADDR		0			/* ��д���Ե�ַ */
#define TEST_SIZE		4096		/* ��д�������ݴ�С */

#define W25Q80 	                      0xEF13 	
#define W25Q16 	                      0xEF14
#define W25Q32 	                      0xEF15
#define W25Q64 	                      0xEF16
#define W25Q128	                      0xEF17  

#define READ_DATA                     0x03       
#define HIGH_SPEED_READ_DATA          0x0B    
#define SECTOR_ERASE_4KB              0x20     
#define BLOCK_ERASE_32KB              0x52       
#define BLOCK_ERASE_64KB              0xD8       
#define CHIP_ERASE                    0xC7     

#define PAGE_PROGRAM                  0x02    
#define READ_STATUS_REG               0x05       
#define ENABLE_WRITE_STATUS_REG       0x50
#define WRITE_STATUS_REG              0x01      

#define W25X_POWER_DOWN			          0xB9 
#define W25X_WAKE_UP	                0xAB 

#define WRITE_ENABLE                  0x06      
#define WRITE_DISABLE                 0x04      
#define READ_DEVICE_ID                0x90      

#define READ_JEDEC_ID                 0x9F     

#define EBSY                          0X70      
#define DBSY                          0X80  

#define WIP_FLAG      0x01		/* ״̬�Ĵ����е����ڱ�̱�־��WIP) */

#define CMD_AAI       0xAD  	/* AAI �������ָ��(FOR SST25VF016B) */
#define CMD_DISWR	    0x04		/* ��ֹд, �˳�AAI״̬ */
#define CMD_EWRSR	    0x50		/* ����д״̬�Ĵ��������� */
#define CMD_WRSR      0x01  	/* д״̬�Ĵ������� */
#define CMD_WREN      0x06		/* дʹ������ */
#define CMD_READ      0x03  	/* ������������ */
#define CMD_RDSR      0x05		/* ��״̬�Ĵ������� */
#define CMD_RDID      0x9F		/* ������ID���� */
#define CMD_SE        0x20		/* ������������ */
#define CMD_BE        0xC7		/* ������������ */
#define DUMMY_BYTE    0xA5		/* ���������Ϊ����ֵ�����ڶ����� */

#define SF_MAX_PAGE_SIZE	(4 * 1024)

/* ���崮��Flash ID */
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
	uint32_t ChipID;		/* оƬID */
	char ChipName[16];		/* оƬ�ͺ��ַ�������Ҫ������ʾ */
	uint32_t TotalSize;		/* ������ */
	uint16_t PageSize;		/* ҳ���С */
}SFLASH_T;

extern SFLASH_T g_tSF;

uint8_t w25qxx_init(void);
void w25qxx_disable(void);
void w25qxx_read(uint8_t *cbuf, uint32_t addr, uint16_t csize);
void w25qxx_write(uint8_t *cbuf, uint32_t addr, uint16_t csize);
void w25qxx_write_page(uint8_t *cbuf, uint32_t addr, uint16_t csize);
uint16_t w25qxx_read_id(void);
uint8_t w25qxx_read_sr(void);
void w25qxx_write_sr(uint8_t sr);
void w25qxx_write_enable(void);
void w25qxx_write_disable(void);
void w25qxx_wait_busy(void);
void w25qxx_erase_chip(void);
void w25qxx_erase_sector(uint32_t addr);
void w25qxx_power_down(void);
void w25qxx_wake_up(void);

void sf_ReadBuffer(uint8_t * _pBuf, uint32_t _uiReadAddr, uint32_t _uiSize);
void DemoSpiFlash(void);
void sfReadTest(void);
void sf_ReadInfo(void);
uint32_t sf_ReadID(void);
void sfTestReadSpeed(void);
void sfWriteAll(uint8_t _ch);
uint8_t sf_WriteBuffer(uint8_t* _pBuf, uint32_t _uiWriteAddr, uint16_t _usWriteSize);
static uint8_t sf_AutoWritePage(uint8_t *_ucpSrc, uint32_t _uiWrAddr, uint16_t _usWrLen);
uint8_t sf_NeedErase(uint8_t * _ucpOldBuf, uint8_t *_ucpNewBuf, uint16_t _usLen);
void sf_EraseChip(void);
void sf_WaitForWriteEnd(void);
void sf_WriteEnable(void);
void sf_EraseSector(uint32_t _uiSectorAddr);
uint8_t sf_CmpData(uint32_t _uiSrcAddr, uint8_t *_ucpTar, uint32_t _uiSize);
void sf_PageWrite(uint8_t * _pBuf, uint32_t _uiWriteAddr, uint16_t _usSize);
void sfErase(void);
void sfWriteTest(void);

#endif 
