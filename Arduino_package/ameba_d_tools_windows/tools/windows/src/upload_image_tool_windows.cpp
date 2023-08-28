/*

Compile:

windows:
mingw32-g++.exe -o upload_image_tool_windows.exe upload_image_tool_windows.cpp -static
i686-w64-mingw32-g++.exe -o upload_image_tool_windows.exe upload_image_tool_windows.cpp -static
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

#include <windows.h>

#include <thread>
//#include <mutex>

using namespace std;

//mutex mu;
int check_image_upload = 0;

void image_tool_thread(char* t1_com) {
	stringstream t1_cmdss;
	string t1_cmd;
	t1_cmdss.clear();
	t1_cmdss << ".\\tools\\windows\\image_tool\\amebad_image_tool.exe " << t1_com;
	getline(t1_cmdss, t1_cmd);
	system(t1_cmd.c_str());
//	mu.lock();
	check_image_upload = 100;
//	mu.unlock();
}

void image_tool_progress() {
	cout << "Uploading." << flush;
	while (1) {
		Sleep(500);
//		mu.lock();
		if (check_image_upload != 0) {
			break;
		} else {
			cout << "." << flush;
		}
//		mu.unlock();
	}
	cout << "    Upload Image done. " << endl;
}


int main(int argc, char *argv[]) {
	string cmd;
	chdir(argv[1]);

	cmd = "Please enter the upload mode (wait 5s)";
	cout << cmd << endl;
	for (int i = 5; i > 0; i--)
	{
		Sleep(1000);
		cmd = to_string(i);
		cout << "    0" << cmd << endl;
	}

	thread t2(image_tool_progress);
	thread t1(image_tool_thread, argv[2]);

//	t2.detach();
//	t1.detach();
	t2.join();
	t1.join();

	return 0;
}
