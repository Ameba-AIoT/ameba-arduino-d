#ifndef _AUTOCONF_H_
#define _AUTOCONF_H_


/******************************************

        Chip Selection
        
// Field     RTLCHIP        CHIPCUT
// BITS  [15:4]       [3:0]


*******************************************/
//Warning: The line format can't be modified(from Chip Select to Special Configuration). 
//It is used for parsing.

#define AUTOBUILD_START_PREFIX 1

//2 Main Config
//3 1.) Chip Select
//#define CONFIG_CHIP_SEL RTL8721D_TEST_CHIP_S      //8721D has no testchip
#define CONFIG_CHIP_SEL RTL8721D_A_CUT_S
//#define CONFIG_CHIP_SEL RTL8721D_B_CUT_U




//3 2.) Category Select  [NIC/AP/General] [SDIO/USB/PCIE/General]
//Warning: The line format can't be modified. It is used for parsing.
//#define CONFIG_CATEGORY_SEL RTLCHIP_CATEGORY_NIC_GENERAL
//#define CONFIG_CATEGORY_SEL RTLCHIP_CATEGORY_AP_GENERAL
//#define CONFIG_CATEGORY_SEL RTLCHIP_CATEGORY_NIC_WOWLAN
#define CONFIG_CATEGORY_SEL RTLCHIP_CATEGORY_NIC_IoT

//3 3.) Special Configuration (This should be disable in ASIC!)
#define CONFIG_ICE_MODE                 0
#define CONFIG_VERA_ENV                 0
#define CONFIG_KEIL_C_SIMULATION        0
#define CONFIG_VERIFICATION_MODE        0
#define CONFIG_8051_INT_TESTING         0

#define AUTOBUILD_END_PREFIX 1
/******************************************

    System Configuration                                                

*******************************************/

//2 Memory Layout
// TODO: Check below setting
#define MINIMUM_STACK_SIZE          0x40
#define RAM_CODE_START_ADDRESS      0x8000
#define XDATA_START_ADDRESS         0x8000
#define TXBUFF_CODE_START_ADDRESS   0xE000
#define XDATA_LENGTH                0x3000  //12k bytes

#if IS_CUT_A(CONFIG_CHIP_SEL)
#define PDATA_START_ADDRESS         0x9000  //XDATA_START_ADDRESS
#define PDATA_LENGTH                0x100   // 256 bytes
#define PPAGE_NUM                   (PDATA_START_ADDRESS>>8)
#else
#define PDATA_START_ADDRESS         0
#define PDATA_LENGTH                0
#define PPAGE_NUM                   0
#endif      //#if IS_CUT_A(CONFIG_CHIP_SEL)

#define SFR_MPAGE                   0x92

//Stretch Memory Cycle
//#define CKCON_NORMAL                0x2       //Can't be modified. Bounding for ROM code
#define CKCON_NORMAL                0x1     //Can't be modified. Bounding for ROM code
#define CKCON_ICE_MODE              0x3     //Can't be modified. Bounding for ROM code

//Write PON Delay
#define WAIT_US_WRITE_POWERON_8723B   16      //Can't be modified. Bounding for ROM code

//TRX Packet Buffer Indirect Access Page Size
#define PAGE_SIZE_IA_TXPKTBUF       256
#define PAGE_SIZE_IA_RXPKTBUF       256

//Page Size
#define PAGESIZE_TX                 128
#define PAGESIZE_RX                 8

//TXPKTBUF page
//TXPKTBUF : 32K
#define TX_PAGE_NUM			256		


//TRX DESC Size
#define SIZE_RX_DESC_MAC            24
#define SIZE_TX_DESC_MAC            40

#define EFUSE_SIZE                  512
#define PKTBUF_DBG_DATASIZE         8

#define MACID_NUM                   16

// Para Alan
#define LENGTH_IEEE80211_HDR_3ADDR  24
#define FIX_IE_LENGTH               12

//security cam
#define SECCAM_ENTRY_NUM        32    
#define SECCAM_DIRECT_ENTRY_SIZE                  32
#define SECCAM_START_ADDR        0x1A00
/******************************************

    ROM Code Configuration                                              

*******************************************/
//1 ROM Version  
#if IS_CHIP_8721D(CONFIG_CHIP_SEL)
    #if IS_CUT_TEST(CONFIG_CHIP_SEL)
        #define ROM_VERSION   1
    #elif IS_CUT_A(CONFIG_CHIP_SEL)
        #define ROM_VERSION   2
    #elif IS_CUT_B(CONFIG_CHIP_SEL) 
        #define ROM_VERSION   3
    #else
        #error "Cut Error"
    #endif  //IS_CUT_TEST(CONFIG_CHIP_SEL)
#else
    #error  "Chip Error"
#endif
#define ROM_VERSION_VERA            0xFF



//1 Functionality

//2 Common - Immobilization 
//Can't be modified. Bounding for ROM code
#define CONFIG_INIT_OFFLOAD             0   
#define CONFIG_FWLB                     1   
#define CONFIG_WLAN_ENGINE_ROM          1   //Can't be modified. Bounding for ROM code    
#define CONFIG_ROMDLFW                  0   

#define CONFIG_RF_JAGUAR                0   //8723d no support new 3-wire

#define CONFIG_U3_INT                   0

#define CONFIG_ROM_PATCH                0
#define CONFIG_ROM_32K                  1




//2 Common 
#define CONFIG_TXBF                     0
#if CONFIG_TXBF
    #define CONFIG_FW_SND               0       //8723b not support TxBF
#else
    #define CONFIG_FW_SND               0
#endif
#define CONFIG_DRV_FW_CTRL              0
#define CONFIG_8BYTE_H2C                1
#define CONFIG_C2H_PKT                  1
#define CONFIG_C2H_REG                  0
#define CONFIG_PSD_OFFLOAD              0
#define CONFIG_RLPS                     0   // disable RLPS
#define CONFIG_RLPS_METHOD0             1
#define CONFIG_RLPS_METHOD1             0
#define CONFIG_BB_GAIN_RPT              0
#define CONFIG_BB_NHM                   0
#define CONFIG_Efuse_Hidden_Request     0
#define CONFIG_Defeature                0


//2 1.) CATEGORY : NIC
#if IS_CATEGORY_NIC(CONFIG_CATEGORY_SEL)
//3 General
#define CONFIG_DISCONNECT_DECISION      0
#define CONFIG_KEEP_ALIVE               0
#define CONFIG_FAST_CHANNEL_SWITCH      1
#if CONFIG_FAST_CHANNEL_SWITCH
#define CONFIG_FCS_LOGO                 1
#define CONFIG_MCC                      0
#define CONFIG_NOA_ISR                  0 //use NOA isr replace GTimer, no work 0717...
#define CONFIG_PS_TIMER                 0
#else
#define CONFIG_FCS_LOGO                 0
#define CONFIG_MCC                      0
#define CONFIG_NOA_ISR                  0
#define CONFIG_PS_TIMER                 0
#endif //#if CONFIG_FAST_CHANNEL_SWITCH

