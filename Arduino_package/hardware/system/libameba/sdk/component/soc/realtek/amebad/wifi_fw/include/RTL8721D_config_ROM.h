#ifndef _RTLWISOCFW_CONFIG_H
#define _RTLWISOCFW_CONFIG_H

//1 1.) Common

//AmebaD SOC
#include "ameba_soc.h"

//RTLChip
#include "RTL8721D_RTLChip.h"

//User decides attribute
#include "RTL8721D_autoconf.h"

//3 a.) System (No Change)

#ifndef __ASSEMBLY__
    //KeilC Lib
    //#include <absacc.h>

    //General Type
    #include "RTL8721D_GeneralDef.h"
    #include "RTL8721D_types.h"

#endif  //#ifndef   __ASSEMBLY__

//8051
//#include "reg51.h"


//1 2.) Each Chip
#if IS_CHIP_8721D(CONFIG_CHIP_SEL)
    //3 8721D
    #ifndef __ASSEMBLY__
    //1.) System
    //#include "boot.h"
    //#include "..\source_boot\8721D\System\Util.h"   

    //2.) Wlan
    #include "RTL8721D_Wlan_ROM.h"
    #endif  //#ifndef   __ASSEMBLY__

    //3.) HAL
    #include "RTL8721D_HAL_ROM.h"

    //4.) Final Data Structure
    #include "RTL8721D_Global_Declaration.h"

    //5.) FuncPtr
    #include "RTL8721D_FuncPtr_ROM.h"

#else
    #error  "CONFIG_CHIP_SEL Error"
#endif  //IS_CHIP_8188E(CONFIG_CHIP_SEL)


#endif  //_RTLWISOCFW_CONFIG_H



