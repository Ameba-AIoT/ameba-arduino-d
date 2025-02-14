#include "AudioCodec.h"

#if defined(BOARD_AITHINKER_BW16) || defined(BOARD_AMB25) || defined(BOARD_AMB26) || defined(BOARD_UBLOX_NORAW30) || defined(BOARD_SPARKFUN_NORAW306) || defined(BOARD_DATALOGGER_AMB26)
// error Sorry, AudioCodec is not supported on RTL8720DN and RTL8720DF.

#else

#ifdef __cplusplus
extern "C" {
#endif

#include "rl6548.h"
#include "rtl8721d_audio.h"

void PLL_Sel(u32 sel);
void PLL_Div(u32 div);
BOOL AUDIO_SP_TXGDMA_Restart( u8 GDMA_Index, u8 GDMA_ChNum, u32 tx_addr, u32 tx_length);
BOOL AUDIO_SP_RXGDMA_Restart( u8 GDMA_Index, u8 GDMA_ChNum, u32 rx_addr, u32 rx_length);

#ifdef __cplusplus
}
#endif

AudioCodec Codec;

/*/
uint32_t AudioCodec::_DMA_PAGE_SIZE = 512;   // bytes per page, 2 ~ 4096
uint32_t AudioCodec::_DMA_PAGE_NUM = 4;
uint32_t AudioCodec::_DMA_ZERO_BUF_SIZE = 128;
uint32_t AudioCodec::_DMA_FULL_BUF_SIZE = 128;
//*/

uint8_t AudioCodec::_inputEn = 0;
uint8_t AudioCodec::_outputEn = 0;
uint32_t AudioCodec::_sampleRate = SR_48K;
uint32_t AudioCodec::_wordLen = WL_16;
uint32_t AudioCodec::_channelCount = CH_MONO;
uint32_t AudioCodec::_ioApplication = APP_AMIC_IN|APP_LINE_OUT;
uint32_t AudioCodec::_lrMux = RX_CH_LL;
TaskHandle_t AudioCodec::callback_task_handle = NULL;

uint8_t AudioCodec::_txBuffer[_DMA_PAGE_SIZE*_DMA_PAGE_NUM]__attribute__((aligned(32)));
uint8_t AudioCodec::_zeroBuffer[_DMA_ZERO_BUF_SIZE]__attribute__((aligned(32)));
SRAM_NOCACHE_DATA_SECTION uint8_t AudioCodec::_rxBuffer[_DMA_PAGE_SIZE*_DMA_PAGE_NUM]__attribute__((aligned(32)));
uint8_t AudioCodec::_fullBuffer[_DMA_FULL_BUF_SIZE]__attribute__((aligned(32)));

GDMA_InitTypeDef AudioCodec::_TxGdmaInitStruct;
GDMA_InitTypeDef AudioCodec::_RxGdmaInitStruct;

void (*AudioCodec::_pWriteCB)(void) = nullptr;
void (*AudioCodec::_pReadCB)(void) = nullptr;

TX_INFO AudioCodec::_txBufferInfo;
RX_INFO AudioCodec::_rxBufferInfo;

AudioCodec::AudioCodec() {
}

