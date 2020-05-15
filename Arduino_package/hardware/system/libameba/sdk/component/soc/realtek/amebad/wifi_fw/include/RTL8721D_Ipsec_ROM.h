#ifndef __RTL8721D_IPSEC_ROM_H__
#define __RTL8721D_IPSEC_ROM_H__
//#include <string.h>


/*--------------------Define --------------------------------------------*/

#define GTK_POLLING_TIMEOUT             1000
#define POLLING_TIMEOUT                 1000

#define AES_UNWRAP_UNIT                 8



#define IPS_CFGADR                      0x180
#define IPS_CFGRWD                      0x184

//definition for IPS_CFGADR_
#define IPSEC_POLLONG                   0x00
#define IPSEC_DESSTRADR                 0x08
#define IPSEC_EXMSTRADR                 0x09
#define IPSEC_DES_WD00                  0x10
#define IPSEC_DES_WD01                  0x11
#define IPSEC_BUF_WD00                  0x20

//definition for CFGADR=0x00,byte3
#define IPS_POLLING_BIT                 BIT7
//byte 1
#define IPS_ENABLE                      BIT0





#define IPSEC_DESC_LENGTH               8
#define IPSEC_OPERATION_BLOCK           64

#define IPS_TYPE                        0x04

//definition for IPS_TYPE
#define IPS_TYPE_SHA                    0x00
#define IPS_TYPE_MD5                    0x01
#define IPS_TYPE_Michael                0x02
#define IPS_TYPE_RC4                    0x08
#define IPS_TYPE_AES_ENC                0x0A                //AES encrypt
#define IPS_TYPE_AES_DEC                0x0B                //AES decrypt
#define IPS_TYPE_HMAC_SHA               0x010
#define IPS_TYPE_HMAC_MD5               0x011
#define IPS_TYPE_FWRAP_AES              0x02A         //AES key wrap
#define IPS_TYPE_RWRAP_AES              0x02B         //AES key unwrap

//definition for data type selection
#define DATA_TYPE_KEY                   BIT0
#define DATA_TYPE_MESSAGE               BIT1
#define DATA_TYPE_DUMMY_MESSAGE         BIT2
#define DATA_TYPE_INITIALIZE            BIT3
#define DATA_TYPE_MESSAGE_END           BIT4

/* AES definition & structure */
#define AES_STATE_ROWS_1W 4     /* Block size: 4*4*8 = 128 bits */
#define AES_STATE_COLUMNS_1W 4
#define AES_BLOCK_SIZES_1W AES_STATE_ROWS_1W*AES_STATE_COLUMNS_1W
#define AES_KEY_ROWS_1W 4
#define AES_KEY_COLUMNS_1W 8    /*Key length: 4*{4,6,8}*8 = 128, 192, 256 bits */
#define AES_KEY128_LENGTH_1W 16
#define AES_KEY192_LENGTH_1W 24
#define AES_KEY256_LENGTH_1W 32
#define AES_CBC_IV_LENGTH_1W 16

/*--------------------Define Enum---------------------------------------*/

typedef enum _CHECK_SUM_PROTOCOL_ {
    IP_CHECK_SUM    = 0,
    TCP_CHECK_SUM   = 6,    
    UDP_CHECK_SUM   = 17,        
    ICMPV6_CHECK_SUM =58,
}CHECK_SUM_PROTOCOL,*PCHECK_SUM_PROTOCOL;

enum{
 IPSEC_OPMODE_KEY_MSG_TXBUFF_DESC_TXBUFF   = 0,
 IPSEC_OPMODE_KEY_MSG_TXBUFF_DESC_IO  = 1,
 IPSEC_OPMODE_KEY_MSG_IO_DESC_TXBUFF   = 2,
 IPSEC_OPMODE_KEY_MSG_IO_DESC_IO  = 3,
};


/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/
typedef struct _DESC_PARM_{
    u8  u1MsgLengthDesc;
    u8  u1MsgOffsetLSBDesc;
    u8  u1MsgOffsetMSBDesc;
    //u16     MsgOffset;
    u8  b6Rsvd:6;
    u8  b1Ls:1;
    u8  b1Fs:1;
    u8  u1KeyLengthDesc;
    u8  u1KeyOffsetLSBDesc;
    u8  u1KeyOffsetMSBDesc;
    //u16    KeyOffs;
    u8  u1IPSType;
}DESC_PARM, *PDESC_PARM;  

typedef struct _DATA_CTRL_PARM_{ 
    u8  b1DataTypeKey:1;    
    u8  b1DataTypeMsg:1;
    u8  b1DataTypeDummyMsg:1;
    u8  b1DataTypeInitialize:1;
    u8  b1DataTypeMsgEnd:1;
    u8  b3Rsvd:3;
}DATA_CTRL_PARM, *PDATA_CTRL_PARM;  

typedef struct _OP_MODE_PARM_{ 
    u8  b1DescType:1;    
    u8  b1MsgType:1;
    u8  b7Rsvd:6;
}OP_MODE_PARM, *POP_MODE_PARM;  

typedef struct _MSG_KEY_PARM_{ 
    u16 u2MsgOffset;
    u16 u2KeyOffset;
    u8  u1MsgLength;
    u8  u1KeyLength;
}MSG_KEY_PARM, *PMSG_KEY_PARM;


