#ifndef _RTLCHIP_H_
#define _RTLCHIP_H_


/*
*****************************************

        Chip Selection
        
Field    FOUNDRY     RTLCHIP               CHIPCUT
BITS   [17:16]         [15:4]                 [3:0]


*****************************************
*/

//Chip
#define RTL8188C_CHIP               0x88C0
#define RTL8192C_CHIP               0x92C0
#define RTL8723_CHIP                0x2300
#define RTL8188E_CHIP               0x88E0
#define RTLWISOC_CHIP               0x1500
#define RTL8195_CHIP                0x9500
#define RTL8821_CHIP                0x2100
#define RTL8881_CHIP                0x8100
#define RTL8192E_CHIP               0x92E0
#define RTL8723B_CHIP               0x5300
#define RTL8703B_CHIP               0x03B0
#define RTL8188F_CHIP               0x88F0
#define RTL8723D_CHIP               0x23D0
#define RTL8710B_CHIP               0x10B0
#define RTL8192F_CHIP               0x92F0
#define RTL8721D_CHIP               0x21D0


#define RTLCHIP_MASK                0xFFF0


//Cut
#define CHIPCUT_TEST                0x0
#define CHIPCUT_A                   0x1
#define CHIPCUT_B                   0x2
#define CHIPCUT_C                   0x3
#define CHIPCUT_D                   0x4
#define CHIPCUT_E                   0x5

#define CHIPCUT_MASK                0xF

//Chip + Cut (Signature)
#define CHIP_SIGNATURE_MASK         0xFFFF

//Foundry
#define CHIP_FOUNDRY_SHIFT          16
#define CHIP_FOUNDRY_MASK           (0x3<<CHIP_FOUNDRY_SHIFT)

#define CHIP_FOUNDRY_TSMC           (0x0<<CHIP_FOUNDRY_SHIFT)
#define CHIP_FOUNDRY_UMC            (0x1<<CHIP_FOUNDRY_SHIFT)
#define CHIP_FOUNDRY_SMIC           (0x2<<CHIP_FOUNDRY_SHIFT)


//Macro for Chip
#define IS_CHIP_8188E(CurSel)       ( ((CurSel) & RTLCHIP_MASK) == RTL8188E_CHIP )
#define IS_CHIP_WISOC(CurSel)       ( ((CurSel) & RTLCHIP_MASK) == RTLWISOC_CHIP )
#define IS_CHIP_8195(CurSel)        ( ((CurSel) & RTLCHIP_MASK) == RTL8195_CHIP )
#define IS_CHIP_8821(CurSel)        ( ((CurSel) & RTLCHIP_MASK) == RTL8821_CHIP )
#define IS_CHIP_8881(CurSel)        ( ((CurSel) & RTLCHIP_MASK) == RTL8881_CHIP )
#define IS_CHIP_8192E(CurSel)        ( ((CurSel) & RTLCHIP_MASK) == RTL8192E_CHIP )
#define IS_CHIP_8723B(CurSel)        ( ((CurSel) & RTLCHIP_MASK) == RTL8723B_CHIP )
#define IS_CHIP_8703B(CurSel)       ( ((CurSel) & RTLCHIP_MASK) == RTL8703B_CHIP )
#define IS_CHIP_8188F(CurSel)       ( ((CurSel) & RTLCHIP_MASK) == RTL8188F_CHIP )
#define IS_CHIP_8723D(CurSel)       ( ((CurSel) & RTLCHIP_MASK) == RTL8723D_CHIP )
#define IS_CHIP_8710B(CurSel)       ( ((CurSel) & RTLCHIP_MASK) == RTL8710B_CHIP )
#define IS_CHIP_8192F(CurSel)       ( ((CurSel) & RTLCHIP_MASK) == RTL8192F_CHIP )
#define IS_CHIP_8721D(CurSel)       ( ((CurSel) & RTLCHIP_MASK) == RTL8721D_CHIP )

