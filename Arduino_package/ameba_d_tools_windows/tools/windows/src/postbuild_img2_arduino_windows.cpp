/*

Compile:

windows:
mingw32-g++.exe -o postbuild_img2_arduino_windows.exe tools\windows\src\postbuild_img2_arduino_windows.cpp -static

*/

#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <vector>

using namespace std;

void replaceAll( string& source, const string& from, const string& to )
{
    string newString;
    newString.reserve(source.length()); //avoids a few memory allocations

    string::size_type lastPos = 0;
    string::size_type findPos;

    while (string::npos != (findPos = source.find(from, lastPos))) {
        newString.append( source, lastPos, (findPos - lastPos));
        newString += to;
        lastPos = findPos + from.length();
    }

    // Care for the rest after last occurrence
    newString += source.substr(lastPos);

    source.swap(newString);
}

int main(int argc, char *argv[]) {

    int ret = 0;
    stringstream cmdss;
    string cmd, line, msg;
    vector<string> lines;
    vector<string>::iterator iter;
    string path_tool;
    string path_arm_none_eabi_gcc;
    string path_symbol_black_list;
    string bksym;
    ifstream fin;
    ofstream fout;

    bool has_xip = false;
    bool has_psram = false;

    string sram_start_st = "", sram_end_st = "", xip_start_st = "", xip_end_st = "", psram_start_st = "", psram_end_st = "";
    unsigned int sram_start = 0, sram_end = 0, xip_start = 0, xip_end = 0, psram_start = 0, psram_end = 0;

    size_t pos;

    int check_powersavemode = 0;

    // 0. change work folder
    chdir(argv[1]);

    // 1. copy elf application.axf to current folder
    cmdss.clear();
    cmdss << "xcopy /y " << argv[2] << " .\\";
    getline(cmdss, cmd);
    cout << cmd << endl;
    system(cmd.c_str());

    // 2. remove previous files
    cmd = "if exist application.map del application.map";
    cout << cmd << endl;
    system(cmd.c_str());

    cmd = "if exist application.asm del application.asm";
    cout << cmd << endl;
    system(cmd.c_str());

    cmd = "if exist *.bin del *.bin";
    cout << cmd << endl;
    system(cmd.c_str());

    // 3. generate information files
    path_arm_none_eabi_gcc.assign(argv[3]);
    replaceAll(path_arm_none_eabi_gcc, "/", "\\");

    cmdss.clear();
    cmdss << "\"" <<path_arm_none_eabi_gcc << "arm-none-eabi-nm.exe\" application.axf | sort > application.map";
    getline(cmdss, cmd);
    cout << cmd << endl;
    system(cmd.c_str());

    fin.open("application.map");
    while (getline(fin, line)) {
        lines.push_back(line);
    }
    fin.close();

    cmdss.clear();
    cmdss << "\"" <<path_arm_none_eabi_gcc << "arm-none-eabi-objdump.exe\" -d application.axf > application.asm";
    getline(cmdss, cmd);
    cout << cmd << endl;
    system(cmd.c_str());

    // 3.1 check if any forbidden symbols
    path_symbol_black_list.assign(argv[4]);
    replaceAll(path_symbol_black_list, "/", "\\");
    fin.open(path_symbol_black_list.c_str(), ifstream::in);
    cout << path_symbol_black_list << endl;
    ret = 0;
    if (fin) {
        while (!fin.eof() && ret == 0) {
            fin >> bksym;
            getline(fin, msg);

            // check if this symbole appears in the map file
            for (iter = lines.begin(); iter != lines.end(); ++iter) {
                if ((iter->find(bksym)) != string::npos) {
                    cerr << "ERROR: " << msg << endl;
                    ret = -1;
                    break;
                }
            }
        }
    }
    fin.close();

    if (ret != 0) {
        return -1;
    }

    // 4. grep sram, xip/flash and psram information
    fout.open("application.map");
    for (iter = lines.begin(); iter != lines.end(); ++iter) {
        fout << *iter << endl;
        line = *iter;
        pos = line.find("__ram_image2_text_start__");
        if (pos != string::npos) {
            sram_start_st = line.substr(0, (pos - 3));
            sram_start = strtol(sram_start_st.c_str(), NULL, 16);
        }
        pos = line.find("__ram_image2_text_end__");
        if (pos != string::npos) {
            sram_end_st = line.substr(0, (pos - 3));
            sram_end = strtol(sram_end_st.c_str(), NULL, 16);
        }

        pos = line.find("__flash_text_start__");
        if (pos != string::npos) {
            xip_start_st = line.substr(0, (pos - 3));
            xip_start = strtol(xip_start_st.c_str(), NULL, 16);
        }
        pos = line.find("__flash_text_end__");
        if (pos != string::npos) {
            xip_end_st = line.substr(0, (pos - 3));
            xip_end = strtol(xip_end_st.c_str(), NULL, 16);
        }

        pos = line.find("__psram_image2_text_start__");
        if (pos != string::npos) {
            psram_start_st = line.substr(0, (pos - 3));
            psram_start = strtol(psram_start_st.c_str(), NULL, 16);
        }
        pos = line.find("__psram_image2_text_end__");
        if (pos != string::npos) {
            psram_end_st = line.substr(0, (pos - 3));
            psram_end = strtol(psram_end_st.c_str(), NULL, 16);
        }

        //4.1 power save bin handling
        //DS_AON_TIMER_WAKEUP               101
        //DS_RTC_WAKEUP                     102
        //DS_AON_WAKEPIN_WAKEUP_D16         103
        //DS_AON_WAKEPIN_WAKEUP_D17         104
        //DS_AON_WAKEPIN_WAKEUP_D26         105
        //DS_AON_WAKEPIN_WAKEUP_D27         106

        //TL_UART_WAKEUP                    201
        //TL_RTC_WAKEUP                     202
        //TL_AON_WAKEPIN_WAKEUP_D16         203
        //TL_AON_WAKEPIN_WAKEUP_D17         204
        //TL_AON_WAKEPIN_WAKEUP_D26         205
        //TL_AON_WAKEPIN_WAKEUP_D27         206

        pos = line.find("DS_AON_TIMER_WAKEUP");
        if (pos != string::npos) {
            check_powersavemode = 101;
        }

        pos = line.find("DS_RTC_WAKEUP");
        if (pos != string::npos) {
            check_powersavemode = 102;
        }

        pos = line.find("DS_AON_WAKEPIN_WAKEUP_D16");
        if (pos != string::npos) {
            check_powersavemode = 103;
        }

        pos = line.find("DS_AON_WAKEPIN_WAKEUP_D17");
        if (pos != string::npos) {
            check_powersavemode = 104;
        }

        pos = line.find("DS_AON_WAKEPIN_WAKEUP_D26");
        if (pos != string::npos) {
            check_powersavemode = 105;
        }

        pos = line.find("DS_AON_WAKEPIN_WAKEUP_D27");
        if (pos != string::npos) {
            check_powersavemode = 106;
        }

        pos = line.find("TL_UART_WAKEUP");
        if (pos != string::npos) {
            check_powersavemode = 201;
        }

        pos = line.find("TL_RTC_WAKEUP");
        if (pos != string::npos) {
            check_powersavemode = 202;
        }

        pos = line.find("TL_AON_WAKEPIN_WAKEUP_D16");
        if (pos != string::npos) {
            check_powersavemode = 203;
        }

        pos = line.find("TL_AON_WAKEPIN_WAKEUP_D17");
        if (pos != string::npos) {
            check_powersavemode = 204;
        }

        pos = line.find("TL_AON_WAKEPIN_WAKEUP_D26");
        if (pos != string::npos) {
            check_powersavemode = 205;
        }

        pos = line.find("TL_AON_WAKEPIN_WAKEUP_D27");
        if (pos != string::npos) {
            check_powersavemode = 206;
        }
    }
    fout.close();

    if (xip_start > 0 && xip_end > 0) {
        has_xip = true;
    }
    if (psram_start > 0 && psram_end > 0) {
        has_psram = true;
    }

    cout << "sram  " << sram_start_st << " ~ " << sram_end_st << endl;
    if (has_xip) {
        cout << "xip " << xip_start_st << " ~ " << xip_end_st << endl;
    }
    if (has_psram) {
        cout << "psram " << psram_start_st << " ~ " << psram_end_st << endl;
    }

    // 5. generate image 2, image xip and image psram
    cmdss.clear();
    //cmdss << "\"" <<path_arm_none_eabi_gcc << "arm-none-eabi-objcopy.exe\" -j .image2.start.table -j .ram_image2.text -j .ram.data -Obinary .\\application.axf .\\ram_2.bin";
    cmdss << "\"" <<path_arm_none_eabi_gcc << "arm-none-eabi-objcopy.exe\" -j .ram_image2.entry -j .ram_image2.text -j .ram_image2.data -Obinary .\\application.axf .\\ram_2.r.bin";
    getline(cmdss, cmd);
    cout << cmd << endl;
    system(cmd.c_str());

    if (has_xip) {
        cmdss.clear();
        //cmdss << "\"" << path_arm_none_eabi_gcc << "arm-none-eabi-objcopy.exe\" -j .image3 -j .sdr_data -Obinary .\\application.axf .\\xip_image2.bin";
        cmdss << "\"" << path_arm_none_eabi_gcc << "arm-none-eabi-objcopy.exe\" -j .xip_image2.text -Obinary .\\application.axf .\\xip_image2.bin";
        getline(cmdss, cmd);
        cout << cmd << endl;
        system(cmd.c_str());
    }

    if (has_psram) {
        cmdss.clear();
        //cmdss << "\"" << path_arm_none_eabi_gcc << "arm-none-eabi-objcopy.exe\" -j .image3 -j .sdr_data -Obinary .\\application.axf .\\psram_2.bin";
        cmdss << "\"" << path_arm_none_eabi_gcc << "arm-none-eabi-objcopy.exe\" -j .psram_image2.text -j .psram_image2.data -Obinary .\\application.axf .\\psram_2.r.bin";
        getline(cmdss, cmd);
        cout << cmd << endl;
        system(cmd.c_str());
    }


    // 6. fulfill header
    // 6.1 remove bss sections
    cmdss.clear();
    cmdss << ".\\tools\\windows\\pick.exe " << sram_start << " " << sram_end << " ram_2.r.bin ram_2.bin raw";
    getline(cmdss, cmd);
    cout << cmd << endl;
    system(cmd.c_str());

    if (has_psram) {
        cmdss.clear();
        cmdss << ".\\tools\\windows\\pick.exe " << psram_start << " " << psram_end << " psram_2.r.bin psram_2.bin raw";
        getline(cmdss, cmd);
        cout << cmd << endl;
        system(cmd.c_str());
    }

    // 6.2 add header
    cmdss.clear();
    cmdss << ".\\tools\\windows\\pick.exe " << sram_start << " " << sram_end << " ram_2.bin ram_2.p.bin";
    getline(cmdss, cmd);
    cout << cmd << endl;
    system(cmd.c_str());

    if (has_xip) {
        cmdss.clear();
        cmdss << ".\\tools\\windows\\pick.exe " << xip_start << " " << xip_end << " xip_image2.bin xip_image2.p.bin";
        getline(cmdss, cmd);
        cout << cmd << endl;
        system(cmd.c_str());
    }

    if (has_psram) {
        cmdss.clear();
        cmdss << ".\\tools\\windows\\pick.exe " << psram_start << " " << psram_end << " psram_2.bin psram_2.p.bin";
        getline(cmdss, cmd);
        cout << cmd << endl;
        system(cmd.c_str());
    }

    // 6.3 generate km4_image2_all
    cmd = "copy /b xip_image2.p.bin+ram_2.p.bin+psram_2.p.bin km4_image2_all.bin";
    cout << cmd << endl;
    system(cmd.c_str());

    cmdss.clear();
    cmdss << ".\\tools\\windows\\pad.exe " << "km4_image2_all.bin 4096";
    getline(cmdss, cmd);
    cout << cmd << endl;
    system(cmd.c_str());


    // 7. prepare image 1
    // 7.1 power save mode bins
    //DS_AON_TIMER_WAKEUP               101
    //DS_RTC_WAKEUP                     102
    //DS_AON_WAKEPIN_WAKEUP_D16         103
    //DS_AON_WAKEPIN_WAKEUP_D17         104
    //DS_AON_WAKEPIN_WAKEUP_D26         105
    //DS_AON_WAKEPIN_WAKEUP_D27         106

    //TL_UART_WAKEUP                    201
    //TL_RTC_WAKEUP                     202
    //TL_AON_WAKEPIN_WAKEUP_D16         203
    //TL_AON_WAKEPIN_WAKEUP_D17         204
    //TL_AON_WAKEPIN_WAKEUP_D26         205
    //TL_AON_WAKEPIN_WAKEUP_D27         206

    cmd = "if exist bsp\\image*.bin del bsp\\image*.bin";
    cout << cmd << endl;
    system(cmd.c_str());

    if (check_powersavemode == 101) {
        cmd = "copy bsp\\image\\PMU_bins\\NONE\\km0_boot_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());

        cmd = "copy bsp\\image\\PMU_bins\\NONE\\km4_boot_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());

        cmd = "copy bsp\\image\\PMU_bins\\NONE\\km0_image2_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());
    } else if (check_powersavemode == 102) {
        cmd = "copy bsp\\image\\PMU_bins\\DS_RTC\\km0_boot_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());

        cmd = "copy bsp\\image\\PMU_bins\\DS_RTC\\km4_boot_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());

        cmd = "copy bsp\\image\\PMU_bins\\DS_RTC\\km0_image2_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());
    } else if (check_powersavemode == 201) {
        cmd = "copy bsp\\image\\PMU_bins\\TL_UART\\km0_boot_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());

        cmd = "copy bsp\\image\\PMU_bins\\TL_UART\\km4_boot_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());

        cmd = "copy bsp\\image\\PMU_bins\\TL_UART\\km0_image2_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());
    } else if (check_powersavemode == 202) {
        cmd = "copy bsp\\image\\PMU_bins\\TL_RTC\\km0_boot_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());

        cmd = "copy bsp\\image\\PMU_bins\\TL_RTC\\km4_boot_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());

        cmd = "copy bsp\\image\\PMU_bins\\TL_RTC\\km0_image2_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());
    } else if ((check_powersavemode == 103) || (check_powersavemode == 203)) {
        cmd = "copy bsp\\image\\PMU_bins\\D16_PA25\\km0_boot_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());

        cmd = "copy bsp\\image\\PMU_bins\\D16_PA25\\km4_boot_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());

        cmd = "copy bsp\\image\\PMU_bins\\D16_PA25\\km0_image2_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());
    } else if ((check_powersavemode == 104) || (check_powersavemode == 204)) {
        cmd = "copy bsp\\image\\PMU_bins\\D17_PA26\\km0_boot_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());

        cmd = "copy bsp\\image\\PMU_bins\\D17_PA26\\km4_boot_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());

        cmd = "copy bsp\\image\\PMU_bins\\D17_PA26\\km0_image2_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());
    } else if ((check_powersavemode == 105) || (check_powersavemode == 205)) {
        cmd = "copy bsp\\image\\PMU_bins\\D26_PA21\\km0_boot_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());

        cmd = "copy bsp\\image\\PMU_bins\\D26_PA21\\km4_boot_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());

        cmd = "copy bsp\\image\\PMU_bins\\D26_PA21\\km0_image2_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());
    } else if ((check_powersavemode == 106) || (check_powersavemode == 206)) {
        cmd = "copy bsp\\image\\PMU_bins\\D27_PA20\\km0_boot_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());

        cmd = "copy bsp\\image\\PMU_bins\\D27_PA20\\km4_boot_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());

        cmd = "copy bsp\\image\\PMU_bins\\D27_PA20\\km0_image2_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());
    } else {
        cmd = "copy bsp\\image\\PMU_bins\\NONE\\km0_boot_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());

        cmd = "copy bsp\\image\\PMU_bins\\NONE\\km4_boot_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());

        cmd = "copy bsp\\image\\PMU_bins\\NONE\\km0_image2_all.bin bsp\\image";
        cout << cmd << endl;
        system(cmd.c_str());
    }

    check_powersavemode = 0;

    cmd = "copy bsp\\image\\km0_boot_all.bin .\\";
    cout << cmd << endl;
    system(cmd.c_str());

    cmd = "copy bsp\\image\\km4_boot_all.bin .\\";
    cout << cmd << endl;
    system(cmd.c_str());

    cmd = "copy bsp\\image\\km0_image2_all.bin .\\";
    cout << cmd << endl;
    system(cmd.c_str());

    // 8. generate .bin
    cmd = "copy /b km0_image2_all.bin+km4_image2_all.bin km0_km4_image2.bin";
    cout << cmd << endl;
    system(cmd.c_str());

#if 0
    // 9. add checksum
    cmd = ".\\tools\\windows\\checksum.exe ota.bin";
    cout << cmd << endl;
    system(cmd.c_str());
#endif

    return 0;
}
