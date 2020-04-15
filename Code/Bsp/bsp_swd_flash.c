#include "bsp.h"

//#include "algo/STM32F4xx_1024.c"
#include "algo/STM32F10x_128.c"
//#include "algo/FM33A0XX_FLASH256.c"

uint32_t Flash_Page_Size = 512;
uint32_t Flash_Start_Addr = 0x80000000;
uint8_t buff[1024] = {0};

extern const program_target_t flash_algo;

error_t target_flash_init(uint32_t flash_start)
{
    if (SWJ_SetTargetState(RESET_PROGRAM)) {
								printf("*RESET_PROGRAM_SUCCESS \r\n");
        return ERROR_RESET;
    }
    printf("*RESET_SUCCESS \r\n");
    // Download flash programming algorithm to target and initialise.
    if (SWJ_WriteMem(flash_algo.algo_start, (uint8_t *)flash_algo.algo_blob, flash_algo.algo_size)) {
        printf("*ERROR_ALGO_DL \r\n");
        return ERROR_ALGO_DL;
    }
    printf("*ALGO_DL_SUCCESS \r\n");
    if (swd_flash_syscall_exec(&flash_algo.sys_call_s, flash_algo.init, flash_start, 0, 0, 0)) {
								printf("*ERROR_INIT \r\n");
        return ERROR_INIT;
    }
    printf("*flash_init_SUCCESS \r\n");
    return ERROR_SUCCESS;
}


error_t target_flash_uninit(void)
{
    SWJ_SetTargetState(RESET_RUN);
    SW_Off();
		  
    return ERROR_SUCCESS;
}


error_t target_flash_program_page(uint32_t addr, const uint8_t *buf, uint32_t size)
{
    while (size > 0) {
        uint32_t write_size = size > flash_algo.program_buffer_size ? flash_algo.program_buffer_size : size;

        // Write page to buffer
        if (SWJ_WriteMem(flash_algo.program_buffer, (uint8_t *)buf, write_size)) {
            printf("*ERROR_ALGO_DATA_SEQ \r\n");
            return ERROR_ALGO_DATA_SEQ;
        }
								 printf("*SUCCESS_ALGO_DATA_SEQ \r\n");
        // Run flash programming
        if (swd_flash_syscall_exec(&flash_algo.sys_call_s,
                                    flash_algo.program_page,
                                    addr,
                                    flash_algo.program_buffer_size,
                                    flash_algo.program_buffer,
                                    0)) {
												printf("*ERROR_WRITE \r\n");
            return ERROR_WRITE;
        }
        printf("*SUCCESS_WRITE \r\n");
		addr += write_size;
		buf  += write_size;
		size -= write_size;
    }

    return ERROR_SUCCESS;
}

error_t target_flash_erase_sector(uint32_t addr)
{
    if (swd_flash_syscall_exec(&flash_algo.sys_call_s, flash_algo.erase_sector, addr, 0, 0, 0)) {
        printf("*ERROR_ERASE_SECTOR \r\n");
        return ERROR_ERASE_SECTOR;
    }
				printf("*SUCCESS_ERASE_SECTOR \r\n");
    return ERROR_SUCCESS;
}


error_t target_flash_erase_chip(void)
{
    error_t status = ERROR_SUCCESS;

    if (swd_flash_syscall_exec(&flash_algo.sys_call_s, flash_algo.erase_chip, 0, 0, 0, 0)) {
        printf("*ERROR_ERASE_ALL \r\n");
        return ERROR_ERASE_ALL;
    }
    printf("*SUCCESS;_ERASE_ALL \r\n");
    return status;
}

extern FIL file;
extern FILINFO FileInf;
extern unsigned int bw;	
extern void CloseFile(void);
extern  u8 Maintask_Status;

void SWD_UpData(void)
{
	OLED_ClrScr(0);
	OLED_DispStr(2,2,"进入升级模式" ,&tFont12);	

	SWJ_InitDebug();
	
	target_flash_init(Flash_Start_Addr);
	OLED_DispStr(2,18,"开始升级" ,&tFont12);

//	for(uint32_t addr = 0; addr < FileInf.fsize; addr += 1024) //擦部分扇区
//	{
//		OLED_DispStr(2,34,"开始擦扇区" ,&tFont12);
//		target_flash_erase_sector(0x00000000+addr); 
//	}

 target_flash_erase_chip();                        //擦所有扇区
	OLED_DispStr(2,34,"所有扇区擦除成功" ,&tFont12);

	for(uint32_t addr = 0; addr < FileInf.fsize; addr += 1024)   //读扇区内容
	{
		SWJ_ReadMem(0x0000000 + addr, buff, 1024);
		for(uint32_t i = 0; i < 1024; i++) printf("%02X ", buff[i]);
		printf("\r\n\r\n\r\n");
	}
	
	for(uint32_t addr = 0; addr < FileInf.fsize; addr += 1024)
	{
		memset(File_buf,0xFF,1024);
		OLED_DispStr(2,48,"开始写入Flash" ,&tFont12);
		f_read(&file, File_buf, 1024, &bw);
  for(uint32_t i = 0; i < 1024; i++) printf("%02X ", File_buf[i]);
		printf("\r\n\r\n\r\n");
  printf("addr:%d\r\n",addr);
  printf("\r\n\r\n\r\n");
		target_flash_program_page(0x00000000+addr, File_buf, 1024);
	}

	for(uint32_t addr = 0; addr < FileInf.fsize; addr += 1024)
	{
		SWJ_ReadMem(0x00000000 + addr, buff, 1024);
		for(uint32_t i = 0; i < 1024; i++) printf("%02X ", buff[i]);
		printf("\r\n\r\n\r\n");
	}
	
	SWJ_SetTargetState(RUN);
	
	OLED_ClrScr(0);
	OLED_DispStr(2,2,"升级成功" ,&tFont12);
	OLED_DispStr(2,18,"跳转用户区" ,&tFont12);
	
	Maintask_Status = State_Init;
	CloseFile();
}