#define IS_CHIP_RF_N(CurSel)        (( ((CurSel) & RTLCHIP_MASK) == RTL8188E_CHIP ) \
                                    || ( ((CurSel) & RTLCHIP_MASK) == RTLWISOC_CHIP ) \
                                    || ( ((CurSel) & RTLCHIP_MASK) == RTL8192E_CHIP ) \
                                    || ( ((CurSel) & RTLCHIP_MASK) == RTL8723B_CHIP ) \
                                    || ( ((CurSel) & RTLCHIP_MASK) == RTL8703B_CHIP ) \
                                    || ( ((CurSel) & RTLCHIP_MASK) == RTL8188F_CHIP )\
                                    || ( ((CurSel) & RTLCHIP_MASK) == RTL8710B_CHIP )\
                                    || ( ((CurSel) & RTLCHIP_MASK) == RTL8192F_CHIP )\
                                    || ( ((CurSel) & RTLCHIP_MASK) == RTL8721D_CHIP )\
                                    || ( ((CurSel) & RTLCHIP_MASK) == RTL8723D_CHIP ))

#define IS_CHIP_RF_AC(CurSel)       (( ((CurSel) & RTLCHIP_MASK) == RTL8195_CHIP )\
                                    || ( ((CurSel) & RTLCHIP_MASK) == RTL8821_CHIP ) \
                                    || ( ((CurSel) & RTLCHIP_MASK) == RTL8881_CHIP ) )


#define GET_CHIP_ID(CurSel)         ( (CurSel) & RTLCHIP_MASK )

//Macro for Cut
#define IS_CUT_TEST(CurSel)         ( ((CurSel) & CHIPCUT_MASK) == CHIPCUT_TEST )
#define IS_CUT_A(CurSel)            ( ((CurSel) & CHIPCUT_MASK) == CHIPCUT_A )
#define IS_CUT_B(CurSel)            ( ((CurSel) & CHIPCUT_MASK) == CHIPCUT_B )
#define IS_CUT_C(CurSel)            ( ((CurSel) & CHIPCUT_MASK) == CHIPCUT_C )
#define IS_CUT_D(CurSel)            ( ((CurSel) & CHIPCUT_MASK) == CHIPCUT_D )
#define GET_CUT(CurSel)             ((CurSel) & CHIPCUT_MASK)

//Macro for Chip + Cut
#define GET_SIGNATURE(CurSel)       ((CurSel) & CHIP_SIGNATURE_MASK)

//Macro for Foundry
#define GET_FOUNDRY(CurSel)         ((CurSel) & CHIP_FOUNDRY_MASK)


//3 Final Combination (Chip Select)
//3 1.) RTL8188E
#define RTL8188E_TEST_CHIP_T            (RTL8188E_CHIP | CHIPCUT_TEST | CHIP_FOUNDRY_TSMC)
#define RTL8188E_A_CUT_T                (RTL8188E_CHIP | CHIPCUT_A    | CHIP_FOUNDRY_TSMC)
#define RTL8188E_B_CUT_S                (RTL8188E_CHIP | CHIPCUT_B    | CHIP_FOUNDRY_SMIC)
#define RTL8188E_C_CUT_S                (RTL8188E_CHIP | CHIPCUT_C    | CHIP_FOUNDRY_SMIC)

//3 2.) RTL8812
#define RTL8195_TEST_CHIP_T             (RTL8195_CHIP  | CHIPCUT_TEST | CHIP_FOUNDRY_TSMC)
#define RTL8195_A_CUT_T                 (RTL8195_CHIP  | CHIPCUT_A    | CHIP_FOUNDRY_TSMC)

//3 3.) RTL8821
#define RTL8821_TEST_CHIP_T             (RTL8821_CHIP  | CHIPCUT_TEST | CHIP_FOUNDRY_TSMC)
#define RTL8821_A_CUT_T                 (RTL8821_CHIP  | CHIPCUT_A    | CHIP_FOUNDRY_TSMC)

//3 4.) RTL8881
#define RTL8881_TEST_CHIP_T             (RTL8881_CHIP  | CHIPCUT_TEST | CHIP_FOUNDRY_TSMC)
#define RTL8881_A_CUT_T                 (RTL8881_CHIP  | CHIPCUT_A    | CHIP_FOUNDRY_TSMC)