void AudioCodec::begin(bool input, bool output) {
    uint32_t rx_addr, rx_length;
    uint32_t tx_addr, tx_length;

    SP_InitTypeDef SP_InitStruct;
    int result;

    _inputEn = input;
    _outputEn = output;

    initHAL();
    initTxVariables();
    initRxVariables();

    /*configure SPort according to the parameters*/
    AUDIO_SP_StructInit(&SP_InitStruct);
    SP_InitStruct.SP_MonoStereo= _channelCount;
    SP_InitStruct.SP_WordLen = _wordLen;
    SP_InitStruct.SP_SelRxCh = _lrMux;
    AUDIO_SP_Init(AUDIO_SPORT_DEV, &SP_InitStruct);

    if (_outputEn) {
        AUDIO_SP_TdmaCmd(AUDIO_SPORT_DEV, ENABLE);
        AUDIO_SP_TxStart(AUDIO_SPORT_DEV, ENABLE);
    }
    if (_inputEn) {
        AUDIO_SP_RdmaCmd(AUDIO_SPORT_DEV, ENABLE);
        AUDIO_SP_RxStart(AUDIO_SPORT_DEV, ENABLE);
    }

    setOutputVolume(50, 50);
    
    // start task to process incoming ATcommands
    result = xTaskCreate(callback_task, "audioCBtask", 1024, NULL, tskIDLE_PRIORITY + 2, &callback_task_handle);
    if (result != pdPASS) {
        printf("Audio Codec callback task create failed\r\n");
        return;
    }

    if (_outputEn) {
        tx_addr = (uint32_t)getReadyTxPage();
        tx_length = getReadyTxLength();
        AUDIO_SP_TXGDMA_Init(0, &_TxGdmaInitStruct, &_TxGdmaInitStruct, (IRQ_FUN)txCompleteHandler, (uint8_t*)tx_addr, tx_length);
    }
    if (_inputEn) {
        rx_addr = (uint32_t)getFreeRxPage();
        rx_length = getFreeRxLength();
        AUDIO_SP_RXGDMA_Init(0, &_RxGdmaInitStruct, &_RxGdmaInitStruct, (IRQ_FUN)rxCompleteHandler, (uint8_t*)rx_addr, rx_length); 
    }
}

void AudioCodec::end() {
    /* Disable Tx path */
    if (_outputEn) {
        AUDIO_SP_TxStart(AUDIO_SPORT_DEV, DISABLE);
        AUDIO_SP_TdmaCmd(AUDIO_SPORT_DEV, DISABLE);
        GDMA_ClearINT(_TxGdmaInitStruct.GDMA_Index, _TxGdmaInitStruct.GDMA_ChNum);
        GDMA_Cmd(_TxGdmaInitStruct.GDMA_Index, _TxGdmaInitStruct.GDMA_ChNum, DISABLE);
        GDMA_ChnlFree(_TxGdmaInitStruct.GDMA_Index, _TxGdmaInitStruct.GDMA_ChNum);
    }
    /* Disable Rx path */
    if (_inputEn) {
        AUDIO_SP_RxStart(AUDIO_SPORT_DEV, DISABLE);
        AUDIO_SP_RdmaCmd(AUDIO_SPORT_DEV, DISABLE);
        GDMA_ClearINT(_RxGdmaInitStruct.GDMA_Index, _RxGdmaInitStruct.GDMA_ChNum);
        GDMA_Cmd(_RxGdmaInitStruct.GDMA_Index, _RxGdmaInitStruct.GDMA_ChNum, DISABLE);
        GDMA_ChnlFree(_RxGdmaInitStruct.GDMA_Index, _RxGdmaInitStruct.GDMA_ChNum);
    }

    _inputEn = 0;
    _outputEn = 0;

    deinitHAL();
}

uint32_t AudioCodec::getBufferPageSize() {
    return _DMA_PAGE_SIZE;
}

void AudioCodec::setSampleRate(uint32_t sampleRate) {
    switch(sampleRate) {
        case 48000:
            _sampleRate = SR_48K;
            break;
        case 96000:
            _sampleRate = SR_96K;
            break;
        case 32000:
            _sampleRate = SR_32K;
            break;
        case 16000:
            _sampleRate = SR_16K;
            break;
        case 8000:
            _sampleRate = SR_8K;
            break;
        case 44100:
            _sampleRate = SR_44P1K;
            break;
        case 88200:
            _sampleRate = SR_88P2K;
            break;
        default:
            printf("Unsupported audio codec sample rate\n");
            break;
    }
}

void AudioCodec::setBitDepth(uint8_t bitDepth) {
    switch(bitDepth) {
        case 8:
            _wordLen = WL_8;
            break;
        case 16:
            _wordLen = WL_16;
            break;
        case 24:
            _wordLen = WL_24;
            break;
        default:
            printf("Unsupported audio codec sample rate\n");
            break;
    }
}

void AudioCodec::setChannelCount(uint8_t monoStereo) {
    if (monoStereo == 1) {
        _channelCount = CH_MONO;
        setInputLRMux(RX_CH_LL);
    } else if (monoStereo == 2) {
        _channelCount = CH_STEREO;
        setInputLRMux(RX_CH_LR);
    } else {
        printf("Invalid channel count\n");
    }
}

