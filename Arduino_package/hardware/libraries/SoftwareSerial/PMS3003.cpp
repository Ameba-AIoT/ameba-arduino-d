#include "Arduino.h"
#include "PMS3003.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "serial_api.h"
#include "serial_ex_api.h"

extern PinDescription g_APinDescription[];

#ifdef __cplusplus
}
#endif

void pms3003_handle_interrupt(uint32_t id, uint32_t event) {
    PMS3003 *pPMS3003 = (PMS3003 *)id;
    volatile char d = 0;

    if ((SerialIrq)event == RxIrq) {
        d = serial_getc((serial_t *)(pPMS3003->pUART));
        pPMS3003->rb[(pPMS3003->rbidx)] = d;
        pPMS3003->rbidx = ((pPMS3003->rbidx) + 1) % PMS3003_BUF_SIZE;
    }
}

PMS3003::PMS3003(int _rx, int _tx, int _set, int _reset) {
    setpin = _set;
    rxpin = _rx;
    txpin = _tx;
    resetpin = _reset;

    rbidx = 0;
    memset(rb, 0, PMS3003_BUF_SIZE);

    last_update_time = 0;

    pm1p0_cf1 = pm2p5_cf1 = pm10_cf1 = pm1p0_air = pm2p5_air = pm10_air = 0;
}

void PMS3003::begin() {

    pUART = malloc(sizeof(serial_t));
    memset(pUART, 0, sizeof(serial_t));

    serial_init(((serial_t *)pUART), ((PinName)g_APinDescription[txpin].pinname), ((PinName)g_APinDescription[rxpin].pinname));
    serial_baud(((serial_t *)pUART), 9600);
    serial_format(((serial_t *)pUART), 8, ParityNone, 1);

    serial_irq_handler(((serial_t *)pUART), (uart_irq_handler)pms3003_handle_interrupt, (uint32_t)this);
    serial_irq_set(((serial_t *)pUART), RxIrq, 1);
    serial_irq_set(((serial_t *)pUART), TxIrq, 1);

}

void PMS3003::end() {
    serial_free((serial_t *)pUART);
    free(pUART);
}

int PMS3003::get_pm1p0_cf1() {
    update_cache();
    return pm1p0_cf1;
}

int PMS3003::get_pm2p5_cf1() {
    update_cache();
    return pm2p5_cf1;
}

int PMS3003::get_pm10_cf1() {
    update_cache();
    return pm10_cf1;
}

int PMS3003::get_pm1p0_air() {
    update_cache();
    return pm1p0_air;
}

int PMS3003::get_pm2p5_air() {
    update_cache();
    return pm2p5_air;
}

int PMS3003::get_pm10_air() {
    update_cache();
    return pm10_air;
}

/*
One package has 32 bytes. Illustrate the formate by using below raw data:
    42 4d 00 1c 00 1b 00 21 00 29 00 1a 00 21 00 29 2b fb 04 be 00 6b 00 10 00 04 00 04 67 00 04 46 

    42 4d : header signature
    00 1c : frame length, 0x001c = 28 bytes (not include header and this field)
    00 1b : PM1.0 under CF=1 
    00 21 : PM2.5 under CF=1 
    00 29 : PM10 under CF=1 
    00 1a : PM1.0 under air
    00 21 : PM2.5 under air
    00 29 : PM10 under air
    2b fb : number of pariticle, diameter size 0.3 um in 0.1 liter air
    04 be : number of pariticle, diameter size 0.5 um in 0.1 liter air
    00 6b : number of pariticle, diameter size 1.0 um in 0.1 liter air
    00 10 : number of pariticle, diameter size 2.5 um in 0.1 liter air
    00 04 : number of pariticle, diameter size 5.0 um in 0.1 liter air
    00 04 : number of pariticle, diameter size 10 um in 0.1 liter air
    67 : serial number
    00 : error code
    04 46 : checksum,
        0x42+0x4d+0x00+0x1c+0x00+0x1b+0x00+0x21+
        0x00+0x29+0x00+0x1a+0x00+0x21+0x00+0x29+
        0x2b+0xfb+0x04+0xbe+0x00+0x6b+0x00+0x10+
        0x00+0x04+0x00+0x04+0x67+0x00 = 0x0446
*/

void PMS3003::update_cache() {
    int header_idx;
    uint32_t checksum_calculate;
    uint32_t checksum_compare;
    unsigned char buf[PMS3003_BUF_SIZE];

    if (((millis() - last_update_time) < PMS3003_REFRESH_TIME) && (last_update_time != 0)) {
        return;
    }

    while (1) {
        memcpy(buf, rb, PMS3003_BUF_SIZE);

        header_idx = -1;
        for (int i = 0; i < PMS3003_BUF_SIZE - 1; i++) {
            if ((buf[i] == 0x42) && (buf[(i + 1)] == 0x4d)) {
                header_idx = i;
                break;
            }
        }

        if (header_idx >= 0) {
            // calculate checksum
            checksum_calculate = 0;
            for (int i = 0; i < 30; i++) {
                checksum_calculate += buf[(header_idx + i)];
            }
            checksum_compare = buf[(header_idx + 30)] << 8 | buf[(header_idx + 31)];
            if (checksum_calculate == checksum_compare) {
                break;
            }
        }
        delay(100); // no valid packet, wait 100ms and check again
    }
    pm1p0_cf1 = buf[(header_idx +  4)] << 8 | buf[(header_idx +  5)];
    pm2p5_cf1 = buf[(header_idx +  6)] << 8 | buf[(header_idx +  7)];
    pm10_cf1  = buf[(header_idx +  8)] << 8 | buf[(header_idx +  9)];
    pm1p0_air = buf[(header_idx + 10)] << 8 | buf[(header_idx + 11)];
    pm2p5_air = buf[(header_idx + 12)] << 8 | buf[(header_idx + 13)];
    pm10_air  = buf[(header_idx + 14)] << 8 | buf[(header_idx + 15)];

    last_update_time = millis();
}
