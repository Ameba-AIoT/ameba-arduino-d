#ifndef __8721D_GTK_H__
#define __8721D_GTK_H__

/*--------------------------Define -------------------------------------------*/

#define GTK_DEBUG_HMAC_SHA1_VERIFY 1
#define GTK_DEBUG_AES_UNWRAP_VERIFY 2
#define GTK_DEBUG_HMAC_MD5_VERIFY 3
#define GTK_DEBUG_TKIP_MIC_VERIFY 4
#define GTK_DEBUG_RC4_VERIFY   5
#define GTK_DEBUG_AES_128_ENC_VERIFY 6
#define GTK_DEBUG_AES_128_DEC_VERIFY 7
#define GTK_DEBUG_11W_VERIFY 8
#define GTK_DEBUG_INFO 9
#define GTK_DEBUG_SET_TXBUFFER_ADDR 10
#define GTK_DEBUG_SET_MODE 11

#define GTK_DEBUG_HMAC_SHA_SUCCESS    BIT0
#define GTK_DEBUG_AES_UNWRAP_SUCCESS    BIT1
#define GTK_DEBUG_HMAC_MD5_SUCCESS    BIT2
#define GTK_DEBUG_TKIP_MIC_SUCCESS    BIT3
#define GTK_DEBUG_RC4_SUCCESS    BIT4
#define GTK_DEBUG_AES_128_ENC_SUCCESS    BIT5
#define GTK_DEBUG_AES_128_DEC_SUCCESS    BIT6
#define GTK_DEBUG_AES_CMAC_SUCCESS    BIT7

#define GTK_DEBUG_HMAC_SHA_FAIL    BIT0
#define GTK_DEBUG_AES_UNWRAP_FAIL    BIT1
#define GTK_DEBUG_HMAC_MD5_FAIL    BIT2
#define GTK_DEBUG_TKIP_MIC_FAIL    BIT3
#define GTK_DEBUG_RC4_FAIL    BIT4
#define GTK_DEBUG_AES_128_ENC_FAIL    BIT5
#define GTK_DEBUG_AES_128_DEC_FAIL    BIT6
#define GTK_DEBUG_AES_CMAC_FAIL    BIT7

#define SHA1_MAC_LEN 20

//descriptor offset
//sescriptor offset 0
#define FS_OFFSET 31
#define LS_OFFSET 30
#define MSG_LENGTH_OFFSET  0
#define MSG_OFFSET_OFFSET  8
//
#define KEY_Length_OFFSET   0
#define KEY_OFFSET_OFFSET   8
#define IPS_TYPE_OFFSET        24

#define WPA_KEY_INFO_TYPE_MASK ((unsigned short) (BIT(0) | BIT(1) | BIT(2)))
#define WPA_KEY_INFO_TYPE_HMAC_MD5_RC4 BIT(0)
#define WPA_KEY_INFO_TYPE_HMAC_SHA1_AES BIT(1)
#define WPA_KEY_INFO_KEY_TYPE BIT(3) /* 1 = Pairwise, 0 = Group key */
/* bit4..5 is used in WPA, but is reserved in IEEE 802.11i/RSN */
#define WPA_KEY_INFO_KEY_INDEX_MASK (BIT(4) | BIT(5))
#define WPA_KEY_INFO_KEY_INDEX_SHIFT 4

#define EAPOL_PROT_VER_OFFSET       0
#define EAPOL_PACK_TYPE_OFFSET      1
#define EAPOL_BODY_LEN_OFFSET       2
#define EAPOL_KEY_TYPE_OFFSET       4
#define EAPOL_KEY_INFO_OFFSET       5
#define EAPOL_KEY_LEN_OFFSET        7
#define EAPOL_REP_CNT_OFFSET        9
#define EAPOL_KEY_NONCE_OFFSET      17
#define EAPOL_KEY_IV_OFFSET         49
#define EAPOL_KEY_RSC_OFFSET        65
#define EAPOL_KEY_ID_OFFSET         73
#define EAPOL_KEY_MIC_OFFSET        81
#define EAPOL_KEY_DATA_LEN_OFFSET   97
#define EAPOL_KEY_DATA_OFFSET       99

#define EAPOL_PROT_VER_SIZE         1
#define EAPOL_PACK_TYPE_SIZE        1
#define EAPOL_BODY_LEN_SIZE         2
#define EAPOL_KEY_TYPE_SIZE         1
#define EAPOL_KEY_INFO_SIZE         2
#define EAPOL_KEY_LEN_SIZE          2
#define EAPOL_REP_CNT_SIZE          8
#define EAPOL_KEY_NONCE_SIZE        32
#define EAPOL_KEY_IV_SIZE           16
#define EAPOL_KEY_RSC_SIZE          8
#define EAPOL_KEY_ID_SIZE           8
#define EAPOL_KEY_MIC_SIZE          16
#define EAPOL_KEY_DATA_LEN_SIZE     2

