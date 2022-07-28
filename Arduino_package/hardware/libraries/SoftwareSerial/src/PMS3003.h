#ifndef _PMS3003_H_
#define _PMS3003_H_

#include <inttypes.h>

#define PMS3003_BUF_SIZE 64
#define PMS3003_REFRESH_TIME 2000

class PMS3003 {

public:
    PMS3003(int _rx, int _tx, int _set = -1, int _reset = -1);

    void begin();
    void end();

    int get_pm1p0_cf1();
    int get_pm2p5_cf1();
    int get_pm10_cf1();
    int get_pm1p0_air();
    int get_pm2p5_air();
    int get_pm10_air();

    friend void pms3003_handle_interrupt(uint32_t id, uint32_t event);

private:
    void update_cache();

    int setpin;
    int rxpin;
    int txpin;
    int resetpin;

    int rbidx;
    unsigned char rb[PMS3003_BUF_SIZE];

    uint32_t last_update_time;

    int pm1p0_cf1;
    int pm2p5_cf1;
    int pm10_cf1;
    int pm1p0_air;
    int pm2p5_air;
    int pm10_air;

    void *pUART;
};

#endif
