#ifndef _G711_SIP_H
#define _G711_SIP_H
  
#include "dlist.h"      //list management
#include "basic_types.h"
#include "osdep_service.h"
//#include "osdep_api.h"

#define G711_DEBUG 0

#if G711_DEBUG
#define G711_PRINTF(fmt, args...)    printf("\n\r%s: " fmt, __FUNCTION__, ## args)
#define G711_ERROR(fmt, args...)     printf("\n\r%s: " fmt, __FUNCTION__, ## args)
#else
#define G711_PRINTF(fmt, args...)    
#define G711_ERROR(fmt, args...)   
#endif


//struct rtp_g711_obj {
//};


/*for debug purpose*/

#endif /*_G711_SIP_H*/