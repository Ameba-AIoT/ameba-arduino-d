/*

Compile:

macos:
g++ -o upload_image_tool_macos upload_image_tool_macos.cpp

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

//#include <windows.h>

using namespace std;

int main(int argc, char *argv[]) {
    string cmd;
    stringstream cmdss;

    chdir(argv[1]);

    cmd = "cp tools/macos/image_tool/imgtool_flashloader_amebad.bin ./";
    cout << cmd << endl;
    system(cmd.c_str());

    cmd = "Please enter the upload mode (wait 5s)";
    cout << cmd << endl;
    for (int i = 5; i > 0; i--) {
        sleep(1);
        cmd = to_string(i);
        cout << "    0" << cmd << endl;
    }

    cmdss.clear();
    cmdss << "./tools/macos/image_tool/amebad_image_tool " << argv[2];
    getline(cmdss, cmd);
    cout << cmd << endl;
    system(cmd.c_str());

    cmd = "    Upload Image done. ";
    cout << cmd << endl;

    return 0;
}
