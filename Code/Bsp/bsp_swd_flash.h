#ifndef __BSP_SWD_FLASH_H
#define __BSP_SWD_FLASH_H


// Keep in sync with the lists error_message and error_type
typedef enum {
    /* Shared errors */
    ERROR_SUCCESS = 0,
    ERROR_FAILURE,
    ERROR_INTERNAL,

    /* Target flash errors */
    ERROR_RESET,
    ERROR_ALGO_DL,
    ERROR_ALGO_DATA_SEQ,
    ERROR_INIT,
    ERROR_SECURITY_BITS,
    ERROR_UNLOCK,
    ERROR_ERASE_SECTOR,
    ERROR_ERASE_ALL,
    ERROR_WRITE,

    // Add new values here

    ERROR_COUNT
} error_t;





error_t target_flash_init(uint32_t flash_start);
error_t target_flash_uninit(void);
error_t target_flash_program_page(uint32_t addr, const uint8_t *buf, uint32_t size);
error_t target_flash_erase_sector(uint32_t addr);
error_t target_flash_erase_chip(void);


void SWD_UpData(void);










#endif



