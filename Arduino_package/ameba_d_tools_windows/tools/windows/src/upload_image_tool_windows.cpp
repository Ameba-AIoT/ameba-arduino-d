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

using namespace std;

int main(int argc, char *argv[]) {

	string cmd;
	stringstream cmdss;

	chdir(argv[1]);

	cmd = "copy tools\\windows\\image_tool\\imgtool_flashloader_amebad.bin .\\";
	cout << cmd << endl;
	system(cmd.c_str());

	cmd = "Please enter the upload mode (wait 5s)";
	cout << cmd << endl;
	for (int i = 5; i > 0; i--)
	{
		Sleep(1000);
		cmd = to_string(i);
		cout << "    0" << cmd << endl;
	}

	cmdss.clear();
	cmdss << ".\\tools\\windows\\image_tool\\amebad_image_tool.exe " << argv[2];
	getline(cmdss, cmd);
	//cout << cmd << endl;
	system(cmd.c_str());

	printf("\r\n");
	cmd = "    Upload Image done. ";
	cout << cmd << endl;

	return 0;
}