#define CONFIG_D0_SCAN_OFFLOAD          0
#define CONFIG_SWITCH_CAHNNEL_OFFLOAD   0
#define CONFIG_SINGLE_SWITCH_CHANNEL    1
#define CONFIG_ECSA                     0

#if (CONFIG_FAST_CHANNEL_SWITCH || CONFIG_D0_SCAN_OFFLOAD || CONFIG_SWITCH_CAHNNEL_OFFLOAD)
#define CONFIG_RF_SETTING               1
#else
#define CONFIG_RF_SETTING               0
#endif  //#if (CONFIG_FAST_CHANNEL_SWITCH || CONFIG_D0_SCAN_OFFLOAD || CONFIG_SWITCH_CAHNNEL_OFFLOAD)

#define CONFIG_SW_TIMER                 1
#define CONFIG_EFUSE_PART_NUM           0
#define CONFIG_TXBEACON_IGNORE_EDCCA	0
#define CONFIG_TX_PAUSE_DRV_INFO		1
#define CONFIG_MACID_PAUSE_DRV_INFO		0
#define CONFIG_TX_PAUSE_WO_MASK         0


//3 PS Related
#define CONFIG_SAPPS                    0
#define CONFIG_NOLINK_PS                0//must enable in formal FW
#define CONFIG_INACTIVE_PS              1//must enable in formal FW //for RTD3
#define CONFIG_POWERSAVING              1   //must enable in formal FW
#if CONFIG_POWERSAVING
#define CONFIG_HW_PS_CTRL               1
#define CONFIG_PS_SUPPORT_32K           1
#define CONFIG_PARTIAL_OFF              1
#define CONFIG_LOWPWR_RXMODE            0   //It is not supported by 88E
#define CONFIG_WMMPS                    0
#define CONFIG_P2P_PS                   1
#define CONFIG_FAST_CPWM                1
#define CONFIG_TASK_IDLE_CHK            1
#define CONFIG_BCNEARLY_ADJUST          1       //must not both enable!!ben, 170206
#define CONFIG_BCNEARLY_ADJUST_V1       0	//must disable while CONFIG_NAN enable!!ben, 170206
#define CONFIG_BCNEARLY_ADJUST_V2       0   //david_liu,181016
#define CONFIG_LOWPWR_BEACON_MODE       0
#define CONFIG_S1_NULL1_IVL_PATCH      0
#else
#define CONFIG_HW_PS_CTRL               0
#define CONFIG_PS_SUPPORT_32K           0
#define CONFIG_PARTIAL_OFF              0
#define CONFIG_LOWPWR_RXMODE            0   //It is not supported by 88E
#define CONFIG_WMMPS                    0
#define CONFIG_P2P_PS                   0
#define CONFIG_FAST_CPWM                0
    #define CONFIG_TASK_IDLE_CHK        0
#define CONFIG_BCNEARLY_ADJUST          0       //must not both enable!!ben, 170206
#define CONFIG_BCNEARLY_ADJUST_V1       0	//must disable while CONFIG_NAN enable!!ben, 170206
#define CONFIG_BCNEARLY_ADJUST_V2       0   //david_liu,181016
#define CONFIG_LOWPWR_BEACON_MODE       0
#define CONFIG_S1_NULL1_IVL_PATCH      0
#endif

//3NAN Related
#define CONFIG_NAN                      1

//3  LTE Coex Related
#define CONFIG_LTECOEX                  0    //alan, 2014_0207

//3 BT Coex Related
#define CONFIG_BTCOEX_CTRL              0
#define CONFIG_BT_MAILBOX               1//must enable in formal FW
#define CONFIG_SW_MAILBOX               0
#define CONFIG_BT_MP_Mode               0    // alan , 2013_11_27


//3 Remote WakeUp
#define CONFIG_REMOTE_WAKEUP            0
#if CONFIG_REMOTE_WAKEUP
    #define CONFIG_WoWLAN               1
    #define CONFIG_AES_GTK              1   
    #define CONFIG_ARP                  1   
    #define CONFIG_NDP                  1 
    #define CONFIG_TEREDO_REALWOWL      0
    #define CONFIG_REALWOW_V2           1
    #define CONFIG_RTD3_SCAN_OFFLOAD    1    
    #define CONFIG_NLO_COMPLEX_OFFLOAD  0    
    #define CONFIG_NLO_SIMPLE_OFFLOAD   0        
    #define CODE_REDUCE                 1
    #define CONFIG_WOWLAN_APL_QQ        1 
    #define CONFIG_ANT_SWT              1
    #define CONFIG_CUSTOMIZATION_GPIO   0	 
    #define CONFIG_P2P_OFFLOAD	        0
    #define CONFIG_SIDEBAND_WoWLAN      0
    #define CONFIG_RUNTIME_PM_OFFLOAD   0
    #define CONFIG_MULTI_FUNCTION       0
    #define CONFIG_DELAY_WAKEUP       0
    #define CONFIG_WPA2_ENTERPRISE              0
    #define CONFIG_NEGATIVE_PATTERN     0
    #define CONFIG_ALL_DATA_WAKEUP      0
#else
    #define CONFIG_WoWLAN               0
    #define CONFIG_AES_GTK              0   
    #define CONFIG_ARP                  0  
    #define CONFIG_NDP                  0 
    #define CONFIG_TEREDO_REALWOWL      0
    #define CONFIG_REALWOW_V2           0
    #define CONFIG_RTD3_SCAN_OFFLOAD    0    
    #define CONFIG_NLO_COMPLEX_OFFLOAD  0        
    #define CONFIG_NLO_SIMPLE_OFFLOAD   0    
    #define CODE_REDUCE                 0
    #define CONFIG_WOWLAN_APL_QQ        0 
    #define CONFIG_ANT_SWT              0
    #define CONFIG_CUSTOMIZATION_GPIO   0	 
    #define CONFIG_P2P_OFFLOAD	        0	
    #define CONFIG_SIDEBAND_WoWLAN      0
    #define CONFIG_RUNTIME_PM_OFFLOAD   0
    #define CONFIG_MULTI_FUNCTION       0
    #define CONFIG_DELAY_WAKEUP       0
    #define CONFIG_WPA2_ENTERPRISE              0
    #define CONFIG_NEGATIVE_PATTERN     0
    #define CONFIG_ALL_DATA_WAKEUP      0
#endif


