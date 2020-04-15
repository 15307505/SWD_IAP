#include "swd.h"
//#include "errors.h"
#include <stdbool.h>


#ifndef SWD_DEBUG
#define SWD_DEBUG       0
#endif

#if (SWD_DEBUG == 1)
#include <stdio.h>
#define SWD_TRACE   printf
#else
#define SWD_TRACE(...)
#endif

/*  以下寄存器具体功能参见权威指南  */
/*  SCB_Type
#define CPUID 0xE000ED00//CPUID寄存器
#define ICSR  0xE000ED04//中断控制及状态寄存器
#define VTOR  0xE000ED08//向量表偏移量寄存器
#define AIRCR 0xE000ED0C//应用程序中断及复位控制寄存器
*/
#define DHCSR 0xE000EDF0 //调试停机控制及状态寄存器    R/W
#define DCRSR 0xE000EDF4 //调试内核寄存器选择者寄存器   W
#define DCRDR 0xE000EDF8 //调试内核寄存器数据寄存器    R/W
#define DEMCR 0xE000EDFC//调试及监视器控制寄存器       R/W
/***     -END-           **/

#define REGWnR (1 << 16)

#define MAX_TIMEOUT 1

typedef struct
{
    uint32_t r[16];
    uint32_t xpsr;
} DEBUG_STATE;

uint8_t SWD_TransferOnce(uint32_t request, uint32_t *data)
{
    uint32_t ack = 0;
    uint32_t bit;
    uint32_t val;
    uint32_t parity;

    uint32_t n;

    /* Packet Request */
    parity = 0;
    SW_WRITE_BIT(1);         /* Start Bit */
    bit = request & 0x1;
    SW_WRITE_BIT(bit);       /* APnDP Bit */
    parity += bit;
    bit = (request >> 1) & 0x1;
    SW_WRITE_BIT(bit);       /* RnW Bit */
    parity += bit;
    bit = request >> 2;
    SW_WRITE_BIT(bit);       /* A2 Bit */
    parity += bit;
    bit = request >> 3;
    SW_WRITE_BIT(bit);       /* A3 Bit */
    parity += bit;
    SW_WRITE_BIT(parity);    /* Parity Bit */
    SW_WRITE_BIT(0);         /* Stop Bit */
    SW_WRITE_BIT(1);         /* Park Bit */

    SWDIO_SET_INPUT();

    /* Turnaround */
    for(n = 1; n; n--)
    {
        SW_CLOCK_CYCLE();
    }

    /* Acknowledge response */
    for(n = 0; n < 3; n++)
    {
        bit = SW_READ_BIT();
        ack  |= bit << n;
    }
    switch(ack)
    {
        case DAP_TRANSFER_OK:
            if(request & DAP_TRANSFER_RnW)  /* read data */
            {
                val = 0;
                parity = 0;

                for(n = 32; n; n--)
                {
                    bit = SW_READ_BIT();  /* Read RDATA[0:31] */
                    parity += bit;
                    val >>= 1;
                    val  |= bit << 31;
                }

                bit = SW_READ_BIT();    /* Read Parity */

                if((parity ^ bit) & 1)
                {
                    ack = DAP_TRANSFER_ERROR;
                }

                if(data) *data = val;

                /* Turnaround */
                for(n = 1; n; n--)
                {
                    SW_CLOCK_CYCLE();
                }

                SWDIO_SET_OUTPUT();//SWDIO_SET_OUTPUT
            }
            else    /* write data */
            {
                /* Turnaround */
                for(n = 1; n; n--)
                {
                    SW_CLOCK_CYCLE();
                }

                SWDIO_SET_OUTPUT();//SWDIO_SET_OUTPUT
                /* Write data */
                val = *data;
                parity = 0;

                for(n = 32; n; n--)
                {
                    SW_WRITE_BIT(val); /* Write WDATA[0:31] */
                    parity += val;
                    val >>= 1;
                }

                SW_WRITE_BIT(parity);/* Write Parity Bit */
            }

            /* Idle cycles */
            SWDIO_SET_OUTPUT();
            SWDIO_SET();
            for(n = 0; n < 8; n++)
            {
                SW_WRITE_BIT(0);
            }
            return (ack);

        case DAP_TRANSFER_WAIT:
        case DAP_TRANSFER_FAULT:

            /* WAIT or FAULT response */
            if(0 && ((request & DAP_TRANSFER_RnW) != 0))
            {
                for(n = 32 + 1; n; n--)
                {
                    SW_CLOCK_CYCLE();  /* Dummy Read RDATA[0:31] + Parity */
                }
            }

            /* Turnaround */
            for(n = 1; n; n--)
            {
                SW_CLOCK_CYCLE();
            }

            SWDIO_SET_OUTPUT();

            if(0 && ((request & DAP_TRANSFER_RnW) == 0))
            {
                SWCLK_CLR();

                for(n = 32 + 1; n; n--)
                {
                    SW_CLOCK_CYCLE();  /* Dummy Write WDATA[0:31] + Parity */
                }
            }

            SWDIO_SET();
            for(n = 0; n < 8; n++)
            {
                SW_WRITE_BIT(0);
            }
            return (ack);

        default:
            break;
    }

    /* Protocol error */
    for(n = 1 + 32 + 1; n; n--)
    {
        SW_CLOCK_CYCLE();      /* Back off data phase */
    }

    SWDIO_SET_OUTPUT();
    SWDIO_SET();
    for(n = 0; n < 8; n++)
    {
        SW_WRITE_BIT(0);
    }
    return (ack);
}


