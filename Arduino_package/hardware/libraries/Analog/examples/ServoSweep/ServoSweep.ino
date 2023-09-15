/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep

 refined 2016/03/18 by Realtek

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-pwm-servo/
 */

#include <AmebaServo.h>

// create servo object to control a servo
// 4 servo objects can be created correspond to PWM pins
AmebaServo myservo;

// variable to store the servo position
int pos = 0;

// attaches the servo on PWM pins to the servo object
// check the board pin mapping for available PWM pins
int myservo_PWM_pin = 8;

void setup() {
    myservo.attach(myservo_PWM_pin);
}

void loop() {
    // goes from 0 degrees to 180 degrees in steps of 1 degree
    for (pos = 0; pos <= 180; pos += 1) {
        // tell servo to go to position in variable 'pos'
        myservo.write(pos);
        // waits 15ms for the servo to reach the position
        delay(15);
    }

    // goes from 180 degrees to 0 degrees
    for (pos = 180; pos >= 0; pos -= 1) {
        // tell servo to go to position in variable 'pos'
        myservo.write(pos);
        // waits 15ms for the servo to reach the position
        delay(15);
    }
}