//3 Dynamic Mechanism Offload
#define CONFIG_RATE_ADAPTIVE            1//must enable in formal FW
#if CONFIG_RATE_ADAPTIVE
    #define CONFIG_RA_PS_MODE           0
    #define CONFIG_CCX                  1
    #define CONFIG_POWER_TRAINING_WIL   1
    #define CONFIG_HALcom_RA            0
    #define CONFIG_HALcom_RA_2          0
    #define CONFIG_11AC_1SS             0
    #define CONFIG_11AC_2SS             0
    #define CONFIG_11N_1SS				1
    #define CONFIG_11N_2SS				0
    #define CONFIG_RA_MULTI_TRY         1
    #define CONFIG_RA_NSC_CALCULATE     1
    #define CONFIG_RA_SET_RAINFO        1  
    #define CONFIG_1SS_SL_GI_MULTI_TRY   1
#else
    #define CONFIG_RA_PS_MODE           0
    #define CONFIG_CCX                  0
    #define CONFIG_POWER_TRAINING_WIL   0
    #define CONFIG_HALcom_RA            0
    #define CONFIG_HALcom_RA_2          0
    #define CONFIG_11AC_1SS             0
    #define CONFIG_11AC_2SS             0
    #define CONFIG_11N_1SS				1
    #define CONFIG_11N_2SS				0
    #define CONFIG_RA_MULTI_TRY         0
    #define CONFIG_RA_NSC_CALCULATE     0
    #define CONFIG_RA_SET_RAINFO        0 
    #define CONFIG_1SS_SL_GI_MULTI_TRY   0
#endif  //#if CONFIG_RATE_ADAPTIVE

    #define CONFIG_11AC 			(CONFIG_11AC_1SS | CONFIG_11AC_2SS)
    #define CONFIG_11N 				(CONFIG_11N_1SS | CONFIG_11N_2SS)
    
    #define CONFIG_1SS 				(CONFIG_11N_1SS | CONFIG_11AC_1SS)
    #define CONFIG_2SS 				(CONFIG_11N_2SS | CONFIG_11AC_2SS)

#define CONFIG_IQK_OFFLOAD              0


//3 AP offload      //Alan
#define CONFIG_AP_OFFLOAD               0
#if CONFIG_AP_OFFLOAD
#define CONFIG_MIFI_PLATFORM        1
#else
#define CONFIG_MIFI_PLATFORM        0
#endif
#define CONFIG_AP_TXBCNERR_PATCH        0

#define CONFIG_AP_PS_OFFLOAD                    0
#define CONFIG_STA_AGING_FUNC                   0
#define CONFIG_HW_DETECT_PS_OFFLOAD             0

#endif  //#if IS_CATEGORY_NIC(CONFIG_CATEGORY_SEL)


//2 2.) CATEGORY : AP

#if IS_CATEGORY_AP(CONFIG_CATEGORY_SEL)
//3 General
#define CONFIG_DISCONNECT_DECISION      0
#define CONFIG_KEEP_ALIVE               0
#define CONFIG_FAST_CHANNEL_SWITCH      0
#if CONFIG_FAST_CHANNEL_SWITCH
#define CONFIG_FCS_LOGO                 0
#define CONFIG_MCC                      0
#define CONFIG_NOA_ISR                  0 //use NOA isr replace GTimer, no work 0717...
#define CONFIG_PS_TIMER                 0
#else
#define CONFIG_FCS_LOGO                 0
#define CONFIG_MCC                      0
#define CONFIG_NOA_ISR                  0
#define CONFIG_PS_TIMER                 0
#endif //#if CONFIG_FAST_CHANNEL_SWITCH

#define CONFIG_D0_SCAN_OFFLOAD          0
#define CONFIG_SWITCH_CAHNNEL_OFFLOAD   0
#define CONFIG_SINGLE_SWITCH_CHANNEL    0
#define CONFIG_ECSA                     0

#if (CONFIG_FAST_CHANNEL_SWITCH || CONFIG_D0_SCAN_OFFLOAD || CONFIG_SWITCH_CAHNNEL_OFFLOAD)
#define CONFIG_RF_SETTING               1
#else
#define CONFIG_RF_SETTING               0
#endif  //#if (CONFIG_FAST_CHANNEL_SWITCH || CONFIG_D0_SCAN_OFFLOAD || CONFIG_SWITCH_CAHNNEL_OFFLOAD)

#define CONFIG_SW_TIMER                 0
#define CONFIG_EFUSE_PART_NUM           0  
#define CONFIG_TXBEACON_IGNORE_EDCCA	1
#define CONFIG_TX_PAUSE_DRV_INFO		1
#define CONFIG_MACID_PAUSE_DRV_INFO		0
#define CONFIG_TX_PAUSE_WO_MASK         0

//3 PS Related
#define CONFIG_SAPPS                    1
#define CONFIG_NOLINK_PS              0
#define CONFIG_INACTIVE_PS              0
#define CONFIG_POWERSAVING              1
#if CONFIG_POWERSAVING
    #define CONFIG_HW_PS_CTRL           1
    #define CONFIG_PS_SUPPORT_32K       1
    #define CONFIG_PARTIAL_OFF              1
    #define CONFIG_LOWPWR_RXMODE        0   //It is not supported by 88E
    #define CONFIG_WMMPS                0
    #define CONFIG_P2P_PS               0
    #define CONFIG_FAST_CPWM            1
    #define CONFIG_TASK_IDLE_CHK        1
    #define CONFIG_BCNEARLY_ADJUST      1       //must not both enable!!ben, 170206
    #define CONFIG_BCNEARLY_ADJUST_V1       0	//must disable while CONFIG_NAN enable!!ben, 170206
    #define CONFIG_BCNEARLY_ADJUST_V2       0   //david_liu,181016
    #define CONFIG_LOWPWR_BEACON_MODE   0
    #define CONFIG_S1_NULL1_IVL_PATCH      0
#else
    #define CONFIG_HW_PS_CTRL           0
    #define CONFIG_PS_SUPPORT_32K       0
    #define CONFIG_PARTIAL_OFF              0
    #define CONFIG_LOWPWR_RXMODE        0   //It is not supported by 88E
    #define CONFIG_WMMPS                0
    #define CONFIG_P2P_PS               0
    #define CONFIG_FAST_CPWM            0
    #define CONFIG_TASK_IDLE_CHK        0
    #define CONFIG_BCNEARLY_ADJUST      0       //must not both enable!!ben, 170206
    #define CONFIG_BCNEARLY_ADJUST_V1       0	//must disable while CONFIG_NAN enable!!ben, 170206
    #define CONFIG_BCNEARLY_ADJUST_V2       0   //david_liu,181016
    #define CONFIG_LOWPWR_BEACON_MODE   0
    #define CONFIG_S1_NULL1_IVL_PATCH      0
#endif

//3NAN Related
#define CONFIG_NAN                          0

//3  LTE Coex Related
#define CONFIG_LTECOEX                         0    //alan, 2014_0207

