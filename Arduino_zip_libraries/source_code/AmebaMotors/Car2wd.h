#ifndef _CAR2WD_H_
#define _CAR2WD_H_

#include <stdint.h>
#include "Arduino.h"

// The min pwm value. If value lower than this, then the motor won't move.
#define CAR_MIN_SPEED 150

#define CAR_STOP           0x00
#define CAR_FORWARD        0x01
#define CAR_BACKWARD       0x02
#define CAR_ROTATE_RIGHT   0x03
#define CAR_ROTATE_LEFT    0x04
#define CAR_FORWARD_RIGHT  0x05
#define CAR_FORWARD_LEFT   0x06
#define CAR_BACKWARD_RIGHT 0x07
#define CAR_BACKWARD_LEFT  0x08

class Car2wd {
public:
    // These pins correspond pins of L298N brige. "_enA" and "_enB" has to be PWM pin.
	Car2wd(int _enA, int _in1, int _in2, int _in3, int _in4, int _enB);
    ~Car2wd();

	void begin();
    void end();

    void setAction(unsigned char action, unsigned char speed=200);
    void stop();

    friend void carTask(const void *arg);

private:
    int enA;
    int in1;
    int in2;
    int in3;
    int in4;
    int enB;

    uint32_t in1_port;
    uint32_t in1_bitmask;
    uint32_t in2_port;
    uint32_t in2_bitmask;
    uint32_t in3_port;
    uint32_t in3_bitmask;
    uint32_t in4_port;
    uint32_t in4_bitmask;

    uint32_t tid;

    unsigned char currentAction;
    int           currentSpeed;
};

#endif

