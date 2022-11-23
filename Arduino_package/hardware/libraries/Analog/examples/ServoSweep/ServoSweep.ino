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
#if defined(BOARD_RTL8722DM)
    // attaches the servo on PWM pin 8 to the servo object
    myservo.attach(8);
#elif defined(BOARD_RTL8722DM_MINI)
    // attaches the servo on PWM pin 7 to the servo object
    myservo.attach(7);
#elif defined(BOARD_RTL8720DN_BW16)
    // attaches the servo on PWM pin PA25 to the servo object
    myservo.attach(PA25);
#elif defined(BOARD_RTL8721DM)
    // attaches the servo on PWM pin 3 to the servo object
    myservo.attach(3);
#elif defined(BOARD_RTL8720DF)
    // attaches the servo on PWM pin 7 to the servo object
    myservo.attach(7);
#else
    // attaches the servo on PWM pin 10 to the servo object
    myservo.attach(10);
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