//3 BT Coex Related
#define CONFIG_BTCOEX_CTRL              0
#define CONFIG_BT_MAILBOX               1
#define CONFIG_SW_MAILBOX               0
#define CONFIG_BT_MP_Mode              0    // alan , 2013_11_27


//3 Remote WakeUp
#define CONFIG_REMOTE_WAKEUP            0
#if CONFIG_REMOTE_WAKEUP
    #define CONFIG_WoWLAN               0
    #define CONFIG_AES_GTK              0   
    #define CONFIG_ARP                  0   
    #define CONFIG_NDP                  0 
    #define CONFIG_TEREDO_REALWOWL      0
    #define CONFIG_REALWOW_V2           0
    #define CONFIG_RTD3_SCAN_OFFLOAD    0    
    #define CONFIG_NLO_COMPLEX_OFFLOAD  0        
    #define CONFIG_NLO_SIMPLE_OFFLOAD   0    
    #define CODE_REDUCE                 0
    #define CONFIG_WOWLAN_APL_QQ        0
    #define CONFIG_ANT_SWT              0
    #define CONFIG_CUSTOMIZATION_GPIO   0	
    #define CONFIG_P2P_OFFLOAD	        0	
    #define CONFIG_SIDEBAND_WoWLAN      0
    #define CONFIG_RUNTIME_PM_OFFLOAD   0
    #define CONFIG_MULTI_FUNCTION       0
    #define CONFIG_DELAY_WAKEUP       0
    #define CONFIG_WPA2_ENTERPRISE              0
    #define CONFIG_NEGATIVE_PATTERN     0
    #define CONFIG_ALL_DATA_WAKEUP      0
#else
    #define CONFIG_WoWLAN               0
    #define CONFIG_AES_GTK              0  
    #define CONFIG_ARP                  0   
    #define CONFIG_NDP                  0 
    #define CONFIG_TEREDO_REALWOWL      0
    #define CONFIG_REALWOW_V2           0
    #define CONFIG_RTD3_SCAN_OFFLOAD    0    
    #define CONFIG_NLO_COMPLEX_OFFLOAD  0        
    #define CONFIG_NLO_SIMPLE_OFFLOAD   0    
    #define CODE_REDUCE                 0
    #define CONFIG_WOWLAN_APL_QQ        0
    #define CONFIG_ANT_SWT              0
    #define CONFIG_CUSTOMIZATION_GPIO   0	
    #define CONFIG_P2P_OFFLOAD	        0	
    #define CONFIG_SIDEBAND_WoWLAN      0
    #define CONFIG_RUNTIME_PM_OFFLOAD   0
    #define CONFIG_MULTI_FUNCTION       0
    #define CONFIG_DELAY_WAKEUP       0
    #define CONFIG_WPA2_ENTERPRISE              0
    #define CONFIG_NEGATIVE_PATTERN     0
    #define CONFIG_ALL_DATA_WAKEUP      0
#endif


//3 Dynamic Mechanism Offload
#define CONFIG_RATE_ADAPTIVE            1
#if CONFIG_RATE_ADAPTIVE
    #define CONFIG_RA_PS_MODE           0
    #define CONFIG_CCX                  1
    #define CONFIG_POWER_TRAINING_WIL   1
    #define CONFIG_HALcom_RA            0
    #define CONFIG_HALcom_RA_2          0
    #define CONFIG_11AC_1SS             0
    #define CONFIG_11AC_2SS             0
    #define CONFIG_11N_1SS				1
    #define CONFIG_11N_2SS				0
    #define CONFIG_RA_MULTI_TRY         1
    #define CONFIG_RA_NSC_CALCULATE     1
    #define CONFIG_RA_SET_RAINFO        1
    #define CONFIG_1SS_SL_GI_MULTI_TRY   1
#else
    #define CONFIG_RA_PS_MODE           0
    #define CONFIG_CCX                  0
    #define CONFIG_POWER_TRAINING_WIL   0
    #define CONFIG_HALcom_RA            0
    #define CONFIG_HALcom_RA_2          0
    #define CONFIG_11AC_1SS             0
    #define CONFIG_11AC_2SS             0
    #define CONFIG_11N_1SS				0
    #define CONFIG_11N_2SS				0
    #define CONFIG_RA_MULTI_TRY         0
    #define CONFIG_RA_NSC_CALCULATE     0
    #define CONFIG_RA_SET_RAINFO        0 
    #define CONFIG_1SS_SL_GI_MULTI_TRY   0
#endif  //#if CONFIG_RATE_ADAPTIVE

    #define CONFIG_11AC 			(CONFIG_11AC_1SS | CONFIG_11AC_2SS)
    #define CONFIG_11N 				(CONFIG_11N_1SS | CONFIG_11N_2SS)
    
    #define CONFIG_1SS 				(CONFIG_11N_1SS | CONFIG_11AC_1SS)
    #define CONFIG_2SS 				(CONFIG_11N_2SS | CONFIG_11AC_2SS)
    
#define CONFIG_IQK_OFFLOAD              0


//3 AP offload  //Alan
#define CONFIG_AP_OFFLOAD               1
#if CONFIG_AP_OFFLOAD
#define CONFIG_MIFI_PLATFORM        0
#else
#define CONFIG_MIFI_PLATFORM        0
#endif
#define CONFIG_AP_TXBCNERR_PATCH        0

#define CONFIG_AP_PS_OFFLOAD                    0
#define CONFIG_STA_AGING_FUNC                   0
#define CONFIG_HW_DETECT_PS_OFFLOAD             0

#endif  //#if IS_CATEGORY_AP(CONFIG_CATEGORY_SEL)


//2 3.) CATEGORY : WOWLAN

#if IS_CATEGORY_WOWLAN(CONFIG_CATEGORY_SEL)
//3 General
#define CONFIG_DISCONNECT_DECISION      1
#define CONFIG_KEEP_ALIVE               1
#define CONFIG_FAST_CHANNEL_SWITCH      0
#if CONFIG_FAST_CHANNEL_SWITCH
#define CONFIG_FCS_LOGO                 0
#define CONFIG_MCC                      0
#define CONFIG_NOA_ISR                  0 //use NOA isr replace GTimer, no work 0717...
#define CONFIG_PS_TIMER                 0
#else
#define CONFIG_FCS_LOGO                 0
#define CONFIG_MCC                      0
#define CONFIG_NOA_ISR                  0
#define CONFIG_PS_TIMER                 0
#endif //#if CONFIG_FAST_CHANNEL_SWITCH

#define CONFIG_D0_SCAN_OFFLOAD          1
#define CONFIG_SWITCH_CAHNNEL_OFFLOAD   0
#define CONFIG_SINGLE_SWITCH_CHANNEL    0
#define CONFIG_ECSA                     0