#define MAX_SWD_RETRY   1
uint8_t SWD_Transfer(uint32_t request, uint32_t *data)
{
    uint8_t i, ack;

    for(i = 0; i < MAX_SWD_RETRY; i++)
    {
        ack = SWD_TransferOnce(request, data);

        // if ack != WAIT
        if(ack != DAP_TRANSFER_WAIT)
        {
            return ack;
        }
    }

    return ack;
}

//count 时钟个数
//swdio_logic swdio此时状态
/*
static void SWJ_SendClock(uint32_t count, uint8_t swdio_logic)
{
    while(count--)
    {
        SW_WRITE_BIT((swdio_logic) ? (1) : (0));
    }
}
*/
static void SWJ_SendData(uint16_t data)
{
    uint8_t i;

    for(i = 0; i < 16; i++)
    {
        SW_WRITE_BIT(((data & 0x1) == 1) ? (1) : (0));
        data >>= 1;
    }
}
//JTAG切换到SWD
//1故障 0正常
uint8_t SWJ_JTAG2SWD(void)
{
    uint32_t i;
    SWDIO_SET();
    for(i = 0; i < 56; i++)
    {
        SW_CLOCK_CYCLE();
    }


    SWJ_SendData(0xE79E);//SWJ_SendData(0xB76D);
    for(i = 0; i < 56; i++)
    {
        SW_CLOCK_CYCLE();
    }
    SWDIO_CLR();


    for(i = 0; i < 16; i++)
    {
        SW_CLOCK_CYCLE();
    }
    return 0;
}
//1故障 0正常
uint8_t SWJ_ReadDP(uint8_t adr, uint32_t *val)
{
    uint32_t tmp_in;
    uint8_t ack;
    uint8_t err;

    tmp_in = SWD_REG_DP | SWD_REG_R | SWD_REG_ADR(adr);
    ack = SWD_Transfer(tmp_in, val);

    (ack == DAP_TRANSFER_OK) ? (err = 0) : (err = 1);
    return err;
}
//1故障 0正常
uint8_t SWJ_WriteDP(uint8_t adr, uint32_t val)
{
    uint32_t req;
    uint8_t ack;
    uint8_t err;

    req = SWD_REG_DP | SWD_REG_W | SWD_REG_ADR(adr);
    ack = SWD_Transfer(req, &val);

    (ack == DAP_TRANSFER_OK) ? (err = 0) : (err = 1);
    return err;
}

/* Read access port register. */
//读AP寄存器
//1故障 0正常
uint8_t SWJ_ReadAP(uint32_t adr, uint32_t *val)
{
    uint8_t tmp_in, ack, err;

    uint32_t apsel = adr & APSEL;
    uint32_t bank_sel = adr & APBANKSEL;

    if(SWJ_WriteDP(DP_SELECT, apsel | bank_sel))
    {
        return 1;
    }

    tmp_in = SWD_REG_AP | SWD_REG_R | SWD_REG_ADR(adr);

    /* first dummy read */
    ack = SWD_Transfer(tmp_in, val);
    ack = SWD_Transfer(tmp_in, val);

    (ack == DAP_TRANSFER_OK) ? (err = 0) : (err = 1);
    return err;
}