void AudioCodec::setInputMicType(Mic_Type micType) {
    switch(micType) {
        case ANALOGMIC:
            _ioApplication &= ~(APP_AMIC_IN | APP_DMIC_IN);
            _ioApplication |= APP_AMIC_IN;
            break;
        case PDMMIC:
            _ioApplication &= ~(APP_AMIC_IN | APP_DMIC_IN);
            _ioApplication |= APP_DMIC_IN;
            break;
        default:
            printf("Invalid input mic type\n");
            break;
    }
}

void AudioCodec::setInputLRMux(uint32_t mux) {
    if (mux < 4) {
        _lrMux = mux;
    } else {
        printf("Invalid audio codec LR mux \n");
    }
}

void AudioCodec::setDMicBoost(uint32_t leftBoost, uint32_t rightBoost) {
    if ((leftBoost < 4) && (rightBoost < 4)) {
        CODEC_SetDmicBst(leftBoost, rightBoost);
    } else {
        printf("Invalid DMIC boost values\n");
    }
}

void AudioCodec::setAMicBoost(uint32_t leftBoost, uint32_t rightBoost) {
    if ((leftBoost < 4) && (rightBoost < 4)) {
        CODEC_SetAmicBst(leftBoost, rightBoost);
    } else {
        printf("Invalid AMIC boost values\n");
    }
}

void AudioCodec::setADCGain(uint32_t leftGain, uint32_t rightGain) {
    if ((leftGain <= 0x7f) && (rightGain <= 0x7f)) {
        CODEC_SetAdcGain(leftGain, rightGain);
    } else {
        printf("Invalid ADC gain values\n");
    }
}

void AudioCodec::muteInput(uint8_t leftMute, uint8_t rightMute) {
    CODEC_MuteRecord((leftMute == true), (rightMute == true));
}

void AudioCodec::setOutputVolume(uint8_t leftVol, uint8_t rightVol) {
    if (leftVol > 100) {
        leftVol = 100;
    }
    if (rightVol > 100) {
        rightVol = 100;
    }
    uint8_t vol_lch = (leftVol/100.0) * 0xAF;
    uint8_t vol_rch = (rightVol/100.0) * 0xAF;
    CODEC_SetVolume(vol_lch, vol_rch);
}

void AudioCodec::muteOutput(uint8_t leftMute, uint8_t rightMute) {
    CODEC_MutePlay((leftMute == true), (rightMute == true));
}

bool AudioCodec::writeAvaliable() {
    return (getFreeTxPage() != NULL);
}

uint32_t AudioCodec::writeDataPage(int8_t* src, uint32_t len) {
    if (writeAvaliable()) {
        if (len > _DMA_PAGE_SIZE) {
            len = _DMA_PAGE_SIZE;
        }
        writeTxPage((uint8_t*)src, len);
        return len;
    } else {
        return 0;
    }
}

uint32_t AudioCodec::writeDataPage(int16_t* src, uint32_t len) {
    return (writeDataPage((int8_t*)src, (len * 2)) / 2);
}

bool AudioCodec::readAvaliable() {
    return (getReadyRxPage() != NULL);
}

uint32_t AudioCodec::readDataPage(int8_t* dst, uint32_t len) {
    if (readAvaliable()) {
        if (len > _DMA_PAGE_SIZE) {
            len = _DMA_PAGE_SIZE;
        }
        readRxPage((uint8_t*)dst, len);
        return len;
    } else {
        return 0;
    }
}

uint32_t AudioCodec::readDataPage(int16_t* dst, uint32_t len) {
    return (readDataPage((int8_t*)dst, (len * 2)) / 2);
}

void AudioCodec::amplifyReadData(int16_t* dst, uint32_t len, uint8_t m) {
    uint32_t i;
    for (i=0; i<len; i++) {
        dst[i]*=m;
    }
}

void AudioCodec::setWriteCallback(void (*writeCB)(void)) {
    _pWriteCB = writeCB;
}

void AudioCodec::setReadCallback(void (*readCB)(void)) {
    _pReadCB = readCB;
}

void AudioCodec::deinitHAL() {
    CODEC_DeInit(APP_AMIC_IN|APP_DMIC_IN|APP_LINE_OUT);

    RCC_PeriphClockCmd(APBPeriph_AUDIOC, APBPeriph_AUDIOC_CLOCK, DISABLE);
    RCC_PeriphClockCmd(APBPeriph_SPORT, APBPeriph_SPORT_CLOCK, DISABLE);   

    PLL_I2S_Set(DISABLE);
    PLL_PCM_Set(DISABLE);
}

