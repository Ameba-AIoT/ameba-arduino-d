#include "Car2wd.h"
#include "Arduino.h"

#define CAR_ACTION_CHANGE 0x01
#define CAR_SPEED_CHANGE  0x02

/* The pwm output to enA and enB of L28N can only output to motoer with 2.5V~3V.
 * It cannot make signaficantly forward right/left.
 * We use solution that make the slow moter run a little then stop.
 **/
#define DELAY_TURN (1)

#if defined(BOARD_RTL8721D)
#define os_signal_wait os_signal_wait_arduino
#define os_signal_set os_signal_set_arduino
#define os_thread_create os_thread_create_arduino
#define os_thread_terminate os_thread_terminate_arduino
#endif

// A thread which handle car's actions without interfear main thread.
void carTask(const void *arg) {

    Car2wd *pCar = (Car2wd *) arg;
    os_event_t evt;

    uint32_t dfactor;
    uint32_t dport;
    uint32_t dbitmask;

    while(1) {
        // wait signal for infinite timeout
        evt = os_signal_wait(0, 0xFFFFFFFF);

        if (evt.status != OS_EVENT_SIGNAL) {
            continue;
        }

        if (evt.value.signals & CAR_SPEED_CHANGE) {
            analogWrite(pCar->enA, pCar->currentSpeed);
            analogWrite(pCar->enB, pCar->currentSpeed);

            /* only apply to forward right/left and backward right/left
             * The motor run time is 20ms, and stop time is 20ms~360ms.
             *                           speed - min_speed
             *     dfactor = 20 + 360 * -------------------
             *                            255 - min_speed
             */ 
            dfactor = 20 + (360 * (pCar->currentSpeed - CAR_MIN_SPEED))/(255 - CAR_MIN_SPEED);
        }

        if (evt.value.signals & CAR_ACTION_CHANGE) {
            switch(pCar->currentAction) {
                case CAR_STOP:
                    *portOutputRegister(pCar->in1_port) &= ~(pCar->in1_bitmask);
                    *portOutputRegister(pCar->in2_port) &= ~(pCar->in2_bitmask);
                    *portOutputRegister(pCar->in3_port) &= ~(pCar->in3_bitmask);
                    *portOutputRegister(pCar->in4_port) &= ~(pCar->in4_bitmask);
                    break;
                case CAR_FORWARD:
                case CAR_FORWARD_RIGHT:
                case CAR_FORWARD_LEFT:
                    *portOutputRegister(pCar->in1_port) |=  (pCar->in1_bitmask);
                    *portOutputRegister(pCar->in2_port) &= ~(pCar->in2_bitmask);
                    *portOutputRegister(pCar->in3_port) |=  (pCar->in3_bitmask);
                    *portOutputRegister(pCar->in4_port) &= ~(pCar->in4_bitmask);
                    if (pCar->currentAction == CAR_FORWARD_RIGHT) {
                        dport = pCar->in3_port;
                        dbitmask = pCar->in3_bitmask;
                    }
                    if (pCar->currentAction == CAR_FORWARD_LEFT) {
                        dport = pCar->in1_port;
                        dbitmask = pCar->in1_bitmask;
                    }
                    break;
                case CAR_BACKWARD:
                case CAR_BACKWARD_RIGHT:
                case CAR_BACKWARD_LEFT:
                    *portOutputRegister(pCar->in1_port) &= ~(pCar->in1_bitmask);
                    *portOutputRegister(pCar->in2_port) |=  (pCar->in2_bitmask);
                    *portOutputRegister(pCar->in3_port) &= ~(pCar->in3_bitmask);
                    *portOutputRegister(pCar->in4_port) |=  (pCar->in4_bitmask);
                    if (pCar->currentAction == CAR_BACKWARD_RIGHT) {
                        dport = pCar->in4_port;
                        dbitmask = pCar->in4_bitmask;
                    }
                    if (pCar->currentAction == CAR_BACKWARD_LEFT) {
                        dport = pCar->in2_port;
                        dbitmask = pCar->in2_bitmask;
                    }
                    break;
                case CAR_ROTATE_RIGHT:
                    *portOutputRegister(pCar->in1_port) |=  (pCar->in1_bitmask);
                    *portOutputRegister(pCar->in2_port) &= ~(pCar->in2_bitmask);
                    *portOutputRegister(pCar->in3_port) &= ~(pCar->in3_bitmask);
                    *portOutputRegister(pCar->in4_port) |=  (pCar->in4_bitmask);
                    break;
                case CAR_ROTATE_LEFT:
                    *portOutputRegister(pCar->in1_port) &= ~(pCar->in1_bitmask);
                    *portOutputRegister(pCar->in2_port) |=  (pCar->in2_bitmask);
                    *portOutputRegister(pCar->in3_port) |=  (pCar->in3_bitmask);
                    *portOutputRegister(pCar->in4_port) &= ~(pCar->in4_bitmask);
                    break;
            }
        }
        if (pCar->currentAction >= CAR_FORWARD_RIGHT && pCar->currentAction <= CAR_BACKWARD_LEFT) {
#if DELAY_TURN
            while(1) {
                *portOutputRegister(dport) |=  dbitmask;
                evt = os_signal_wait(0, 20);
                if (evt.status == OS_EVENT_SIGNAL) {
                    os_signal_set(pCar->tid, evt.value.signals);
                    break;
                }

                *portOutputRegister(dport) &= ~dbitmask;
                evt = os_signal_wait(0, dfactor);
                if (evt.status == OS_EVENT_SIGNAL) {
                    *portOutputRegister(dport) |=  dbitmask;
                    os_signal_set(pCar->tid, evt.value.signals);
                    break;
                }
            }
#else
            *portOutputRegister(dport) &= ~dbitmask;
#endif
        }
    }
}