#define GENERIC_INFO_ELEM 0xdd
#define RSN_INFO_ELEM 0x30
 
//static const long xdata WPA_SELECTOR_LEN = 4;
//static const unsigned char xdata WPA_OUI_TYPE[] = { 0x00, 0x50, 0xf2, 1 };
//static const long xdata RSN_SELECTOR_LEN = 4;
#define WPA_SELECTOR_LEN    4
#define RSN_SELECTOR_LEN    4


#define PMKID_LEN 16
/* EAPOL-Key Key Data Encapsulation
 * GroupKey and PeerKey require encryption, otherwise, encryption is optional.
 */
#define IV_LEN 8 //TKIP CCMP IV_LEN =8
#define LLC_LEN 8 

#define IEEE802_1X_HDR_LEN 4

#define EAPOL_KEY_TYPE_SIZE 1
#define EAPOL_KEY_KEYINFO_SIZE 2
#define EAPOL_KEY_KEYLEN_SIZE 2
#define EAPOL_KEY_RPYCNT_SIZE 8
#define EAPOL_KEY_NOUNCE_SIZE 32
#define EAPOL_KEY_IV_SIZE 16
#define EAPOL_KEY_RSC_SIZE 8
#define EAPOL_KEY_ID_SIZE 8
#define EAPOL_KEY_MIC_SIZE 16
#define EAPOL_KEY_KEYDATALEN_SIZE 2

#define WPA_REPLAY_COUNTER_LEN 8
#define WPA_NONCE_LEN 32

//CAM description
#define KEY_TYPE_WEP40           0x04
#define KEY_TYPE_TKIP_NO_MIC     0x08
#define KEY_TYPE_TKIP_WITH_MIC   0x0C
#define KEY_TYPE_AES             0x10
#define KEY_TYPE_WEP104          0x14
#define KEY_TYPE_WAPI            0x18

#define KEY_IS_MIC_KEY           BIT5
#define KEY_IS_GROUP_KEY         BIT6

#define SYMBOL_CHECK_PTK                    BIT0
#define SYMBOL_CHECK_GTK                    BIT1

/*------------------------------Define Enum-----------------------------------*/
typedef enum{
 NO_Encryption   = 0,
 WEP40_Encryption  = 1,
 TKIP_Encryption   = 2,
 Reserved_Encryption  = 3,
 AESCCMP_Encryption  = 4,
 WEP104_Encryption  = 5,
}RT_ENC_ALG, *PRT_ENC_ALG;

enum { EAPOL_KEY_TYPE_RC4 = 1, EAPOL_KEY_TYPE_RSN = 2,EAPOL_KEY_TYPE_RSN_2 = 3,
       EAPOL_KEY_TYPE_WPA = 254 };

/*--------------------------Define MACRO--------------------------------------*/
#define WPA_GET_BE16(a) ((unsigned short) (((a)[0] << 8) | (a)[1]))

#define WPA_GET_LE32(a) ((((unsigned int) (a)[0]) << 0) | (((unsigned int) (a)[1]) << 8) | \
             (((unsigned int) (a)[2]) << 16) | ((unsigned int) (a)[3]<<24))

#define WPA_PUT_BE16(a, val)            \
    do {                    \
        (a)[0] = ((unsigned short) (val)) >> 8; \
        (a)[1] = ((unsigned short) (val)) & 0xff;   \
    } while (0)
    
/*------------------------------Define Struct---------------------------------*/

union pn48  {
    
//  u64 val;
    /*
    struct {
        u32 TSC03;
        u32 TSC47;
    } _4byte_;
    */
    struct {
        u8 TSC0;
        u8 TSC1;
        u8 TSC2;
        u8 TSC3;
        u8 TSC4;
        u8 TSC5;
        u8 TSC6;
        u8 TSC7;
    } _byte_;
};

typedef struct _wpa_sm {
    u8 replay_counter[WPA_REPLAY_COUNTER_LEN];
    u32  protocol;
    u8 pairwise_cipher;
    u8 group_cipher;
    union pn48 iv_counter;
    u8 keyidx;
}wpa_sm;


typedef struct _ieee802_1x_hdr {
    u8 version;
    u8 type;
    u16 length;
    /* followed by length octets of data */
}ieee802_1x_hdr,*pieee802_1x_hdr;

typedef struct _wpa_eapol_key {
    u8 type;
    /* Note: key_info, key_length, and key_data_length are unaligned */
    u8 key_info[2];
    u8 key_length[2];
    u8 replay_counter[WPA_REPLAY_COUNTER_LEN];
    u8 key_nonce[WPA_NONCE_LEN];
    u8 key_iv[16];
    u8 key_rsc[8];
    u8 key_id[8]; /* Reserved in IEEE 802.11i/RSN */
    u8 key_mic[16];
    u8 key_data_length[2];
    /* followed by key_data_length bytes of key_data */
}wpa_eapol_key, *pwpa_eapol_key; 