void AudioCodec::initHAL() {
    uint32_t div;

    PLL_I2S_Set(ENABLE);        //enable 98.304MHz PLL. needed if fs=8k/16k/32k/48k/96k
    PLL_PCM_Set(ENABLE);        //enable 45.1584MHz PLL. needed if fs=44.1k/8.2k
    PLL_Sel(PLL_I2S);

    RCC_PeriphClockCmd(APBPeriph_AUDIOC, APBPeriph_AUDIOC_CLOCK, ENABLE);
    RCC_PeriphClockCmd(APBPeriph_SPORT, APBPeriph_SPORT_CLOCK, ENABLE); 

    //set clock divider to gen clock sample_rate*256 from 98.304M.
    switch(_sampleRate){
        case SR_48K:
            div = 8;
            break;
        case SR_96K:
            div = 4;
            break;
        case SR_32K:
            div = 12;
            break;
        case SR_16K:
            div = 24;
            break;
        case SR_8K:
            div = 48;
            break;
        case SR_44P1K:
            div = 4;
            PLL_Sel(PLL_PCM);
            break;
        case SR_88P2K:
            div = 2;
            PLL_Sel(PLL_PCM);
            break;
        default:
            printf("Unsupported audio codec sample rate\n");
            break;
    }
    PLL_Div(div);

    if ((_ioApplication & APP_DMIC_IN) == APP_DMIC_IN) {
        // Configure PDM DMIC input pins
        PAD_CMD(PB_1, DISABLE);
        PAD_CMD(PB_2, DISABLE);
        Pinmux_Config(PB_1, PINMUX_FUNCTION_DMIC);
        Pinmux_Config(PB_2, PINMUX_FUNCTION_DMIC);
    } else {
        // Configure analog mic input pins
        PAD_CMD(PA_0, DISABLE);
        PAD_CMD(PA_1, DISABLE);
        PAD_CMD(PA_4, DISABLE);
        if (_channelCount == CH_STEREO) {
            // Configure 2nd analog mic input pin
            PAD_CMD(PA_2, DISABLE);
        }
    }

    // Configure analog out pins
    PAD_CMD(PB_28, DISABLE);
    PAD_CMD(PB_29, DISABLE);
    PAD_CMD(PB_30, DISABLE);
    PAD_CMD(PB_31, DISABLE);
    // Codec init
    CODEC_Init(_sampleRate, _wordLen, _channelCount, _ioApplication); 
}

void AudioCodec::callback_task(void* param) {
    (void)param;

    while (1) {
        if ((getReadyRxPage() != NULL) && (_pReadCB != nullptr)) {
            _pReadCB();
        }

        if ((getFreeTxPage() != NULL) && (_pWriteCB != nullptr)) {
            _pWriteCB();
        }

        if ((_inputEn == 0) && (_outputEn == 0) && (getReadyRxPage() == NULL)) {
            break;
        }
        vTaskDelay(1/portTICK_PERIOD_MS);
    }

    // Audio Codec not active, stop & delete task
    callback_task_handle = NULL;
    vTaskDelete(NULL);
}

// Check for presence of a free Tx buffer page available for data write
// Returns pointer to first byte of buffer if available
uint8_t* AudioCodec::getFreeTxPage(void) {
    TX_BLOCK* ptx_block = &(_txBufferInfo.tx_block[_txBufferInfo.tx_usr_cnt]);

    if (ptx_block->tx_gdma_own)
        return NULL;
    else{
        return (uint8_t*)ptx_block->tx_addr;
    }
}

// Write user data into next Tx buffer page
// Note: does not check if next page is free, use getFreeTxPage first to verify
void AudioCodec::writeTxPage(uint8_t *src, uint32_t length) {
    TX_BLOCK* ptx_block = &(_txBufferInfo.tx_block[_txBufferInfo.tx_usr_cnt]);

    memcpy((void*)ptx_block->tx_addr, src, length);
    ptx_block->tx_length = length;
    // Mark newly written page as ready for GDMA transfer
    ptx_block->tx_gdma_own = 1;
    // Increment user counter to point to next page for user write
    _txBufferInfo.tx_usr_cnt++;
    if (_txBufferInfo.tx_usr_cnt == _DMA_PAGE_NUM){
        _txBufferInfo.tx_usr_cnt = 0;
    }
}