//3 5.) RTL8192E
#define RTL8192E_TEST_CHIP_T             (RTL8192E_CHIP  | CHIPCUT_TEST | CHIP_FOUNDRY_TSMC)
#define RTL8192E_A_CUT_T                 (RTL8192E_CHIP  | CHIPCUT_A    | CHIP_FOUNDRY_TSMC)



//3 6.) RTL8723B
#define RTL8723B_TEST_CHIP_T             (RTL8723B_CHIP  | CHIPCUT_TEST | CHIP_FOUNDRY_TSMC)
#define RTL8723B_A_CUT_T                 (RTL8723B_CHIP  | CHIPCUT_A    | CHIP_FOUNDRY_TSMC)
//#define RTL8723B_B_CUT_T                 (RTL8723B_CHIP  | CHIPCUT_B    | CHIP_FOUNDRY_TSMC)//8703AS

//3 7.) RTL8703B
#define RTL8703B_TEST_CHIP_S             (RTL8703B_CHIP  | CHIPCUT_TEST | CHIP_FOUNDRY_SMIC)
#define RTL8703B_A_CUT_S                 (RTL8703B_CHIP  | CHIPCUT_A    | CHIP_FOUNDRY_SMIC)
#define RTL8703B_B_CUT_S                 (RTL8703B_CHIP  | CHIPCUT_B    | CHIP_FOUNDRY_SMIC)


//3 8.) RTL8188F
#define RTL8188F_TEST_CHIP_S             (RTL8188F_CHIP  | CHIPCUT_TEST | CHIP_FOUNDRY_SMIC)
#define RTL8188F_A_CUT_S                 (RTL8188F_CHIP  | CHIPCUT_A    | CHIP_FOUNDRY_SMIC)
#define RTL8188F_B_CUT_S                 (RTL8188F_CHIP  | CHIPCUT_B    | CHIP_FOUNDRY_SMIC)

//3 9.) RTL8723D
#define RTL8723D_TEST_CHIP_S             (RTL8723D_CHIP  | CHIPCUT_TEST | CHIP_FOUNDRY_SMIC)
#define RTL8723D_A_CUT_S                 (RTL8723D_CHIP  | CHIPCUT_A    | CHIP_FOUNDRY_SMIC)
#define RTL8723D_B_CUT_S                 (RTL8723D_CHIP  | CHIPCUT_B    | CHIP_FOUNDRY_SMIC)

//3 10.) RTL8710B
#define RTL8710B_TEST_CHIP_S             (RTL8710B_CHIP  | CHIPCUT_TEST | CHIP_FOUNDRY_SMIC)
#define RTL8710B_A_CUT_S                 (RTL8710B_CHIP  | CHIPCUT_A    | CHIP_FOUNDRY_SMIC)
#define RTL8710B_B_CUT_U                 (RTL8710B_CHIP  | CHIPCUT_B    | CHIP_FOUNDRY_UMC)
//3 10.) RTL8192F
#define RTL8192F_TEST_CHIP_S             (RTL8192F_CHIP  | CHIPCUT_TEST | CHIP_FOUNDRY_SMIC)
#define RTL8192F_A_CUT_S                 (RTL8192F_CHIP  | CHIPCUT_A    | CHIP_FOUNDRY_SMIC)
#define RTL8192F_B_CUT_S                 (RTL8192F_CHIP  | CHIPCUT_B    | CHIP_FOUNDRY_SMIC)
//3 10.) RTL8721D
#define RTL8721D_TEST_CHIP_S             (RTL8721D_CHIP  | CHIPCUT_TEST | CHIP_FOUNDRY_SMIC)
#define RTL8721D_A_CUT_S                 (RTL8721D_CHIP  | CHIPCUT_A    | CHIP_FOUNDRY_SMIC)
#define RTL8721D_B_CUT_S                 (RTL8721D_CHIP  | CHIPCUT_B    | CHIP_FOUNDRY_SMIC)