//写AP寄存器
//1故障 0正常
uint8_t SWJ_WriteAP(uint32_t adr, uint32_t val)
{
    uint8_t req, ack, err;

    uint32_t apsel = adr & APSEL;
    uint32_t bank_sel = adr & APBANKSEL;

    /* write DP select */
    if(SWJ_WriteDP(DP_SELECT, apsel | bank_sel))
    {

    }

    /* write AP data */
    req = SWD_REG_AP | SWD_REG_W | SWD_REG_ADR(adr);
    ack = SWD_Transfer(req, &val);

    /* read DP buff */
    req = SWD_REG_DP | SWD_REG_R | SWD_REG_ADR(DP_RDBUFF);
    ack = SWD_Transfer(req, NULL);
    (ack == DAP_TRANSFER_OK) ? (err = 0) : (err = 1);

    return err;
}

/* Read 32-bit word from target memory. */
// AP CSW register, base value
#define CSW_VALUE (CSW_RESERVED | CSW_MSTRDBG | CSW_HPROT | CSW_DBGSTAT | CSW_SADDRINC)


// Write target memory.
//往目标内存写数据
//1故障 0正常
static uint8_t SWJ_WriteData(uint32_t addr, uint32_t data)
{
    uint8_t req, ack, err;

    SWJ_WriteAP(AP_TAR, addr);

    /* write data */
    req = SWD_REG_AP | SWD_REG_W | AP_DRW;
    ack = SWD_Transfer(req, &data);

    /* read DP buff */
    req = SWD_REG_DP | SWD_REG_R | SWD_REG_ADR(DP_RDBUFF);
    ack = SWD_Transfer(req, NULL);

    (ack == DAP_TRANSFER_OK) ? (err = 0) : (err = 1);
    return err;
}
//往目标内存写数据u8
//1故障 0正常
static uint8_t SWJ_WriteMem8(uint32_t addr, uint8_t val)
{
    uint32_t tmp;
    uint8_t err;

    SWJ_WriteAP(AP_CSW, CSW_VALUE | CSW_SIZE8);

    tmp = val << ((addr & 0x03) << 3);
    err = SWJ_WriteData(addr, tmp);
    return err;
}
//往目标内存写数据u32
//1故障 0正常
uint8_t SWJ_WriteMem32(uint32_t addr, uint32_t val)
{
    uint8_t err;

    SWJ_WriteAP(AP_CSW, CSW_VALUE | CSW_SIZE32);
    err = SWJ_WriteData(addr, val);
    return err;
}

// Write 32-bit word aligned values to target memory using address auto-increment.
// size is in bytes.
//
//1故障 0正常
static uint8_t SWJ_WriteBlock(uint32_t addr, uint8_t *buf, uint32_t len)
{
    uint8_t err, req;
    uint32_t size_in_words;
    uint32_t i, ack;

    if(len == 0)  return 0;

    err = 0;
    size_in_words = len / 4;


    err += SWJ_WriteAP(AP_CSW, CSW_VALUE | CSW_SIZE32);

    if(err)
    {
        //SWD_TRACE("AP_CSW, CSW_VALUE | CSW_SIZE32 failed\r\n");
    }

    err += SWJ_WriteAP(AP_TAR, addr);

    if(err) return err;

    /* DRW write */
    req = SWD_REG_AP | SWD_REG_W | (3 << 2);

    for(i = 0; i < size_in_words; i++)
    {
        if(SWD_Transfer(req, (uint32_t *)buf) != DAP_TRANSFER_OK)
        {
            return 1;
        }

        buf += 4;
    }

    /* read DP buff */
    req = SWD_REG_DP | SWD_REG_R | SWD_REG_ADR(DP_RDBUFF);
    ack = SWD_Transfer(req, NULL);

    (ack == DAP_TRANSFER_OK) ? (err = 0) : (err = 1);
    return 0;
}