// Mark a Tx buffer page as ready for user data write
// Should only be called after a GDMA transfer is complete
void AudioCodec::releaseTxPage(void) {
    TX_BLOCK* ptx_block = &(_txBufferInfo.tx_block[_txBufferInfo.tx_gdma_cnt]);

    if (_txBufferInfo.tx_empty_flag){
    }
    else{
        // Mark page as ready for user data write
        ptx_block->tx_gdma_own = 0;
        // Increment GDMA counter to point to next page for GDMA transfer
        _txBufferInfo.tx_gdma_cnt++;
        if (_txBufferInfo.tx_gdma_cnt == _DMA_PAGE_NUM){
            _txBufferInfo.tx_gdma_cnt = 0;
        }
    }
}

// Check for presence of a Tx buffer page with user data ready for GDMA transfer
// Returns pointer to first byte of buffer if available
// Otherwise returns pointer to buffer of zeros, (no data output)
uint8_t* AudioCodec::getReadyTxPage(void) {
    TX_BLOCK* ptx_block = &(_txBufferInfo.tx_block[_txBufferInfo.tx_gdma_cnt]);

    if (ptx_block->tx_gdma_own){
        _txBufferInfo.tx_empty_flag = 0;
        return (uint8_t*)ptx_block->tx_addr;
    }
    else{
        _txBufferInfo.tx_empty_flag = 1;
        return (uint8_t*)_txBufferInfo.tx_zero_block.tx_addr;
    }
}

// Get the length (Bytes) of user data in the next Tx buffer page ready for GDMA transfer
uint32_t AudioCodec::getReadyTxLength(void) {
    TX_BLOCK* ptx_block = &(_txBufferInfo.tx_block[_txBufferInfo.tx_gdma_cnt]);

    if (_txBufferInfo.tx_empty_flag){
        return _txBufferInfo.tx_zero_block.tx_length;
    }
    else{
        return ptx_block->tx_length;
    }
}

// Check for presence of a Rx buffer page with data ready for user read
// Returns pointer to first byte of buffer if available
uint8_t* AudioCodec::getReadyRxPage(void) {
    RX_BLOCK* prx_block = &(_rxBufferInfo.rx_block[_rxBufferInfo.rx_usr_cnt]);

    if (prx_block->rx_gdma_own)
        return NULL;
    else{
        return (uint8_t*)prx_block->rx_addr;
    }
}

// Read data from next Rx buffer page into user memory
// Note: does not check if next page is ready, use getReadyRxPage first to verify
void AudioCodec::readRxPage(uint8_t* dst, uint32_t length) {
    RX_BLOCK* prx_block = &(_rxBufferInfo.rx_block[_rxBufferInfo.rx_usr_cnt]);

    memcpy(dst, (void const*)(prx_block->rx_addr), length);
    // Mark newly read page as ready for GDMA transfer of new data
    prx_block->rx_gdma_own = 1;
    // Increment user counter to point to next page for user read
    _rxBufferInfo.rx_usr_cnt++;
    if (_rxBufferInfo.rx_usr_cnt == _DMA_PAGE_NUM){
        _rxBufferInfo.rx_usr_cnt = 0;
    }
}

// Mark a Rx buffer page as ready for user data read
// Should only be called after a GDMA transfer is complete
void AudioCodec::releaseRxPage(void) {
    RX_BLOCK* prx_block = &(_rxBufferInfo.rx_block[_rxBufferInfo.rx_gdma_cnt]);

    if (_rxBufferInfo.rx_full_flag){
    }
    else{
        prx_block->rx_gdma_own = 0;
        _rxBufferInfo.rx_gdma_cnt++;
        if (_rxBufferInfo.rx_gdma_cnt == _DMA_PAGE_NUM){
            _rxBufferInfo.rx_gdma_cnt = 0;
        }
    }
}

