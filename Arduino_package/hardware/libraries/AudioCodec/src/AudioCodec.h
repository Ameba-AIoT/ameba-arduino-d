#ifndef _AUDIO_CODEC_H_
#define _AUDIO_CODEC_H_

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "rl6548.h"
#include "rtl8721d_audio.h"

#ifdef __cplusplus
}
#endif

#define _DMA_PAGE_NUM 4
#define _DMA_PAGE_SIZE 1024
#define _DMA_ZERO_BUF_SIZE 128
#define _DMA_FULL_BUF_SIZE 128

typedef struct {
    u8 tx_gdma_own;
    u32 tx_addr;
    u32 tx_length;
}TX_BLOCK;

typedef struct {
    TX_BLOCK tx_block[_DMA_PAGE_NUM];
    TX_BLOCK tx_zero_block;
    u8 tx_gdma_cnt;
    u8 tx_usr_cnt;
    u8 tx_empty_flag;
}TX_INFO;

typedef struct {
    u8 rx_gdma_own;
    u32 rx_addr;
    u32 rx_length;
}RX_BLOCK;

typedef struct {
    RX_BLOCK rx_block[_DMA_PAGE_NUM];
    RX_BLOCK rx_full_block;
    u8 rx_gdma_cnt;
    u8 rx_usr_cnt;
    u8 rx_full_flag;
}RX_INFO;

typedef enum
{
	ANALOGMIC,
	PDMMIC,
} Mic_Type;

class AudioCodec {
    public:
        AudioCodec();
        void begin(bool input, bool output);
        void end();

        uint32_t getBufferPageSize();

        void setSampleRate(uint32_t sampleRate);
        void setBitDepth(uint8_t bitDepth);
        void setChannelCount(uint8_t monoStereo);
        void setInputMicType(Mic_Type micType);
        void setInputLRMux(uint32_t mux);

        void setDMicBoost(uint32_t leftBoost, uint32_t rightBoost);

        void setAMicBoost(uint32_t leftBoost, uint32_t rightBoost);
        void setADCGain(uint32_t leftGain, uint32_t rightGain);
        void muteInput(uint8_t leftMute, uint8_t rightMute);

        void setOutputVolume(uint8_t leftVol, uint8_t rightVol);
        void muteOutput(uint8_t leftMute, uint8_t rightMute);

        bool writeAvaliable();
        uint32_t writeDataPage(int8_t* buffer, uint32_t len);
        uint32_t writeDataPage(int16_t* buffer, uint32_t len);
        bool readAvaliable();
        uint32_t readDataPage(int8_t* buffer, uint32_t len);
        uint32_t readDataPage(int16_t* buffer, uint32_t len);

        void setWriteCallback(void (*writeCB)(void));
        void setReadCallback(void (*readCB)(void));

    private:
        // Functions for configuring hardware for Audio Codec
        static void initHAL();
        static void deinitHAL();

        // Functions for accessing and managing DMA buffer use
        static uint8_t* getFreeTxPage(void);
        static void writeTxPage(uint8_t *src, uint32_t length);
        static void releaseTxPage(void);
        static uint8_t* getReadyTxPage(void);
        static uint32_t getReadyTxLength(void);

        static uint8_t* getReadyRxPage(void);
        static void readRxPage(uint8_t *dst, uint32_t length);
        static void releaseRxPage(void);
        static uint8_t* getFreeRxPage(void);
        static uint32_t getFreeRxLength(void);

        // Functions to initialise DMA buffer usage tracking variables
        static void initTxVariables(void);
        static void initRxVariables(void);

        // DMA transfer complete interrupt handlers
        static void txCompleteHandler(void* DMAinfo);
        static void rxCompleteHandler(void* DMAinfo);

        static uint8_t _inputEn;
        static uint8_t _outputEn;
        static uint32_t _sampleRate;
        static uint32_t _wordLen;
        static uint32_t _channelCount;
        static uint32_t _ioApplication;
        static uint32_t _lrMux;

        /*/
        static uint32_t _DMA_PAGE_SIZE;   // bytes per page, 2 ~ 4096
        static uint32_t _DMA_PAGE_NUM;
        static uint32_t _DMA_ZERO_BUF_SIZE;
        static uint32_t _DMA_FULL_BUF_SIZE;
        //*/

        //The size of this buffer should be multiples of 32 and its head address should align to 32 
        //to prevent problems that may occur when CPU and DMA access this area simultaneously. 
        static uint8_t _txBuffer[_DMA_PAGE_SIZE*_DMA_PAGE_NUM]__attribute__((aligned(32)));
        static uint8_t _zeroBuffer[_DMA_ZERO_BUF_SIZE]__attribute__((aligned(32)));
        SRAM_NOCACHE_DATA_SECTION static uint8_t _rxBuffer[_DMA_PAGE_SIZE*_DMA_PAGE_NUM]__attribute__((aligned(32)));
        static uint8_t _fullBuffer[_DMA_FULL_BUF_SIZE]__attribute__((aligned(32)));

        static GDMA_InitTypeDef _TxGdmaInitStruct;
        static GDMA_InitTypeDef _RxGdmaInitStruct;

        static void (*_pWriteCB)(void);
        static void (*_pReadCB)(void);

        static TX_INFO _txBufferInfo;
        static RX_INFO _rxBufferInfo;

};

extern AudioCodec Codec;

#endif
