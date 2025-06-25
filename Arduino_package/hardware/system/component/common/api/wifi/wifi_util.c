#include <wireless.h>
#include <wlan_intf.h>
#include <platform/platform_stdlib.h>
#include <wifi/wifi_conf.h>
#include <wifi/wifi_ind.h>
#include <osdep_service.h>

int iw_ioctl(const char * ifname, unsigned long request, struct rtwreq *	pwrq)
{
	memcpy(pwrq->ifr_name, ifname, 5);
	return rltk_wlan_control(request, (void *) pwrq);
}

int wext_get_ssid(const char *ifname, __u8 *ssid)
{
	struct rtwreq iwr;
	int ret = 0;

	memset(&iwr, 0, sizeof(iwr));
	iwr.u.essid.pointer = ssid;
	iwr.u.essid.length = 32;

	if (iw_ioctl(ifname, RTKIOCGIWESSID, &iwr) < 0) {
#if defined(CONFIG_EXAMPLE_BT_CONFIG) && (CONFIG_EXAMPLE_BT_CONFIG!=1)
		RTW_API_INFO("\n\rioctl[RTKIOCGIWESSID] ssid = NULL, not connected"); //do not use perror
#endif
		ret = -1;
	} else {
		ret = iwr.u.essid.length;
		if (ret > 32)
			ret = 32;
		/* Some drivers include nul termination in the SSID, so let's
		 * remove it here before further processing. WE-21 changes this
		 * to explicitly require the length _not_ to include nul
		 * termination. */
		if (ret > 0 && ssid[ret - 1] == '\0')
			ret--;
		ssid[ret] = '\0';
	}

	return ret;
}

