class WifiSerial {
    public:
    WifiSerial();
    const char* readInput();

    private:
        const char* currentInput;
};
