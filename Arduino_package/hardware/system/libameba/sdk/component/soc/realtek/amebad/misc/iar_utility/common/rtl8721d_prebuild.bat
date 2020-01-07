cd /D %1
set build_dir=%2

:: Generate build_info.h
::echo off
::echo %date:~0,10%-%time:~0,8%
::echo %USERNAME%
for /f "usebackq" %%i in (`hostname`) do set hostname=%%i
::echo %hostname%

echo #define UTS_VERSION "%date:~0,10%-%time:~0,8%" > %build_dir%\build_info.h
echo #define RTL_FW_COMPILE_TIME "%date:~0,10%-%time:~0,8%" >> %build_dir%\build_info.h
echo #define RTL_FW_COMPILE_DATE "%date:~0,10%" >> %build_dir%\build_info.h
echo #define RTL_FW_COMPILE_BY "%USERNAME%" >> %build_dir%\build_info.h
echo #define RTL_FW_COMPILE_HOST "%hostname%" >> %build_dir%\build_info.h
echo #define RTL_FW_COMPILE_DOMAIN >> %build_dir%\build_info.h
echo #define RTL_FW_COMPILER "IAR compiler" >> %build_dir%\build_info.h

:end
exit

