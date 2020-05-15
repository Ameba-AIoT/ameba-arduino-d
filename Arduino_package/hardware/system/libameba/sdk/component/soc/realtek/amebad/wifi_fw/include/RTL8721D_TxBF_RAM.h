#ifndef __HALCOM_TxBF_RAM_H__
#define __HALCOM_TxBF_RAM_H__

#define ndpa 0x54

typedef struct _TXBF_PARM_{

    u8      NDPA0; //NDPA1; //head page location
    u16     period0;    //period1;  //sounding period
    u8      seqN;   //sounding sequence number
    u8      SND_BW0:2; //sounding BW
    u8      rsvd:6;
    u16     NDPA_cnt;   //total number of NDPA bening sent
    //u16     SND_ok;
    BOOLEAN      SND_START; // 1: start, 0: end

}TXBF_PARM, *PTXBF_PARM;


extern void
InitSounding(
void
);

extern void
PeriodicSounding(
void
);


extern void 
H2CHDL_TxBF(
    u8 *pbuf
) ;

extern void
C2H_SoundDone(
void
) ;





#endif  //#ifndef   __HALCOM_TxBF_RAM_H__

