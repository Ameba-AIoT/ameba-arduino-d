#include <Arduino.h>
#include "IRDevice.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "basic_types.h"
#include "hal_platform.h"
#include "ir_nec_protocol.h"
#include "rtl8721d_pinmux.h"

#ifdef __cplusplus
}
#endif

static IR_InitTypeDef IR_InitStruct;  // Structure to store configuration information
static IR_DataTypeDef IR_DataStruct;  // Structure to store encoded data
xSemaphoreHandle IR_Recv_end_sema = NULL;

void IR_rx_recv_raw(void) {
    uint16_t len = 0;
    static u8 continue_filter = 0;
    const uint32_t MAX_CARRIER = 2000;
    u32 data;
    u32 duration;

    len = IR_GetRxDataLen(IR_DEV);
    while (len) {
        len--;
        data = IR_ReceiveData(IR_DEV);
        duration = data & 0x7fffffff;

        if (IR_DataStruct.bufLen >= 67) {
            //printf("the waveform is inverse, you should set: \n");
            //printf("IR_InitStruct.IR_RxCntThrType = IR_RX_COUNT_HIGH_LEVEL \n");
            //printf("#define INVERSE_DATA in Ir_nec_protocol.h \n");
            break;
        }

        // demodulate and filter out carrier
        if ((IR_DataStruct.bufLen == 0) && (IR_DataStruct.irBuf[0] == 0)) {
            IR_DataStruct.irBuf[0] = data;
        } else {
            if (duration > MAX_CARRIER) {
                IR_DataStruct.irBuf[++IR_DataStruct.bufLen] = data;
            } else if (continue_filter == 0) {
                IR_DataStruct.irBuf[++IR_DataStruct.bufLen] = data;
            } else {
                IR_DataStruct.irBuf[IR_DataStruct.bufLen] += duration;
            }

            if (duration > MAX_CARRIER) {
                continue_filter = 0;
            } else {
                continue_filter = 1;
            }
        }
        len = IR_GetRxDataLen(IR_DEV);
    }
}

void IR_rx_recv_demod(void) {
    uint16_t len = 0;
    u32 data;

    len = IR_GetRxDataLen(IR_DEV);
    while (len) {
        data = IR_ReceiveData(IR_DEV);

        if (IR_DataStruct.bufLen >= 69) {
            //printf("the waveform is inverse, you should set: \n");
            //printf("IR_InitStruct.IR_RxCntThrType = IR_RX_COUNT_HIGH_LEVEL \n");
            //printf("#define INVERSE_DATA in Ir_nec_protocol.h \n");
            break;
        }

        if ((IR_DataStruct.bufLen == 0) && (IR_DataStruct.irBuf[0] == 0)) {
            IR_DataStruct.irBuf[0] = data;
        } else {
            IR_DataStruct.irBuf[IR_DataStruct.bufLen] = data;
        }
        IR_DataStruct.bufLen++;
        len = IR_GetRxDataLen(IR_DEV);
    }
}

