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
#endif    // defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)

#ifndef ARDUINO_EXCLUDE_CODE

#include "output_handler.h"

#include "Arduino.h"

#if defined(BOARD_AMB21_AMB22)
#define LEDB 13
#define LEDG 12

#elif defined(BOARD_AMB23)
#define LEDB LED_B
#define LEDG LED_G

#elif defined(BOARD_AITHINKER_BW16)
#define LEDB LED_B
#define LEDG LED_G

#elif defined(BOARD_SPARKFUN_AWCU488)
#define LEDB 13
#define LEDG 12

#elif defined(BOARD_AMB25) || defined(BOARD_AMB26) || defined(BOARD_UBLOX_NORAW30) || defined(BOARD_SPARKFUN_NORAW306) || defined(BOARD_DATALOGGER_AMB26)
#define LEDB 4
#define LEDG 5

#endif

void HandleOutput(tflite::ErrorReporter* error_reporter, int kind)
{
    // The first time this method runs, set up our LED
    static bool is_initialized = false;
    if (!is_initialized) {
        pinMode(LEDB, OUTPUT);
        pinMode(LEDG, OUTPUT);
        digitalWrite(LEDB, LOW);
        digitalWrite(LEDG, LOW);
        is_initialized = true;
    }

    // Print some ASCII art for each gesture and control the LED.
    if (kind == 0) {
        digitalWrite(LEDB, HIGH);
        digitalWrite(LEDG, HIGH);
        TF_LITE_REPORT_ERROR(
            error_reporter,
            "WING:\n\r*         *         *\n\r *       * *       "
            "*\n\r  *     *   *     *\n\r   *   *     *   *\n\r    * *       "
            "* *\n\r     *         *\n\r");
    } else if (kind == 1) {
        digitalWrite(LEDB, LOW);
        digitalWrite(LEDG, HIGH);
        TF_LITE_REPORT_ERROR(
            error_reporter,
            "RING:\n\r          *\n\r       *     *\n\r     *         *\n\r "
            "   *           *\n\r     *         *\n\r       *     *\n\r      "
            "    *\n\r");
    } else if (kind == 2) {
        digitalWrite(LEDB, HIGH);
        digitalWrite(LEDG, LOW);
        TF_LITE_REPORT_ERROR(
            error_reporter,
            "SLOPE:\n\r        *\n\r       *\n\r      *\n\r     *\n\r    "
            "*\n\r   *\n\r  *\n\r * * * * * * * *\n\r");
    }
}

#endif    // ARDUINO_EXCLUDE_CODE
