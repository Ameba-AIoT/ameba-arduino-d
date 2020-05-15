#include "Arduino.h"
#include "IRDevice.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "ir_nec_protocol.h"
#include "rtl8721d_pinmux.h"
#include "hal_platform.h"
#include "basic_types.h"

#ifdef __cplusplus
}
#endif

static IR_InitTypeDef IR_InitStruct;   // Structure to store configuration information
static IR_DataTypeDef IR_DataStruct;   // Structure to store encoded data
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

void IRDevice::setPins(uint8_t receivePin, uint8_t transmitPin) {
    if (receivePin == 3) {
        Pinmux_Config(_PB_31, PINMUX_FUNCTION_IR);
    } else if (receivePin == 8) {
        Pinmux_Config(_PB_22, PINMUX_FUNCTION_IR);
    } else if (receivePin == 17) {
        Pinmux_Config(_PA_26, PINMUX_FUNCTION_IR);
    } else {
        printf("Hardware IR functionality is not supported on selected receive pin!\r\n");
        return;
    }

    if (transmitPin == 6) {
        Pinmux_Config(_PB_29, PINMUX_FUNCTION_IR);
    } else if (transmitPin == 9) {
        Pinmux_Config(_PB_23, PINMUX_FUNCTION_IR);
    } else if (transmitPin == 16) {
        Pinmux_Config(_PA_25, PINMUX_FUNCTION_IR);
    } else {
        printf("Hardware IR functionality is not supported on selected transmit pin!\r\n");
        return;
    }
    _receivePin = receivePin;
    _transmitPin = transmitPin;
}

uint8_t IRDevice::getFreq() {
    return _frequency;
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
    IR_InitStruct.IR_Freq = _frequency;            //Hz
    IR_Init(IR_DEV, &IR_InitStruct);
}

void IRDevice::end() {
    IR_Cmd(IR_DEV, IR_InitStruct.IR_Mode, DISABLE);
    IR_INTConfig(IR_DEV, IR_RX_INT_ALL_EN, DISABLE);
    IR_INTConfig(IR_DEV, IR_TX_INT_ALL_EN, DISABLE);
    Pinmux_Config(_receivePin, PINMUX_FUNCTION_GPIO);
    Pinmux_Config(_transmitPin, PINMUX_FUNCTION_GPIO);
}

void IRDevice::send() {
}

void IRDevice::recv() {
}

void IRDevice::beginNEC(uint8_t receivePin, uint8_t transmitPin, uint32_t irMode) {
    setPins(receivePin, transmitPin);
    IR_Cmd(IR_DEV, IR_InitStruct.IR_Mode, DISABLE);
    IR_StructInit(&IR_InitStruct);

    if (irMode == IR_MODE_TX) {
        _frequency = 38000;         // Tx mode frequency corresponds to IR carrier frequency
        _mode = irMode;
    } else if (irMode = IR_MODE_RX) {
        _frequency = 10000000;      // Rx mode frequency corresponds to Ameba sampling frequency
        _mode = irMode;
        IR_InitStruct.IR_RxFIFOThrLevel = 2;
        if (IR_RX_INVERTED) {
            IR_InitStruct.IR_RxCntThrType = IR_RX_COUNT_HIGH_LEVEL;     //the idle level of receiving waveform is high
        } else {
            IR_InitStruct.IR_RxCntThrType = IR_RX_COUNT_LOW_LEVEL;      //the idle level of receiving waveform is low
        }
        IR_InitStruct.IR_RxCntThr = 0xa1644; // 66.1ms at 10MHz	if(IR_Recv_end_sema == NULL) {
        vSemaphoreCreateBinary(IR_Recv_end_sema);
        xSemaphoreTake(IR_Recv_end_sema, 1/portTICK_RATE_MS);
    } else {
        printf("Invalid IR mode!\r\n");
        return;
    }

    IR_InitStruct.IR_Mode = _mode;
    IR_InitStruct.IR_Freq = _frequency; //Hz
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

    IR_NECEncode( IR_InitStruct.IR_Freq, (uint8_t *)&data, &IR_DataStruct);

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

    vTaskDelay((200 / portTICK_RATE_MS));      // delay for IR to finish sending
    IR_Cmd(IR_DEV, IR_InitStruct.IR_Mode, DISABLE);
}

uint8_t IRDevice::recvNEC(uint8_t& adr, uint8_t& cmd, uint32_t timeout) {
    adr = 0;
    cmd = 0;
    uint8_t data[4] = {0, 0, 0, 0};
    uint8_t result;
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
    if (xSemaphoreTake(IR_Recv_end_sema, (timeout/portTICK_RATE_MS))) {
        if (IR_RX_INVERTED) {
            InvertPulse(IR_DataStruct.irBuf, IR_DataStruct.bufLen);
        }
        result = IR_NECDecode(IR_InitStruct.IR_Freq, (uint8_t *)&data, &IR_DataStruct);
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