void IR_rx_irq_handler(void) {
    u32 IntStatus, IntMask;
    portBASE_TYPE taskWoken = pdFALSE;

    IntStatus = IR_GetINTStatus(IR_DEV);
    IntMask = IR_GetIMR(IR_DEV);

    if (IR_InitStruct.IR_Mode == IR_MODE_RX) {
        IR_MaskINTConfig(IR_DEV, IR_RX_INT_ALL_MASK, DISABLE);

        if (IntStatus & IR_RX_FIFO_FULL_INT_STATUS) {
            IR_ClearINTPendingBit(IR_DEV, IR_RX_FIFO_FULL_INT_CLR);
        }

        if (IntStatus & IR_RX_FIFO_LEVEL_INT_STATUS) {
            IR_ClearINTPendingBit(IR_DEV, IR_RX_FIFO_LEVEL_INT_CLR);
            if (IR_RX_RAW) {
                IR_rx_recv_raw();
            } else {
                IR_rx_recv_demod();
            }
        }

        if (IntStatus & IR_RX_CNT_OF_INT_STATUS) {
            IR_ClearINTPendingBit(IR_DEV, IR_RX_CNT_OF_INT_CLR);
        }

        if (IntStatus & IR_RX_FIFO_OF_INT_STATUS) {
            IR_ClearINTPendingBit(IR_DEV, IR_RX_FIFO_OF_INT_CLR);
        }

        if (IntStatus & IR_RX_CNT_THR_INT_STATUS) {
            taskWoken = pdFALSE;
            IR_ClearINTPendingBit(IR_DEV, IR_RX_CNT_THR_INT_CLR);
            xSemaphoreGiveFromISR(IR_Recv_end_sema, &taskWoken);
        }

        if (IntStatus & IR_RX_FIFO_ERROR_INT_STATUS) {
            IR_ClearINTPendingBit(IR_DEV, IR_RX_FIFO_ERROR_INT_CLR);
        }
    }
    IR_MaskINTConfig(IR_DEV, IntMask, ENABLE);
}

IRDevice::IRDevice() {
}

IRDevice::~IRDevice() {
    free(_pIrBuf);
}

void IRDevice::setTxPin(uint8_t transmitPin) {
    /* there are three groups of pinmux and pad settings:
    *  |  IR_TX  |  _PA_25  |  _PB_23 |  _PB_31 |
    */
#if defined(BOARD_RTL8722DM)
    if (transmitPin == 3) {
        Pinmux_Config(_PB_31, PINMUX_FUNCTION_IR);
    } else if (transmitPin == 9) {
        Pinmux_Config(_PB_23, PINMUX_FUNCTION_IR);
    } else if (transmitPin == 16) {
        Pinmux_Config(_PA_25, PINMUX_FUNCTION_IR);
    } else {
        printf("Hardware IR functionality is not supported on selected transmit pin!\r\n");
        return;
    }
#elif defined(BOARD_RTL8720DN_BW16)
//    if (transmitPin == 3) {
    if (transmitPin == PA25) {
        Pinmux_Config(_PA_25, PINMUX_FUNCTION_IR);
    } else {
        printf("Hardware IR functionality is not supported on selected transmit pin!\r\n");
        return;
    }
#else
#error
#endif

    _transmitPin = transmitPin;
}

void IRDevice::setRxPin(uint8_t receivePin) {
    /* there are three groups of pinmux and pad settings:
    *  |  IR_RX  |  _PA_26  |  _PB_22 |  _PB_29 |
    */
#if defined(BOARD_RTL8722DM)
    if (receivePin == 6) {
        PAD_PullCtrl(_PB_29, PullNone);
        Pinmux_Config(_PB_29, PINMUX_FUNCTION_IR);
    } else if (receivePin == 8) {
        Pinmux_Config(_PB_22, PINMUX_FUNCTION_IR);
    } else if (receivePin == 17) {
        PAD_PullCtrl(_PA_26, PullNone);
        Pinmux_Config(_PA_26, PINMUX_FUNCTION_IR);
    } else {
        printf("Hardware IR functionality is not supported on selected receive pin!\r\n");
        return;
    }
#elif defined(BOARD_RTL8720DN_BW16)
//    if (receivePin == 10) {
    if (receivePin == PA26) {
        PAD_PullCtrl(_PA_26, PullNone);
        Pinmux_Config(_PA_26, PINMUX_FUNCTION_IR);
    } else {
        printf("Hardware IR functionality is not supported on selected receive pin!\r\n");
        return;
    }
#else
#error
#endif

    _receivePin = receivePin;
}

void IRDevice::setPins(uint8_t receivePin, uint8_t transmitPin) {
    /* there are three groups of pinmux and pad settings:
    *  |  IR_TX  |  _PA_25  |  _PB_23 |  _PB_31 |
    *  |  IR_RX  |  _PA_26  |  _PB_22 |  _PB_29 |
    */
    setTxPin(transmitPin);
    setRxPin(receivePin);
}