typedef struct _MICHAEL_PKT_PARM_{ 
    u8  b1QosOrNot:1;
    u8  b7rsvd:7;
    u16 u2DaOffset;
    u16 u2SaOffset;
    u16 u2QosOffset;
    u16 u2ContentOffset;
}MICHAEL_PKT_PARM, *PMICHAEL_PKT_PARM;

typedef struct _HW_PARM_ {
    u16 u2DesStradr;
    u16 u2ExmStradr;
    u8  u1IPSType;
}HW_PARM, *PHW_PARM;

typedef struct _IPSEC_FUNC_{
    DESC_PARM           DescParm;
    DATA_CTRL_PARM      DataCtrlParm;
    OP_MODE_PARM        OpModeParm;
    MSG_KEY_PARM        MsgKeyParm;
    MICHAEL_PKT_PARM    MichaelPktParm;
    HW_PARM             HwParm;
}IPSEC_FUNC, *PIPSEC_FUNC;


typedef struct {
    u8 State[AES_STATE_ROWS_1W][AES_STATE_COLUMNS_1W];
    u8 KeyWordExpansion[AES_KEY_ROWS_1W][AES_KEY_ROWS_1W*((AES_KEY256_LENGTH_1W >> 2) + 6 + 1)];
} AES_CTX_STRUC, *PAES_CTX_STRUC;



/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/
extern u32 getSumValue_8721D_ROM(u32 offset,u16 pktLen) ;
extern u16 getCheckSum_8721D_ROM(u8 protocol,u16 pktLen,u32 src_addr,u32 dest_addr,u32 input) ;

extern void IPSecDataIn_8721D_ROM(u32 *DataIn,u8 DataSel);
extern void IPSecDataOut_8721D_ROM(u32 *DataOut,u8 DataSel);
extern void IPSecFillDesrciptor_8721D_ROM(IPSEC_FUNC *IpsecInfo,u16 MsgOffsetDesc,u8 MsgLengthDesc);
extern void IPSWriteDescriptorIO_8721D_ROM(u32 *Desc);
extern void IPSecDesrciptor_8721D_ROM(IPSEC_FUNC *IpsecInfo);
extern void IPSecWriteDataIOMode_8721D_ROM(u8 *Data,u8 DataLength);
extern void IPSecGetDataIOMode_8721D_ROM(u8 *Data,u8 DataLength);
extern void IPSecSetOPMode_8721D_ROM(u8 OPMode,u8 PollingBit);
extern void IPSecEngine_8721D_ROM(IPSEC_FUNC *IpsecInfo,u8 *Msg,u8 *Key) ;
extern void IPSecSetExMemoryAddr_8721D_ROM(u16 ExMStradr);
extern void IPSecSetDescAddr_8721D_ROM(u16 DesStradr);
extern void AesUnwrapIpsec_8721D_ROM(u8 *Kek,u8 *KeyData,u8 *Gtk,u8 GtkLen);
extern void AESUnwrapSwap_8721D_ROM(u8 *AESData,u8 DataLength);
extern void HmacSha1Ipsec_8721D_ROM(u8 *Kck,u8 *Pframe,u8 *Hash,u8 HashLen);
extern void HmcMd5Ipsec_8721D_ROM(u8 *Pframe,u8 *Kck,u8 *Mic,u8 MicLen);
extern void RC4Ipsec_8721D_ROM(u8 *Ek,u8 *KeyData, u8 *GtkData,u8 GtkDataLen);
extern void TkipMicIpsec_8721D_ROM(u8 *MicKey,u8 *Message,u8 *Mic,u8 MicLen);
extern void Aes128EncIpsec_8721D_ROM(u8 *InitialKey,u8 *InputMessage,u8 *Gtk,u8 GtkLen);
extern void Aes128DecIpsec_8721D_ROM(u8 *InitialKey,u8 *InputMessage,u8 *Gtk,u8 GtkLen);
extern void FillParameterIpsec_8721D_ROM(u16 MsgOffset,u16 KeyOffset,u8 MsgLength,u8 KeyLength,u16 DesStradr);
extern void FillMichaelPacketInfo_8721D_ROM(u16 DaOffset,u16 SaOffset,u16 QosOffset,u16 ContentOffset,u8 QosOrNot);
/* AES operations */
extern void AESEncrypt128_8721D_ROM(u8 *PlainBlock,u32   PlainBlockSize,u8 *Key,u32 KeyLength,u8 *CipherBlock,u32 *CipherBlockSize);
extern void AESDecrypt128_8721D_ROM(u8 *PlainBlock,u32 PlainBlockSize,u8 *Key,u32 KeyLength,u8   *CipherBlock,u32 *CipherBlockSize);
extern void AES_KeyExpansion_1W_8721D_ROM(u8 Key[],u32 KeyLength,AES_CTX_STRUC *paes_ctx);
extern void AES_CMAC_GenerateSubKey_1W_8721D_ROM(u8  *Key,u8 KeyLength,u8 *SubKey1,u8 *SubKey2);
extern void AES_CMAC_1W_8721D_ROM(u8 *PlainText,u8 PlainTextLength,u8 *Key,u8 KeyLength,u8 *MACText,u32 *MACTextLength);
extern void AES_Encrypt_1W_8721D_ROM(u8 *PlainBlock,u32 PlainBlockSize,u8 *Key,u32 KeyLength,u8 *CipherBlock,u32 *CipherBlockSize);


#endif//#ifndef __WLAN_ENGINE_H__
