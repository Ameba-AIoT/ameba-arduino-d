/*

Compile:
linux:
g++ -o upload_image_tool_linux tools/linux/src/upload_image_tool_linux.cpp -static
*/

#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <string>
#include <cstdio>
#include <sys/stat.h>
#include <fstream>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
    string cmd;
    stringstream cmdss;

    // change directory to {runtime.tools.ameba_d_tools.path}
    chdir(argv[1]);

    // copy the "imgtool_flashloader_amebad.bin" to current directory
    cmd = "cp tools/linux/image_tool/imgtool_flashloader_amebad.bin ./";
    cout << cmd << endl;
    system(cmd.c_str());

    // 5 seconds count down to allow user setting ameba D to UART download mode
    cmd = "Please enter the upload mode (wait 5s)";
    cout << cmd << endl;
    for (int i = 5; i > 0; i--) {
        sleep(1);
        cmd = to_string(i);
        cout << "    0" << cmd << endl;
    }

    cmdss.clear();
    // run image tool with serial port as argument
    cmdss << "./tools/linux/image_tool/amebad_image_tool " << argv[2];

    // copy command in cmdss to cmd
    cmd = cmdss.str();
    system(cmd.c_str());

    cmd = "    Upload Image done. ";
    cout << cmd << endl;

    return 0;
}
