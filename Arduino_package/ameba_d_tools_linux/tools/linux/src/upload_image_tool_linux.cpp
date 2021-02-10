/*

Compile:
linux:
g++ -o upload_image_tool_linux tools/linux/src/upload_image_tool_linux.cpp -static
g++ -std=c++11 -o upload_image_tool_linux upload_image_tool_linux.cpp -static
g++ -std=c++11 -o upload_image_tool_linux upload_image_tool_linux.cpp -lpthread

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

#include <thread>
//#include <mutex>

#include <time.h>
#include <errno.h>  

using namespace std;

//mutex mu;
int check_image_upload = 0;

int msleep(long msec) {
    struct timespec ts;
    int res;

    if (msec < 0) {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

void image_tool_thread(char* t1_com) {
    stringstream t1_cmdss;
    string t1_cmd;

    t1_cmdss.clear();
    // run image tool with serial port as argument
    t1_cmdss << "./tools/linux/image_tool/amebad_image_tool " << t1_com;

    // copy command in cmdss to cmd
    t1_cmd = t1_cmdss.str();
    system(t1_cmd.c_str());

//  mu.lock();
    check_image_upload = 100;
//  mu.unlock();
}

void image_tool_progress() {
    cout << "Uploading." << flush;
    while (1) {
        msleep(500);
//      mu.lock();
        if (check_image_upload != 0) {
            break;
        } else {
            cout << "." << flush;
        }
//      mu.unlock();
    }
    cout << "    Upload Image done. " << endl;
}

int main(int argc, char *argv[]) {
    string cmd;

    // change directory to {runtime.tools.ameba_d_tools.path}
    chdir(argv[1]);

    // 5 seconds count down to allow user setting ameba D to UART download mode
    cmd = "Please enter the upload mode (wait 5s)";
    cout << cmd << endl;
    for (int i = 5; i > 0; i--) {
        sleep(1);
        cmd = to_string(i);
        cout << "    0" << cmd << endl;
    }

    thread t2(image_tool_progress);
    thread t1(image_tool_thread, argv[2]);

//  t2.detach();
//  t1.detach();
    t2.join();
    t1.join();

    return 0;
}