uint8_t IRDevice::getFreq() {
    return _frequency;
}

void IRDevice::begin(uint8_t irPin, uint32_t irMode, uint32_t freq) {
    if (irMode == IR_MODE_TX) {
        setTxPin(irPin);
    } else if (irMode == IR_MODE_RX) {
        setRxPin(irPin);
    } else {
        printf("Invalid IR mode!\r\n");
        return;
    }
    _mode = irMode;
    _frequency = freq;
    IR_Cmd(IR_DEV, IR_InitStruct.IR_Mode, DISABLE);
    IR_StructInit(&IR_InitStruct);
    IR_InitStruct.IR_Mode = _mode;
    IR_InitStruct.IR_Freq = _frequency;  //Hz
    IR_Init(IR_DEV, &IR_InitStruct);
}

void IRDevice::begin(uint8_t receivePin, uint8_t transmitPin, uint32_t irMode, uint32_t freq) {
    setPins(receivePin, transmitPin);
    if ((irMode != IR_MODE_TX) && (irMode != IR_MODE_RX)) {
        printf("Invalid IR mode!\r\n");
        return;
    } else {
        _mode = irMode;
        _frequency = freq;
    }
    IR_Cmd(IR_DEV, IR_InitStruct.IR_Mode, DISABLE);
    IR_StructInit(&IR_InitStruct);
    IR_InitStruct.IR_Mode = _mode;
    IR_InitStruct.IR_Freq = _frequency;  //Hz
    IR_Init(IR_DEV, &IR_InitStruct);
}

void IRDevice::end() {
    free(_pIrBuf);
    _pIrBuf = NULL;
    _bufSize = 0;
    IR_Cmd(IR_DEV, IR_InitStruct.IR_Mode, DISABLE);
    IR_INTConfig(IR_DEV, IR_RX_INT_ALL_EN, DISABLE);
    IR_INTConfig(IR_DEV, IR_TX_INT_ALL_EN, DISABLE);
    Pinmux_Config(_receivePin, PINMUX_FUNCTION_GPIO);
    Pinmux_Config(_transmitPin, PINMUX_FUNCTION_GPIO);
}

static IR_DataType ConvertToCarrierCycle(uint32_t time, uint32_t freq) {
    return ((time & PULSE_HIGH) | ((time & IR_DATA_MSK) * freq / 1000000));
}

