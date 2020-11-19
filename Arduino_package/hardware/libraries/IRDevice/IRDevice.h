#ifndef _IRDEVICE_H_
#define _IRDEVICE_H_

#include <inttypes.h>
#include "rtl8721d_ir.h"
#include "ir_nec_protocol.h"

#define IR_RX_RAW       0
#define IR_RX_INVERTED  1

#define IR_MODE_TX      (0x00000000U << 31)
#define IR_MODE_RX      (0x00000001U << 31)

typedef struct
{
	uint16_t        carrierFreq;
    uint8_t         headerLen;
    uint32_t        headerBuf[MAX_HEADDER_LEN];
    IR_DataType     log0Buf[MAX_LOG_WAVFORM_SIZE];
    IR_DataType     log1Buf[MAX_LOG_WAVFORM_SIZE];
} IR_ProtocolTypeDef_RC5;

typedef struct
{
	uint16_t        carrierFreq;
    uint8_t         headerLen;
    uint32_t        headerBuf[MAX_HEADDER_LEN];
    IR_DataType     log0Buf[MAX_LOG_WAVFORM_SIZE];
    IR_DataType     log1Buf[MAX_LOG_WAVFORM_SIZE];
} IR_ProtocolTypeDef_Sony;


class IRDevice {
    public:
        IRDevice();
        uint8_t getFreq();
        void begin(uint8_t receivePin, uint8_t transmitPin, uint32_t irMode, uint32_t freq);
        void end();
        void send(const unsigned int buf[], uint16_t len);
        void recv();
        void beginNEC(uint8_t receivePin, uint8_t transmitPin, uint32_t irMode);
        void sendNEC(uint8_t adr, uint8_t cmd);
        uint8_t recvNEC(uint8_t& adr, uint8_t& cmd, uint32_t timeout);
        void sendSONY(unsigned long data, uint16_t len);

    private:
        uint8_t _receivePin = 8;        // default PB_22
        uint8_t _transmitPin = 9;       // default PB_23
        uint32_t _frequency = 38000;    // default 38kHz
        uint32_t _mode = IR_MODE_TX;    // default transmit
        void InvertPulse(IR_DataType * pBuf, uint16_t len);
        void setPins(uint8_t receivePin, uint8_t transmitPin);
};

extern IRDevice IR;

#endif
