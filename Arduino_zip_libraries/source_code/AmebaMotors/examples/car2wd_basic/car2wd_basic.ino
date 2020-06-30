/*
 *  This sketch shows how to use Car2wd class to control L298N on 2WD car
 *
 **/

#include "Car2wd.h"

Car2wd car(8, 9, 10, 11, 12, 13);

void setup() {
    car.begin();
}

void loop() {
    car.setAction(CAR_FORWARD);
    delay(2000);
    car.setAction(CAR_STOP);
    delay(1000);
    car.setAction(CAR_BACKWARD);
    delay(2000);
    car.setAction(CAR_STOP);
    delay(1000);
    car.setAction(CAR_FORWARD_RIGHT);
    delay(2000);
    car.setAction(CAR_STOP);
    delay(1000);
    car.setAction(CAR_FORWARD_LEFT);
    delay(2000);
    car.setAction(CAR_STOP);
    delay(1000);
    car.setAction(CAR_BACKWARD_LEFT);
    delay(2000);
    car.setAction(CAR_STOP);
    delay(1000);
    car.setAction(CAR_BACKWARD_RIGHT);
    delay(2000);
    car.setAction(CAR_STOP);
    delay(1000);
}
