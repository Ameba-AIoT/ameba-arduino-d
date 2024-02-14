/*
  Blink and Fade for the RGB LED on the Ai-Thinker BW16 (RTL8720DN)

  Turns the RGB LED on for one second, then off for one second. 
  Then fades RED and BLUE up and down.
  
  Note: GREEN can only Blink, it is not connected to a PWM Pin.

  This example code is in the public domain.
*/

#ifndef BOARD_RTL8720DN_BW16
#error "Please use this example on the BW16 Board"
#endif

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin's from all 3 colors as an output.
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
}

// blink RED, GREEN, BLUE
void blink() {
  digitalWrite(LED_R, HIGH);  // turn the RED LED on (HIGH is the voltage level)
  delay(1000);                // wait for a second
  digitalWrite(LED_R, LOW);   // turn the RED LED off by making the voltage LOW
  delay(1000);                // wait for a second
  digitalWrite(LED_G, HIGH);  // turn the GREEN LED on (HIGH is the voltage level)
  delay(1000);                // wait for a second
  digitalWrite(LED_G, LOW);   // turn the GREEN LED off by making the voltage LOW
  delay(1000);                // wait for a second
  digitalWrite(LED_B, HIGH);  // turn the BLUE LED on (HIGH is the voltage level)
  delay(1000);                // wait for a second
  digitalWrite(LED_B, LOW);   // turn the BLUE LED off by making the voltage LOW
  delay(1000);                // wait for a second
}

// fade RED and BLUE
void fade() {
  // fade RED in from min to max in increments of 5 points:
  for (int fadeValue = 0; fadeValue <= 255; fadeValue += 5) {
    // sets the value (range from 0 to 255):
    analogWrite(LED_R, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }

  // fade RED out from max to min in increments of 5 points:
  for (int fadeValue = 255; fadeValue >= 0; fadeValue -= 5) {
    // sets the value (range from 0 to 255):
    analogWrite(LED_R, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }

  // fade BLUE in from min to max in increments of 5 points:
  for (int fadeValue = 0; fadeValue <= 255; fadeValue += 5) {
    // sets the value (range from 0 to 255):
    analogWrite(LED_B, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }

  // fade BLUE out from max to min in increments of 5 points:
  for (int fadeValue = 255; fadeValue >= 0; fadeValue -= 5) {
    // sets the value (range from 0 to 255):
    analogWrite(LED_B, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }
}

// the loop function runs over and over again forever
void loop() {
  blink();      // blink RED, GREEN, BLUE
  fade();       // fade RED, BLUE
  delay(1000);  // wait for a second
}
