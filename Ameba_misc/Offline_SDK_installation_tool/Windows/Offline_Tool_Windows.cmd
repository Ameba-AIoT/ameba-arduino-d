@echo welcome amebad
robocopy "..\..\Arduino_package\release" "%LOCALAPPDATA%\Arduino15\staging\packages"  /e
copy "..\..\Arduino_package\package_realtek.com_amebad_early_index.json" "%LOCALAPPDATA%\Arduino15"
copy "..\..\Arduino_package\package_realtek.com_amebad_index.json" "%LOCALAPPDATA%\Arduino15"
