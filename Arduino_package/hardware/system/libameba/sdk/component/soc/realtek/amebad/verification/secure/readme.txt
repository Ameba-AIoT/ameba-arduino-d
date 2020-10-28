Secure Test Description

When test HW crypto secure mode，there are some requirement as follows:
1) BD_RAM_S must enlarge
	This must modify rlx8721d_layout.ld and tz_config(boot_trustzone_hp.c)
2) HW crypto secure test item must be selected(make menuconfig)
