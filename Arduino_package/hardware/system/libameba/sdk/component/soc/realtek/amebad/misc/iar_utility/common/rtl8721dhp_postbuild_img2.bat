cd /D %2
set tooldir=%2\..\..\..\component\soc\realtek\amebad\misc\iar_utility\common\tools
set libdir=%2\..\..\..\component\soc\realtek\amebad\misc\bsp
set iartooldir=%3
set build_type=%4
set km4_dir=Debug\Exe\km4_image
set km0_dir=Debug\Exe\km0_image

del %km4_dir%\km4_application.map %km4_dir%\km4_application.asm %km4_dir%\km4_application.dbg.axf
cmd /c "%tooldir%\nm Debug/Exe/km4_image/km4_application.axf | %tooldir%\sort > Debug/Exe/km4_image/km4_application.map"
cmd /c "%tooldir%\objdump -d Debug/Exe/km4_image/km4_application.axf > Debug/Exe/km4_image/km4_application.asm"

for /f "delims=" %%i in ('cmd /c "%tooldir%\grep IMAGE2 Debug/Exe/km4_image/km4_application.map | %tooldir%\grep Base | %tooldir%\gawk '{print $1}'"') do set ram2_start=0x%%i
for /f "delims=" %%i in ('cmd /c "%tooldir%\grep IMAGE2 Debug/Exe/km4_image/km4_application.map | %tooldir%\grep Limit | %tooldir%\gawk '{print $1}'"') do set ram2_end=0x%%i

for /f "delims=" %%i in ('cmd /c "%tooldir%\grep xip_image2 Debug/Exe/km4_image/km4_application.map | %tooldir%\grep Base | %tooldir%\gawk '{print $1}'"') do set xip_image2_start=0x%%i
for /f "delims=" %%i in ('cmd /c "%tooldir%\grep xip_image2 Debug/Exe/km4_image/km4_application.map | %tooldir%\grep Limit | %tooldir%\gawk '{print $1}'"') do set xip_image2_end=0x%%i

for /f "delims=" %%i in ('cmd /c "%tooldir%\grep IMG2_PSRAM Debug/Exe/km4_image/km4_application.map | %tooldir%\grep Base | %tooldir%\gawk '{print $1}'"') do set psram2_start=0x%%i
for /f "delims=" %%i in ('cmd /c "%tooldir%\grep IMG2_PSRAM Debug/Exe/km4_image/km4_application.map | %tooldir%\grep Limit | %tooldir%\gawk '{print $1}'"') do set psram2_end=0x%%i

@echo off&setlocal enabledelayedexpansion
for /f "delims=:" %%i in ('findstr /n /c:"PLACEMENT" Debug\List\km4_application\km4_application.map') do (
   set skipline=%%i
)
@echo off&setlocal enabledelayedexpansion
for /f "delims=:" %%i in ('findstr /n /c:"Kind" Debug\List\km4_application\km4_application.map') do (
    set endline=%%i
)
set /a line=endline-skipline

@echo off&setlocal enabledelayedexpansion
set n=0
(for /f "skip=%skipline% delims=" %%a in (Debug\List\km4_application\km4_application.map) do (
set /a n+=1
if !n! leq %line% echo %%a
))>km4_application1.txt

(for /f "delims=" %%a in (km4_application1.txt) do (
set /p="%%a"<nul | find /V "<Block>"
))>km4_application2.txt

@echo off&setlocal enabledelayedexpansion
set strstart={
set strend=}
set /a m=1
(for /f "delims=" %%a in (km4_application2.txt) do (
set /p="%%a"<nul
echo %%a | find "%strstart%" >nul && set /a m-=1
echo %%a | find "%strend%" >nul && set /a m+=1
if !m!==1 (echo.)
))>km4_application3.txt
findstr /r "place" km4_application3.txt > tmp.txt

del km4_application1.txt
del km4_application2.txt
del km4_application3.txt

setlocal enabledelayedexpansion
for /f "delims=:" %%i in ('findstr /r "IMAGE2" tmp.txt') do (
    set "var=%%i"
    set "sectname_ram2=!var:~1,2!"
)
for /f "delims=:" %%i in ('findstr /r "xip_image2.text" tmp.txt') do (
    set "var=%%i"
    set "sectname_xip=!var:~1,2!"
)
for /f "delims=:" %%i in ('findstr /r "IMG2_PSRAM" tmp.txt') do (
    set "var=%%i"
    set "sectname_psram2=!var:~1,2!"
)