#if (CONFIG_FAST_CHANNEL_SWITCH || CONFIG_D0_SCAN_OFFLOAD || CONFIG_SWITCH_CAHNNEL_OFFLOAD)
#define CONFIG_RF_SETTING               1
#else
#define CONFIG_RF_SETTING               0
#endif  //#if (CONFIG_FAST_CHANNEL_SWITCH || CONFIG_D0_SCAN_OFFLOAD || CONFIG_SWITCH_CAHNNEL_OFFLOAD)

#define CONFIG_SW_TIMER                 1
#define CONFIG_EFUSE_PART_NUM           0  
#define CONFIG_TXBEACON_IGNORE_EDCCA	0
#define CONFIG_TX_PAUSE_DRV_INFO		1
#define CONFIG_MACID_PAUSE_DRV_INFO		0
#define CONFIG_TX_PAUSE_WO_MASK         0


//3 PS Related
#define CONFIG_SAPPS                    0
#define CONFIG_NOLINK_PS                0
#define CONFIG_INACTIVE_PS              1
#define CONFIG_POWERSAVING              1
#if CONFIG_POWERSAVING
    #define CONFIG_HW_PS_CTRL           1
    #define CONFIG_PS_SUPPORT_32K       1
    #define CONFIG_PARTIAL_OFF              1
    #define CONFIG_LOWPWR_RXMODE        0   //It is not supported by 88E
    #define CONFIG_WMMPS                0
    #define CONFIG_P2P_PS               0
    #define CONFIG_FAST_CPWM            1
    #define CONFIG_TASK_IDLE_CHK        1
    #define CONFIG_BCNEARLY_ADJUST      1       //must not both enable!!ben, 170206
    #define CONFIG_BCNEARLY_ADJUST_V1       0	//must disable while CONFIG_NAN enable!!ben, 170206
    #define CONFIG_BCNEARLY_ADJUST_V2       0   //david_liu,181016
    #define CONFIG_LOWPWR_BEACON_MODE   0
    #define CONFIG_S1_NULL1_IVL_PATCH      0
#else
    #define CONFIG_HW_PS_CTRL           0
    #define CONFIG_PS_SUPPORT_32K       0
    #define CONFIG_PARTIAL_OFF              0
    #define CONFIG_LOWPWR_RXMODE        0   //It is not supported by 88E
    #define CONFIG_WMMPS                0
    #define CONFIG_P2P_PS               0
    #define CONFIG_FAST_CPWM            0
    #define CONFIG_TASK_IDLE_CHK        0
    #define CONFIG_BCNEARLY_ADJUST      0       //must not both enable!!ben, 170206
    #define CONFIG_BCNEARLY_ADJUST_V1       0	//must disable while CONFIG_NAN enable!!ben, 170206
    #define CONFIG_BCNEARLY_ADJUST_V2       0   //david_liu,181016
    #define CONFIG_LOWPWR_BEACON_MODE   0
    #define CONFIG_S1_NULL1_IVL_PATCH      0
#endif

//3NAN Related
#define CONFIG_NAN                          0

//3  LTE Coex Related
#define CONFIG_LTECOEX                         0    //alan, 2014_0207

//3 BT Coex Related
#define CONFIG_BTCOEX_CTRL              0
#define CONFIG_BT_MAILBOX               1
#define CONFIG_SW_MAILBOX               0
#define CONFIG_BT_MP_Mode              0    // alan , 2013_11_27


//3 Remote WakeUp
#define CONFIG_REMOTE_WAKEUP            1
#if CONFIG_REMOTE_WAKEUP
    #define CONFIG_WoWLAN               1
    #define CONFIG_AES_GTK              1   
    #define CONFIG_ARP                  1   
    #define CONFIG_NDP                  1 
    #define CONFIG_TEREDO_REALWOWL      0
    #define CONFIG_REALWOW_V2           1
    #define CONFIG_RTD3_SCAN_OFFLOAD    1    
    #define CONFIG_NLO_COMPLEX_OFFLOAD  0    // For Windows driver version
    #define CONFIG_NLO_SIMPLE_OFFLOAD   0    // For Linux driver version
    #define CODE_REDUCE                 1
    #define CONFIG_WOWLAN_APL_QQ        1
    #define CONFIG_ANT_SWT              1	//antenna switch in wowlan mode
    #define CONFIG_CUSTOMIZATION_GPIO   0
    #define CONFIG_P2P_OFFLOAD	        0
    #define CONFIG_SIDEBAND_WoWLAN      0
    #define CONFIG_RUNTIME_PM_OFFLOAD   0
    #define CONFIG_MULTI_FUNCTION       0
    #define CONFIG_DELAY_WAKEUP       0
    #define CONFIG_WPA2_ENTERPRISE              0
    #define CONFIG_NEGATIVE_PATTERN     1
    #define CONFIG_ALL_DATA_WAKEUP      0
#else
    #define CONFIG_WoWLAN               0
    #define CONFIG_AES_GTK              0   
    #define CONFIG_ARP                  0   
    #define CONFIG_NDP                  0 
    #define CONFIG_TEREDO_REALWOWL      0
    #define CONFIG_REALWOW_V2           0
    #define CONFIG_RTD3_SCAN_OFFLOAD    0  
    #define CONFIG_NLO_COMPLEX_OFFLOAD  0        
    #define CONFIG_NLO_SIMPLE_OFFLOAD   0    
    #define CODE_REDUCE                 0
    #define CONFIG_WOWLAN_APL_QQ        0
    #define CONFIG_ANT_SWT              0	
    #define CONFIG_CUSTOMIZATION_GPIO   0
    #define CONFIG_P2P_OFFLOAD	        0	
    #define CONFIG_SIDEBAND_WoWLAN      0
    #define CONFIG_RUNTIME_PM_OFFLOAD   0
    #define CONFIG_MULTI_FUNCTION       0
    #define CONFIG_DELAY_WAKEUP       0
    #define CONFIG_WPA2_ENTERPRISE              0
    #define CONFIG_NEGATIVE_PATTERN     0
    #define CONFIG_ALL_DATA_WAKEUP      0
#endif


//3 Dynamic Mechanism Offload
#define CONFIG_RATE_ADAPTIVE            0
#if CONFIG_RATE_ADAPTIVE
    #define CONFIG_RA_PS_MODE           0
    #define CONFIG_CCX                  1
    #define CONFIG_POWER_TRAINING_WIL   1
    #define CONFIG_HALcom_RA            0
    #define CONFIG_HALcom_RA_2          0
    #define CONFIG_11AC_1SS             0
    #define CONFIG_11AC_2SS             0
    #define CONFIG_11N_1SS              1
    #define CONFIG_11N_2SS              0
    #define CONFIG_RA_MULTI_TRY         1
    #define CONFIG_RA_NSC_CALCULATE     1
    #define CONFIG_RA_SET_RAINFO        1 
    #define CONFIG_1SS_SL_GI_MULTI_TRY   1
