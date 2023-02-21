#ifndef WifiSerial_h
#define WifiSerial_h

class WifiSerial {
    public:
    WifiSerial();
    const char* readInput();

    private:
        const char* currentInput;
};

#endif
