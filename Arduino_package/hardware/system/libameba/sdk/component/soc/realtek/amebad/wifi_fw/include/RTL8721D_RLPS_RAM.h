#ifndef __WLAN_DYM_LPS_RAM_H__
#define __WLAN_DYM_LPS_RAM_H__

/*--------------------Define --------------------------------------------*/
#define RPATTERN0 0x02
#define RPATTERN1 0x04
#define RPATTERN2 0x06
#define RPATTERN3 0x0C
/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/
typedef struct _RLPS_PARM_ {
    u8                      VectorUpdate;
    u8                      PatternUpdate;
    u8                      RLPSOn;
    u8                      RLPSRFOn;
    u8                      RVector[4];
    u8                      RPattern;
    u8                      PatternLen;
}RLPS_Parm, *PRLPS_Parm;

/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/
#if CONFIG_RLPS
void 
UpdateRandomParameter(
    void
) ;

void 
InitRLPS(
    void
) ;

void
RLPSTBTTHDL(
    void
) ;

void
RLPSGtimer4HDL(
    void
) ;

u8
RLPSBCNEarlyHDL(
    u8 Wakeup
) ;
#endif  //CONFIG_RLPS
#endif  //__WLAN_DYM_LPS_RAM_H__