typedef struct _H2C_EAPOL_KEY_STRUCT
{
// u32   key_data_len;
    ieee802_1x_hdr  ieee802_1x_hdr_data;
    wpa_eapol_key   eapol_key_data;
    u8   key_data[128];
}H2C_EAPOL_KEY_STRUCT, *pH2C_EAPOL_KEY_STRUCT;

//typedef unsigned long size_t;

//For RSN IE
struct wpa_eapol_ie_parse {
    const u8 *wpa_ie;
    u8 wpa_ie_len;
    const u8 *rsn_ie;
    u8 rsn_ie_len;
    const u8 *pmkid;
    const u8 *gtk;
    u8 gtk_len;
    const u8 *mac_addr;
    u8 mac_addr_len;

};

typedef struct _H2C_WPA_PTK {
    u8      kck[16]; /* EAPOL-Key Key Confirmation Key (KCK) */
    u8      kek[16]; /* EAPOL-Key Key Encryption Key (KEK) */
    u8  tk1[16]; /* Temporal Key 1 (TK1) */
    union {
        u8  tk2[16]; //Temporal Key 2 (TK2)
        struct {
            u8  tx_mic_key[8];
            u8  rx_mic_key[8];
        }Athu;
    }U;
}H2C_WPA_PTK;
 
typedef struct _H2C_WPA_TWO_WAY_PARA{
    //algorithm TKIP or AES
//  H2C_WPA_ALG     alg;
#if (IS_CHIP_8188E(CONFIG_CHIP_SEL)||IS_CHIP_8195(CONFIG_CHIP_SEL))
    u8      pairwise_en_alg;
    u8      group_en_alg;
#endif       
    H2C_WPA_PTK     wpa_ptk_value;


}H2C_WPA_TWO_WAY_PARA, *PH2C_WPA_TWO_WAY_PARA;

typedef struct _WPA_LOC {
    u8      gtk_rsp;
    u8      gtk_info;
    u8      enable;
} WPA_LOC;


/*------------------------Export global variable------------------------------*/

//extern xdata WPA_LOC wpa_loc;

/*------------------------------Function declaration--------------------------*/

#if 1//CONFIG_AES_GTK
extern unsigned char OnWpa_offload_8721D_ROM(  u16 mac_header_offset, u8 MacHeaderLen) ;

extern void wpa_supplicant_process_1_of_2_8721D_ROM( u16 version,u8 retry_bit) ;
extern void wpa_supplicant_decrypt_key_data_8721D_ROM(u16 key_desc_version) ;
extern void wpa_supplicant_send_2_of_2_8721D_ROM( u16 verson,u8 retry_bit) ;
//extern int wpa_supplicant_parse_generic(const u8 *pos, const u8 *end, struct wpa_eapol_ie_parse *ie) ;
//extern int wpa_supplicant_parse_ies(const u8 *buf, u32 len, struct wpa_eapol_ie_parse *ie) ;
//void write_cam_wow( u8  entry,  u16 ctrl,  u8 *mac,  u8 *key) ;
extern void write_cam_wow_8721D_ROM( u8  entry, u8 encAlg, u8 *key) ;
extern void WriteCAM_8721D_ROM(u16 Addr, u8* Data);
extern void ReadCAM_8721D_ROM(u16 Addr, u8* Data);
extern void ReadGTKData_8721D_ROM(u8 entry, u8 * key);
extern void AppendTkipMIC_8721D_ROM(u8  pktLoc, u8  pyldOffset);
extern s8 RTmemcmpBackward_8721D_ROM(const void * Dest, const void * Src, u8 count);
extern u8 AesTkipIvCheck_8721D_ROM(u8 * rx_iv_ori, u8 * rx_iv, u8 SecurityType);
extern u8 WepIvCheck_8721D_ROM(u8 * rx_iv_ori, u8 * rx_iv);
extern void AssignIvToKeyRsc_8721D_ROM(u8 * pattrib_iv, u8 * KeyRsc, u8 * KeyID, u8 SecurityType);
extern void AssignKeyRscToIV_8721D_ROM(u8 * pattrib_iv, u8 * KeyRsc, u8 SecurityType);
#endif 
#if 1//(CONFIG_AES_GTK||CONFIG_REMOTE_WAKEUP||CONFIG_ARP_RELAY)    //ben, 140612

//void AesTkipIvFun(u8* pattrib_iv, union pn48* pdot11txpn, u8 RetryBit,u8 SecurityType)  ;
#if 0
extern void AesTkipIvFun_CallPtr(
    IN u8* pattrib_iv,
    IN u8  RetryBit,
    IN u8  SecurityType
);
#endif
extern void AesTkipIvFun_8721D_ROM(u8* pattrib_iv, u8 RetryBit,u8 SecurityType) ;
extern void WepIvFun_8721D_ROM(u8* iv, u8 retry_bit) ;
//void ResetIPSEC(void);

#endif

#endif