/*
*****************************************

        Category Selection
        
Field   NET_TYPE           HCI_TYPE           
BITS   [7:4]                 [3:0]


*****************************************
*/

// NET_TYPE
// NIC / AP / GENERAL
#define RTLCHIP_NET_TYPE_SHIFT      4
#define RTLCHIP_NET_TYPE_MASK       (0x0F<<RTLCHIP_NET_TYPE_SHIFT)


#define RTLCHIP_NET_TYPE_GENERAL    (0x00<<RTLCHIP_NET_TYPE_SHIFT)
#define RTLCHIP_NET_TYPE_NIC        (0x01<<RTLCHIP_NET_TYPE_SHIFT)
#define RTLCHIP_NET_TYPE_AP         (0x02<<RTLCHIP_NET_TYPE_SHIFT)
#define RTLCHIP_NET_TYPE_WOWLAN     (0x03<<RTLCHIP_NET_TYPE_SHIFT)
#define RTLCHIP_NET_TYPE_NIC_89EM   (0x04<<RTLCHIP_NET_TYPE_SHIFT)
#define RTLCHIP_NET_TYPE_NIC_MCC    (0x05<<RTLCHIP_NET_TYPE_SHIFT)
#define RTLCHIP_NET_TYPE_NIC_IoT    (0x06<<RTLCHIP_NET_TYPE_SHIFT)

//Macro for NetType
#define IS_TYPE_NIC(CurSel)         ( ((CurSel) & RTLCHIP_NET_TYPE_MASK) == RTLCHIP_NET_TYPE_NIC )
#define IS_TYPE_AP(CurSel)          ( ((CurSel) & RTLCHIP_NET_TYPE_MASK) == RTLCHIP_NET_TYPE_AP )
#define IS_TYPE_WOWLAN(CurSel)      ( ((CurSel) & RTLCHIP_NET_TYPE_MASK) == RTLCHIP_NET_TYPE_WOWLAN )
#define IS_TYPE_NIC_89EM(CurSel)    ( ((CurSel) & RTLCHIP_NET_TYPE_MASK) == RTLCHIP_NET_TYPE_NIC_89EM )
#define IS_TYPE_NIC_MCC(CurSel)     ( ((CurSel) & RTLCHIP_NET_TYPE_MASK) == RTLCHIP_NET_TYPE_NIC_MCC )
#define IS_TYPE_NIC_IoT(CurSel)     ( ((CurSel) & RTLCHIP_NET_TYPE_MASK) == RTLCHIP_NET_TYPE_NIC_IoT )

//HCI_Type
#define RTLCHIP_HCI_TYPE_SHIFT      0
#define RTLCHIP_HCI_TYPE_MASK       (0x0F<<RTLCHIP_HCI_TYPE_SHIFT)


#define RTLCHIP_HCI_TYPE_GENERAL    (0x00<<RTLCHIP_HCI_TYPE_SHIFT)
#define RTLCHIP_HCI_TYPE_USB        (0x01<<RTLCHIP_HCI_TYPE_SHIFT)
#define RTLCHIP_HCI_TYPE_SDIO       (0x02<<RTLCHIP_HCI_TYPE_SHIFT)
#define RTLCHIP_HCI_TYPE_PCIE       (0x03<<RTLCHIP_HCI_TYPE_SHIFT)

//Macro for HCI Type
#define IS_HCITYPE_GENERAL(CurSel)  ( ((CurSel) & RTLCHIP_HCI_TYPE_MASK) == RTLCHIP_HCI_TYPE_GENERAL)
#define IS_HCITYPE_USB(CurSel)      ( ((CurSel) & RTLCHIP_HCI_TYPE_MASK) == RTLCHIP_HCI_TYPE_USB )
#define IS_HCITYPE_SDIO(CurSel)     ( ((CurSel) & RTLCHIP_HCI_TYPE_MASK) == RTLCHIP_HCI_TYPE_SDIO )
#define IS_HCITYPE_PCIE(CurSel)     ( ((CurSel) & RTLCHIP_HCI_TYPE_MASK) == RTLCHIP_HCI_TYPE_PCIE )