#else
    #define CONFIG_RA_PS_MODE           0
    #define CONFIG_CCX                  0
    #define CONFIG_POWER_TRAINING_WIL   0
    #define CONFIG_HALcom_RA            0
    #define CONFIG_HALcom_RA_2          0
    #define CONFIG_11AC_1SS             0
    #define CONFIG_11AC_2SS             0
    #define CONFIG_11N_1SS              1
    #define CONFIG_11N_2SS              0
    #define CONFIG_RA_MULTI_TRY         0
    #define CONFIG_RA_NSC_CALCULATE     0
    #define CONFIG_RA_SET_RAINFO        0  
    #define CONFIG_1SS_SL_GI_MULTI_TRY   0
#endif  //#if CONFIG_RATE_ADAPTIVE

    #define CONFIG_11AC 			(CONFIG_11AC_1SS | CONFIG_11AC_2SS)
    #define CONFIG_11N 				(CONFIG_11N_1SS | CONFIG_11N_2SS)
    
    #define CONFIG_1SS 				(CONFIG_11N_1SS | CONFIG_11AC_1SS)
    #define CONFIG_2SS 				(CONFIG_11N_2SS | CONFIG_11AC_2SS)
    
#define CONFIG_IQK_OFFLOAD              0


//3 AP offload      //Alan
#define CONFIG_AP_OFFLOAD               0
#if CONFIG_AP_OFFLOAD
#define CONFIG_MIFI_PLATFORM        0
#else
#define CONFIG_MIFI_PLATFORM        0
#endif
#define CONFIG_AP_TXBCNERR_PATCH        0

#define CONFIG_AP_PS_OFFLOAD                    0
#define CONFIG_STA_AGING_FUNC                   0
#define CONFIG_HW_DETECT_PS_OFFLOAD             0

#endif //#if IS_CATEGORY_WOWLAN(CONFIG_CATEGORY_SEL)



//2 4.) CATEGORY : NIC_IoT

#if IS_CATEGORY_NIC_IoT(CONFIG_CATEGORY_SEL)
//3 General
#define CONFIG_DISCONNECT_DECISION      1
#define CONFIG_KEEP_ALIVE               1
#define CONFIG_FAST_CHANNEL_SWITCH      0
#if CONFIG_FAST_CHANNEL_SWITCH
#define CONFIG_FCS_LOGO                 1
#define CONFIG_MCC                      0
#define CONFIG_NOA_ISR                  0 //use NOA isr replace GTimer, no work 0717...
#define CONFIG_PS_TIMER                 0
#else
#define CONFIG_FCS_LOGO                 0
#define CONFIG_MCC                      0
#define CONFIG_NOA_ISR                  0
#define CONFIG_PS_TIMER                 0
#endif //#if CONFIG_FAST_CHANNEL_SWITCH

#define CONFIG_D0_SCAN_OFFLOAD          1
#define CONFIG_SWITCH_CAHNNEL_OFFLOAD   0
#define CONFIG_SINGLE_SWITCH_CHANNEL    1
#define CONFIG_ECSA                     0

#if (CONFIG_FAST_CHANNEL_SWITCH || CONFIG_D0_SCAN_OFFLOAD || CONFIG_SWITCH_CAHNNEL_OFFLOAD)
#define CONFIG_RF_SETTING               1
#else
#define CONFIG_RF_SETTING               0
#endif  //#if (CONFIG_FAST_CHANNEL_SWITCH || CONFIG_D0_SCAN_OFFLOAD || CONFIG_SWITCH_CAHNNEL_OFFLOAD)

#define CONFIG_SW_TIMER                 1
#define CONFIG_EFUSE_PART_NUM           0
#define CONFIG_TXBEACON_IGNORE_EDCCA	0
#define CONFIG_TX_PAUSE_DRV_INFO		1
#define CONFIG_MACID_PAUSE_DRV_INFO		0
#define CONFIG_TX_PAUSE_WO_MASK         0


//3 PS Related
#define CONFIG_SAPPS                    1
#define CONFIG_NOLINK_PS                0//must enable in formal FW
#define CONFIG_INACTIVE_PS              1//must enable in formal FW //for RTD3
#define CONFIG_POWERSAVING              1   //must enable in formal FW
#if CONFIG_POWERSAVING
#define CONFIG_HW_PS_CTRL               1
#define CONFIG_PS_SUPPORT_32K           1
#define CONFIG_PARTIAL_OFF              1
#define CONFIG_LOWPWR_RXMODE            0   //It is not supported by 88E
#define CONFIG_WMMPS                    0
#define CONFIG_P2P_PS                   1
#define CONFIG_FAST_CPWM                1
#define CONFIG_TASK_IDLE_CHK            1
#define CONFIG_BCNEARLY_ADJUST          0       //must not both enable!!ben, 170206
#define CONFIG_BCNEARLY_ADJUST_V1       0	//must disable while CONFIG_NAN enable!!ben, 170206
#define CONFIG_BCNEARLY_ADJUST_V2       1   //david_liu,181016
#define CONFIG_LOWPWR_BEACON_MODE       0
#define CONFIG_S1_NULL1_IVL_PATCH      0
#define CONFIG_DLPS                     1
#else
#define CONFIG_HW_PS_CTRL               0
#define CONFIG_PS_SUPPORT_32K           0
#define CONFIG_PARTIAL_OFF              0
#define CONFIG_LOWPWR_RXMODE            0   //It is not supported by 88E
#define CONFIG_WMMPS                    0
#define CONFIG_P2P_PS                   0
#define CONFIG_FAST_CPWM                0
#define CONFIG_TASK_IDLE_CHK            0
#define CONFIG_BCNEARLY_ADJUST          0       //must not both enable!!ben, 170206
#define CONFIG_BCNEARLY_ADJUST_V1       0	//must disable while CONFIG_NAN enable!!ben, 170206
#define CONFIG_BCNEARLY_ADJUST_V2       0   //david_liu,181016
#define CONFIG_LOWPWR_BEACON_MODE       0
#define CONFIG_S1_NULL1_IVL_PATCH      0
#define CONFIG_DLPS                     0
#endif

//3NAN Related
#define CONFIG_NAN                      0

//3  LTE Coex Related
#define CONFIG_LTECOEX                  0    //alan, 2014_0207

//3 BT Coex Related
#define CONFIG_BTCOEX_CTRL              0
#define CONFIG_BT_MAILBOX               1//must enable in formal FW
#define CONFIG_SW_MAILBOX               0
#define CONFIG_BT_MP_Mode               0    // alan , 2013_11_27

#define CONFIG_CP_AFE_LBK_TEST       0

