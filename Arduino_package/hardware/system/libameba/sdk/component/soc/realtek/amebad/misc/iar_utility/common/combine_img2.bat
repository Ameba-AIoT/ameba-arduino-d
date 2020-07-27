call security_config.cmd

if exist %target_dir%\km0_km4_image2.bin (
	del %target_dir%\km0_km4_image2.bin
)

:: copy km0_image2_all.bin
if not exist %km0_dir%\km0_image2_all.bin (
	xcopy %libdir%\image\km0_image2_all.bin %km0_dir%\
)

:: copy km4_image2_all.bin
if not exist %km4_dir%\km4_image2_all.bin (
	xcopy %libdir%\image\km4_image2_all.bin %km4_dir%\
)

if %RSIP_ENABLE% equ 1 (
	:: encrpyt km0_image2_all.bin => km0_image2_all-en.bin
	%tooldir%\EncTool.exe rsip %km0_dir%\km0_image2_all.bin %km0_dir%\km0_image2_all-en.bin 0x0c000000 %RSIP_KEY% %RSIP_IV%

	:: encrpyt km4_image2_all.bin => km4_image2_all-en.bin
	%tooldir%\EncTool.exe rsip %km4_dir%\km4_image2_all.bin %km4_dir%\km4_image2_all-en.bin 0x0e000000 %RSIP_KEY% %RSIP_IV%

	copy /b %km0_dir%\km0_image2_all-en.bin+%km4_dir%\km4_image2_all-en.bin %target_dir%\km0_km4_image2_tmp.bin

) else (
	copy /b %km0_dir%\km0_image2_all.bin+%km4_dir%\km4_image2_all.bin %target_dir%\km0_km4_image2_tmp.bin
)

if %RDP_ENABLE% equ 1 (
	if exist %km4_dir%\km4_image3_all-en.bin (
		copy /b %target_dir%\km0_km4_image2_tmp.bin+%km4_dir%\km4_image3_all-en.bin+%km4_dir%\km4_image3_psram-en.bin %target_dir%\km0_km4_image2.bin
	) else (
		rename %target_dir%\km0_km4_image2_tmp.bin km0_km4_image2.bin
	)
) else (
	rename %target_dir%\km0_km4_image2_tmp.bin km0_km4_image2.bin
)

copy %target_dir%\km0_km4_image2.bin %km0_dir%\
copy %target_dir%\km0_km4_image2.bin %km4_dir%\

if exist %target_dir%\km0_km4_image2_tmp.bin (
	del %target_dir%\km0_km4_image2_tmp.bin
)
