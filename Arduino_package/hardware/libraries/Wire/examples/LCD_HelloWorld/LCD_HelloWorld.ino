/*
 This example demonstrate how to use I2C LCD

 LCD usually has these pins:
    En: Enable
    Rw: Read/Write
    Rs: Reset
    Bl: Backlight
    D4: LCD data 0
    D5: LCD data 1
    D6: LCD data 2
    D7: LCD data 3

 The I2C extender module use half byte as control signal, 
 and the other half byte as data signal.
 So different LCD may correspond different bit mapping of these pins.

 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCM1602 I2C LCD
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

//LiquidCrystal_I2C lcd(0x38);  // Set the LCD I2C address

//LiquidCrystal_I2C lcd(0x38, BACKLIGHT_PIN, POSITIVE);  // Set the LCD I2C address

void setup() {
    Serial.begin(115200);

    lcd.begin(16, 2);               // initialize the lcd 
    lcd.backlight();

    lcd.setCursor (0, 0);           // go to home
    lcd.print("Hello World!");
    lcd.setCursor (0, 1);           // go to the next line
    lcd.print ("Ameba");
    delay(8000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Type to display");
    lcd.setCursor(0, 1);
    lcd.print("On SerialMonitor");
}

void loop() {
    if (Serial.available()) {
        delay(100);
        lcd.clear();
        while (Serial.available() > 0) {
            lcd.write(Serial.read());
        }
    }
}
