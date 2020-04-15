#ifndef __BSP_SWD_H
#define __BSP_SWD_H



#define SWCLK_PORT             GPIOB
#define SWCLK_PIN              GPIO_Pin_4
#define SWCLK_NUM              4

#define SWDIO_PORT             GPIOB
#define SWDIO_PIN              GPIO_Pin_5
#define SWDIO_NUM              5

#define RESET_PORT             GPIOA
#define RESET_PIN              GPIO_Pin_4
#define RESET_NUM              4


#define SWCLK_SET_OUTPUT()     {SWCLK_PORT->MODER&=~(3<<(SWCLK_NUM*2));SWCLK_PORT->MODER|=1<<SWCLK_NUM*2;}

#define SWDIO_SET_OUTPUT()     {SWDIO_PORT->MODER&=~(3<<(SWDIO_NUM*2));SWDIO_PORT->MODER|=1<<SWDIO_NUM*2;}
#define SWDIO_SET_INPUT()      {SWDIO_PORT->MODER&=~(3<<(SWDIO_NUM*2));SWDIO_PORT->MODER|=0<<SWDIO_NUM*2;}

#define RESET_SET_OUTPUT()     {RESET_PORT->MODER&=~(3<<(RESET_NUM*2));RESET_PORT->MODER|=1<<RESET_NUM*2;}


#define SWCLK_SET()            GPIO_SetBits(SWCLK_PORT,SWCLK_PIN)  //TCK(1)
#define SWCLK_CLR()            GPIO_ResetBits(SWCLK_PORT,SWCLK_PIN)  //TCK(0)

#define SWDIO_SET()            GPIO_SetBits(SWDIO_PORT,SWDIO_PIN)   //SWDIO_OUT(1)
#define SWDIO_CLR()            GPIO_ResetBits(SWDIO_PORT,SWDIO_PIN)  //SWDIO_OUT(0)

#define RESET_SET()            GPIO_SetBits(RESET_PORT,RESET_PIN)  //TRST(1)
#define RESET_CLR()            GPIO_ResetBits(RESET_PORT,RESET_PIN)  //TRST(0) 


#define SWDIO_IN()             GPIO_ReadInputDataBit(SWDIO_PORT,SWDIO_PIN)
#define SWDIO_OUT(n)           GPIO_WriteBit(SWDIO_PORT,SWDIO_PIN,(BitAction)n);



#define ERROR                           1
#define SUCCESS                         0


// DAP Transfer Response
#define DAP_TRANSFER_OK                 (1<<0)
#define DAP_TRANSFER_WAIT               (1<<1)
#define DAP_TRANSFER_FAULT              (1<<2)
#define DAP_TRANSFER_ERROR              (1<<3)
#define DAP_TRANSFER_MISMATCH           (1<<4)
#define DAP_TRANSFER_TIMEOUT            (1<<5)

/*debug_cm.h????
// Debug Port Register Addresses
#define DP_IDCODE                       0x00    // IDCODE Register (SW Read only)
#define DP_ABORT                        0x00    // Abort Register (SW Write only)
#define DP_CTRL_STAT                    0x04    // Control & Status
#define DP_WCR                          0x04    // Wire Control Register (SW Only)
#define DP_SELECT                       0x08    // Select Register (JTAG R/W & SW W)
#define DP_RESEND                       0x08    // Resend (SW Read Only)
#define DP_RDBUFF                       0x0C    // Read Buffer (Read Only)
*/

// DAP Transfer Request
#define DAP_TRANSFER_APnDP              (1<<0)
#define DAP_TRANSFER_RnW                (1<<1)
#define DAP_TRANSFER_A2                 (1<<2)
#define DAP_TRANSFER_A3                 (1<<3)
#define DAP_TRANSFER_MATCH_VALUE        (1<<4)
#define DAP_TRANSFER_MATCH_MASK         (1<<5)

// SWD register access
#define SWD_REG_AP        (1)
#define SWD_REG_DP        (0)
#define SWD_REG_R        (1<<1)
#define SWD_REG_W        (0<<1)
#define SWD_REG_ADR(a)    (a & 0x0C)




/*debug_cm.h????
// Debug Select Register definitions
#define CTRLSEL        0x00000001  // CTRLSEL (SW Only)
#define APBANKSEL      0x000000F0  // APBANKSEL Mask
#define APSEL          0xFF000000  // APSEL Mask
*/


typedef enum
{
    RESET_HOLD,              // Hold target in reset
    RESET_PROGRAM,           // Reset target and setup for flash programming.
    RESET_RUN,               // Reset target and run normally
    NO_DEBUG,                // Disable debug on running target
    DEBUG,                   // Enable debug on running target
    HALT,                    // Halt the target without resetting it
    RUN,                     // Resume the target without resetting it
	  RESET_RUN_WITH_DEBUG,    // Reset target and run with debug enabled (required for semihost)
    disDEBUG,                // Disable debug on running target
    enDEBUG                  // Enable debug on running target
} TARGET_RESET_STATE;

typedef struct
{
    uint32_t r[16];
    uint32_t xpsr;
} DEBUG_STATE;


#ifndef BSWAP_32//?u32 0xAABBCCDD ?? 0xDDCCBBAA   ?????
#define BSWAP_32(val)   (uint32_t)((BSWAP_16((uint32_t)(val) & (uint32_t)0xFFFF) << 0x10) |  (BSWAP_16((uint32_t)((val) >> 0x10))))
#endif

#ifndef BSWAP_16//?u16 0xAABB  ?? 0xBBAA   ?????
#define BSWAP_16(x)     (uint16_t)((((x) & 0xFF00) >> 0x8) | (((x) & 0xFF) << 0x8))
#endif

#ifndef NULL
#define NULL    0
#endif


/* pin interface */
void bsp_SWDInit(void);
u32 SW_READ_BIT(void);
void SW_CLOCK_CYCLE(void);
void SW_WRITE_BIT(u32 bit);
void SW_Off(void);

//!< API
uint8_t SWJ_ReadDP(uint8_t adr, uint32_t *val);
uint8_t SWJ_WriteDP(uint8_t adr, uint32_t val);
uint8_t SWJ_ReadAP(uint32_t adr, uint32_t *val);
uint8_t SWJ_WriteAP(uint32_t adr, uint32_t val);
uint8_t SWJ_InitDebug(void);
uint8_t SWJ_ReadMem(uint32_t addr, uint8_t *buf, uint32_t len);
uint8_t SWJ_WriteMem(uint32_t addr, uint8_t *buf, uint32_t len);
uint8_t SWJ_SetTargetState(TARGET_RESET_STATE state);
uint8_t swd_flash_syscall_exec(const program_syscall_t *sysCallParam, uint32_t entry, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4);;
uint8_t swd_write_debug_state(DEBUG_STATE *state);

uint8_t SWJ_WriteMem32(uint32_t addr, uint32_t val);
uint8_t SWJ_ReadMem32(uint32_t addr, uint32_t *val);

void test(void);



#endif