// Read target memory.
//往目标内存读数据
//1故障 0正常
static uint8_t SWJ_ReadData(uint32_t addr, uint32_t *val)
{
    uint8_t req, ack, err;

    SWJ_WriteAP(AP_TAR, addr);

    /* read data */
    req = SWD_REG_AP | SWD_REG_R | AP_DRW;
    ack = SWD_Transfer(req, val);

    /* dummy read */
    req = SWD_REG_DP | SWD_REG_R | SWD_REG_ADR(DP_RDBUFF);
    ack = SWD_Transfer(req, val);

    (ack == DAP_TRANSFER_OK) ? (err = 0) : (err = 1);
    return err;
}
//往目标内存读数据u32
//1故障 0正常
static uint8_t SWJ_ReadMem32(uint32_t addr, uint32_t *val)
{
    uint8_t err;
    SWJ_WriteAP(AP_CSW, CSW_VALUE | CSW_SIZE32);
    err = SWJ_ReadData(addr, val);
    return err;
}
//往目标内存读数据u8
//1故障 0正常
static uint8_t SWJ_ReadMem8(uint32_t addr, uint8_t *val)
{
    uint32_t tmp;
    uint8_t err;

    SWJ_WriteAP(AP_CSW, CSW_VALUE | CSW_SIZE8);

    err = SWJ_ReadData(addr, &tmp);

    *val = (uint8_t)(tmp >> ((addr & 0x03) << 3));

    return err;
}

// Read 32-bit word aligned values from target memory using address auto-increment.
// size is in bytes.
//往目标内存读数据
//1故障 0正常
static uint8_t SWJ_ReadBlock(uint32_t addr, uint8_t *buf, uint32_t len)
{
    uint8_t err, req;
    uint32_t size_in_words;
    uint32_t i, ack;

    if(len == 0)  return 0;

    err = 0;
    size_in_words = len / 4;


    err += SWJ_WriteAP(AP_CSW, CSW_VALUE | CSW_SIZE32);
    err += SWJ_WriteAP(AP_TAR, addr);

    if(err) return err;

    req = SWD_REG_AP | SWD_REG_R | (3 << 2);

    /* dummy read */
    if(SWD_Transfer(req, (uint32_t *)buf) != 0x01)
    {
        return 1;
    }

    for(i = 0; i < size_in_words; i++)
    {
        if(SWD_Transfer(req, (uint32_t *)buf) != DAP_TRANSFER_OK)
        {
            return 1;
        }

        buf += 4;
    }

    /* read DP buff */
    req = SWD_REG_DP | SWD_REG_R | SWD_REG_ADR(DP_RDBUFF);
    ack = SWD_Transfer(req, NULL);

    (ack == DAP_TRANSFER_OK) ? (err = 0) : (err = 1);
    return err;
}

#define TARGET_AUTO_INCREMENT_PAGE_SIZE    (0x400)

// Write unaligned data to target memory.
// size is in bytes.
//往目标内存写数据
//1故障 0正常
uint8_t swd_write_memory(uint32_t address, uint8_t *data, uint32_t size)
{
    uint32_t n;

    // Write bytes until word aligned
    while((size > 0) && (address & 0x3))
    {
        SWJ_WriteMem8(address, *data);
        address++;
        data++;
        size--;
    }

    // Write word aligned blocks
    while(size > 3)
    {
        // Limit to auto increment page size
        n = TARGET_AUTO_INCREMENT_PAGE_SIZE - (address & (TARGET_AUTO_INCREMENT_PAGE_SIZE - 1));

        if(size < n)
        {
            n = size & 0xFFFFFFFC; // Only count complete words remaining
        }

        if(SWJ_WriteBlock(address, data, n))
        {
            SWD_TRACE("SWJ write block failed\r\n");
            return 1;
        }

        address += n;
        data += n;
        size -= n;
    }

    // Write remaining bytes
    while(size > 0)
    {
        SWJ_WriteMem8(address, *data);

        address++;
        data++;
        size--;
    }

    return 0;
}

