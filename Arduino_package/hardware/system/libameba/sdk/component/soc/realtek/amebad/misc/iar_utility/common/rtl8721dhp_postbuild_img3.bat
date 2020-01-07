cd /D %2
set tooldir=%2\..\..\..\component\soc\realtek\amebad\misc\iar_utility\common\tools
set libdir=%2\..\..\..\component\soc\realtek\amebad\misc\bsp
set build_type=%3
set km4_dir=Debug\Exe\km4_image

del %km4_dir%\km4_secure.map %km4_dir%\km4_secure.asm *.bin
cmd /c "%tooldir%\nm Debug/Exe/km4_image/km4_secure.axf | %tooldir%\sort > Debug/Exe/km4_image/km4_secure.map"
cmd /c "%tooldir%\objdump -d Debug/Exe/km4_image/km4_secure.axf > Debug/Exe/km4_image/km4_secure.asm"

for /f "delims=" %%i in ('cmd /c "%tooldir%\grep IMAGE3 Debug/Exe/km4_image/km4_secure.map | %tooldir%\grep Base | %tooldir%\gawk '{print $1}'"') do set ram3_s_start=0x%%i
for /f "delims=" %%i in ('cmd /c "%tooldir%\grep IMAGE3 Debug/Exe/km4_image/km4_secure.map | %tooldir%\grep Limit | %tooldir%\gawk '{print $1}'"') do set ram3_s_end=0x%%i

for /f "delims=" %%i in ('cmd /c "%tooldir%\grep CMSE Debug/Exe/km4_image/km4_secure.map | %tooldir%\grep Base | %tooldir%\gawk '{print $1}'"') do set ram3_nsc_start=0x%%i
for /f "delims=" %%i in ('cmd /c "%tooldir%\grep CMSE Debug/Exe/km4_image/km4_secure.map | %tooldir%\grep Limit | %tooldir%\gawk '{print $1}'"') do set ram3_nsc_end=0x%%i

for /f "delims=" %%i in ('cmd /c "%tooldir%\grep IMG3_PSRAM Debug/Exe/km4_image/km4_secure.map | %tooldir%\grep Base | %tooldir%\gawk '{print $1}'"') do set psram3_s_start=0x%%i
for /f "delims=" %%i in ('cmd /c "%tooldir%\grep IMG3_PSRAM Debug/Exe/km4_image/km4_secure.map | %tooldir%\grep Limit | %tooldir%\gawk '{print $1}'"') do set psram3_s_end=0x%%i

@echo off&setlocal enabledelayedexpansion
for /f "delims=:" %%i in ('findstr /n /c:"PLACEMENT" Debug\List\km4_secure\km4_secure.map') do (
   set skipline=%%i
)
@echo off&setlocal enabledelayedexpansion
for /f "delims=:" %%i in ('findstr /n /c:"Kind" Debug\List\km4_secure\km4_secure.map') do (
    set endline=%%i
)
set /a line=endline-skipline

@echo off&setlocal enabledelayedexpansion
set n=0
(for /f "skip=%skipline% delims=" %%a in (Debug\List\km4_secure\km4_secure.map) do (
set /a n+=1
if !n! leq %line% echo %%a
))>km4_secure1.txt

(for /f "delims=" %%a in (km4_secure1.txt) do (
set /p="%%a"<nul | find /V "<Block>"
))>km4_secure2.txt

@echo off&setlocal enabledelayedexpansion
set strstart={
set strend=}
set /a m=1
(for /f "delims=" %%a in (km4_secure2.txt) do (
set /p="%%a"<nul
echo %%a | find "%strstart%" >nul && set /a m-=1
echo %%a | find "%strend%" >nul && set /a m+=1
if !m!==1 (echo.)
))>km4_secure3.txt
findstr /r "place" km4_secure3.txt > tmp.txt

del km4_secure1.txt
del km4_secure2.txt
del km4_secure3.txt

setlocal enabledelayedexpansion
for /f "delims=:" %%i in ('findstr /r "IMAGE3" tmp.txt') do (
    set "var=%%i"
    set "sectname_ram3=!var:~1,2!"
)
for /f "delims=:" %%i in ('findstr /r ".nsc.text" tmp.txt') do (
    set "var=%%i"
    set "sectname_ram3_nsc=!var:~1,2!"
)
for /f "delims=:" %%i in ('findstr /r "IMG3_PSRAM" tmp.txt') do (
    set "var=%%i"
    set "sectname_psram3=!var:~1,2!"
)

