#ifndef _RTL8721D_PSD_OFFLOAD_H_
#define _RTL8721D_PSD_OFFLOAD_H_

/*--------------------Define --------------------------------------------*/

#define REPORTLENGTH 80

/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/
typedef struct _PSD_RPT_PARM_ {
    u8 Data0;
    u8 Data1;
    u8 Data2;
    u8 Data3;
    u8 Data4;
    u8 Data5;
    u8 Data6;
    u8 Data7;
    u8 Data8;
    u8 Data9;
    u8 Data10;
    u8 Data11;
    u8 Data12;
}PSD_RPT_PARM, *PPSD_RPT_PARM;

typedef struct _PSD_BACKUP_PARM_ {
    u32 CurrentChannel;
    u32 RxIQI;
    u8  InitialGain;
    u8  RxIdleLowPwr:1;
    u8  rsvd:7;
}PSD_BACKUP_PARM, *PPSD_BACKUP_PARM;

typedef struct _PSD_CTRL_PARM_ {
    u8 Enable:1;
    u8 Is40MHz:1;
    u8 rsvd:6;
    u8 InitialGainPSD;
    u8 ChIdx;
}PSD_CTRL_PARM,*PPSD_CTRL_PARM;

typedef struct _PSD_PARM_ {
    PSD_BACKUP_PARM         PSDBackUpSetting;
    PSD_CTRL_PARM           PSDCtrl;
    u8                      PSDReport[REPORTLENGTH];
    u8                      FTIdx[20];
}PSD_PARM, *PPSD_PARM;
/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/  

#if CONFIG_PSD_OFFLOAD
void
InitPSDOffload(
    void
) ;

void
H2CHDL_PSDOffload(
    u8 * pbuf
);

void
PSDProcess(
    void
) ;
#endif  //CONFIG_PSD_OFFLOAD
#endif  //_RTL8721D_PSD_OFFLOAD_H_