void IRDevice::send(const unsigned int buf[], uint16_t len) {
    uint16_t tx_count = 0;
    const u8 tx_thres = 15;
    uint32_t inputTime = 0;

    // If odd number of data, add in one last zero data to ensure transmission ends on PULSE_LOW with no carrier output
    // Somehow this is necessary for the last data to transmit normally
    if (len % 2) {
        len = len + 1;
    }

    // Ensure code buffer has sufficient size to contain data
    if ((_pIrBuf == NULL) && (_bufSize == 0)) {
        _pIrBuf = (uint32_t*)malloc(len * sizeof(uint32_t));
        _bufSize = len;
    } else if (_bufSize < len) {
        free(_pIrBuf);
        _pIrBuf = (uint32_t*)malloc(len * sizeof(uint32_t));
        _bufSize = len;
    }

    // Zero out sufficient code buffer space for data transmission
    memset(_pIrBuf, 0, (len * sizeof(uint32_t)));

    // Convert IR timings into carrier cycle counts for IR peripheral
    // Assuming a logical bit comprises of a time duration with pulses followed by another duration with no pulses
    for (int i = 0; i < len; i++) {
        if((i % 2) == 0){ 
            inputTime = buf[i] | PULSE_HIGH;
            _pIrBuf[i] = ConvertToCarrierCycle(inputTime, _frequency);
        } else {
            inputTime = (buf[i]) | PULSE_LOW;
            _pIrBuf[i] = ConvertToCarrierCycle(inputTime, _frequency);
        }
    }

    IR_ClearTxFIFO(IR_DEV);
    // Copy initial batch of data into IR peripheral and send
    if (len > IR_TX_FIFO_SIZE) {
        IR_SendBuf(IR_DEV, _pIrBuf, IR_TX_FIFO_SIZE, FALSE);
        tx_count += IR_TX_FIFO_SIZE;
    } else {
        IR_SendBuf(IR_DEV, _pIrBuf, len, TRUE);
        tx_count += len;
    }
    IR_Cmd(IR_DEV, IR_InitStruct.IR_Mode, ENABLE);

    // Check if there is remaining data to send
    while ((len - tx_count) > 0) {
        // Wait for IR peripheral to transmit data until FIFO is at least half empty (IR_TX_FIFO_SIZE = 32, tx_thres = 15)
        while (IR_GetTxFIFOFreeLen(IR_DEV) < tx_thres) {
            taskYIELD();
        }
        // Top up IR peripheral FIFO with data
        if ((len - tx_count) > tx_thres) {
            IR_SendBuf(IR_DEV, (_pIrBuf + tx_count), tx_thres, FALSE);
            tx_count += tx_thres;
        } else {
            IR_SendBuf(IR_DEV, (_pIrBuf + tx_count), (len - tx_count), TRUE);
            tx_count = len;
        }
    }

    // Wait for IR to finish transmitting all data in FIFO
    while (IR_GetTxFIFOFreeLen(IR_DEV) != (IR_TX_FIFO_SIZE - 1)) {
        taskYIELD();
    }
    IR_Cmd(IR_DEV, IR_InitStruct.IR_Mode, DISABLE);
}

void IRDevice::recv() {
}

void IRDevice::beginNEC(uint8_t receivePin, uint8_t transmitPin, uint32_t irMode) {
    setPins(receivePin, transmitPin);
    IR_Cmd(IR_DEV, IR_InitStruct.IR_Mode, DISABLE);
    IR_StructInit(&IR_InitStruct);

    if (irMode == IR_MODE_TX) {
        _frequency = 38000;  // Tx mode frequency corresponds to IR carrier frequency
        _mode = irMode;
    } else if (irMode == IR_MODE_RX) {
        _frequency = 10000000;  // Rx mode frequency corresponds to Ameba sampling frequency
        _mode = irMode;
        IR_InitStruct.IR_RxFIFOThrLevel = 2;
        if (IR_RX_INVERTED) {
            IR_InitStruct.IR_RxCntThrType = IR_RX_COUNT_HIGH_LEVEL;  //the idle level of receiving waveform is high
        } else {
            IR_InitStruct.IR_RxCntThrType = IR_RX_COUNT_LOW_LEVEL;  //the idle level of receiving waveform is low
        }
        IR_InitStruct.IR_RxCntThr = 0xa1644;  // 66.1ms at 10MHz	if(IR_Recv_end_sema == NULL) {
        vSemaphoreCreateBinary(IR_Recv_end_sema);
        xSemaphoreTake(IR_Recv_end_sema, 1 / portTICK_RATE_MS);
    } else {
        printf("Invalid IR mode!\r\n");
        return;
    }

    IR_InitStruct.IR_Mode = _mode;
    IR_InitStruct.IR_Freq = _frequency;  //Hz
    IR_Init(IR_DEV, &IR_InitStruct);
}

