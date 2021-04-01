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

#if defined(ARDUINO) && !defined(RTL8722DM)
#define ARDUINO_EXCLUDE_CODE
#endif  // defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)

#ifndef ARDUINO_EXCLUDE_CODE

#include "output_handler.h"

#include "Arduino.h"

#if defined(BOARD_RTL8722D)
    #define LED_B 13
    #define LED_G 12
#endif

void HandleOutput(tflite::ErrorReporter* error_reporter, int kind) {
  // The first time this method runs, set up our LED
  static bool is_initialized = false;
  if (!is_initialized) {
    pinMode(LED_B, OUTPUT);
    pinMode(LED_G, OUTPUT);
    digitalWrite(LED_B, LOW);
    digitalWrite(LED_G, LOW);
    is_initialized = true;
  }

  // Print some ASCII art for each gesture and control the LED.
  if (kind == 0) {
    digitalWrite(LED_B, HIGH);
    digitalWrite(LED_G, HIGH);
    TF_LITE_REPORT_ERROR(
        error_reporter,
        "WING:\n\r*         *         *\n\r *       * *       "
        "*\n\r  *     *   *     *\n\r   *   *     *   *\n\r    * *       "
        "* *\n\r     *         *\n\r");
  } else if (kind == 1) {
    digitalWrite(LED_B, LOW);
    digitalWrite(LED_G, HIGH);
    TF_LITE_REPORT_ERROR(
        error_reporter,
        "RING:\n\r          *\n\r       *     *\n\r     *         *\n\r "
        "   *           *\n\r     *         *\n\r       *     *\n\r      "
        "    *\n\r");
  } else if (kind == 2) {
    digitalWrite(LED_B, HIGH);
    digitalWrite(LED_G, LOW);
    TF_LITE_REPORT_ERROR(
        error_reporter,
        "SLOPE:\n\r        *\n\r       *\n\r      *\n\r     *\n\r    "
        "*\n\r   *\n\r  *\n\r * * * * * * * *\n\r");
  }
}

#endif  // ARDUINO_EXCLUDE_CODE
