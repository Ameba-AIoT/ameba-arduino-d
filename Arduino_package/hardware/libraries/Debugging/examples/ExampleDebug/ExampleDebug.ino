
// Debugging requires the use of a SEGGER JLink debug probe, and that SEGGER JLink Software Pack is installed
// For Windows, SEGGER JLink Software Pack should be installed at C:/Program Files/SEGGER/JLink/
// SEGGER JLink Software Pack can be downloaded at https://www.segger.com/downloads/jlink/

// Before debugging, make sure to edit debug_custom.json and ensure that the "serverArgs" field has the correct directory to the JLinkScript file
// Default directories are shown below, where you will need to replace * with your account username, and replace ** with the version number of the installed tools:
// Windows: C:/Users/*/AppData/Local/Arduino15/packages/realtek/tools/ameba_d_tools/**/debug/RTL872x_Arduino_debug.JLinkScript
// Linux:	/home/*/.arduino15/packages/realtek/tools/ameba_d_tools/**/debug/RTL872x_Arduino_debug.JLinkScript
// MacOS:	/Users/*/Library/Arduino15/packages/realtek/tools/ameba_d_tools/**/debug/RTL872x_Arduino_debug.JLinkScript

// Also, ensure you have enabled Sketch > Optimize for Debugging

// If you wish to debug your own sketches, copy the debug_custom.json file into your sketch folder

#define LED_PIN LED_BUILTIN

int i;
void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    for (i = 0; i < 10; i++) {
        digitalWrite(LED_PIN, HIGH);
        delay(100);
        digitalWrite(LED_PIN, LOW);
        delay(100);
    }
    i = 0;
}

void loop() {
    i++;
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    if ((i % 10) == 0) {
        Serial.println(i);
        //RTL8722 only supports a maximum of 2 hardware breakpoints. If you need more breakpoints, you can use either of the instructions below to add a software breakpoint in program execution
        //Note: To continue program execution from a software breakpoint, use "Step Over" first before "Continue"
        //__BKPT();
        __asm__("BKPT");
    }
    digitalWrite(LED_PIN, LOW);
    delay(500);
}
