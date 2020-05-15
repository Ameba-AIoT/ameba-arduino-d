#ifndef __HALCOM_IQK_RAM_H__
#define __HALCOM_IQK_RAM_H__

/*--------------------Define --------------------------------------------*/
#define cal_num                     3
#define MAC_REG_NUM                 3
#define BB_REG_NUM                  7
#define AFE_REG_NUM                 6
#define AFE1_REG_NUM                15
#define RF_REG_NUM                  3
#define TARGET_CHNL_NUM_2G_5G       38      
#define IQK_CAL_TX                  0
#define IQK_CAL_RX                  1
#define IQK_CH_NUM                  7

//BwBandInfo
#define BANDWIDTH_20M               BIT0
#define BANDWIDTH_40M               BIT1
#define BANDWIDTH_80M               BIT2
#define BANDWIDTH_160M              BIT3
#define BANDTYPE_2G                 BIT4
#define BANDTYPE_5G                 BIT5

//RFInfo
#define EXTPA5G                     BIT0
#define EXTPA2G                     BIT1
#define INTERFACE                 	(BIT4|BIT3|BIT2)		//ODM_ITRF_PCIE =	0x1, ODM_ITRF_USB	= 0x2, ODM_ITRF_SDIO =	0x4, ODM_ITRF_ALL = 0x7,
#define RFETYPE                   	(BIT7|BIT6|BIT5)

#define DIR_TX                      0
#define DIR_RX                      1

//#define IQK_Matrix_REG_NUM	        6

/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/
typedef struct _IQK_OFFLOAD_PARM_ {
    //Info
    u8          BwBandInfo;
    u8          ExtPALNA;

    //MAC
    u8          MACReg522;
    u8          MACReg550;
    u8          MACReg551;
    //BB
    u32         BBReg808;
    u32         BBReg82c;
    u32         BBReg838;
    u32         BBReg8c4;
    u32         BBReg90c;
    u32         BBRegb00;    
    u32         AFEReg00[2];
    //AFE
    u32         AFEReg5c[2];
    u32         AFEReg60[2];
    u32         AFEReg64[2];
    u32         AFEReg68[2];
    u32         AFEReg6c[2];
    u32         AFEReg70[2];
    u32         AFEReg74[2];
    u32         AFEReg78[2];
    u32         AFEReg7c[2];
    u32         AFEReg80[2];
    u32         AFEReg84[2];
    u32         AFERegb8[2];
    u32         AFERegb0[2];
    u32         AFERegb4[2];
    //RF
    u32         RFReg65[2];
    u32         RFReg8f[2];
    u32         RFReg00[2];    
}IQK_OFFLOAD_PARM, *PIQK_OFFLOAD_PARM;
/*
typedef struct _IQK_MATRIX_REGS_SETTING{

    u16	        Value[7][2][2];//[reload_idx][path][Tx,Rx]
    u32	        ValueVDF[7][2];
    //2.4G 20M, 
    //2.4G 40M, 
    //5G Band1 80M, 
    //5G Band2 80M, 
    //5G Band3 up 80M, 
    //5G Band3 down 80M, 
    //5G Band4 80M, 

}IQK_MATRIX_REGS_SETTING,*PIQK_MATRIX_REGS_SETTING;
*/
/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/
extern void 
_IQK_RX_FillIQC(
    RF90_RADIO_PATH_E    Path,
    u32                  RX_X,
    u32                  RX_Y
) ;


extern void 
_IQK_TX_FillIQC(
    RF90_RADIO_PATH_E    Path,
    u32                  TX_X,
    u32                  TX_Y
) ;

extern void
C2H_DBG_MSG(
    u8*  dbgmsg
);

extern void    
PHY_IQCalibrate(
    void
);

extern void
C2H_IQK_Offload(
    u8  step
);

extern void
H2CHDL_IQK_Offload(
    u8  *pbuf
);

#endif  // #ifndef __HALCOM_IQK_RAM_H__                             