setlocal disabledelayedexpansion
del tmp.txt
echo sectname_ram2: %sectname_ram2% sectname_xip: %sectname_xip% sectname_psram2: %sectname_psram2%

%tooldir%\objcopy --rename-section "%sectname_ram2% rw"="%sectname_ram2%" Debug/Exe/km4_image/km4_application.axf Debug/Exe/km4_image/km4_application.axf
%tooldir%\objcopy --rename-section "%sectname_xip% rw"="%sectname_xip%" Debug/Exe/km4_image/km4_application.axf Debug/Exe/km4_image/km4_application.axf
%tooldir%\objcopy --rename-section "%sectname_psram2% rw"="%sectname_psram2%" Debug/Exe/km4_image/km4_application.axf Debug/Exe/km4_image/km4_application.axf

%tooldir%\objcopy -j "%sectname_ram2%" -Obinary Debug/Exe/km4_image/km4_application.axf Debug/Exe/km4_image/ram_2.r.bin
%tooldir%\objcopy -j "%sectname_xip%" -Obinary Debug/Exe/km4_image/km4_application.axf Debug/Exe/km4_image/xip_image2.bin
%tooldir%\objcopy -j "%sectname_psram2%" -Obinary Debug/Exe/km4_image/km4_application.axf Debug/Exe/km4_image/psram_2.r.bin

:: remove bss sections
%tooldir%\pick %ram2_start% %ram2_end% %km4_dir%\ram_2.r.bin %km4_dir%\ram_2.bin raw
del %km4_dir%\ram_2.r.bin

%tooldir%\pick %psram2_start% %psram2_end% %km4_dir%\psram_2.r.bin %km4_dir%\psram_2.bin raw
del %km4_dir%\psram_2.r.bin

:: dword aligned
%tooldir%\pad %km4_dir%\ram_2.bin 4
%tooldir%\pad %km4_dir%\xip_image2.bin 4
%tooldir%\pad %km4_dir%\psram_2.bin 4
set /a ram2_end_align=(((%ram2_end% - 1)/4) + 1)*4
set /a xip_image2_end_align=(((%xip_image2_end% - 1)/4) + 1)*4
set /a psram2_end_align=(((%psram2_end% - 1)/4) + 1)*4

:: add header
%tooldir%\pick %ram2_start% %ram2_end_align% %km4_dir%\ram_2.bin %km4_dir%\ram_2.p.bin
%tooldir%\pick %xip_image2_start% %xip_image2_end_align% %km4_dir%\xip_image2.bin %km4_dir%\xip_image2.p.bin
%tooldir%\pick %psram2_start% %psram2_end_align% %km4_dir%\psram_2.bin %km4_dir%\psram_2.p.bin

:: aggregate image2_all.bin
copy /b %km4_dir%\xip_image2.p.bin+%km4_dir%\ram_2.p.bin+%km4_dir%\psram_2.p.bin %km4_dir%\km4_image2_all.bin
%tooldir%\pad %km4_dir%\km4_image2_all.bin 4096

del %km4_dir%\ram_2.bin
del %km4_dir%\ram_2.p.bin
del %km4_dir%\xip_image2.bin
del %km4_dir%\xip_image2.p.bin
del %km4_dir%\psram_2.bin
del %km4_dir%\psram_2.p.bin

rename %km4_dir%\km4_application.axf km4_application.dbg.axf

if %build_type% equ "mp" (
	copy /b %km0_dir%\km0_image2_all.bin+%km4_dir%\km4_image2_all.bin+%km4_dir%\km4_image3_all.bin+%km4_dir%\km4_image3_psram.bin %km4_dir%\km0_km4_image2.bin
) else (
	set target_dir=%km4_dir%
	call %tooldir%\..\combine_img2.bat
)

%iartooldir%\bin\ilinkarm.exe %tooldir%\link_dummy_hp.o --image_input %km4_dir%\km0_km4_image2.bin,flash_start,firmware,32 --cpu Cortex-M33 --fpu=VFPv5 --no_entry --keep flash_start --config rtl8721d_FLASH.icf --no_library_search --define_symbol __vector_table=0x10100000 -o %km4_dir%\km4_application.axf

if %build_type% equ "mp" (
	del %km4_dir%\km0_km4_image2_mp.bin
	rename %km4_dir%\km0_km4_image2.bin km0_km4_image2_mp.bin
)

%tooldir%\objcopy -I elf32-little -j "BTTRACE rw" -Obinary %km4_dir%\km4_application.dbg.axf %km4_dir%\APP.trace

exit