Car2wd::Car2wd(int _enA, int _in1, int _in2, int _in3, int _in4, int _enB) {
    enA = _enA;
    in1 = _in1;
    in2 = _in2;
    in3 = _in3;
    in4 = _in4;
    enB = _enB;

    currentAction = CAR_STOP;
}

Car2wd::~Car2wd() {
    end();
}

void Car2wd::begin() {
    // set all the motor control pins to outputs with value 0
    pinMode(enA, OUTPUT);
    analogWrite(enA, 0);
    pinMode(enB, OUTPUT);
    analogWrite(enB, 0);

    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);

    // cache each pin's port and bit mask for faster gpio control
    in1_port = digitalPinToPort(in1);
    in1_bitmask = digitalPinToBitMask(in1);
    in2_port = digitalPinToPort(in2);
    in2_bitmask = digitalPinToBitMask(in2);
    in3_port = digitalPinToPort(in3);
    in3_bitmask = digitalPinToBitMask(in3);
    in4_port = digitalPinToPort(in4);
    in4_bitmask = digitalPinToBitMask(in4);

    // create a thread to control the car
    tid = os_thread_create(carTask, this, OS_PRIORITY_REALTIME, 1024);
}

void Car2wd::end() {
    os_thread_terminate(tid);
}

void Car2wd::setAction(unsigned char action, unsigned char speed) {

    int32_t sig = 0;

    if (currentAction != action) {
        currentAction = action;
        sig |= CAR_ACTION_CHANGE;
    }
    if (currentSpeed != speed) {
        currentSpeed = speed;
        sig |= CAR_SPEED_CHANGE;
    }
    if (sig != 0) {
        // send signal to car thread
        os_signal_set(tid, sig);
    }
}

void Car2wd::stop() {
    setAction(CAR_STOP, currentSpeed);
}

#if defined(BOARD_RTL8721D)
#undef os_signal_wait
#undef os_signal_set
#undef os_thread_create
#undef os_thread_terminate
#endif
