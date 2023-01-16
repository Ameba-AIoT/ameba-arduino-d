#include <string>

class WifiSerial {
	public:
	    WifiSerial();
	    std::string readInput();

	private:
	    std::string currentInput;
};
