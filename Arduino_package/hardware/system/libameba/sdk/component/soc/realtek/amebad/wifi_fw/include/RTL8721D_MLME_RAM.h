#ifndef __WLAN_MLME_RAM_H__
#define __WLAN_MLME_RAM_H__

/*--------------------Define --------------------------------------------*/

/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/

//Scan
typedef struct _ScanInfo_ {
    BOOLEAN     bScan;

}ScanInfo, *PScanInfo;

/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/

extern void
InitMLME(
    void
);


#endif  //__WLAN_MLME_RAM_H__

