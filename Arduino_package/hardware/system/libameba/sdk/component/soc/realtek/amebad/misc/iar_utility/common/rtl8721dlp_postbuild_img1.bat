cd /D %2

set tooldir=%2\..\..\..\component\soc\realtek\amebad\misc\iar_utility\common\tools
set libdir=%2\..\..\..\component\soc\realtek\amebad\misc\bsp
set iartooldir=%3

set km0_dir=Debug\Exe\km0_image

del %km0_dir%\km0_bootloader.map %km0_dir%\km0_bootloader.asm %km0_dir%\km0_bootloader.dbg.axf
cmd /c "%tooldir%\nm Debug/Exe/km0_image/km0_bootloader.axf | %tooldir%\sort > Debug/Exe/km0_image/km0_bootloader.map"
cmd /c "%tooldir%\objdump -d Debug/Exe/km0_image/km0_bootloader.axf > Debug/Exe/km0_image/km0_bootloader.asm"

for /f "delims=" %%i in ('cmd /c "%tooldir%\grep IMAGE1 Debug/Exe/km0_image/km0_bootloader.map | %tooldir%\grep Base | %tooldir%\gawk '{print $1}'"') do set ram1_start=0x%%i
for /f "delims=" %%i in ('cmd /c "%tooldir%\grep IMAGE1 Debug/Exe/km0_image/km0_bootloader.map | %tooldir%\grep Limit | %tooldir%\gawk '{print $1}'"') do set ram1_end=0x%%i

for /f "delims=" %%i in ('cmd /c "%tooldir%\grep xip_image1 Debug/Exe/km0_image/km0_bootloader.map | %tooldir%\grep Base | %tooldir%\gawk '{print $1}'"') do set xip_image1_start=0x%%i
for /f "delims=" %%i in ('cmd /c "%tooldir%\grep xip_image1 Debug/Exe/km0_image/km0_bootloader.map | %tooldir%\grep Limit | %tooldir%\gawk '{print $1}'"') do set xip_image1_end=0x%%i

@echo off&setlocal enabledelayedexpansion
for /f "delims=:" %%i in ('findstr /n /c:"PLACEMENT" Debug\List\km0_bootloader\km0_bootloader.map') do (
   set skipline=%%i
)
@echo off&setlocal enabledelayedexpansion
for /f "delims=:" %%i in ('findstr /n /c:"Kind" Debug\List\km0_bootloader\km0_bootloader.map') do (
    set endline=%%i
)
set /a line=endline-skipline

@echo off&setlocal enabledelayedexpansion
set n=0
(for /f "skip=%skipline% delims=" %%a in (Debug\List\km0_bootloader\km0_bootloader.map) do (
set /a n+=1
if !n! leq %line% echo %%a
))>km0_bootloader1.txt

(for /f "delims=" %%a in (km0_bootloader1.txt) do (
set /p="%%a"<nul | find /V "<Block>"
))>km0_bootloader2.txt

@echo off&setlocal enabledelayedexpansion
set strstart={
set strend=}
set /a m=1
(for /f "delims=" %%a in (km0_bootloader2.txt) do (
set /p="%%a"<nul
echo %%a | find "%strstart%" >nul && set /a m-=1
echo %%a | find "%strend%" >nul && set /a m+=1
if !m!==1 (echo.)
))>km0_bootloader3.txt
findstr /r "place" km0_bootloader3.txt > tmp.txt

del km0_bootloader1.txt
del km0_bootloader2.txt
del km0_bootloader3.txt

setlocal enabledelayedexpansion
for /f "delims=:" %%i in ('findstr /r "IMAGE1" tmp.txt') do (
    set "var=%%i"
    set "sectname_ram1=!var:~1,2!"
)
for /f "delims=:" %%i in ('findstr /r "xip_image1.text" tmp.txt') do (
    set "var=%%i"
    set "sectname_xip1=!var:~1,2!"
)
setlocal disabledelayedexpansion
::del tmp.txt
echo sectname_ram1: %sectname_ram1% sectname_xip: %sectname_xip1%

%tooldir%\objcopy --rename-section "%sectname_ram1% rw"="%sectname_ram1%" Debug/Exe/km0_image/km0_bootloader.axf Debug/Exe/km0_image/km0_bootloader.axf
%tooldir%\objcopy --rename-section "%sectname_xip1% rw"="%sectname_xip1%" Debug/Exe/km0_image/km0_bootloader.axf Debug/Exe/km0_image/km0_bootloader.axf

:: pick ram_1.bin
%tooldir%\objcopy -j "%sectname_ram1%" -Obinary Debug/Exe/km0_image/km0_bootloader.axf Debug/Exe/km0_image/ram_1.bin
:: add header
%tooldir%\pick %ram1_start% %ram1_end% %km0_dir%\ram_1.bin %km0_dir%\ram_1.p.bin boot

:: pick xip_image1.bin
%tooldir%\objcopy -j "%sectname_xip1%" -Obinary Debug/Exe/km0_image/km0_bootloader.axf Debug/Exe/km0_image/xip_image1.bin
:: add header
%tooldir%\pick %xip_image1_start% %xip_image1_end% %km0_dir%\xip_image1.bin %km0_dir%\xip_image1.p.bin boot

:: aggregate km0_boot_all.bin
copy /b %km0_dir%\xip_image1.p.bin+%km0_dir%\ram_1.p.bin %km0_dir%\km0_boot_all.bin

del %km0_dir%\ram_1.bin
del %km0_dir%\ram_1.p.bin
del %km0_dir%\xip_image1.bin
del %km0_dir%\xip_image1.p.bin

rename %km0_dir%\km0_bootloader.axf km0_bootloader.dbg.axf

call security_config.cmd

if %RSIP_ENABLE% equ 1 (
	:: RSIP encryption km4_boot_all.bin ==> km4_boot_all-en.bin	
	%tooldir%\EncTool.exe rsip %km0_dir%\km0_boot_all.bin %km0_dir%\km0_boot_all-en.bin 0x08000000 %RSIP_KEY% %RSIP_IV%
	del %km0_dir%\km0_boot_all.bin
	rename %km0_dir%\km0_boot_all-en.bin km0_boot_all.bin
)

%iartooldir%\bin\ilinkarm.exe %tooldir%\link_dummy_lp.o --image_input %km0_dir%\km0_boot_all.bin,boot_start,ls_boot,32 --cpu Cortex-M23 --no_entry --keep boot_start --config rtl8721d_FLASH.icf --no_library_search --define_symbol __vector_table=0x00 -o %km0_dir%\km0_bootloader.axf

exit