setlocal disabledelayedexpansion
del tmp.txt
echo sectname_ram3: %sectname_ram3% sectname_ram3_nsc: %sectname_ram3_nsc% sectname_psram3: %sectname_psram3%

%tooldir%\objcopy --rename-section "%sectname_ram3% rw"="%sectname_ram3%" Debug/Exe/km4_image/km4_secure.axf Debug/Exe/km4_image/km4_secure.axf
%tooldir%\objcopy --rename-section "%sectname_ram3_nsc% rw"="%sectname_ram3_nsc%" Debug/Exe/km4_image/km4_secure.axf Debug/Exe/km4_image/km4_secure.axf
%tooldir%\objcopy --rename-section "%sectname_psram3% rw"="%sectname_psram3%" Debug/Exe/km4_image/km4_secure.axf Debug/Exe/km4_image/km4_secure.axf

%tooldir%\objcopy -j "%sectname_ram3%" -Obinary Debug/Exe/km4_image/km4_secure.axf Debug/Exe/km4_image/ram3_s.r.bin
%tooldir%\objcopy -j "%sectname_ram3_nsc%" -Obinary Debug/Exe/km4_image/km4_secure.axf Debug/Exe/km4_image/ram3_nsc.bin
%tooldir%\objcopy -j "%sectname_psram3%" -Obinary Debug/Exe/km4_image/km4_secure.axf Debug/Exe/km4_image/psram3_s.r.bin

:: remove bss sections
%tooldir%\pick %ram3_s_start% %ram3_s_end% %km4_dir%\ram3_s.r.bin %km4_dir%\ram3_s.bin raw
%tooldir%\pick %psram3_s_start% %psram3_s_end% %km4_dir%\psram3_s.r.bin %km4_dir%\psram3_s.bin raw

%tooldir%\pad %km4_dir%\ram3_s.bin 16
%tooldir%\pad %km4_dir%\ram3_nsc.bin 16
%tooldir%\pad %km4_dir%\psram3_s.bin 16

set /a ram3_s_end_aligned=(((%ram3_s_end%-1)">>"4)+1)"<<"4
set /a ram3_nsc_end_aligned=(((%ram3_nsc_end%-1)">>"4)+1)"<<"4
set /a psram3_s_end_aligned=(((%psram3_s_end%-1)">>"4)+1)"<<"4

:: add header
%tooldir%\pick %ram3_s_start% %ram3_s_end_aligned% %km4_dir%\ram3_s.bin %km4_dir%\ram3_s.p.bin
%tooldir%\pick %ram3_nsc_start% %ram3_nsc_end_aligned% %km4_dir%\ram3_nsc.bin %km4_dir%\ram3_nsc.p.bin
%tooldir%\pick %psram3_s_start% %psram3_s_end_aligned% %km4_dir%\psram3_s.bin %km4_dir%\km4_image3_psram.bin

copy /b %km4_dir%\ram3_s.p.bin+%km4_dir%\ram3_nsc.p.bin %km4_dir%\km4_image3_all.bin
::copy /b %km4_dir%\km0_km4_image2.bin+%km4_dir%\km4_image3_all.bin %km4_dir%\km4_image_all.bin

call security_config.cmd

:: RDP encryption km4_image3_all.bin ==> km4_image3_all-en.bin	
if %RDP_ENABLE% equ 1 (
	%tooldir%\EncTool.exe rdp %km4_dir%\km4_image3_all.bin %km4_dir%\km4_image3_all-en.bin %RDP_KEY%
	%tooldir%\EncTool.exe rdp %km4_dir%\km4_image3_psram.bin %km4_dir%\km4_image3_psram-en.bin %RDP_KEY%
)

del %km4_dir%\ram3_s.r.bin
del %km4_dir%\ram3_s.bin
del %km4_dir%\ram3_s.p.bin
del %km4_dir%\ram3_nsc.bin
del %km4_dir%\ram3_nsc.p.bin
del %km4_dir%\psram3_s.r.bin
del %km4_dir%\psram3_s.bin

if %build_type% equ mp (
	copy %km4_dir%\cmse_implib_mp.a %libdir%\lib\common\IAR\cmse_implib_mp.a
) else (
	copy %km4_dir%\cmse_implib.a %libdir%\lib\common\IAR\cmse_implib.a
)

exit
