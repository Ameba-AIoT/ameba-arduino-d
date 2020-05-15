#ifndef __RTL8721D_DLFW_H__
#define __RTL8721D_DLFW_H__

#ifndef __ASSEMBLY__

/*--------------------------Define -------------------------------------------*/
#define FW_SIZE                 0xC000  //Including TXPKTBUF
#define FW_START_ADDRESS        0x1000
#define FW_END_ADDRESS      0x3FFF

#define FW_PAGE_SIZE            0x1000
#define TXBUF_BLOCK_SIZE        256

#define CONFIG_ROMDLFW_DBG  0

/*------------------------------Function declaration--------------------------*/

#if CONFIG_ROMDLFW_DBG
BOOLEAN
RAMVerify(
    IN u16 length
    );
#endif

extern void 
DL_RAM(
    void
    );

#endif  //__ASSEMBLY__

#endif  //__RTL8721D_DLFW_H__
