#ifndef _RTLWISOCFW_CONFIG_RAM_H
#define _RTLWISOCFW_CONFIG_RAM_H

//1 1.) Common

//ROM Code Config
#include "RTL8721D_config_ROM.h"


#ifndef __ASSEMBLY__

//RTX51 Tiny
//#include "System\RTX51TNY.h"

//3 a.) System
//8051
//#include <string.h>

//3 b.) Wlan
//2.) Wlan
#include "RTL8721D_Wlan_RAM.h"

//3 c.) TxBuff
//#include "..\source_app2\8188E\app2.h"


//1 2.) HAL
#include "RTL8721D_Common_RAM.h"

#if IS_CHIP_8721D(CONFIG_CHIP_SEL)
//8721D
#include "RTL8721D_HAL_RAM.h"
//4 Final Data Structure
#include "RTL8721D_SysMib_RAM.h"
#include "RTL8721D_version.h"
#include "RTL8721D_RegConfig_RAM.h"

#else
#error  "CONFIG_CHIP_SEL Error"
#endif  //IS_CHIP_8188E(CONFIG_CHIP_SEL)

#endif  //#ifndef   __ASSEMBLY__

#endif  //_RTLWISOCFW_CONFIG_RAM_H



