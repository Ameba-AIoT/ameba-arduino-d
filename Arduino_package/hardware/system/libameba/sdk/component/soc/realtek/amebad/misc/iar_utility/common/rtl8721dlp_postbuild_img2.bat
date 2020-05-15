cd /D %2
set tooldir=%2\..\..\..\component\soc\realtek\amebad\misc\iar_utility\common\tools
set libdir=%2\..\..\..\component\soc\realtek\amebad\misc\bsp
set iartooldir=%3

set km4_dir=Debug\Exe\km4_image
set km0_dir=Debug\Exe\km0_image

echo input1=%1 >tmp.txt
echo input2=%2 >>tmp.txt

del %km0_dir%\km0_application.map %km0_dir%\km0_application.asm %km0_dir%\km0_application.dbg.axf
cmd /c "%tooldir%\nm Debug/Exe/km0_image/km0_application.axf | %tooldir%\sort > Debug/Exe/km0_image/km0_application.map"
cmd /c "%tooldir%\objdump -d Debug/Exe/km0_image/km0_application.axf > Debug/Exe/km0_image/km0_application.asm"

for /f "delims=" %%i in ('cmd /c "%tooldir%\grep IMAGE2 Debug/Exe/km0_image/km0_application.map | %tooldir%\grep Base | %tooldir%\gawk '{print $1}'"') do set ram2_start=0x%%i
for /f "delims=" %%i in ('cmd /c "%tooldir%\grep IMAGE2 Debug/Exe/km0_image/km0_application.map | %tooldir%\grep Limit | %tooldir%\gawk '{print $1}'"') do set ram2_end=0x%%i

for /f "delims=" %%i in ('cmd /c "%tooldir%\grep xip_image2 Debug/Exe/km0_image/km0_application.map | %tooldir%\grep Base | %tooldir%\gawk '{print $1}'"') do set xip_image2_start=0x%%i
for /f "delims=" %%i in ('cmd /c "%tooldir%\grep xip_image2 Debug/Exe/km0_image/km0_application.map | %tooldir%\grep Limit | %tooldir%\gawk '{print $1}'"') do set xip_image2_end=0x%%i

echo ram2_start: %ram2_start% > tmp.txt
echo ram2_end: %ram2_end% >> tmp.txt
echo xip_image2_start: %xip_image2_start% >> tmp.txt
echo xip_image2_end: %xip_image2_end% >> tmp.txt

@echo off&setlocal enabledelayedexpansion
for /f "delims=:" %%i in ('findstr /n /c:"PLACEMENT" Debug\List\km0_application\km0_application.map') do (
   set skipline=%%i
)
@echo off&setlocal enabledelayedexpansion
for /f "delims=:" %%i in ('findstr /n /c:"Kind" Debug\List\km0_application\km0_application.map') do (
    set endline=%%i
)
set /a line=endline-skipline

@echo off&setlocal enabledelayedexpansion
set n=0
(for /f "skip=%skipline% delims=" %%a in (Debug\List\km0_application\km0_application.map) do (
set /a n+=1
if !n! leq %line% echo %%a
))>km0_application1.txt

(for /f "delims=" %%a in (km0_application1.txt) do (
set /p="%%a"<nul | find /V "<Block>"
))>km0_application2.txt

@echo off&setlocal enabledelayedexpansion
set strstart={
set strend=}
set /a m=1
(for /f "delims=" %%a in (km0_application2.txt) do (
set /p="%%a"<nul
echo %%a | find "%strstart%" >nul && set /a m-=1
echo %%a | find "%strend%" >nul && set /a m+=1
if !m!==1 (echo.)
))>km0_application3.txt
findstr /r "place" km0_application3.txt > tmp.txt

del km0_application1.txt
del km0_application2.txt
del km0_application3.txt

setlocal enabledelayedexpansion
for /f "delims=:" %%i in ('findstr /r "IMAGE2" tmp.txt') do (
    set "var=%%i"
    set "sectname_ram2=!var:~1,2!"
)
for /f "delims=:" %%i in ('findstr /r "xip_image2.text" tmp.txt') do (
    set "var=%%i"
    set "sectname_xip=!var:~1,2!"
)

setlocal disabledelayedexpansion
del tmp.txt
::echo sectname_ram2: %sectname_ram2% sectname_xip: %sectname_xip% 

%tooldir%\objcopy --rename-section "%sectname_ram2% rw"="%sectname_ram2%" Debug/Exe/km0_image/km0_application.axf Debug/Exe/km0_image/km0_application.axf
%tooldir%\objcopy --rename-section "%sectname_xip% rw"="%sectname_xip%" Debug/Exe/km0_image/km0_application.axf Debug/Exe/km0_image/km0_application.axf

%tooldir%\objcopy -j "%sectname_ram2%" -Obinary Debug/Exe/km0_image/km0_application.axf Debug/Exe/km0_image/ram_2.r.bin
%tooldir%\objcopy -j "%sectname_xip%" -Obinary Debug/Exe/km0_image/km0_application.axf Debug/Exe/km0_image/xip_image2.bin

:: remove bss sections
%tooldir%\pick %ram2_start% %ram2_end% %km0_dir%\ram_2.r.bin %km0_dir%\ram_2.bin raw
del %km0_dir%\ram_2.r.bin

:: dword aligned
%tooldir%\pad %km0_dir%\ram_2.bin 4
%tooldir%\pad %km0_dir%\xip_image2.bin 4
set /a ram2_end_align=(((%ram2_end% - 1)/4) + 1)*4
set /a xip_image2_end_align=(((%xip_image2_end% - 1)/4) + 1)*4

:: add header
%tooldir%\pick %ram2_start% %ram2_end_align% %km0_dir%\ram_2.bin %km0_dir%\ram_2.p.bin
%tooldir%\pick %xip_image2_start% %xip_image2_end_align% %km0_dir%\xip_image2.bin %km0_dir%\xip_image2.p.bin

:: aggregate image2_all.bin
copy /b %km0_dir%\xip_image2.p.bin+%km0_dir%\ram_2.p.bin %km0_dir%\km0_image2_all.bin
%tooldir%\pad %km0_dir%\km0_image2_all.bin 4096

del %km0_dir%\ram_2.bin
del %km0_dir%\ram_2.p.bin
del %km0_dir%\xip_image2.bin
del %km0_dir%\xip_image2.p.bin

rename %km0_dir%\km0_application.axf km0_application.dbg.axf

set target_dir=%km0_dir%
call %tooldir%\..\combine_img2.bat

%iartooldir%\bin\ilinkarm.exe %tooldir%\link_dummy_lp.o --image_input %km0_dir%\km0_km4_image2.bin,flash_start,firmware,32 --cpu Cortex-M23 --no_entry --keep flash_start --config rtl8721d_FLASH.icf --no_library_search --define_symbol __vector_table=0x00 -o %km0_dir%\km0_application.axf

exit
