#ifndef __WLAN_NLO_RAM_H__
#define __WLAN_NLO_RAM_H__

/*--------------------Define --------------------------------------------*/

#define MAX_SUPPORT_NL_NUM  16
#define MAX_SUPPORT_CHANNEL_NUM 13
#define SSID_MAX_LEN        32
#define PROBE_OFFSET        0x44
#define SSID_IE_OFFSET      2
#define PROBE_NUM           11
#define MAX_PROBE_REQ_NUM   8       

// below define for security 
#define _PRIVACY_	BIT(4)
#define GetPrivacy(pbuf)	((pbuf & _PRIVACY_) != 0)
#define RSN_PAIRWISEKEY_CHIPER_COUNT_OFFSET     8
#define RSN_PAIRWISEKEY_CHIPER_OUI_TYPE_OFFSET  13
#define WPA_PAIRWISEKEY_CHIPER_COUNT_OFFSET     12
#define WPA_PAIRWISEKEY_CHIPER_OUI_TYPE_OFFSET  17


/*--------------------Define Enum---------------------------------------*/

typedef enum _PAIRWISE_CHIPER
{
	NONE_OPEN 		=  0x00,
    NONE_WEP        =  0x01,
	WPA_TKIP		=  0x02,
	WPA_AES			=  0x04,
	WPA2_TKIP		=  0x20,
	WPA2_AES		=  0x40,
}PAIRWISE_CHIPER,*PPAIRWISE_CHIPER;

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/
typedef struct _NLO_OFFLOAD_NETWORK_LIST_INFO_ {
    u8      uNumOfEntries;
    u8      uNumOfHiddenAP;    
    u8      Rsvd[2] ;
    u32     FastScanPeriod; 
    u32     FastScanIterations;        
    u32     SlowScanPeriod;
    u8      SSIDlen[MAX_SUPPORT_NL_NUM];
#if 1   //CONFIG_NLO_COMPLEX_OFFLOAD    
    u8      ChiperType[MAX_SUPPORT_NL_NUM];    
    u8      Channel[MAX_SUPPORT_NL_NUM];        
#endif //CONFIG_NLO_COMPLEX_OFFLOAD    
    u8      loc_probeReq[MAX_PROBE_REQ_NUM];
    u8      SSID[MAX_SUPPORT_NL_NUM][SSID_MAX_LEN];
    u32     ScanCount;
}NLO_OFFLOAD_NETWORK_LIST_INFO,*PNLO_OFFLOAD_NETWORK_LIST_INFO;

typedef struct _SCAN_OFFLOAD_ENABLE_ {
	u8      fun_En:1;
    u8      rsvd:7;
}SCAN_OFFLOAD_ENABLE,*PSCAN_OFFLOAD_ENABLE;


/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/
void 
enableNLOScan(
    void
) ;

void 
disableNLOScan(
    void
) ;

void 
getNetwrokListInfo(
    void
) ; 

void 
InitNLOOffload(
    void
) ;

#endif //#ifndef __WLAN_NLO_RAM_H__

