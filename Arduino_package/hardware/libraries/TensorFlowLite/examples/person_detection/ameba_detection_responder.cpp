/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#if defined(ARDUINO) && !defined(CORE_RTL8722DM)
#define ARDUINO_EXCLUDE_CODE
#endif    // defined(ARDUINO) && !defined(RTL8722DM)

#ifndef ARDUINO_EXCLUDE_CODE

#include "detection_responder.h"

#include "Arduino.h"

#if defined(BOARD_AMB21_AMB22)
#define LEDR 4
#define LEDG 5
#define LEDB 6

#elif defined(BOARD_AMB23)
#define LEDR 4
#define LEDG 5
#define LEDB 6

#elif defined(BOARD_AITHINKER_BW16)
#define LEDR 2
#define LEDG 6
#define LEDB 9

#elif defined(BOARD_SPARKFUN_AWCU488)
#define LEDR 11
#define LEDG 12
#define LEDB 13

#elif defined(BOARD_AMB25) || defined(BOARD_AMB26) || defined(BOARD_UBLOX_NORAW30) || defined(BOARD_SPARKFUN_NORAW306) || defined(BOARD_DATALOGGER_AMB26)
#define LEDR 3
#define LEDG 4
#define LEDB 5

#else
#error check the board supported
#endif

// Flash the blue LED after each inference
void RespondToDetection(tflite::ErrorReporter* error_reporter,
                        uint8_t person_score, uint8_t no_person_score)
{
    static bool is_initialized = false;
    if (!is_initialized) {
        // Pins for the built-in RGB LEDs on the Arduino Nano 33 BLE Sense
        pinMode(LEDR, OUTPUT);
        pinMode(LEDG, OUTPUT);
        pinMode(LEDB, OUTPUT);
        is_initialized = true;
    }

    // Switch the person/not person LEDs off
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDR, LOW);

    // Flash the blue LED after every inference.
    digitalWrite(LEDB, HIGH);
    delay(100);
    digitalWrite(LEDB, LOW);

    // Switch on the green LED when a person is detected,
    // the red when no person is detected
    if (person_score > no_person_score) {
        digitalWrite(LEDG, HIGH);
        digitalWrite(LEDR, LOW);
    } else {
        digitalWrite(LEDG, LOW);
        digitalWrite(LEDR, HIGH);
    }

    TF_LITE_REPORT_ERROR(error_reporter, "Person score: %d No person score: %d",
                         person_score, no_person_score);
}

#endif    // ARDUINO_EXCLUDE_CODE
