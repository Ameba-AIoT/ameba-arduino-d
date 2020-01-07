#include <basic_types.h>
#include "ameba_soc.h"


void get_rom_version(void);
VOID *_memset(void *dst0, int Val, SIZE_T length);
void *_memcpy( void *s1, const void *s2, size_t n );
void FLASH_WaitBusy(u32 WaitType);
void FLASH_WriteEn(void);
void FLASH_Erase(u32 EraseType, u32 Address);
void FLASH_RxCmd(u8 cmd, u32 read_len, u8* read_data);
void FLASH_SetStatus(u8 Cmd, u32 Len, u8* Status);
void FLASH_TxData12B(u32 StartAddr, u8 DataPhaseLen, u8* pData);
char xmodem_uart_readable(void);
char xmodem_uart_getc(void);
void xmodem_uart_putc(char c);
void xmodem_uart_putdata(u8* buf, u32 cnt);
void xmodem_uart_port_init(u8 uart_idx, u8 pin_mux, u32 baud_rate);
void xmodem_uart_port_deinit(u8 uart_idx);
char xmodem_uart_writable(void);
void UART_SetBaud(UART_TypeDef* UARTx, u32 BaudRate);
void UART_WaitBusy(UART_TypeDef* UARTx, u32 PollTimes);
void GPIO_Init(GPIO_InitTypeDef  *GPIO_InitStruct);
void GPIO_WriteBit(u32 GPIO_Pin, u32 Pin_State);
void DelayMs(u32 ms);
void DelayUs(u32 us);
u32 DiagPrintf(const char *fmt, ...);
void io_assert_failed(uint8_t* file, uint32_t line);
u32 BKUP_Read(u32 DwordIdx);