//3 Remote WakeUp
#define CONFIG_REMOTE_WAKEUP            1
#if CONFIG_REMOTE_WAKEUP
    #define CONFIG_WoWLAN               1
    #define CONFIG_AES_GTK              1   
    #define CONFIG_ARP                  1   
    #define CONFIG_NDP                  1 
    #define CONFIG_TEREDO_REALWOWL      0
    #define CONFIG_REALWOW_V2           1
    #define CONFIG_RTD3_SCAN_OFFLOAD    1    
    #define CONFIG_NLO_COMPLEX_OFFLOAD  0    
    #define CONFIG_NLO_SIMPLE_OFFLOAD   0        
    #define CODE_REDUCE                 1
    #define CONFIG_WOWLAN_APL_QQ        1 
    #define CONFIG_ANT_SWT              1
    #define CONFIG_CUSTOMIZATION_GPIO   0	 
    #define CONFIG_P2P_OFFLOAD	        0
    #define CONFIG_SIDEBAND_WoWLAN      0
    #define CONFIG_RUNTIME_PM_OFFLOAD   0
    #define CONFIG_MULTI_FUNCTION       0
    #define CONFIG_DELAY_WAKEUP       1     //ben, for release, 170302
    #define CONFIG_WPA2_ENTERPRISE              0
    #define CONFIG_NEGATIVE_PATTERN     0
    #define CONFIG_ALL_DATA_WAKEUP      0
#else
    #define CONFIG_WoWLAN               0
    #define CONFIG_AES_GTK              0   
    #define CONFIG_ARP                  0  
    #define CONFIG_NDP                  0 
    #define CONFIG_TEREDO_REALWOWL      0
    #define CONFIG_REALWOW_V2           0
    #define CONFIG_RTD3_SCAN_OFFLOAD    0    
    #define CONFIG_NLO_COMPLEX_OFFLOAD  0        
    #define CONFIG_NLO_SIMPLE_OFFLOAD   0    
    #define CODE_REDUCE                 0
    #define CONFIG_WOWLAN_APL_QQ        0 
    #define CONFIG_ANT_SWT              0
    #define CONFIG_CUSTOMIZATION_GPIO   0	 
    #define CONFIG_P2P_OFFLOAD	        0	
    #define CONFIG_SIDEBAND_WoWLAN      0
    #define CONFIG_RUNTIME_PM_OFFLOAD   0
    #define CONFIG_MULTI_FUNCTION       0
    #define CONFIG_DELAY_WAKEUP       0
    #define CONFIG_WPA2_ENTERPRISE              0
    #define CONFIG_NEGATIVE_PATTERN     0
    #define CONFIG_ALL_DATA_WAKEUP      0
#endif


//3 Dynamic Mechanism Offload
#define CONFIG_RATE_ADAPTIVE            1//must enable in formal FW
#if CONFIG_RATE_ADAPTIVE
    #define CONFIG_RA_PS_MODE           0
    #define CONFIG_CCX                  1
    #define CONFIG_POWER_TRAINING_WIL   1
    #define CONFIG_HALcom_RA            0
    #define CONFIG_HALcom_RA_2          0
    #define CONFIG_11AC_1SS             0
    #define CONFIG_11AC_2SS             0
    #define CONFIG_11N_1SS				1
    #define CONFIG_11N_2SS				0
    #define CONFIG_RA_MULTI_TRY         1
    #define CONFIG_RA_NSC_CALCULATE     1
    #define CONFIG_RA_SET_RAINFO        1  
    #define CONFIG_1SS_SL_GI_MULTI_TRY   1
#else
    #define CONFIG_RA_PS_MODE           0
    #define CONFIG_CCX                  0
    #define CONFIG_POWER_TRAINING_WIL   0
    #define CONFIG_HALcom_RA            0
    #define CONFIG_HALcom_RA_2          0
    #define CONFIG_11AC_1SS             0
    #define CONFIG_11AC_2SS             0
    #define CONFIG_11N_1SS				1
    #define CONFIG_11N_2SS				0
    #define CONFIG_RA_MULTI_TRY         0
    #define CONFIG_RA_NSC_CALCULATE     0
    #define CONFIG_RA_SET_RAINFO        0 
    #define CONFIG_1SS_SL_GI_MULTI_TRY   0
#endif  //#if CONFIG_RATE_ADAPTIVE

    #define CONFIG_11AC 			(CONFIG_11AC_1SS | CONFIG_11AC_2SS)
    #define CONFIG_11N 				(CONFIG_11N_1SS | CONFIG_11N_2SS)
    
    #define CONFIG_1SS 				(CONFIG_11N_1SS | CONFIG_11AC_1SS)
    #define CONFIG_2SS 				(CONFIG_11N_2SS | CONFIG_11AC_2SS)

#define CONFIG_IQK_OFFLOAD              0


//3 AP offload      //Alan
#define CONFIG_AP_OFFLOAD               0
#if CONFIG_AP_OFFLOAD
#define CONFIG_MIFI_PLATFORM        1
#else
#define CONFIG_MIFI_PLATFORM        0
#endif
#define CONFIG_AP_TXBCNERR_PATCH        0

#define CONFIG_AP_PS_OFFLOAD                    0
#define CONFIG_STA_AGING_FUNC                   0
#define CONFIG_HW_DETECT_PS_OFFLOAD             0

#endif  //#if IS_CATEGORY_NIC_IoT(CONFIG_CATEGORY_SEL)


//1  Patch Control
#define CONFIG_FWLBK_PATCH              0   //8188e fwlbk patch 
#define CONFIG_FWLBK_VHT_PATCH          0   //8812 testchip:patch for afelbk, this flag can't be enabled in normal release fw
#define CONFIG_TSF_RESET_PATCH          0
#define CONFIG_INITOFLD_PATCH_8812      0
#define CONFIG_IPSEC_ENGINE_RAM         0
#define CONFIG_H2CSWQUEUE               1
#define CONFIG_ADC_MASK_HANG_8812A      0

//1  Debug control
#define CONFIG_PC_DEBUG                 1
#define CONFIG_REG_DEBUG                1
#define CONFIG_PS_DEBUG                 1
#define CONFIG_WOW_DEBUG                0
#define CONFIG_RA_DEBUG                 0
#define CONFIG_RA_FPGA_DEBUG            0
#define CONFIG_DBGSTEP                  0   // 1
#define CONFIG_RA_TP_DEBUG              0   // 1   //verify with test program
#define CONFIG_MAILBOX_DEBUG            0
#define CONFIG_32K_DEBUG                1

/******************************************

    Debug Configuration                                              

*******************************************/

//3 Debug Mode Select: Below Flags must be clear to 0 before commit.
#define CONFIG_C2H_DEBUG                0
#define CONFIG_C2H_DEBUG_COMP           0
#define CONFIG_C2H_RA_DEBUG_CODE          1
#define CONFIG_UART_DEBUG               0
#if CONFIG_UART_DEBUG
#define CONFIG_UART_POLLING         1
#endif
#define CONFIG_SNIFFER_DEBUG            0