// Read unaligned data from target memory.
// size is in bytes.
//往目标内存读数据
//1故障 0正常
uint8_t swd_read_memory(uint32_t address, uint8_t *data, uint32_t size)
{
    uint32_t n;

    // Read bytes until word aligned
    while((size > 0) && (address & 0x3))
    {
        SWJ_ReadMem8(address, data);
        address++;
        data++;
        size--;
    }

    // Read word aligned blocks
    while(size > 3)
    {
        // Limit to auto increment page size
        n = TARGET_AUTO_INCREMENT_PAGE_SIZE - (address & (TARGET_AUTO_INCREMENT_PAGE_SIZE - 1));

        if(size < n)
        {
            n = size & 0xFFFFFFFC; // Only count complete words remaining
        }

        if(SWJ_ReadBlock(address, data, n))
        {
            return 1;
        }

        address += n;
        data += n;
        size -= n;
    }

    // Read remaining bytes
    while(size > 0)
    {
        SWJ_ReadMem8(address, data);
        address++;
        data++;
        size--;
    }

    return 0;
}
//往目标内存读数据
//1故障 0正常
uint8_t SWJ_ReadMem(uint32_t addr, uint8_t *buf, uint32_t len)
{
    return swd_read_memory(addr, buf, len);
}
//往目标内存写数据
//1故障 0正常
uint8_t SWJ_WriteMem(uint32_t addr, uint8_t *buf, uint32_t len)
{
    return swd_write_memory(addr, buf, len);
}


//等目标Halt
//1故障 0正常
static uint8_t SWJ_WaitUntilHalted(void)
{
    // Wait for target to stop
    uint32_t val, i, timeout = MAX_TIMEOUT;

    for(i = 0; i < timeout; i++)
    {

        if(SWJ_ReadMem32(DBG_HCSR, &val))
        {
            return 1;
        }

        if(val & S_HALT)
        {
            return 0;
        }
    }

    return DAP_TRANSFER_ERROR;
}
//设置目标复位状态
//1故障 0正常
uint8_t SWJ_SetTargetState(TARGET_RESET_STATE state)
{
    switch(state)
    {
        case RESET_HOLD:
            RESET_CLR();
            break;

        case RESET_RUN:
            RESET_CLR();
            DELAY_US(20 * 1000);
            RESET_SET();
            DELAY_US(20 * 1000);
            break;

        case RESET_PROGRAM:
            RESET_CLR();
            DELAY_US(20);
            //   RESET_SET();
            //   DelayMs(20);

            SWJ_InitDebug();

            // Enable debug
            SWJ_WriteMem32(DBG_HCSR, DBGKEY | C_DEBUGEN);

            // Enable halt on reset
            SWJ_WriteMem32(DBG_EMCR, VC_CORERESET);

            // Reset again
            RESET_CLR();
            DELAY_US(20 * 1000);
            RESET_SET();

            break;

        default:
            break;
    }

    return 0;
}
//写目标核寄存器
//1故障 0正常
static uint8_t SWJ_WriteCoreReg(uint32_t n, uint32_t val)
{
    int i = 0, timeout = MAX_TIMEOUT;

    SWJ_WriteMem32(DCRDR, val);

    SWJ_WriteMem32(DCRSR, n | REGWnR);

    // wait for S_REGRDY
    for(i = 0; i < timeout; i++)
    {
        SWJ_ReadMem32(DHCSR, &val);

        if(val & S_REGRDY)
        {
            return 0;
        }
    }

    return 1;
}
//读目标核寄存器
//1故障 0正常
static uint8_t SWJ_ReadCoreReg(uint32_t n, uint32_t *val)
{
    int i = 0, timeout = MAX_TIMEOUT, err;

    if(SWJ_WriteMem32(DCRSR, n))
        return 1;

    // wait for S_REGRDY
    for(i = 0; i < timeout; i++)
    {

        if(SWJ_ReadMem32(DHCSR, val))
            return 1;

        if(*val & S_REGRDY)
            break;
    }

    if(i == timeout)
        return 1;

    err = SWJ_ReadMem32(DCRDR, val);

    return err;
}
//写debug状态
//1故障 0正常
uint8_t swd_write_debug_state(DEBUG_STATE *state)
{
    uint32_t i, status;

    SWJ_WriteDP(DP_SELECT, 0);

    // R0, R1, R2, R3
    for(i = 0; i < 4; i++)
    {
        SWJ_WriteCoreReg(i, state->r[i]);
    }

    // R9
    SWJ_WriteCoreReg(9, state->r[9]);

    // R13, R14, R15
    for(i = 13; i < 16; i++)
    {
        SWJ_WriteCoreReg(i, state->r[i]);
    }

    // xPSR
    SWJ_WriteCoreReg(16, state->xpsr);
    SWJ_WriteMem32(DBG_HCSR, DBGKEY | C_DEBUGEN);

    // check status
    SWJ_ReadDP(DP_CTRL_STAT, &status);

    if(status & (STICKYERR | WDATAERR))
    {
        SWD_TRACE("write debug states failed\r\n");
        return 1;
    }

    return 0;
}