// Check for presence of a Rx buffer page ready for GDMA transfer of new data
// Returns pointer to first byte of buffer if available
// Otherwise returns pointer to overflow buffer, (new data lost)
uint8_t* AudioCodec::getFreeRxPage(void) {
    RX_BLOCK* prx_block = &(_rxBufferInfo.rx_block[_rxBufferInfo.rx_gdma_cnt]);

    if (prx_block->rx_gdma_own){
        _rxBufferInfo.rx_full_flag = 0;
        return (uint8_t*)prx_block->rx_addr;
    }
    else{
        _rxBufferInfo.rx_full_flag = 1;
        return (uint8_t*)_rxBufferInfo.rx_full_block.rx_addr;
    }
}

// Get the length (Bytes) of free space in the next Rx buffer page ready for GDMA transfer
uint32_t AudioCodec::getFreeRxLength(void) {
    RX_BLOCK* prx_block = &(_rxBufferInfo.rx_block[_rxBufferInfo.rx_gdma_cnt]);

    if (_rxBufferInfo.rx_full_flag){
        return _rxBufferInfo.rx_full_block.rx_length;
    }
    else{
        return prx_block->rx_length;
    }
}

void AudioCodec::initTxVariables(void) {
    int i;

    // Fill Tx zero buffer with zeros for no output
    for(i=0; i<_DMA_ZERO_BUF_SIZE; i++){
        _zeroBuffer[i] = 0;
    }
    _txBufferInfo.tx_zero_block.tx_addr = (uint32_t)_zeroBuffer;
    _txBufferInfo.tx_zero_block.tx_length = (uint32_t)_DMA_ZERO_BUF_SIZE;

    _txBufferInfo.tx_gdma_cnt = 0;
    _txBufferInfo.tx_usr_cnt = 0;
    _txBufferInfo.tx_empty_flag = 0;

    for(i=0; i<_DMA_PAGE_NUM; i++){
        _txBufferInfo.tx_block[i].tx_gdma_own = 0;
        _txBufferInfo.tx_block[i].tx_addr = (uint32_t)(_txBuffer + i * _DMA_PAGE_SIZE);
        _txBufferInfo.tx_block[i].tx_length = _DMA_PAGE_SIZE;
    }
}

void AudioCodec::initRxVariables(void) {
    int i;

    _rxBufferInfo.rx_full_block.rx_addr = (uint32_t)_fullBuffer;
    _rxBufferInfo.rx_full_block.rx_length = (uint32_t)_DMA_FULL_BUF_SIZE;

    _rxBufferInfo.rx_gdma_cnt = 0;
    _rxBufferInfo.rx_usr_cnt = 0;
    _rxBufferInfo.rx_full_flag = 0;

    for(i=0; i<_DMA_PAGE_NUM; i++){
        _rxBufferInfo.rx_block[i].rx_gdma_own = 1;
        _rxBufferInfo.rx_block[i].rx_addr = (uint32_t)(_rxBuffer + i * _DMA_PAGE_SIZE);
        _rxBufferInfo.rx_block[i].rx_length = _DMA_PAGE_SIZE;
    }
}

void AudioCodec::txCompleteHandler(void* DMAinfo) {
    PGDMA_InitTypeDef GDMA_InitStruct = (PGDMA_InitTypeDef) DMAinfo;
    uint32_t tx_addr;
    uint32_t tx_length;

    /* Clear Pending ISR */
    GDMA_ClearINT(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);

    releaseTxPage();
    tx_addr = (uint32_t)getReadyTxPage();
    tx_length = getReadyTxLength();
    AUDIO_SP_TXGDMA_Restart(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, tx_addr, tx_length);
}

void AudioCodec::rxCompleteHandler(void* DMAinfo) {
    PGDMA_InitTypeDef GDMA_InitStruct = (PGDMA_InitTypeDef) DMAinfo;
    uint32_t rx_addr;
    uint32_t rx_length;

    DCache_Invalidate(GDMA_InitStruct->GDMA_DstAddr, GDMA_InitStruct->GDMA_BlockSize<<2);
    /* Clear Pending ISR */
    GDMA_ClearINT(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);

    releaseRxPage();
    rx_addr = (uint32_t)getFreeRxPage();
    rx_length = getFreeRxLength();
    AUDIO_SP_RXGDMA_Restart(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, rx_addr, rx_length);
}

#endif
