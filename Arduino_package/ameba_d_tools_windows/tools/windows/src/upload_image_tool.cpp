/*

Compile command:
mingw32-g++.exe -o upload_image_tool.exe tools\windows\src\upload_image_tool.cpp -static

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

	string filepath_km0_boot_all;
	string filepath_km4_boot_all;
	string filepath_km0_km4_image2;

	filepath_km0_boot_all.assign("km0_boot_all.bin");
	filepath_km4_boot_all.assign("km4_boot_all.bin");
	filepath_km0_km4_image2.assign("km0_km4_image2.bin");

	chdir(argv[1]);

	cmd = "copy tools\\windows\\image_tool\\imgtool_flashloader_amebad.bin .\\";
	//cout << cmd << endl;
	system(cmd.c_str());

	cmdss.clear();
	cmdss << ".\\tools\\windows\\image_tool\\1-10_MP_Image_Tool.exe -set baudrate 1500000";
	getline(cmdss, cmd);
	//cout << cmd << endl;
	system(cmd.c_str());

	cmdss.clear();
	cmdss << ".\\tools\\windows\\image_tool\\1-10_MP_Image_Tool.exe -remove device COM0";
	getline(cmdss, cmd);
	//cout << cmd << endl;
	system(cmd.c_str());

	cmdss.clear();
	cmdss << ".\\tools\\windows\\image_tool\\1-10_MP_Image_Tool.exe -add device " << argv[2];
	getline(cmdss, cmd);
	//cout << cmd << endl;
	system(cmd.c_str());

	cmd = "Please enter the upload mode (wait 5s)";
	cout << cmd << endl;
	for (int i = 5; i > 0; i--)
	{
		Sleep(1000);
		cmd = to_string(i);
		cout << "    0" << cmd << endl;
	}

// km0_boot_all
	cmdss.clear();
	cmdss << ".\\tools\\windows\\image_tool\\1-10_MP_Image_Tool.exe -set address 0x08000000";
	getline(cmdss, cmd);
	//cout << cmd << endl;
	system(cmd.c_str());

	cmdss.clear();
	cmdss << ".\\tools\\windows\\image_tool\\1-10_MP_Image_Tool.exe -set length 0x2000";
	getline(cmdss, cmd);
	//cout << cmd << endl;
	system(cmd.c_str());

	cmdss.clear();
	cmdss << ".\\tools\\windows\\image_tool\\1-10_MP_Image_Tool.exe -set image km0_boot_all.bin";
	getline(cmdss, cmd);
	//cout << cmd << endl;
	system(cmd.c_str());

	cmdss.clear();
	cmdss << ".\\tools\\windows\\image_tool\\1-10_MP_Image_Tool.exe -download";
	getline(cmdss, cmd);
	//cout << cmd << endl;
	system(cmd.c_str());

	cmd = "    Upload km0_boot_all done. ";
	cout << cmd << endl;

// km4_boot_all
	cmdss.clear();
	cmdss << ".\\tools\\windows\\image_tool\\1-10_MP_Image_Tool.exe -set address 0x08004000";
	getline(cmdss, cmd);
	//cout << cmd << endl;
	system(cmd.c_str());

	cmdss.clear();
	cmdss << ".\\tools\\windows\\image_tool\\1-10_MP_Image_Tool.exe -set length 0x2000";
	getline(cmdss, cmd);
	//cout << cmd << endl;
	system(cmd.c_str());

	cmdss.clear();
	cmdss << ".\\tools\\windows\\image_tool\\1-10_MP_Image_Tool.exe -set image km4_boot_all.bin";
	getline(cmdss, cmd);
	//cout << cmd << endl;
	system(cmd.c_str());

	cmdss.clear();
	cmdss << ".\\tools\\windows\\image_tool\\1-10_MP_Image_Tool.exe -download";
	getline(cmdss, cmd);
	//cout << cmd << endl;
	system(cmd.c_str());

	cmd = "    Upload km4_boot_all done. ";
	cout << cmd << endl;

// km0_km4_image2
	cmdss.clear();
	cmdss << ".\\tools\\windows\\image_tool\\1-10_MP_Image_Tool.exe -set address 0x08006000";
	getline(cmdss, cmd);
	//cout << cmd << endl;
	system(cmd.c_str());

	cmdss.clear();
	cmdss << ".\\tools\\windows\\image_tool\\1-10_MP_Image_Tool.exe -set length 0xF9FFF";
	getline(cmdss, cmd);
	//cout << cmd << endl;
	system(cmd.c_str());

	cmdss.clear();
	cmdss << ".\\tools\\windows\\image_tool\\1-10_MP_Image_Tool.exe -set image km0_km4_image2.bin";
	getline(cmdss, cmd);
	//cout << cmd << endl;
	system(cmd.c_str());

	cmdss.clear();
	cmdss << ".\\tools\\windows\\image_tool\\1-10_MP_Image_Tool.exe -download";
	getline(cmdss, cmd);
	//cout << cmd << endl;
	system(cmd.c_str());

	cmd = "    Upload km0_km4_image2 done. ";
	cout << cmd << endl;

	return 0;
}