//flash状态回读????????????????????
//1故障 0正常
uint8_t swd_flash_syscall_exec(const FLASH_SYSCALL *sysCallParam, uint32_t entry, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4)
{
    DEBUG_STATE state;

    // Call flash algorithm function on target and wait for result.
    state.xpsr     = 0x01000000;          // xPSR: T = 1, ISR = 0
    state.r[0]     = arg1;                   // R0: Argument 1
    state.r[1]     = arg2;                   // R1: Argument 2
    state.r[2]     = arg3;                   // R2: Argument 3
    state.r[3]     = arg4;                   // R3: Argument 4

    state.r[9]     = sysCallParam->static_base;    // SB: Static Base

    state.r[13]    = sysCallParam->stack_pointer;  // SP: Stack Pointer
    state.r[14]    = sysCallParam->breakpoint;       // LR: Exit Point
    state.r[15]    = entry;                           // PC: Entry Point

    if(swd_write_debug_state(&state))
    {
        SWD_TRACE("swd_write_debug_status failed\r\n");
        return 1;
    }

    if(SWJ_WaitUntilHalted())
    {
        SWD_TRACE("SWJ_WaitUntilHalted failed\r\n");
        return 1;
    }

    if(SWJ_ReadCoreReg(0, &state.r[0]))
    {
        SWD_TRACE("SWJ_ReadCoreReg failed\r\n");
        return 1;
    }


    // Flash functions return 0 if successful.
    if(state.r[0] != 0)
    {
        SWD_TRACE("resutlt failed:0x%X\r\n", state.r[0]);
        return 1;
    }

    return 0;
}

//将设备JTAG/SWD状态初始化
//1故障 0正常
uint8_t SWJ_InitDebug(void)
{
    uint32_t tmp = 0;
    uint32_t val;
    /*JTAG 转到 SWD*/
    SWJ_JTAG2SWD();

    if(SWJ_ReadDP(DP_IDCODE, &val))
    {
        return 1;
    }
    SWD_TRACE("DP_IDCODE:0x%X\r\n", val);

    SWJ_WriteDP(DP_ABORT, STKCMPCLR | STKERRCLR | WDERRCLR | ORUNERRCLR);

    /* Ensure CTRL/STAT register selected in DPBANKSEL */
    SWJ_WriteDP(DP_SELECT, 0);

    /* Power ups */
    SWJ_WriteDP(DP_CTRL_STAT, CSYSPWRUPREQ | CDBGPWRUPREQ);

    do
    {
        if(!SWJ_ReadDP(DP_CTRL_STAT, &tmp))
        {
            return 0;
        }
    }
    while((tmp & (CDBGPWRUPACK | CSYSPWRUPACK)) != (CDBGPWRUPACK | CSYSPWRUPACK));

    SWJ_WriteDP(DP_CTRL_STAT, CSYSPWRUPREQ | CDBGPWRUPREQ | TRNNORMAL | MASKLANE);

    return 0;
}

void softdelay(uint32_t us)
{
    volatile uint32_t i, j;

    for(i = 0; i < us; i++)
    {
        for(j = 0; j < 30U; j++)
        {
            __NOP();
        }
    }
}
uint32_t ss[10];
void test(void)
{
    SWDIO_SET_OUTPUT();
    SWCLK_SET_OUTPUT();

    SWCLK_CLR();
    SWDIO_SET();

    uint32_t i = 10, val;
    SWJ_InitDebug();

    while(i--)
    {
        SWJ_ReadDP(DP_IDCODE, &val);
        ss[i] = val;


        softdelay(100);
        //SWJ_WriteAP(0XAA,0X55);
        //SWJ_InitDebug();
    }
    while(1);
}