void IRDevice::sendNEC(uint8_t adr, uint8_t cmd) {
    uint8_t data[4];
    u32 tx_count = 0;
    const u8 tx_thres = 15;

    data[0] = adr;
    data[1] = ~adr;
    data[2] = cmd;
    data[3] = ~cmd;

    IR_NECEncode(IR_InitStruct.IR_Freq, (uint8_t *)&data, &IR_DataStruct);

    IR_SendBuf(IR_DEV, IR_DataStruct.irBuf, IR_TX_FIFO_SIZE, FALSE);
    IR_Cmd(IR_DEV, IR_InitStruct.IR_Mode, ENABLE);

    tx_count += IR_TX_FIFO_SIZE;
    while ((IR_DataStruct.bufLen - tx_count) > 0) {
        while (IR_GetTxFIFOFreeLen(IR_DEV) < tx_thres) {
            taskYIELD();
        }
        if ((IR_DataStruct.bufLen - tx_count) > tx_thres) {
            IR_SendBuf(IR_DEV, (IR_DataStruct.irBuf + tx_count), tx_thres, FALSE);
            tx_count += tx_thres;
        } else {
            IR_SendBuf(IR_DEV, (IR_DataStruct.irBuf + tx_count), (IR_DataStruct.bufLen - tx_count), TRUE);
            tx_count = IR_DataStruct.bufLen;
        }
    }

    vTaskDelay((200 / portTICK_RATE_MS));  // delay for IR to finish sending
    IR_Cmd(IR_DEV, IR_InitStruct.IR_Mode, DISABLE);
}

uint8_t IRDevice::recvNEC(uint8_t &adr, uint8_t &cmd, uint32_t timeout) {
    adr = 0;
    cmd = 0;
    uint8_t data[4] = {0, 0, 0, 0};
    //uint8_t result;
    uint8_t data_received = 0;

    IR_DataStruct.bufLen = 0;
    IR_DataStruct.codeLen = 0;
    IR_DataStruct.irBuf[0] = 0;
    IR_ClearRxFIFO(IR_DEV);

    // Configure interrupt and register callback function
    IR_ClearINTPendingBit(IR_DEV, (IR_RX_FIFO_LEVEL_INT_CLR | IR_RX_CNT_THR_INT_CLR));
    IR_INTConfig(IR_DEV, (IR_RX_FIFO_LEVEL_INT_EN | IR_RX_CNT_THR_INT_EN), ENABLE);
    InterruptRegister(((IRQ_FUN)IR_rx_irq_handler), IR_IRQ, (u32)NULL, 10);
    // Enable interrupt for IR rx processing
    InterruptEn(IR_IRQ, 10);
    // Enable IR receive
    IR_Cmd(IR_DEV, IR_InitStruct.IR_Mode, ENABLE);

    // Wait for a valid transmission up to timeout period
    if (xSemaphoreTake(IR_Recv_end_sema, (timeout / portTICK_RATE_MS))) {
        if (IR_RX_INVERTED) {
            InvertPulse(IR_DataStruct.irBuf, IR_DataStruct.bufLen);
        }

        // result = IR_NECDecode(IR_InitStruct.IR_Freq, (uint8_t *)&data, &IR_DataStruct);
        IR_NECDecode(IR_InitStruct.IR_Freq, (uint8_t *)&data, &IR_DataStruct);
        adr = data[0];
        cmd = data[2];
        //printf("result %d RX %2x%2x\n",result, adr, cmd);
        data_received = 1;
    } else {
        data_received = 0;
    }

    // Disable IR receiving
    IR_Cmd(IR_DEV, IR_InitStruct.IR_Mode, DISABLE);

    // Disable interrupt
    IR_INTConfig(IR_DEV, (IR_RX_FIFO_LEVEL_INT_EN | IR_RX_CNT_THR_INT_EN), DISABLE);
    InterruptDis(IR_IRQ);
    InterruptUnRegister(IR_IRQ);

    return (data_received);
}

/*!
* @ brief:Sony protocol structure.
* @ note: Store parameters of Sony protocol.
* @ Carrier frequency = 40,000Hz
* @ duty factor = 1/2
* @ first pulse : 2400 ms 600 ms
* @ Command (7 bits) 
* @ Address (5 bits) 
* @ LSB is sent first !
*/
const IR_ProtocolTypeDef_Sony SONY_PROTOCOL =
    {
        40000,                                      /* Carrier freqency */
        2,                                          /* headerLen */
        {PULSE_HIGH | 2400, PULSE_LOW | (600 / 5)}, /* headerBuf unit: us*/
        {PULSE_HIGH | 600, PULSE_LOW | 600},        /* log0Buf */
        {PULSE_HIGH | 1200, PULSE_LOW | 600},       /* log1Buf */
};

