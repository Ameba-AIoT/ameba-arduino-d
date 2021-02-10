/*

Compile:

macos:
g++ -o upload_image_tool_macos upload_image_tool_macos.cpp
g++ -std=c++11 -o upload_image_tool_macos upload_image_tool_macos.cpp


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
    t1_cmdss << "./tools/macos/image_tool/amebad_image_tool " << t1_com;
    getline(t1_cmdss, t1_cmd);
    cout << t1_cmd << endl;
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

    chdir(argv[1]);

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
