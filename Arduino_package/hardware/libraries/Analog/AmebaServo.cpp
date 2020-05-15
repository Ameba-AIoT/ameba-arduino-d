/*
 Servo.cpp - Interrupt driven Servo library for Arduino using 16 bit timers- Version 2
 Copyright (c) 2009 Michael Margolis.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 2016/03/18 Refined by Realtek
 */

#include <inttypes.h>
#include <Arduino.h>
#include "AmebaServo.h"

#ifdef __cplusplus
extern "C" {

#include "pwmout_api.h"

extern void *gpio_pin_struct[];

}
#endif

AmebaServo::AmebaServo()
{
    servoPin = 0xFFFFFFFF;
}

uint8_t AmebaServo::attach(int pin)
{
    return this->attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t AmebaServo::attach(int pin, int min, int max)
{
    pinRemoveMode(pin);
    analogWrite(pin, 0);
    pwmout_period_us(((pwmout_t *)gpio_pin_struct[pin]), 20000);

    servoPin = pin;
    this->min = min;
    this->max = max;

    return 0;
}

void AmebaServo::detach()
{
    pinRemoveMode(servoPin);
    servoPin = 0xFFFFFFFF;
}

void AmebaServo::write(int value)
{
    if(value < 0) value = 0;
    if(value > 180) value = 180;

    currentWidth = value;

    value = (max - min) * value / 180 + min;
    this->writeMicroseconds(value);
}

void AmebaServo::writeMicroseconds(int value)
{
    if (value < (int)min) value = min;
    if (value > (int)max) value = max;

    currentWidth = 180 * (value - min) / (max - min);

    pwmout_write(((pwmout_t *)gpio_pin_struct[servoPin]), (value * 1.0 / 20000));
}

int AmebaServo::read() // return the value as degrees
{
    return (180 * (currentWidth - min) / (max - min));
}

int AmebaServo::readMicroseconds()
{
    return currentWidth;
}

bool AmebaServo::attached()
{
    return (servoPin == 0xFFFFFFFF);
}
