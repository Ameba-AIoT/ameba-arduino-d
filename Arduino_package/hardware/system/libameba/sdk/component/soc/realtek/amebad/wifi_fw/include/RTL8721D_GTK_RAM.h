#ifndef __WLAN_GTK_RAM_H__
#define __WLAN_GTK_RAM_H__

/*------------------------Export global variable------------------------------*/

//extern xdata WPA_LOC wpa_loc;
/*------------------------------Function declaration--------------------------*/

#if CONFIG_AES_GTK
int wpa_supplicant_parse_generic(const u8 *pos, const u8 *end, struct wpa_eapol_ie_parse *ie) ;
int wpa_supplicant_parse_ies(const u8 *buf, u32 len, struct wpa_eapol_ie_parse *ie) ;
//void write_cam_wow( u8  entry,  u16 ctrl,  u8 *mac,  u8 *key) ;
#endif 

#if CONFIG_GTK_VERIFICATION
void GTKDebug(void);
void ClearGTKDbg(void);
void DumpGTKInfo(void);


void HMACShalVerification(u8*Pframe,u8 PframeLen,u8* Kck,u8 KckLen, u8* answer);
void HMACMD5Verification(u8*Pframe,u8 PframeLen,u8* Kck,u8 KckLen, u8* answer);
void TkipMicVerification(u8 *Message,u8 MessageLen,u8* MicKey,u8 MicKeyLen,u8* answer);
void AESUnwrapVerification(u8* key_data,  u8 key_data_len, u8* kek,u8 kek_len, u8* answer);
void AES128EncVerification(u8* key_data,  u8 key_data_len, u8* kek,u8 kek_len, u8* answer);
void AES128DecVerification(u8* key_data,  u8 key_data_len, u8* kek,u8 kek_len, u8* answer);
void GTK_802_11WVerification(u8* Message,  u8 MessageLen, u8* iGTK,u8 iGTKLen, u8* answer);
void RC4Verification(u8 *Pframe,u8 PframeLen,u8* Kck,u8 KckLen,u8* answer);
#endif //#if CONFIG_GTK_VERIFICATION


#if CONFIG_IPSEC_ENGINE_RAM
void AESUnwrapSwapRam(u8* AESData,u8 DataLength);
void IPSecDataInRam(u8* DataIn,u8   DataSel);
void IPSecDataOutRam(u8* DataOut,u8   DataSel);
void IPSecFillDesrciptorRam(IPSEC_INFO* IpsecInfo,u16 MsgOffsetDesc,u8 MsgLengthDesc ) ;
void IPSWriteDescriptorIORam(u8* Desc );
void IPSecDesrciptorRam(IPSEC_INFO* IpsecInfo);
void IPSecWriteDataIOModeRam(u8* Data,u8 DataLength);
void IPSecGetDataIOModeRam(u8* Data,u8 DataLength);
void IPSecSetOPModeRam(u8 OPMode,u8 PollingBit);
void IPSecEngineRam(IPSEC_INFO* IpsecInfo,u8* Msg,u8* Key) ;
void IPSecSetExMemoryAddrRam( u16 ExMStradr);
void IPSecSetDescAddrRam(u16 DesStradr);


void AesUnwrapIpsecRam(u8* Kek,u8* KeyData,u8* Gtk,u8 GtkLen);
void HmacSha1IpsecRam(u8* Kck,u8 *Pframe,u8* Hash,u8 HashLen);
void HmcMd5IpsecRam( u8 *Pframe,u8* Kck,u8* Mic,u8 MicLen);
void RC4IpsecRam( u8* Ek,u8* KeyData, u8* GtkData, u8 GtkDataLen);
void TkipMicIpsecRam(u8* MicKey,u8 *Message,u8* Mic,u8 MicLen);
void FillParameterIpsecRam(u16 MsgOffset,u16 KeyOffset,u8 MsgLength,u8 KeyLength,u16 DesStradr);
void FillMichaelPacketInfoRam(u16 DaOffset,u16 SaOffset,u16 QosOffset,u16 ContentOffset,u8 QosOrNot);
u8 PollingRegRam( u16 RegOffset,u8 PollingBit,u8 PollingResult,u16 PollingLimit, u16* PollingCount);


#endif//CONFIG_IPSEC_ENGINE_RAM

#endif