// RT_TRACE according to this compile flag, not yet add UART
#define CONFIG_DBGMODE                  ( (CONFIG_C2H_DEBUG&CONFIG_C2H_PKT) || CONFIG_SNIFFER_DEBUG || CONFIG_UART_DEBUG)

#if CONFIG_DBGMODE
#define CONFIG_C2HPRT_WOWLAN            0
#define CONFIG_C2HPRT_FCS               0
#define CONFIG_C2HPRT_POWERSAVEING      0

// ===> AP mode  debug­nª`·N!!! macid number = 16
#undef  MACID_NUM                   
#define MACID_NUM                       16

#else
#define CONFIG_C2HPRT_WOWLAN            0
#define CONFIG_C2HPRT_FCS               0
#define CONFIG_C2HPRT_POWERSAVEING      0

#endif  //#if CONFIG_DBGMODE



//3  DebugTool: Register Dump 


#define CONFIG_PRINT_DEBUG              1
#if CONFIG_PRINT_DEBUG
#define CONFIG_DEBUGPRT_COMMON          0
#define CONFIG_DEBUGPRT_RA              (CONFIG_PRINT_DEBUG & CONFIG_RATE_ADAPTIVE)
#define CONFIG_DEBUGPRT_CCX             0//(CONFIG_PRINT_DEBUG & CONFIG_CCX)    
#define CONFIG_DEBUGPRT_PS              0//(CONFIG_PRINT_DEBUG & CONFIG_POWERSAVING)
#define CONFIG_DEBUGPRT_BT              0//(CONFIG_PRINT_DEBUG & CONFIG_BT_MAILBOX)
#define CONFIG_DEBUGPRT_IQK             0//(CONFIG_PRINT_DEBUG & CONFIG_IQK_OFFLOAD)
#define CONFIG_DEBUGPRT_WOWLAN          0//(CONFIG_PRINT_DEBUG & CONFIG_REMOTE_WAKEUP)
#define CONFIG_DEBUGPRT_FCS             0//(CONFIG_PRINT_DEBUG & CONFIG_FAST_CHANNEL_SWITCH)
#else
#define CONFIG_DEBUGPRT_COMMON          0
#define CONFIG_DEBUGPRT_RA              0
#define CONFIG_DEBUGPRT_CCX             0
#define CONFIG_DEBUGPRT_PS              0
#define CONFIG_DEBUGPRT_BT              0
#define CONFIG_DEBUGPRT_IQK             0
#define CONFIG_DEBUGPRT_WOWLAN          0
#define CONFIG_DEBUGPRT_FCS             0

#endif


/******************************************

    Test & verification Configuration                                              

*******************************************/


//1 Test & verification control
#if CONFIG_VERIFICATION_MODE   
#define CONFIG_8051_IOPATH_RESET_TESTING        1   //mantis number: 9700
#define CONFIG_FULL_RANGE_MEMORY_TEST           0   //full range RAM and xdata memoey test
#define CONFIG_IO_DEBUG                         0
#define CONFIG_VERA_SIMULATION                  0   // 1:speed up vera pattern, don't initialize xdata/pdata
#define CONFIG_AFE_LOOPBACK                     0       
#define CONFIG_TRX_BUFF_RW                      0
#define CONFIG_H2C2H_LB                         0
#define CONFIG_RPWM_CPWM_LB                     0
#define CONFIG_TXBUFF_ROM_EN                    0 
#define CONFIG_ROM_VERIFICATION                 0   // 1: add test in ROM code (main_test())
#define CONFIG_TXPKTBUFTEST                     0	// Alan
#define CONFIG_IPSEC_ENGINE_RAM                 0
#define CONFIG_GTK_AES_128                      0
#define CONFIG_802_11W                          0
#define CONFIG_GTK_11W_HW					    0
#define CONFIG_GTK_VERIFICATION                 0
#if CONFIG_GTK_VERIFICATION    
    #define CONFIG_IPSEC_ENGINE_NEW             1
#if CONFIG_IPSEC_ENGINE_NEW    
    #define CONFIG_GTK_AES_VERIFICATION         1
    #define CONFIG_GTK_TKIP_VERIFICATION        0
	#define CONFIG_GTK_11W_VERIFICATION         1
    #define CONFIG_GTK_AES_128_VERIFICATION	    1
#else
    #define CONFIG_GTK_AES_VERIFICATION         1
    #define CONFIG_GTK_TKIP_VERIFICATION        0
    #define CONFIG_GTK_11W_VERIFICATION         0
	#define CONFIG_GTK_AES_128_VERIFICATION     0
#endif  //#if CONFIG_IPSEC_ENGINE_NEW  
#endif  //#if CONFIG_GTK_VERIFICATION  



#else
#define CONFIG_8051_IOPATH_RESET_TESTING        0
#define CONFIG_FULL_RANGE_MEMORY_TEST           0
#define CONFIG_IO_DEBUG                         0
#if CONFIG_CP_AFE_LBK_TEST
#define CONFIG_VERA_SIMULATION                  1       //ben, 151013
#else
#define CONFIG_VERA_SIMULATION                  0       //ben, 151013
#endif
#define CONFIG_AFE_LOOPBACK                     0       
#define CONFIG_TRX_BUFF_RW                      0
#define CONFIG_H2C2H_LB                         0
#define CONFIG_RPWM_CPWM_LB                     0
#define CONFIG_TXBUFF_ROM_EN                    0 
#define CONFIG_ROM_VERIFICATION                 0
#define CONFIG_TXPKTBUFTEST                     0	// Alan
#define CONFIG_IPSEC_ENGINE_RAM                 0
#define CONFIG_GTK_AES_128                      0
#define CONFIG_802_11W                          0
#define CONFIG_GTK_11W_HW					    0
#define CONFIG_GTK_VERIFICATION                 0
#if CONFIG_GTK_VERIFICATION    
    #define CONFIG_IPSEC_ENGINE_NEW             0
#if CONFIG_IPSEC_ENGINE_NEW    
    #define CONFIG_GTK_AES_VERIFICATION         0
    #define CONFIG_GTK_TKIP_VERIFICATION        0
	#define CONFIG_GTK_11W_VERIFICATION         0
    #define CONFIG_GTK_AES_128_VERIFICATION	    0
#else
    #define CONFIG_GTK_AES_VERIFICATION         0
    #define CONFIG_GTK_TKIP_VERIFICATION        0
    #define CONFIG_GTK_11W_VERIFICATION         0
	#define CONFIG_GTK_AES_128_VERIFICATION     0
#endif  //#if CONFIG_IPSEC_ENGINE_NEW  
#endif  //#if CONFIG_GTK_VERIFICATION  

#endif  //#if CONFIG_VERIFICATION_MODE

/******************************************

    RAM Code Configuration                                              

*******************************************/

#endif  //#ifndef _AUTOCONF_H_

