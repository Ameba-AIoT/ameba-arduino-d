low powersaving function for GEL scenario

Description:
This example shows how to enter/exit power saving mode requested by GEL

Configuration:

[platform_opts.h] 
	#define CONFIG_BT_MESH_IDLE_CHECK   1

if use power saving mode:
[bt_mesh_device_multiple_profile_app_main.c]
	#include "low_power.h"
                Insert >>bt_mesh_power_saving_init();<< at the end of function bt_mesh_device_multiple_profile_app_init()
                Insert >>bt_mesh_power_saving_deinit();<< following the text of >>bt_mesh_idle_check_deinit();<< in function bt_mesh_device_multiple_profile_app_deinit()
                Make sure sourcefile low_power.c will be compiled 
                Make sure current directory is included

Steps to use:
	ATBm=1

[Supported List]
	Supported :
	    Ameba-z2