//3 Final Combination (Category Select)
//Common
#define RTLCHIP_CATEGORY_GENERAL_GENERAL    (RTLCHIP_NET_TYPE_GENERAL   | RTLCHIP_HCI_TYPE_GENERAL)

//NIC
#define RTLCHIP_CATEGORY_NIC_GENERAL        (RTLCHIP_NET_TYPE_NIC       | RTLCHIP_HCI_TYPE_GENERAL)
#define RTLCHIP_CATEGORY_NIC_USB            (RTLCHIP_NET_TYPE_NIC       | RTLCHIP_HCI_TYPE_USB)
#define RTLCHIP_CATEGORY_NIC_SDIO           (RTLCHIP_NET_TYPE_NIC       | RTLCHIP_HCI_TYPE_SDIO)
#define RTLCHIP_CATEGORY_NIC_PCIE           (RTLCHIP_NET_TYPE_NIC       | RTLCHIP_HCI_TYPE_PCIE)
#define RTLCHIP_CATEGORY_NIC_WOWLAN         (RTLCHIP_NET_TYPE_WOWLAN    | RTLCHIP_HCI_TYPE_GENERAL)
#define RTLCHIP_CATEGORY_NIC_89EM           (RTLCHIP_NET_TYPE_NIC_89EM  | RTLCHIP_HCI_TYPE_GENERAL)
#define RTLCHIP_CATEGORY_NIC_MCC            (RTLCHIP_NET_TYPE_NIC_MCC   | RTLCHIP_HCI_TYPE_GENERAL)
#define RTLCHIP_CATEGORY_NIC_IoT            (RTLCHIP_NET_TYPE_NIC_IoT   | RTLCHIP_HCI_TYPE_GENERAL)

//AP
#define RTLCHIP_CATEGORY_AP_GENERAL         (RTLCHIP_NET_TYPE_AP        | RTLCHIP_HCI_TYPE_GENERAL)
#define RTLCHIP_CATEGORY_AP_USB             (RTLCHIP_NET_TYPE_AP        | RTLCHIP_HCI_TYPE_USB)
#define RTLCHIP_CATEGORY_AP_SDIO            (RTLCHIP_NET_TYPE_AP        | RTLCHIP_HCI_TYPE_SDIO)
#define RTLCHIP_CATEGORY_AP_PCIE            (RTLCHIP_NET_TYPE_AP        | RTLCHIP_HCI_TYPE_PCIE)


//Macro for Category Select
#define IS_CATEGORY_NIC(CurSel)             ( (IS_TYPE_NIC(CurSel)) && (IS_HCITYPE_GENERAL(CurSel)) )
#define IS_CATEGORY_AP(CurSel)              ( (IS_TYPE_AP(CurSel)) && (IS_HCITYPE_GENERAL(CurSel)) )
#define IS_CATEGORY_WOWLAN(CurSel)          ( (IS_TYPE_WOWLAN(CurSel)) && (IS_HCITYPE_GENERAL(CurSel)) )
#define IS_CATEGORY_NIC_PCIE(CurSel)        ( (IS_TYPE_NIC(CurSel)) && (IS_HCITYPE_PCIE(CurSel)) )
#define IS_CATEGORY_NIC_89EM(CurSel)        ( (IS_TYPE_NIC_89EM(CurSel)) && (IS_HCITYPE_GENERAL(CurSel)) )
#define IS_CATEGORY_NIC_MCC(CurSel)         ( (IS_TYPE_NIC_MCC(CurSel)) && (IS_HCITYPE_GENERAL(CurSel)) )
#define IS_CATEGORY_NIC_IoT(CurSel)         ( (IS_TYPE_NIC_IoT(CurSel)) && (IS_HCITYPE_GENERAL(CurSel)) )


//FW Header
#define FW_WITH_HDR                 1
#define FW_WITHOUT_HDR              0

#endif  //#ifndef _RTLCHIP_H_

