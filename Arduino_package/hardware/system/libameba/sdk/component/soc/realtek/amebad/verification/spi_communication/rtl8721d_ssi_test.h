#ifndef _RTL8195A_SSI_TEST_H_
#define _RTL8195A_SSI_TEST_H_

/*SPI Test Case*/
typedef enum _SSI_TEST_TYPE_
{
	SSI_Master_LOOPBACK = 0,		// 0
	SSI_Master_TRX_POLL,			// 1
	SSI_Master_TRX_INTERRUPT,		// 2
	SSI_Master_TRX_DMA,			// 3
	SSI_Slave_TRX_POLL,				// 4
	SSI_Slave_TRX_INTERRUPT,		// 5
	SSI_Slave_TRX_DMA,				// 6
	SSI_Master_RX_ONLY,				// 7
	SSI_Slave_RX_INTERRUPT,		// 8
	SSI_Master_CS_SOFTWARE,		// 9
	SSI_MASTER_LONG_RUN,			// 10
	SSI_SLAVE_LONG_RUN,			// 11

	SSI_TT_BASIC_IO = 12,			// 12
	SSI_TT_INTERRUPT,				// 13
	SSI_TT_DMA_TO_TX,				// 14
	SSI_TT_RX_TO_DMA,				// 15
	SSI_TT_DMA_TX_TO_RX,			// 16
	SSI_MASTER_RX_INTERRUPT,		// 17
	SSI_MASTER_RX_TO_DMA,		// 18
	SSI_MASTER_TX_INTERRUPT,		// 19
	SSI_SLAVE_TX_INTERRUPT,		// 20
	SSI_MASTER_RX_ONLY,			// 21
	SSI_TT_TEST_ALL,				// 22
	SSI_MASTER_CS_SOFTWARE,		// 23
	OTHER_INTERFACE				// 24
} SSI_TEST_TYPE, *PSSI_TEST_TYPE;

typedef enum _SSI_TEST_SRC_DATA_MODE_
{
	SSI_TEST_SRCDATA_SEQ,
	SSI_TEST_SRCDATA_RND
} SSI_TEST_SRC_DATA_MODE, *PSSI_TEST_SRC_DATA_MODE;


/* Function Prototype */
void SsiDBGPrint(u16 *pSrc, u16 *pDst, u32 Length);
BOOL SsiDataCompare(u16 *pSrc, u16 *pDst, u32 Length);
VOID SsiGenerateRandomTxData(u16 *pTxArray, u32 Length, u32 Mode, BOOL MICROWIRE);

#endif
