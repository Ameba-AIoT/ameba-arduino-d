/*
 * This sketch shows how to use L298N to control 2 wheels car (or 4 wheels by 2 phases)
 *
 * Materials needed:
 *     L298N x 1
 *     Car kit:
 *         car body x 1
 *         DC motors x 2 (or x 4)
 *         wheels x 2 (or x4)
 *
 * L298N has 6 input controls pins:
 *     ENA - control the strength of motor A
 *     IN1 - control line of motor A
 *     IN2 - control line of motor A
 *     IN3 - control line of motor B
 *     IN4 - control line of motor B
 *     ENA - control the strength of motor B
 *
 * IN1:1 IN2:0 - motor clockwise rotation
 * IN1:0 IN2:1 - motor counter-clockwirse rotation
 * IN1:0 IN2:0 - motor stops
 * 
 * The wheel may have poor grip or the motors has poor sensibility.
 * It makes "forward right" cannot performed by makeing 2 motors with different 
 * motor rotating speed. To achieve "forward right" we can just make right 
 * wheels work in partial of times.
 **/

// motor left
int enA = 8;
int in1 = 9;
int in2 = 10;

// motor right
int in3 = 11;
int in4 = 12;
int enB = 13;

uint32_t in1_port, in1_bitmask;
uint32_t in2_port, in2_bitmask;
uint32_t in3_port, in3_bitmask;
uint32_t in4_port, in4_bitmask;

void initMotor() {
    // set all the motor control pins to outputs with value 0
    pinMode(enA, OUTPUT);
    analogWrite(enA, 0);
    pinMode(enB, OUTPUT);
    analogWrite(enB, 0);

    pinMode(in1, OUTPUT);
    digitalWrite(in1, LOW);
    pinMode(in2, OUTPUT);
    digitalWrite(in2, LOW);
    pinMode(in3, OUTPUT);
    digitalWrite(in3, LOW);
    pinMode(in4, OUTPUT);
    digitalWrite(in4, LOW);

    in1_port = digitalPinToPort(in1);
    in1_bitmask = digitalPinToBitMask(in1);
    in2_port = digitalPinToPort(in2);
    in2_bitmask = digitalPinToBitMask(in2);
    in3_port = digitalPinToPort(in3);
    in3_bitmask = digitalPinToBitMask(in3);
    in4_port = digitalPinToPort(in4);
    in4_bitmask = digitalPinToBitMask(in4);
}

void motorsControl(int speedLeft, int speedRight) {
    analogWrite(enA, abs(speedLeft));
    analogWrite(enB, abs(speedRight));

    if (speedLeft > 0) {
        *portOutputRegister(in1_port) |=  in1_bitmask;
        *portOutputRegister(in2_port) &= ~in2_bitmask;
    } else if (speedLeft < 0) {
        *portOutputRegister(in1_port) &= ~in1_bitmask;
        *portOutputRegister(in2_port) |=  in2_bitmask;
    } else {
        *portOutputRegister(in1_port) &= ~in1_bitmask;
        *portOutputRegister(in2_port) &= ~in2_bitmask;
    }

    if (speedRight > 0) {
        *portOutputRegister(in3_port) |=  in3_bitmask;
        *portOutputRegister(in4_port) &= ~in4_bitmask;
    } else if (speedRight < 0) {
        *portOutputRegister(in3_port) &= ~in3_bitmask;
        *portOutputRegister(in4_port) |=  in4_bitmask;
    } else {
        *portOutputRegister(in3_port) &= ~in3_bitmask;
        *portOutputRegister(in4_port) &= ~in4_bitmask;
    }
}

void forward() {
    motorsControl(200, 200);
}

void backward() {
    motorsControl(-200, -200);
}

void stop() {
    motorsControl(0, 0);
}

void rotateRight() {
    motorsControl(200, -200);
}

void rotateLeft() {
    motorsControl(-200, 200);
}

void setup() {
    initMotor();
}

void demo1() {
    forward();
    delay(2000);
    stop();
    delay(1000);

    backward();
    delay(2000);
    stop();
    delay(1000);

    rotateRight();
    delay(2000);
    stop();
    delay(1000);

    rotateLeft();
    delay(2000);
    stop();
    delay(1000);
}

void demo2() {
    /*  This function demo "forward right", "forward left", "backward left", "backward right".
    *  If the motors on the car have good sensitiy on input signal and high grip on the ground, 
    *  then you can just adjust the ratio of 2 pwm values.
    *    Ex. motorControl(255,150) makes car forward right.
    *  Otherwise, we can just adjust the working ratio of tire.
    *    Ex. work 20ms, then stop 80ms, ...
    **/

    forward();
    // forward right
    for (int i = 0; i < 40; i++) {
        *portOutputRegister(in3_port) |=  in3_bitmask;
        delay(20);
        *portOutputRegister(in3_port) &= ~in3_bitmask;
        delay(80);
    }
    *portOutputRegister(in3_port) |=  in3_bitmask;
    // forward left
    for (int i = 0; i < 40; i++) {
        *portOutputRegister(in1_port) |=  in1_bitmask;
        delay(20);
        *portOutputRegister(in1_port) &= ~in1_bitmask;
        delay(80);
    }
    *portOutputRegister(in1_port) |=  in1_bitmask;
    stop();
    delay(1000);

    backward();
    // backward left
    for (int i = 0; i < 40; i++) {
        *portOutputRegister(in2_port) |=  in2_bitmask;
        delay(20);
        *portOutputRegister(in2_port) &= ~in2_bitmask;
        delay(80);
    }
    *portOutputRegister(in2_port) |=  in2_bitmask;
    // backward right
    for (int i = 0; i < 40; i++) {
        *portOutputRegister(in4_port) |=  in4_bitmask;
        delay(20);
        *portOutputRegister(in4_port) &= ~in4_bitmask;
        delay(80);
    }
    *portOutputRegister(in4_port) |=  in4_bitmask;
    stop();
    delay(1000);
}

void loop() {
    demo1();
    demo2();
}