/**
 * @brief:This fucntion is meant to send IR signal with Sony protocol.
 * @param data: Sony IR data 
 * @param len: number of bits containted in the data
 * @return The function returns nothing
*/
void IRDevice::sendSONY(unsigned long data, uint16_t len) {
    u32 tx_count = 0;
    const u8 tx_thres = 1;
    uint16_t index = 0;
    uint16_t bufLen = 0;
    uint32_t Log1[MAX_LOG_WAVFORM_SIZE];
    uint32_t Log0[MAX_LOG_WAVFORM_SIZE];
    int nbits = len;
    IR_ProtocolTypeDef *IR_Protocol = (IR_ProtocolTypeDef *)(&SONY_PROTOCOL);

    IR_DataStruct.carrierFreq = IR_InitStruct.IR_Freq;
    IR_DataStruct.codeLen = len;

    /* Encoding logical 1 and logical 0 */
    for (index = 0; index < MAX_LOG_WAVFORM_SIZE; index++) {
        Log0[index] = ConvertToCarrierCycle(IR_Protocol->log0Buf[index], IR_DataStruct.carrierFreq);
        Log1[index] = ConvertToCarrierCycle(IR_Protocol->log1Buf[index], IR_DataStruct.carrierFreq);
    }

    /* Encoding header */
    for (index = 0; index < 2; index++) {
        IR_DataStruct.irBuf[index] = ConvertToCarrierCycle(IR_Protocol->headerBuf[index],
                                                           IR_DataStruct.carrierFreq);
        bufLen++;
    }

    /* Encoding address & device */
    for (unsigned long mask = 1UL << (nbits - 1); mask; mask >>= 1) {
        if (data & mask) {
            IR_DataStruct.irBuf[bufLen] = Log1[1];
            IR_DataStruct.irBuf[bufLen + 1] = Log1[0];
        } else {
            IR_DataStruct.irBuf[bufLen] = Log0[1];
            IR_DataStruct.irBuf[bufLen + 1] = Log0[0];
        }
        bufLen += MAX_LOG_WAVFORM_SIZE;
    }
    bufLen++;
    IR_DataStruct.bufLen = bufLen;

    IR_SendBuf(IR_DEV, IR_DataStruct.irBuf, IR_TX_FIFO_SIZE, FALSE);
    IR_Cmd(IR_DEV, IR_InitStruct.IR_Mode, ENABLE);
    tx_count += IR_DataStruct.bufLen;
    while ((IR_DataStruct.bufLen - tx_count) > 0) {
        while (IR_GetTxFIFOFreeLen(IR_DEV) < tx_thres) {
            taskYIELD();
        }
        if ((IR_DataStruct.bufLen - tx_count) > tx_thres) {
            IR_SendBuf(IR_DEV, (IR_DataStruct.irBuf + tx_count), tx_thres, FALSE);
            tx_count += tx_thres;
        } else {
            IR_SendBuf(IR_DEV, (IR_DataStruct.irBuf + tx_count), (IR_DataStruct.bufLen - tx_count), TRUE);
            tx_count = IR_DataStruct.bufLen;
        }
    }
    vTaskDelay((40 / portTICK_RATE_MS));
    IR_Cmd(IR_DEV, IR_InitStruct.IR_Mode, DISABLE);
}

void IRDevice::InvertPulse(IR_DataType *pBuf, uint16_t len) {
    uint16_t i = 0;

    for (i = 0; i < len; i++) {
        if (pBuf[i] > PULSE_HIGH) {
            pBuf[i] &= (~PULSE_HIGH);
        } else {
            pBuf[i] |= PULSE_HIGH;
        }
    }
}

IRDevice IR;