int wext_set_ssid(const char *ifname, const __u8 *ssid, __u16 ssid_len)
{
	struct rtwreq iwr;
	int ret = 0;

	memset(&iwr, 0, sizeof(iwr));
	iwr.u.essid.pointer = (void *) ssid;
	iwr.u.essid.length = ssid_len;
	iwr.u.essid.flags = (ssid_len != 0);

	if (iw_ioctl(ifname, RTKIOCSIWESSID, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[RTKIOCSIWESSID] error");
		ret = -1;
	}
	
	return ret;
}

int wext_set_bssid(const char *ifname, const __u8 *bssid)
{
	struct rtwreq iwr;
	int ret = 0;

	memset(&iwr, 0, sizeof(iwr));
	iwr.u.ap_addr.sa_family = ARPHRD_ETHER;
	memcpy(iwr.u.ap_addr.sa_data, bssid, ETH_ALEN);

	if(bssid[ETH_ALEN]=='#' && bssid[ETH_ALEN + 1]=='@'){
		memcpy(iwr.u.ap_addr.sa_data + ETH_ALEN, bssid + ETH_ALEN, 6);
	}

	if (iw_ioctl(ifname, RTKIOCSIWAP, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[RTKIOCSIWAP] error");
		ret = -1;
	}

	return ret;
}

int wext_get_bssid(const char*ifname, __u8 *bssid)
{
	struct rtwreq iwr;
	int ret = 0;

	memset(&iwr, 0, sizeof(iwr));

	if (iw_ioctl(ifname, RTKIOCGIWAP, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[RTKIOCSIWAP] error");
		ret = -1;
	} else {
		memcpy(bssid, iwr.u.ap_addr.sa_data, ETH_ALEN);
    }

	return ret;
}

int is_broadcast_ether_addr(const unsigned char *addr)
{
	return (addr[0] & addr[1] & addr[2] & addr[3] & addr[4] & addr[5]) == 0xff;
}

int wext_set_auth_param(const char *ifname, __u16 idx, __u32 value)
{
	struct rtwreq iwr;
	int ret = 0;

	memset(&iwr, 0, sizeof(iwr));
	iwr.u.param.flags = idx & RTW_AUTH_INDEX;
	iwr.u.param.value = value;

	if (iw_ioctl(ifname, RTKIOCSIWAUTH, &iwr) < 0) {
		RTW_API_INFO("\n\rWEXT: RTKIOCSIWAUTH(param %d value 0x%x) failed)", idx, value);
	}

	return ret;
}

int wext_set_mfp_support(const char *ifname, __u8 value)
{
	int ret = 0;
	struct rtwreq iwr;

	memset(&iwr, 0, sizeof(iwr));
	iwr.u.param.value = value;

	if (iw_ioctl(ifname, RTKIOCSIWMFP, &iwr) < 0) {
		RTW_API_INFO("\n\rWEXT: RTKIOCSIWMFP(value 0x%x) failed)", value);
	}

	return ret;
}

#ifdef CONFIG_SAE_SUPPORT
int wext_set_group_id(const char *ifname, __u8 value)
{
	int ret = 0;
	struct rtwreq iwr;

	memset(&iwr, 0, sizeof(iwr));
	iwr.u.param.value = value;

	if (iw_ioctl(ifname, RTKIOCSIWGRPID, &iwr) < 0) {
		RTW_API_INFO("\n\rWEXT: RTKIOCSIWGRPID(value 0x%x) failed)", value);
	}

	return ret;
}

int wext_set_support_wpa3(__u8 enable)
{
	extern u8 rtw_cmd_tsk_spt_wap3;
	rtw_cmd_tsk_spt_wap3 = enable;
	return 0;
}

unsigned char wext_get_support_wpa3(void)
{
	extern u8 rtw_cmd_tsk_spt_wap3;
	return rtw_cmd_tsk_spt_wap3;
}

#endif

#ifdef CONFIG_PMKSA_CACHING
int wext_set_pmk_cache_enable(const char *ifname, __u8 value)
{
	int ret = 0;
	struct rtwreq iwr;

	memset(&iwr, 0, sizeof(iwr));
	iwr.u.param.value = value;

	if (iw_ioctl(ifname, RTKIOCSIWPMKSA, &iwr) < 0) {
		RTW_API_INFO("\n\rWEXT: RTKIOCSIWPMKSA(value 0x%x) failed)", value);
	}

	return ret;
}
#endif

int wext_set_key_ext(const char *ifname, __u16 alg, const __u8 *addr, int key_idx, int set_tx, const __u8 *seq, __u16 seq_len, __u8 *key, __u16 key_len)
{
	struct rtwreq iwr;
	int ret = 0;
	struct rtw_encode_ext *ext;

	ext = (struct rtw_encode_ext *) malloc(sizeof(struct rtw_encode_ext) + key_len);
	if (ext == NULL)
		return -1;
	else
		memset(ext, 0, sizeof(struct rtw_encode_ext) + key_len);

	memset(&iwr, 0, sizeof(iwr));
	iwr.u.encoding.flags = key_idx + 1;
	iwr.u.encoding.flags |= RTW_ENCODE_TEMP;
	iwr.u.encoding.pointer = ext;
	iwr.u.encoding.length = sizeof(struct rtw_encode_ext) + key_len;

	if (alg == RTW_ENCODE_DISABLED)
		iwr.u.encoding.flags |= RTW_ENCODE_DISABLED;

	if (addr == NULL || is_broadcast_ether_addr(addr))
		ext->ext_flags |= RTW_ENCODE_EXT_GROUP_KEY;

	if (set_tx)
		ext->ext_flags |= RTW_ENCODE_EXT_SET_TX_KEY;

	ext->addr.sa_family = ARPHRD_ETHER;

	if (addr)
		memcpy(ext->addr.sa_data, addr, ETH_ALEN);
	else
		memset(ext->addr.sa_data, 0xff, ETH_ALEN);

	if (key && key_len) {
		memcpy(ext->key, key, key_len);
		ext->key_len = key_len;
	}

	ext->alg = alg;

	if (seq && seq_len) {
		ext->ext_flags |= RTW_ENCODE_EXT_RX_SEQ_VALID;
		memcpy(ext->rx_seq, seq, seq_len);
	}

	if (iw_ioctl(ifname, RTKIOCSIWENCODEEXT, &iwr) < 0) {
		ret = -2;
		RTW_API_INFO("\n\rioctl[RTKIOCSIWENCODEEXT] set key fail");
	}

	free(ext);

	return ret;
}

int wext_get_enc_ext(const char *ifname, __u16 *alg, __u8 *key_idx, __u8 *passphrase)
{
	struct rtwreq iwr;
	int ret = 0;
	struct rtw_encode_ext *ext;

	ext = (struct rtw_encode_ext *) malloc(sizeof(struct rtw_encode_ext) + 16);
	if (ext == NULL)
		return -1;
	else
		memset(ext, 0, sizeof(struct rtw_encode_ext) + 16);

	iwr.u.encoding.pointer = ext;

	if (iw_ioctl(ifname, RTKIOCGIWENCODEEXT, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[RTKIOCGIWENCODEEXT] error");
		ret = -1;
	}
	else
	{
		*alg = ext->alg;
		if(key_idx)
			*key_idx = (__u8)iwr.u.encoding.flags;
		if(passphrase)
			memcpy(passphrase, ext->key, ext->key_len);
	}

	if(ext != NULL)
		free(ext);
	
	return ret;
}

int wext_get_auth_type(const char *ifname, __u32 *auth_type)
{
	struct rtwreq iwr;
	int ret = 0;

	memset(&iwr, 0, sizeof(iwr));

	if (iw_ioctl(ifname, RTKIOCGIWAUTH, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[RTKIOCGIWAUTH] error");
		ret = -1;
	} else {
		*auth_type = (__u32) iwr.u.param.value;
	}

	return ret;
}

int wext_set_passphrase(const char *ifname, const __u8 *passphrase, __u16 passphrase_len)
{
	struct rtwreq iwr;
	int ret = 0;

	memset(&iwr, 0, sizeof(iwr));
	iwr.u.passphrase.pointer = (void *) passphrase;
	iwr.u.passphrase.length = passphrase_len;
	iwr.u.passphrase.flags = (passphrase_len != 0);

	if (iw_ioctl(ifname, RTKIOCSIWPRIVPASSPHRASE, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[RTKIOCSIWESSID+0x1f] error");
		ret = -1;
	}
	
	return ret;
}

int wext_get_passphrase(const char *ifname, __u8 *passphrase)
{
	struct rtwreq iwr;
	int ret = 0;

	memset(&iwr, 0, sizeof(iwr));
	iwr.u.passphrase.pointer = (void *) passphrase;

	if (iw_ioctl(ifname, RTKIOCGIWPRIVPASSPHRASE, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[RTKIOCGIWPRIVPASSPHRASE] error");
		ret = -1;
	}
	else {
		ret = iwr.u.passphrase.length;
		passphrase[ret] = '\0';
	}
	
	return ret;
}

#if 0
int wext_set_mac_address(const char *ifname, char * mac)
{
	char buf[13+17+1];
	rtw_memset(buf, 0, sizeof(buf));
	snprintf(buf, 13+17, "write_mac %s", mac);
	return wext_private_command(ifname, buf, 0);
}

int wext_get_mac_address(const char *ifname, char * mac)
{
	int ret = 0;
	char buf[32];

	rtw_memset(buf, 0, sizeof(buf));
	rtw_memcpy(buf, "read_mac", 8);
	ret = wext_private_command_with_retval(ifname, buf, buf, 32);
	strcpy(mac, buf);
	return ret;
}
#endif

int wext_enable_powersave(const char *ifname, __u8 ips_mode, __u8 lps_mode)
{
	struct rtwreq iwr;
	int ret = 0;
	__u16 pindex = 0;
	__u8 *para = NULL;
	int cmd_len = 0;

	memset(&iwr, 0, sizeof(iwr));
	cmd_len = sizeof("pm_set");

	// Encode parameters as TLV (type, length, value) format
	para = rtw_malloc( 7 + (1+1+1) + (1+1+1) );
	if(para == NULL) return -1;

	snprintf((char*)para, cmd_len, "pm_set");
	pindex = 7;

	para[pindex++] = 0; // type 0 for ips
	para[pindex++] = 1;
	para[pindex++] = ips_mode;

	para[pindex++] = 1; // type 1 for lps
	para[pindex++] = 1;
	para[pindex++] = lps_mode;

	iwr.u.data.pointer = para;
	iwr.u.data.length = pindex;

	if (iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[SIOCSIWPRIVPMSET] error");
		ret = -1;
	}

	rtw_free(para);
	return ret;
}

int wext_resume_powersave(const char *ifname)
{
	struct rtwreq iwr;
	int ret = 0;
	__u16 pindex = 0;
	__u8 *para = NULL;
	int cmd_len = 0;

	memset(&iwr, 0, sizeof(iwr));
	cmd_len = sizeof("pm_set");

	// Encode parameters as TLV (type, length, value) format
	para = rtw_malloc( 7 + (1+1+1) + (1+1+1) );
	if(para == NULL) return -1;

	snprintf((char*)para, cmd_len, "pm_set");
	pindex = 7;

	para[pindex++] = 8; // type 8 for power save resume
	para[pindex++] = 0;

	iwr.u.data.pointer = para;
	iwr.u.data.length = pindex;

	if (iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[SIOCSIWPRIVPMSET] error");
		ret = -1;
	}

	rtw_free(para);
	return ret;
}

int wext_disable_powersave(const char *ifname)
{
	struct rtwreq iwr;
	int ret = 0;
	__u16 pindex = 0;
	__u8 *para = NULL;
	int cmd_len = 0;

	memset(&iwr, 0, sizeof(iwr));
	cmd_len = sizeof("pm_set");

	// Encode parameters as TLV (type, length, value) format
	para = rtw_malloc( 7 + (1+1+1) + (1+1+1) );
	if(para == NULL) return -1;

	snprintf((char*)para, cmd_len, "pm_set");
	pindex = 7;

	para[pindex++] = 0; // type 0 for ips
	para[pindex++] = 1;
	para[pindex++] = 0; // ips = 0

	para[pindex++] = 1; // type 1 for lps
	para[pindex++] = 1;
	para[pindex++] = 0; // lps = 0

	iwr.u.data.pointer = para;
	iwr.u.data.length = pindex;

	if (iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[SIOCSIWPRIVPMSET] error");
		ret = -1;
	}

	rtw_free(para);
	return ret;

}

int wext_set_tdma_param(const char *ifname, __u8 slot_period, __u8 rfon_period_len_1, __u8 rfon_period_len_2, __u8 rfon_period_len_3)
{
	struct rtwreq iwr;
	int ret = 0;
	__u16 pindex = 0;
	__u8 *para = NULL;
	int cmd_len = 0;

	memset(&iwr, 0, sizeof(iwr));
	cmd_len = sizeof("pm_set");

	// Encode parameters as TLV (type, length, value) format
	para = rtw_malloc( 7 + (1+1+4) );
	
	snprintf((char*)para, cmd_len, "pm_set");
	pindex = 7;

	para[pindex++] = 2; // type 2 tdma param
	para[pindex++] = 4;
	para[pindex++] = slot_period;
	para[pindex++] = rfon_period_len_1;
	para[pindex++] = rfon_period_len_2;
	para[pindex++] = rfon_period_len_3;

	iwr.u.data.pointer = para;
	iwr.u.data.length = pindex;

	if (iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[SIOCSIWPRIVPMSET] error");
		ret = -1;
	}

	rtw_free(para);
	return ret;
}

int wext_set_lps_dtim(const char *ifname, __u8 lps_dtim)
{
	struct rtwreq iwr;
	int ret = 0;
	__u16 pindex = 0;
	__u8 *para = NULL;
	int cmd_len = 0;

	memset(&iwr, 0, sizeof(iwr));
	cmd_len = sizeof("pm_set");

	// Encode parameters as TLV (type, length, value) format
	para = rtw_malloc( 7 + (1+1+1) );
	
	snprintf((char*)para, cmd_len, "pm_set");
	pindex = 7;

	para[pindex++] = 3; // type 3 lps dtim
	para[pindex++] = 1;
	para[pindex++] = lps_dtim;

	iwr.u.data.pointer = para;
	iwr.u.data.length = pindex;

	if (iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[SIOCSIWPRIVPMSET] error");
		ret = -1;
	}

	rtw_free(para);
	return ret;
}

int wext_get_lps_dtim(const char *ifname, __u8 *lps_dtim)
{

	struct rtwreq iwr;
	int ret = 0;
	__u16 pindex = 0;
	__u8 *para = NULL;
	int cmd_len = 0;
	
	memset(&iwr, 0, sizeof(iwr));
	cmd_len = sizeof("pm_get");

	// Encode parameters as TLV (type, length, value) format
	para = rtw_malloc( 7 + (1+1+1) );
	
	snprintf((char*)para, cmd_len, "pm_get");
	pindex = 7;

	para[pindex++] = 3; // type 3 for lps dtim
	para[pindex++] = 1;
	para[pindex++] = 0;

	iwr.u.data.pointer = para;
	iwr.u.data.length = pindex;

	if (iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[SIOCSIWPRIVPMGET] error");
		ret = -1;
		goto exit;
	}

	//get result at the beginning of iwr.u.data.pointer
	if((para[0]==3)&&(para[1]==1))
		*lps_dtim = para[2];
	else
		RTW_API_INFO("\n\r%s error", __func__);

exit:
	rtw_free(para);

	return ret;
}

int wext_set_lps_thresh(const char *ifname, u8 low_thresh) {
	struct rtwreq iwr;
	int ret = 0;
	__u16 pindex = 0;
	__u8 *para = NULL;
	int cmd_len = 0;

	memset(&iwr, 0, sizeof(iwr));
	cmd_len = sizeof("pm_set");

	// Encode parameters as TLV (type, length, value) format
	para = rtw_malloc( 7 + (1+1+1) );
	
	snprintf((char*)para, cmd_len, "pm_set");
	pindex = 7;

	para[pindex++] = 6; // type 6 lps threshold
	para[pindex++] = 1; // len
	para[pindex++] = low_thresh;

	iwr.u.data.pointer = para;
	iwr.u.data.length = pindex;

	if (iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[SIOCSIWPRIVPMSET] error");
		ret = -1;
	}

	rtw_free(para);
	return ret;
}

#ifdef LONG_PERIOD_TICKLESS
int wext_set_lps_smartps(const char *ifname, __u8 smartps) {
	struct rtwreq iwr;
	int ret = 0;
	__u16 pindex = 0;
	__u8 *para = NULL;
	int cmd_len = 0;

	memset(&iwr, 0, sizeof(iwr));
	cmd_len = sizeof("pm_set");

	// Encode parameters as TLV (type, length, value) format
	para = rtw_malloc( 7 + (1+1+1) );
	
	snprintf((char*)para, cmd_len, "pm_set");
	pindex = 7;

	para[pindex++] = 9; // type 9 smartps
	para[pindex++] = 1; // len
	para[pindex++] = smartps;

	iwr.u.data.pointer = para;
	iwr.u.data.length = pindex;

	if (iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[SIOCSIWPRIVPMSET] error");
		ret = -1;
	}

	rtw_free(para);
	return ret;

}
#endif
int wext_set_beacon_mode(const char *ifname, __u8 mode) {
	struct rtwreq iwr;
	int ret = 0;
	__u16 pindex = 0;
	__u8 *para = NULL;
	int cmd_len = 0;

	memset(&iwr, 0, sizeof(iwr));
	cmd_len = sizeof("pm_set");

	// Encode parameters as TLV (type, length, value) format
	para = rtw_malloc( 7 + (1+1+1) );
	
	snprintf((char*)para, cmd_len, "pm_set");
	pindex = 7;

	para[pindex++] = 4; // type 4 beacon mode
	para[pindex++] = 1; // len
	para[pindex++] = mode;

	iwr.u.data.pointer = para;
	iwr.u.data.length = pindex;

	if (iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[SIOCSIWPRIVPMSET] error");
		ret = -1;
	}

	rtw_free(para);
	return ret;
}

int wext_set_lps_level(const char *ifname, __u8 lps_level) {
	struct rtwreq iwr;
	int ret = 0;
	__u16 pindex = 0;
	__u8 *para = NULL;
	int cmd_len = 0;

	memset(&iwr, 0, sizeof(iwr));
	cmd_len = sizeof("pm_set");

	// Encode parameters as TLV (type, length, value) format
	para = rtw_malloc( 7 + (1+1+1) );
	
	snprintf((char*)para, cmd_len, "pm_set");
	pindex = 7;

	para[pindex++] = 5; // type 5 lps_level
	para[pindex++] = 1; // len
	para[pindex++] = lps_level;

	iwr.u.data.pointer = para;
	iwr.u.data.length = pindex;

	if (iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[SIOCSIWPRIVPMSET] error");
		ret = -1;
	}

	rtw_free(para);
	return ret;
}


int wext_set_tos_value(const char *ifname, __u8 *tos_value)
{
	struct rtwreq iwr;
	int ret = 0;
	__u8 *para = NULL;
	int cmd_len = sizeof("set_tos_value");

	memset(&iwr, 0, sizeof(iwr));

	para = rtw_malloc(cmd_len + 4);
	snprintf((char*)para, cmd_len, "set_tos_value");

	if(*tos_value <=32){
		*(para + cmd_len)   = 0x4f;
		*(para + cmd_len+1) = 0xa4;
		*(para + cmd_len+2) = 0;
		*(para + cmd_len+3) = 0;
	}
	else if(*tos_value > 32 && *tos_value <=96){
		*(para + cmd_len)   = 0x2b;
		*(para + cmd_len+1) = 0xa4;
		*(para + cmd_len+2) = 0;
		*(para + cmd_len+3) = 0;
	}
	else if(*tos_value > 96 && *tos_value <= 160){
		*(para + cmd_len)   = 0x22;
		*(para + cmd_len+1) = 0x43;
		*(para + cmd_len+2) = 0x5e;
		*(para + cmd_len+3) = 0;
	}
	else if(*tos_value > 160){
		*(para + cmd_len)   = 0x22;
		*(para + cmd_len+1) = 0x32;
		*(para + cmd_len+2) = 0x2f;
		*(para + cmd_len+3) = 0;
	}

	iwr.u.data.pointer = para;
	iwr.u.data.length = cmd_len + 4;

	if (iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr) < 0) {
		RTW_API_INFO("\n\rwext_set_tos_value():ioctl[RTKIOCDEVPRIVATE] error");
		ret = -1;
	}

	rtw_free(para);
	return ret;
}

int wext_get_tx_power(const char *ifname, __u8 *poweridx)
{
	struct rtwreq iwr;
	int ret = 0;
	__u8 *para = NULL;
	int cmd_len = sizeof("get_tx_power");

	memset(&iwr, 0, sizeof(iwr));
	//Tx power size : 20 Bytes
	//CCK 1M,2M,5.5M,11M : 4 Bytes
	//OFDM 6M, 9M, 12M, 18M, 24M, 36M 48M, 54M : 8 Bytes
	//MCS 0~7 : 8 Bytes
	para = rtw_malloc(cmd_len + 20);
	snprintf((char*)para, cmd_len, "get_tx_power");

	iwr.u.data.pointer = para;
	iwr.u.data.length = cmd_len + 20;
	if (iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr) < 0) {
		RTW_API_INFO("\n\rwext_get_tx_power():ioctl[RTKIOCDEVPRIVATE] error");
		ret = -1;
	}

	memcpy(poweridx,(__u8 *)(iwr.u.data.pointer),20);
	rtw_free(para);
	return ret;
}


#ifdef CONFIG_MP_INCLUDED
int wext_set_txpower(const char *ifname, int poweridx)
{
	int ret = 0;
	char buf[30];
	
	rtw_memset(buf, 0, sizeof(buf));
	snprintf(buf, 30, "mp_txpower patha=%d", poweridx);
	ret = wext_private_command(ifname, buf, 0);

	return ret;
}
#endif
#if 0
int wext_get_associated_client_list(const char *ifname, void * client_list_buffer, uint16_t buffer_length)
{
	int ret = 0;
	char buf[25];

	rtw_memset(buf, 0, sizeof(buf));
	snprintf(buf, 25, "get_client_list %x", client_list_buffer);
	ret = wext_private_command(ifname, buf, 0);

	return ret;
}

int wext_get_ap_info(const char *ifname, rtw_bss_info_t * ap_info, rtw_security_t* security)
{
	int ret = 0;
	char buf[24];

	rtw_memset(buf, 0, sizeof(buf));
	snprintf(buf, 24, "get_ap_info %x", ap_info);
	ret = wext_private_command(ifname, buf, 0);

	snprintf(buf, 24, "get_security");
	ret = wext_private_command_with_retval(ifname, buf, buf, 24);
	sscanf(buf, "%d", security);

	return ret;
}
#endif

int wext_set_mode(const char *ifname, int mode)
{
	struct rtwreq iwr;
	int ret = 0;

	memset(&iwr, 0, sizeof(iwr));
	iwr.u.mode = mode;
	if (iw_ioctl(ifname, RTKIOCSIWMODE, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[RTKIOCSIWMODE] error");
		ret = -1;
	}

	return ret;
}

int wext_get_mode(const char *ifname, int *mode)
{
	struct rtwreq iwr;
	int ret = 0;

	memset(&iwr, 0, sizeof(iwr));

	if (iw_ioctl(ifname, RTKIOCGIWMODE, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[RTKIOCGIWMODE] error");
		ret = -1;
	}
	else
		*mode = iwr.u.mode;

	return ret;
}

int wext_set_ap_ssid(const char *ifname, const __u8 *ssid, __u16 ssid_len)
{
	struct rtwreq iwr;
	int ret = 0;

	if(ssid_len > 32){
		printf("Error: SSID should be 0-32 characters\r\n");
		return -1;
	}

	memset(&iwr, 0, sizeof(iwr));
	iwr.u.essid.pointer = (void *) ssid;
	iwr.u.essid.length = ssid_len;
	iwr.u.essid.flags = (ssid_len != 0);

	if (iw_ioctl(ifname, RTKIOCSIWPRIVAPESSID, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[RTKIOCSIWPRIVAPESSID] error");
		ret = -1;
	}
	
	return ret;
}

int wext_set_country(const char *ifname, rtw_country_code_t country_code)
{
	struct rtwreq iwr;
	int ret = 0;

	memset(&iwr, 0, sizeof(iwr));

	iwr.u.param.value = country_code;
	
	if (iw_ioctl(ifname, RTKIOCSIWPRIVCOUNTRY, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[RTKIOCSIWPRIVCOUNTRY] error");
		ret = -1;
	}
	return ret;
}

int wext_get_rssi(const char *ifname, int *rssi)
{
	struct rtwreq iwr;
	int ret = 0;

	memset(&iwr, 0, sizeof(iwr));

	if (iw_ioctl(ifname, RTKIOCGIWSENS, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[RTKIOCGIWSENS] error");
		ret = -1;
	} else {
		*rssi = 0 - iwr.u.sens.value;
	}
	return ret;
}

int wext_get_bcn_rssi(const char *ifname, int *rssi)
{
	struct rtwreq iwr;
	int ret = 0;

	memset(&iwr, 0, sizeof(iwr));

	if (iw_ioctl(ifname, RTKIOCGIWBCNSENS, &iwr) < 0) {
		printf("\n\rioctl[RTKIOCGIWBCNSENS] error");
		ret = -1;
	} else {
		*rssi = 0 - iwr.u.bcnsens.value;
	}
	return ret;
}

int wext_set_bcn_period(__u16 period){
	int ret = 0;
	extern u16 custom_beacon_period;
	custom_beacon_period = period;
	return ret;
}

int wext_set_pscan_channel(const char *ifname, __u8 *ch, __u8 *pscan_config, __u8 length)
{
	struct rtwreq iwr;
	int ret = 0;
	__u8 *para = NULL;
	int i =0;

	memset(&iwr, 0, sizeof(iwr));
	//Format of para:function_name num_channel chan1... pscan_config1 ...
	para = rtw_malloc((length + length + 1) + 12);//size:num_chan + num_time + length + function_name
	if(para == NULL) return -1;

	//Cmd
	snprintf((char*)para, 12, "PartialScan");
	//length
	*(para+12) = length;
	for(i = 0; i < length; i++){
		*(para + 13 + i)= *(ch + i);
		*(para + 13 + length + i)= *(pscan_config + i);
	}
	
	iwr.u.data.pointer = para;
	iwr.u.data.length = (length + length + 1) + 12;
	if (iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr) < 0) {
		RTW_API_INFO("\n\rwext_set_pscan_channel():ioctl[RTKIOCDEVPRIVATE] error");
		ret = -1;
	}
	rtw_free(para);
	return ret;
}
int wext_set_channel(const char *ifname, __u8 ch)
{
	struct rtwreq iwr;
	int ret = 0;

	memset(&iwr, 0, sizeof(iwr));
	iwr.u.freq.m = 0;
	iwr.u.freq.e = 0;
	iwr.u.freq.i = ch;

	if (iw_ioctl(ifname, RTKIOCSIWFREQ, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[RTKIOCSIWFREQ] error");
		ret = -1;
	}

	return ret;
}

int wext_get_channel(const char *ifname, __u8 *ch)
{
	struct rtwreq iwr;
	int ret = 0;

	memset(&iwr, 0, sizeof(iwr));

	if (iw_ioctl(ifname, RTKIOCGIWFREQ, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[RTKIOCGIWFREQ] error");
		ret = -1;
	}
	else
		*ch = iwr.u.freq.i;

	return ret;
}

int wext_register_multicast_address(const char *ifname, rtw_mac_t *mac)
{
	int ret = 0;
	char buf[32];

	rtw_memset(buf, 0, sizeof(buf));
	snprintf(buf, 32, "reg_multicast "MAC_FMT, MAC_ARG(mac->octet));
	ret = wext_private_command(ifname, buf, 0);

	return ret;
}

int wext_unregister_multicast_address(const char *ifname, rtw_mac_t *mac)
{
	int ret = 0;
	char buf[35];

	rtw_memset(buf, 0, sizeof(buf));
	snprintf(buf, 35, "reg_multicast -d "MAC_FMT, MAC_ARG(mac->octet));
	ret = wext_private_command(ifname, buf, 0);

	return ret;
}

int wext_set_scan(const char *ifname, char *buf, __u16 buf_len, __u16 flags)
{
	struct rtwreq iwr;
	int ret = 0;

	memset(&iwr, 0, sizeof(iwr));
#if 0 //for scan_with_ssid	
	if(buf)
		memset(buf, 0, buf_len);
#endif
	iwr.u.data.pointer = buf;
	iwr.u.data.flags = flags;
	iwr.u.data.length = buf_len;
	if (iw_ioctl(ifname, RTKIOCSIWSCAN, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[RTKIOCSIWSCAN] error");
		ret = -1;
	}
	return ret;
}

int wext_get_scan(const char *ifname, char *buf, __u16 buf_len)
{
	struct rtwreq iwr;
	int ret = 0;

	iwr.u.data.pointer = buf;
	iwr.u.data.length = buf_len;
	if (iw_ioctl(ifname, RTKIOCGIWSCAN, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[RTKIOCGIWSCAN] error");
		ret = -1;
	}else
		ret = iwr.u.data.flags;
	return ret;
}

int wext_private_command_with_retval(const char *ifname, char *cmd, char *ret_buf, int ret_len)
{
	struct rtwreq iwr;
	int ret = 0;
	unsigned int buf_size;
	char *buf;
	
	buf_size = 128;
	if(strlen(cmd) >= buf_size)
		buf_size = strlen(cmd) + 1;	// 1 : '\0'
	buf = (char*)rtw_malloc(buf_size);
	if(!buf){
		RTW_API_INFO("\n\rWEXT: Can't malloc memory");
		return -1;
	}
	memset(buf, 0, buf_size);
	strncpy(buf, cmd, buf_size);
	memset(&iwr, 0, sizeof(iwr));
	iwr.u.data.pointer = buf;
	iwr.u.data.length = buf_size;
	iwr.u.data.flags = 0;

	if ((ret = iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr)) < 0) {
		RTW_API_INFO("\n\rioctl[RTKIOCDEVPRIVATE] error. ret=%d\n", ret);
	}
	if(ret_buf){
		if(ret_len > iwr.u.data.length)
			ret_len =  iwr.u.data.length;
		rtw_memcpy(ret_buf, (char *) iwr.u.data.pointer, ret_len);
	}
	rtw_free(buf);
	return ret;
}

int wext_private_command(const char *ifname, char *cmd, int show_msg)
{
	struct rtwreq iwr;
	int ret = 0;
	unsigned int buf_size;
	char *buf;

	u8 cmdname[17] = {0}; // IFNAMSIZ+1

	sscanf(cmd, "%16s", cmdname);
	if((strcmp((const char *)cmdname, "config_get") == 0)
		|| (strcmp((const char *)cmdname, "config_set") == 0)
		|| (strcmp((const char *)cmdname, "efuse_get") == 0)
		|| (strcmp((const char *)cmdname, "efuse_set") == 0)
		|| (strcmp((const char *)cmdname, "mp_psd") == 0))
		buf_size = 2600;//2600 for config_get rmap,0,512 (or realmap)
	else
		buf_size = 512;    

	if(strlen(cmd) >= buf_size)
		buf_size = strlen(cmd) + 1;	// 1 : '\0'
	buf = (char*)rtw_malloc(buf_size);
	if(!buf){
		RTW_API_INFO("\n\rWEXT: Can't malloc memory");
		return -1;
	}
	memset(buf, 0, buf_size);
	strncpy(buf, cmd, buf_size);
	memset(&iwr, 0, sizeof(iwr));
	iwr.u.data.pointer = buf;
	iwr.u.data.length = buf_size;
	iwr.u.data.flags = 0;

	if ((ret = iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr)) < 0) {
		RTW_API_INFO("\n\rioctl[RTKIOCDEVPRIVATE] error. ret=%d\n", ret);
	}
	if (show_msg && iwr.u.data.length) {
		if(iwr.u.data.length > buf_size)
			RTW_API_INFO("\n\rWEXT: Malloc memory is not enough");
		RTW_API_INFO("\n\rPrivate Message: %s", (char *) iwr.u.data.pointer);
	}
	rtw_free(buf);
	return ret;
}

void wext_wlan_indicate(unsigned int cmd, union rtwreq_data *wrqu, char *extra)
{
	unsigned char null_mac[6] = {0};

	switch(cmd)
	{
		case RTKIOCGIWAP:
			if(wrqu->ap_addr.sa_family == ARPHRD_ETHER)
			{
				if(!memcmp(wrqu->ap_addr.sa_data, null_mac, sizeof(null_mac)))
					wifi_indication(WIFI_EVENT_DISCONNECT, wrqu->ap_addr.sa_data, sizeof(null_mac)+ 2, 0);
				else				
					wifi_indication(WIFI_EVENT_CONNECT, wrqu->ap_addr.sa_data, sizeof(null_mac), 0);
			}			
			break;

		case RTWEVCUSTOM:
			if(extra)
			{
				if(!memcmp(IW_EXT_STR_FOURWAY_DONE, extra, strlen(IW_EXT_STR_FOURWAY_DONE)))
					wifi_indication(WIFI_EVENT_FOURWAY_HANDSHAKE_DONE, extra, strlen(IW_EXT_STR_FOURWAY_DONE), 0);
				else if(!memcmp(IW_EXT_STR_RECONNECTION_FAIL, extra, strlen(IW_EXT_STR_RECONNECTION_FAIL)))
					wifi_indication(WIFI_EVENT_RECONNECTION_FAIL, extra, strlen(IW_EXT_STR_RECONNECTION_FAIL), 0);
				else if(!memcmp(IW_EVT_STR_NO_NETWORK, extra, strlen(IW_EVT_STR_NO_NETWORK)))
					wifi_indication(WIFI_EVENT_NO_NETWORK, extra, strlen(IW_EVT_STR_NO_NETWORK), 0);
				else if(!memcmp(IW_EVT_STR_ICV_ERROR, extra, strlen(IW_EVT_STR_ICV_ERROR)))
					wifi_indication(WIFI_EVENT_ICV_ERROR, extra, strlen(IW_EVT_STR_ICV_ERROR), 0);
				else if(!memcmp(IW_EVT_STR_CHALLENGE_FAIL, extra, strlen(IW_EVT_STR_CHALLENGE_FAIL)))
					wifi_indication(WIFI_EVENT_CHALLENGE_FAIL, extra, strlen(IW_EVT_STR_CHALLENGE_FAIL), 0);
#if CONFIG_ENABLE_P2P || defined(CONFIG_AP_MODE)
				else if(!memcmp(IW_EVT_STR_STA_ASSOC, extra, strlen(IW_EVT_STR_STA_ASSOC)))
					wifi_indication(WIFI_EVENT_STA_ASSOC, wrqu->data.pointer, wrqu->data.length, 0);
				else if(!memcmp(IW_EVT_STR_STA_DISASSOC, extra, strlen(IW_EVT_STR_STA_DISASSOC)))
					wifi_indication(WIFI_EVENT_STA_DISASSOC, wrqu->addr.sa_data, sizeof(null_mac), 0);
				else if(!memcmp(IW_EVT_STR_SEND_ACTION_DONE, extra, strlen(IW_EVT_STR_SEND_ACTION_DONE)))
					wifi_indication(WIFI_EVENT_SEND_ACTION_DONE, NULL, 0, wrqu->data.flags);
				else if(!memcmp(IW_EVT_STR_SOFTAP_START, extra, strlen(IW_EVT_STR_SOFTAP_START)))
					wifi_indication(WIFI_EVENT_SOFTAP_START, extra, strlen(IW_EVT_STR_SOFTAP_START), 0);
				else if(!memcmp(IW_EVT_STR_SOFTAP_STOP, extra, strlen(IW_EVT_STR_SOFTAP_STOP)))
					wifi_indication(WIFI_EVENT_SOFTAP_STOP, extra, strlen(IW_EVT_STR_SOFTAP_STOP), 0);
#endif
			}
			break;
		case RTKIOCGIWSCAN:
			if(wrqu->data.pointer == NULL)
				wifi_indication(WIFI_EVENT_SCAN_DONE, NULL, 0, 0);
			else
				wifi_indication(WIFI_EVENT_SCAN_RESULT_REPORT, wrqu->data.pointer, wrqu->data.length, 0);
			break;
		case RTWEVMGNTRECV:
			wifi_indication(WIFI_EVENT_RX_MGNT, wrqu->data.pointer, wrqu->data.length, wrqu->data.flags);
			break;
#ifdef REPORT_STA_EVENT
		case RTWEVREGISTERED:
			if(wrqu->addr.sa_family == ARPHRD_ETHER)
				wifi_indication(WIFI_EVENT_STA_ASSOC, wrqu->addr.sa_data, sizeof(null_mac), 0);
			break;
		case RTWEVEXPIRED:
			if(wrqu->addr.sa_family == ARPHRD_ETHER)
				wifi_indication(WIFI_EVENT_STA_DISASSOC, wrqu->addr.sa_data, sizeof(null_mac), 0);
			break;
#endif
		default:
			break;

	}
	
}


int wext_send_eapol(const char *ifname, char *buf, __u16 buf_len, __u16 flags)
{
	struct rtwreq iwr;
	int ret = 0;

	memset(&iwr, 0, sizeof(iwr));
	iwr.u.data.pointer = buf;
	iwr.u.data.length = buf_len;
	iwr.u.data.flags = flags;	
	if (iw_ioctl(ifname, RTKIOCSIWEAPOLSEND, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[RTKIOCSIWEAPOLSEND] error");
		ret = -1;
	}
	return ret;
}

int wext_send_mgnt(const char *ifname, char *buf, __u16 buf_len, __u16 flags)
{
	struct rtwreq iwr;
	int ret = 0;

	memset(&iwr, 0, sizeof(iwr));
	iwr.u.data.pointer = buf;
	iwr.u.data.length = buf_len;
	iwr.u.data.flags = flags;	
	if (iw_ioctl(ifname, RTKIOCSIWMGNTSEND, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[RTKIOCSIWMGNTSEND] error");
		ret = -1;
	}
	return ret;
}

int wext_set_gen_ie(const char *ifname, char *buf, __u16 buf_len, __u16 flags)
{
	struct rtwreq iwr;
	int ret = 0;

	memset(&iwr, 0, sizeof(iwr));
	iwr.u.data.pointer = buf;
	iwr.u.data.length = buf_len;
	iwr.u.data.flags = flags;	
	if (iw_ioctl(ifname, RTKIOCSIWGENIE, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[RTKIOCSIWGENIE] error");
		ret = -1;
	}
	return ret;
}

int wext_set_autoreconnect(const char *ifname, __u8 mode, __u8 retry_times, __u16 timeout)
{
	struct rtwreq iwr;
	int ret = 0;
	__u8 *para = NULL;
	int cmd_len = 0;

	memset(&iwr, 0, sizeof(iwr));
	cmd_len = sizeof("SetAutoRecnt");
	para = rtw_malloc((4) + cmd_len);//size:para_len+cmd_len
	if(para == NULL) return -1;

	//Cmd
	snprintf((char*)para, cmd_len, "SetAutoRecnt");
	//length
	*(para+cmd_len) = mode;	//para1
	*(para+cmd_len+1) = retry_times; //para2
	*(para+cmd_len+2) = timeout; //para3
	
	iwr.u.data.pointer = para;
	iwr.u.data.length = (4) + cmd_len;
	if (iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr) < 0) {
		RTW_API_INFO("\n\rwext_set_autoreconnect():ioctl[RTKIOCDEVPRIVATE] error");
		ret = -1;
	}
	rtw_free(para);
	return ret;
}

int wext_get_autoreconnect(const char *ifname, __u8 *mode)
{
	struct rtwreq iwr;
	int ret = 0;
	__u8 *para = NULL;
	int cmd_len = 0;

	memset(&iwr, 0, sizeof(iwr));
	cmd_len = sizeof("GetAutoRecnt");
	para = rtw_malloc(cmd_len);//size:para_len+cmd_len
	//Cmd
	snprintf((char*)para, cmd_len, "GetAutoRecnt");
	//length
	
	iwr.u.data.pointer = para;
	iwr.u.data.length = cmd_len;
	if (iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr) < 0) {
		RTW_API_INFO("\n\rwext_get_autoreconnect():ioctl[RTKIOCDEVPRIVATE] error");
		ret = -1;
	}
	*mode = *(__u8 *)(iwr.u.data.pointer);
	rtw_free(para);
	return ret;
}

int wext_get_drv_ability(const char *ifname, __u32 *ability)
{
	int ret = 0;
	char * buf = (char *)rtw_zmalloc(33);
	if(buf == NULL) return -1;

	snprintf(buf, 33, "get_drv_ability %x", (unsigned int)ability);
	ret = wext_private_command(ifname, buf, 0);

	rtw_free(buf);
	return ret;
}

#ifdef CONFIG_CUSTOM_IE
int wext_add_custom_ie(const char *ifname, void *cus_ie, int ie_num)
{
	struct rtwreq iwr;
	int ret = 0;
	__u8 *para = NULL;
	int cmd_len = 0;
	if(ie_num <= 0 || !cus_ie){
		RTW_API_INFO("\n\rwext_add_custom_ie():wrong parameter");
		ret = -1;
		return ret;
	}
	memset(&iwr, 0, sizeof(iwr));
	cmd_len = sizeof("SetCusIE");
	para = rtw_malloc((4)* 2 + cmd_len);//size:addr len+cmd_len
	if(para == NULL) return -1;

	//Cmd
	snprintf((char*)para, cmd_len, "SetCusIE");
	//addr length
	*(__u32 *)(para + cmd_len) = (__u32)cus_ie; //ie addr
	//ie_num
	*(__u32 *)(para + cmd_len + 4) = ie_num; //num of ie

	iwr.u.data.pointer = para;
	iwr.u.data.length = (4)* 2 + cmd_len;// 2 input
	if (iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr) < 0) {
		RTW_API_INFO("\n\rwext_add_custom_ie():ioctl[RTKIOCDEVPRIVATE] error");
		ret = -1;
	}
	rtw_free(para);

	return ret;
}

int wext_update_custom_ie(const char *ifname, void * cus_ie, int ie_index)
{
	struct rtwreq iwr;
	int ret = 0;
	__u8 *para = NULL;
	int cmd_len = 0;
	if(ie_index <= 0 || !cus_ie){
		RTW_API_INFO("\n\rwext_update_custom_ie():wrong parameter");
		ret = -1;
		return ret;
	}
	memset(&iwr, 0, sizeof(iwr));
	cmd_len = sizeof("UpdateIE");
	para = rtw_malloc((4)* 2 + cmd_len);//size:addr len+cmd_len
	if(para == NULL) return -1;

	//Cmd
	snprintf((char*)para, cmd_len, "UpdateIE");
	//addr length
	*(__u32 *)(para + cmd_len) = (__u32)cus_ie; //ie addr
	//ie_index
	*(__u32 *)(para + cmd_len + 4) = ie_index; //num of ie

	iwr.u.data.pointer = para;
	iwr.u.data.length = (4)* 2 + cmd_len;// 2 input
	if (iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr) < 0) {
		RTW_API_INFO("\n\rwext_update_custom_ie():ioctl[RTKIOCDEVPRIVATE] error");
		ret = -1;
	}
	rtw_free(para);

	return ret;

}

int wext_del_custom_ie(const char *ifname)
{
	struct rtwreq iwr;
	int ret = 0;
	__u8 *para = NULL;
	int cmd_len = 0;

	memset(&iwr, 0, sizeof(iwr));
	cmd_len = sizeof("DelIE");
	para = rtw_malloc(cmd_len);//size:addr len+cmd_len
	//Cmd
	snprintf((char*)para, cmd_len, "DelIE");
	
	iwr.u.data.pointer = para;
	iwr.u.data.length = cmd_len;
	if (iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr) < 0) {
		RTW_API_INFO("\n\rwext_del_custom_ie():ioctl[RTKIOCDEVPRIVATE] error");
		ret = -1;
	}
	rtw_free(para);

	return ret;


}

#endif

#ifdef CONFIG_AP_MODE
int wext_enable_forwarding(const char *ifname)
{
	struct rtwreq iwr;
	int ret = 0;
	__u8 *para = NULL;
	int cmd_len = 0;

	memset(&iwr, 0, sizeof(iwr));
	cmd_len = sizeof("forwarding_set");
	para = rtw_malloc(cmd_len + 1);
	if(para == NULL) return -1;

	// forwarding_set 1
	snprintf((char *) para, cmd_len, "forwarding_set");
	*(para + cmd_len) = '1';

	iwr.u.essid.pointer = para;
	iwr.u.essid.length = cmd_len + 1;

	if (iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr) < 0) {
		RTW_API_INFO("\n\rwext_enable_forwarding(): ioctl[RTKIOCDEVPRIVATE] error");
		ret = -1;
	}

	rtw_free(para);
	return ret;
}

int wext_disable_forwarding(const char *ifname)
{
	struct rtwreq iwr;
	int ret = 0;
	__u8 *para = NULL;
	int cmd_len = 0;

	memset(&iwr, 0, sizeof(iwr));
	cmd_len = sizeof("forwarding_set");
	para = rtw_malloc(cmd_len + 1);
	if(para == NULL) return -1;

	// forwarding_set 0
	snprintf((char *) para, cmd_len, "forwarding_set");
	*(para + cmd_len) = '0';

	iwr.u.essid.pointer = para;
	iwr.u.essid.length = cmd_len + 1;

	if (iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr) < 0) {
		RTW_API_INFO("\n\rwext_disable_forwarding(): ioctl[RTKIOCDEVPRIVATE] error");
		ret = -1;
	}

	rtw_free(para);
	return ret;

}
#endif

#ifdef CONFIG_CONCURRENT_MODE
int wext_set_ch_deauth(const char *ifname, __u8 enable)
{
	int ret = 0;
	char * buf = (char *)rtw_zmalloc(16);
	if(buf == NULL) return -1;

	snprintf(buf, 16, "SetChDeauth %d", enable);
	ret = wext_private_command(ifname, buf, 0);

	rtw_free(buf);
	return ret;
}
#endif

int wext_set_adaptivity(rtw_adaptivity_mode_t adaptivity_mode)
{
	extern u8 rtw_adaptivity_en;
	extern u8 rtw_adaptivity_mode;

	switch(adaptivity_mode){
		case RTW_ADAPTIVITY_NORMAL:
			rtw_adaptivity_en = 1; // enable adaptivity
			rtw_adaptivity_mode = RTW_ADAPTIVITY_MODE_NORMAL;
			break;
		case RTW_ADAPTIVITY_CARRIER_SENSE:
			rtw_adaptivity_en = 1; // enable adaptivity
			rtw_adaptivity_mode = RTW_ADAPTIVITY_MODE_CARRIER_SENSE;
			break;		
		case RTW_ADAPTIVITY_DISABLE:
		default:
			rtw_adaptivity_en = 0; //disable adaptivity
			break;
	}
	return 0;
}

int wext_set_trp_tis(__u8 enable)
{
	extern u8 rtw_tx_pwr_lmt_enable;
	extern u8 rtw_tx_pwr_by_rate;
	extern u8 rtw_trp_tis_cert_en;
#ifdef CONFIG_POWER_SAVING
	extern u8 rtw_powersave_en;
#endif
	
	if(enable != RTW_TRP_TIS_DISABLE){
		//close the tx power limit and pwr by rate incase the efficiency of Antenna is not good enough.
		rtw_tx_pwr_lmt_enable = 2;//set 0 to disable, set 2 to use efuse value
		rtw_tx_pwr_by_rate = 2;//set 0 to disable, set 2 to use efuse value
		//disable power save.
#ifdef CONFIG_POWER_SAVING
		rtw_powersave_en = 0;
#endif
		if(enable == RTW_TRP_TIS_NORMAL){
			//disable some dynamic mechanism
			rtw_trp_tis_cert_en = BIT0;
		}else if(enable == RTW_TRP_TIS_DYNAMIC){
			rtw_trp_tis_cert_en = BIT1 | BIT0;
		}else if(enable == RTW_TRP_TIS_FIX_ACK_RATE){
			rtw_trp_tis_cert_en = BIT2 | BIT0;
		}
		//you can change autoreconnct mode to RTW_AUTORECONNECT_INFINITE in init_thread function
	}
	return 0;
}

int wext_set_anti_interference(__u8 enable)
{
	extern u8 rtw_anti_interference_en;

	if(enable == ENABLE){
		rtw_anti_interference_en = 1;
	}else{
		rtw_anti_interference_en = 0;
	}

	return 0;
}

int wext_set_ant_div_gpio(__u8 type)
{
	extern u8 rtw_ant_div_gpio_ext;
	
	rtw_ant_div_gpio_ext = type;

	return 0;
}


int wext_set_adaptivity_th_l2h_ini(__u8 l2h_threshold)
{
	extern s8 rtw_adaptivity_th_l2h_ini;
	rtw_adaptivity_th_l2h_ini = (__s8)l2h_threshold;
	return 0;
}

int wext_set_bw40_enable(__u8 enable)
{
    extern u8 rtw_cbw40_enable;
    /* 0: 20 MHz, 1: 40 MHz, 2: 80 MHz, 3: 160MHz, 4: 80+80MHz
    * 2.4G use bit 0 ~ 3, 5G use bit 4 ~ 7
    * 0x21 means enable 2.4G 40MHz & 5G 80MHz */
    extern u8 rtw_bw_mode;
	if(enable == ENABLE){
        rtw_cbw40_enable = 1;
        rtw_bw_mode = 0x11;
	}else{
        rtw_cbw40_enable = 0;
        rtw_bw_mode = 0;
	}

    return 0;
}


int wext_set_uapsd_enable(__u8 enable){
	int ret = 0;
	extern u8 custom_uapsd;
	custom_uapsd = enable;
	return ret;
}

extern int rltk_get_auto_chl(const char *ifname, unsigned char *channel_set, unsigned char channel_num);
int wext_get_auto_chl(const char *ifname, unsigned char *channel_set, unsigned char channel_num)
{
	int ret = -1;
	int channel = 0;
	wext_disable_powersave(ifname);
	if((channel = rltk_get_auto_chl(ifname,channel_set,channel_num)) != 0 )
		ret = channel ;
	wext_enable_powersave(ifname, 1, 1);
	return ret;
}

extern int rltk_set_sta_num(unsigned char ap_sta_num);
int wext_set_sta_num(unsigned char ap_sta_num)
{
	return rltk_set_sta_num(ap_sta_num);
}

extern int rltk_del_station(const char *ifname, unsigned char* hwaddr);
int wext_del_station(const char *ifname, unsigned char* hwaddr)
{
	return rltk_del_station(ifname, hwaddr);
}

extern struct list_head *mf_list_head;
int wext_init_mac_filter(void)
{
	if(mf_list_head != NULL){
		return -1;
	}

	mf_list_head = (struct list_head *)malloc(sizeof(struct list_head));
	if(mf_list_head == NULL){
		RTW_API_INFO("\n\r[ERROR] %s : can't allocate mf_list_head",__func__);
		return -1;
	}

	INIT_LIST_HEAD(mf_list_head);

	return 0;
}

int wext_deinit_mac_filter(void)
{
	if(mf_list_head == NULL){
		return -1;
	}
	struct list_head *iterator;
	rtw_mac_filter_list_t *item;
	list_for_each(iterator, mf_list_head) {
		item = list_entry(iterator, rtw_mac_filter_list_t, node);
		list_del(iterator);
		free(item);
		item = NULL;
		iterator = mf_list_head;
	}

	free(mf_list_head);
	mf_list_head = NULL;
	return 0;
}

int wext_add_mac_filter(unsigned char* hwaddr)
{
	if(mf_list_head == NULL){
		return -1;
	}

	rtw_mac_filter_list_t *mf_list_new;
	mf_list_new =(rtw_mac_filter_list_t *) malloc(sizeof(rtw_mac_filter_list_t));
	if(mf_list_new == NULL){
		RTW_API_INFO("\n\r[ERROR] %s : can't allocate mf_list_new",__func__);
		return -1;
	}
	memcpy(mf_list_new->mac_addr,hwaddr,6);
	list_add(&(mf_list_new->node), mf_list_head);

	return 0;
}

int wext_del_mac_filter(unsigned char* hwaddr)
{
	if(mf_list_head == NULL){
		return -1;
	}

	struct list_head *iterator;
	rtw_mac_filter_list_t *item;
	list_for_each(iterator, mf_list_head) {
		item = list_entry(iterator, rtw_mac_filter_list_t, node);
		if(memcmp(item->mac_addr,hwaddr,6) == 0){
			list_del(iterator);
			free(item);
			item = NULL;
			return 0;
		}
	}
	return -1;
}

extern void rtw_connect_monitor_mgnt(int enable);
void wext_wifi_connect_monitor_mgnt(int enable)
{
	rtw_wifi_connect_monitor_mgnt(enable);
	return;
}

extern void rtw_set_indicate_mgnt(int enable);
void wext_set_indicate_mgnt(int enable)
{
	rtw_set_indicate_mgnt(enable);
	return;
}

void wext_set_softap_gkey_rekey(__u8 mode)
{
	extern uint8_t gk_rekey;
	extern uint32_t gk_rekey_time;
	gk_rekey = mode;
	if (gk_rekey == ENABLE){
		RTW_API_INFO("\n\rSoftAP:Group key rotation enabled");
		RTW_API_INFO("\n\rCurrent key update interval: %dms",gk_rekey_time);
	}
}

#ifdef CONFIG_AP_MODE
extern void rltk_suspend_softap(const char *ifname);
extern void rltk_suspend_softap_beacon(const char *ifname);
void wext_suspend_softap(const char *ifname)
{
	rltk_suspend_softap(ifname);
}

void wext_suspend_softap_beacon(const char *ifname)
{
	rltk_suspend_softap_beacon(ifname);
}
extern int rtw_ap_switch_chl_and_inform(unsigned char new_channel);
int wext_ap_switch_chl_and_inform(unsigned char new_channel)
{
	if(rtw_ap_switch_chl_and_inform(new_channel))
		return RTW_SUCCESS;
	else
		return RTW_ERROR;
}
#endif

#ifdef CONFIG_SW_MAILBOX_EN
int wext_mailbox_to_wifi(const char *ifname, char *buf, __u16 buf_len)
{
	struct rtwreq iwr;
	int ret = 0;

	memset(&iwr, 0, sizeof(iwr));

	iwr.u.data.pointer = buf;
	iwr.u.data.length = buf_len;
	if (iw_ioctl(ifname, RTKIOCSIMAILBOX, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[RTKIOCSIMAILBOX] error");
		ret = -1;
	}
	return ret;
}
#endif

#ifdef CONFIG_WOWLAN
int wext_wowlan_ctrl(const char *ifname, int enable){
	struct rtwreq iwr;
	int ret = 0;
	__u16 pindex = 0;
	__u8 *para = NULL;
	int cmd_len = 0;

	printf("wext_wowlan_ctrl: enable=%d\n\r", enable);
	memset(&iwr, 0, sizeof(iwr));
	cmd_len = sizeof("wowlan_ctrl");

	// Encode parameters as TLV (type, length, value) format
	para = rtw_malloc( cmd_len + (1+1+1) );
	
	snprintf((char*)para, cmd_len, "wowlan_ctrl");
	pindex = cmd_len;

	para[pindex++] = 0; // type 0 wowlan enable disable
	para[pindex++] = 1;
	para[pindex++] = enable;

	iwr.u.data.pointer = para;
	iwr.u.data.length = pindex;

	if (iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[SIOCSIWPRIVWWCTL] error");
		ret = -1;
	}

	rtw_free(para);

	return ret;
}

int wext_wowlan_set_pattern(const char *ifname, wowlan_pattern_t pattern)
{
	struct rtwreq iwr;
	int ret = 0;
	__u16 pindex = 0;
	__u8 *para = NULL;
	int cmd_len = 0;

	memset(&iwr, 0, sizeof(iwr));
	cmd_len = sizeof("wowlan_ctrl");

	para = rtw_malloc( cmd_len + (1+1+1) + sizeof(pattern));
	snprintf((char*)para, cmd_len, "wowlan_ctrl");
	pindex = cmd_len;

	para[pindex++] = 1; // type 1 wowlan set pattern
	para[pindex++] = 2;
	para[pindex++] = sizeof(pattern);
	memcpy(&(para[pindex]), &pattern, sizeof(pattern));

	iwr.u.data.pointer = para;
	iwr.u.data.length = pindex;

	if (iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[SIOCDEVPRIVWWPTN] error");
		ret = -1;
	}

	rtw_free(para);
	
	return ret;
}

int wext_wlan_redl_fw(const char *ifname){
	struct rtwreq iwr;
	int ret = 0;
	__u16 pindex = 0;
	__u8 *para = NULL;
	int cmd_len = 0;

	printf("+ wext_wlan_redl_fw\n\r");
	memset(&iwr, 0, sizeof(iwr));
	cmd_len = sizeof("wowlan_ctrl");

	// Encode parameters as TLV (type, length, value) format
	para = rtw_malloc( cmd_len + (1+1) );
	
	snprintf((char*)para, cmd_len, "wowlan_ctrl");
	pindex = cmd_len;

	para[pindex++] = 2; // type 2 redownload fw
	para[pindex++] = 0;

	iwr.u.data.pointer = para;
	iwr.u.data.length = pindex;

	if (iw_ioctl(ifname, RTKIOCDEVPRIVATE, &iwr) < 0) {
		RTW_API_INFO("\n\rioctl[SIOCSIWPRIVREDLFW] error");
		ret = -1;
	}

	rtw_free(para);

	return ret;
}
#endif

#if defined(CONFIG_RTW_WNM) && defined(CONFIG_LAYER2_ROAMING)
extern u8 set_roam_on_btm;
void wext_set_roam_on_btm(__u8 enable)
{
	set_roam_on_btm = enable;
}
#endif

#ifdef CONFIG_IEEE80211K
extern u8 rtw_enable_80211k;
void wext_set_enable_80211k(__u8 enable)
{
	rtw_enable_80211k = enable;
}
#endif

extern u8 set_exclude_ext_cap;
void wext_exclude_ext_cap(__u8 enable)
{
	set_exclude_ext_cap = enable;
}

#ifdef CONFIG_POWER_SAVING
extern u8 rtw_power_mgnt;
void wext_set_powersave_mode(__u8 ps_mode){
	
	if((ps_mode!=1)&&(ps_mode!=2)){
		printf("\n\rSet powersave mode fail! Wrong powersave mode value, input value can only be 1(min mode) or 2(max mode)");
		return;
	}
	rtw_power_mgnt = ps_mode;
	return;
}
#endif

#ifdef CONFIG_80211N_HT
extern u8 g_tx_ampdu_enable;
void wext_set_wifi_ampdu_tx(__u8 enable){
	g_tx_ampdu_enable = enable;
}
#endif

extern void rtw_get_bcn_country_code(unsigned char* country_code_get);
void wext_get_country_code(unsigned char* country_code_get){
	rtw_get_bcn_country_code(country_code_get);
}

void wext_auto_set_adaptivity(__u8 mode)
{
	extern u8 rtw_auto_adaptivity_en;
	rtw_auto_adaptivity_en = mode;
}

void wext_change_mgnt_datarate(__u8 mode)
{
	extern u8 change_mgnt_datatrate;
	change_mgnt_datatrate = mode;
}

#ifdef CONFIG_SOFTAP_KEEP_SILENT_TABLE
void wext_enable_softap_slient_table(__u8 mode)
{
	extern u8 softap_keep_silent_table_enable;
	softap_keep_silent_table_enable = mode;
}

void wext_set_softap_slient_table_interval(int interval)
{
	extern int softap_keep_silent_table_interval;
	softap_keep_silent_table_interval = interval;
}

void wext_set_custom_country_code(__u8 mode)
{
	extern u8 rtw_set_custom_country_code_en;
	rtw_set_custom_country_code_en = mode;
}

#endif