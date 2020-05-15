/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep

 refined 2016/03/18 by Realtek
*/

#include <AmebaServo.h>

// create servo object to control a servo
// 4 servo objects can be created correspond to PWM pins
AmebaServo myservo;

// variable to store the servo position
int pos = 0;

void setup() {
#if defined(BOARD_RTL8195A)
    // attaches the servo on pin 9 to the servo object
    myservo.attach(9);
#elif defined(BOARD_RTL8710)
    // attaches the servo on pin 13 to the servo object
    myservo.attach(13);
#elif defined(BOARD_RTL8721D)
    // attaches the servo on pin 8 to the servo object
    myservo.attach(8);
#else
    // attaches the servo on pin 9 to the servo object
    myservo.attach(9);
#endif
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
