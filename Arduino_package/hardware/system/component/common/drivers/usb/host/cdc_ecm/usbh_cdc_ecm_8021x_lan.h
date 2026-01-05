#ifndef USBH_CDC_ECM_8021X_LAN
#define USBH_CDC_ECM_8021X_LAN

#define EAP_TLS_FLAG	1
#define EAP_TLS_LEN		4
#define LIB1X_EAPOL_HDRLEN		4
#define EAP_HLEN		5
#define ETH_ALEN	6
#define ETH_HLEN		14

#define LIB1X_ETHER_EAPOL_TYPE		0x888E
#define LIB1X_ETHER_EAPOL_VERSION	0x0303

#define LIB1X_EAPOL_VER				1	//0000 0001B
#define LIB1X_EAPOL_EAPPKT			0	//0000 0000B
#define LIB1X_EAPOL_START			1	//0000 0001B
#define LIB1X_EAPOL_LOGOFF			2	//0000 0010B
#define LIB1X_EAPOL_KEY				3	//0000 0011B
#define LIB1X_EAPOL_ENCASFALERT		4	//0000 0100B

#define LIB1X_EAP_CODE_REQUEST			(1)
#define LIB1X_EAP_CODE_RESPONSE			(2)
#define LIB1X_EAP_CODE_SUCCESSFUL		(3)
#define LIB1X_EAP_CODE_FAIL				(4)

#define LIB1X_EAP_TYPE_IDENTITY			(1)
#define LIB1X_EAP_TYPE_NOTIFICATION		(2)
#define LIB1X_EAP_TYPE_NAK				(3)
#define LIB1X_EAP_TYPE_EXPANDED			(254)	/* Wi-Fi Simple Configuation(WSC) */

#define LIB1X_EAP_TYPE_MD5_CHALLENGE	(4)
#define LIB1X_EAP_AUTH_TYPE_TLS			(13) // 0x0d
#define LIB1X_EAP_AUTH_TYPE_PEAP		(25) // 0x19
#define LIB1X_EAP_CHANGE_CIPHERSPEC		(20) // 0x14
#define LIB1X_EAP_MSCHAPV2				(26) // 0x1A
#define LIB1X_EAP_TLV					(33) // 0x21

#define LIB1X_EAP_HANDSHAKE								(22) // 0x16
#define LIB1X_EAP_APPLICATION_DATA						(23) // 0x17
#define LIB1X_EAP_FLAG_START_NONE						(0) // 0x00
#define LIB1X_EAP_FLAG_START_TRUE						(32) // 0x20
#define LIB1X_EAP_FLAG_MORE_FRAGMENTS					(64) // 0x40
#define LIB1X_EAP_FLAG_LENGTH_INCLUDED					(128) // 0x80
#define LIB1X_EAP_FLAG_START_TRUE_LENGTH_INCLUDED		(160) // 0xa0
#define LIB1X_EAP_FLAG_LENGTH_INCLUDED_MORE_FRAGMENTS	(192) // 0xC0

#define LIB1X_EAP_RESERVED_FILED_LEN		(8) // 0x08
#define LIB1X_EAP_PEER_CHALLENGE_LEN		(16) // 0x10
#define LIB1X_EAP_AUTH_CHALLENGE_LEN		(16) // 0x10
#define LIB1X_EAP_AUTH_RESPONSE_LEN			(20) // 0x14
#define LIB1X_EAP_NT_CHALLENGE_LEN			(24) // 0x18
#define LIB1X_EAP_AUTH_RESPONSE_IN_HEX_LEN	(40) // 0x28

#define LIB1X_EAP_TLV_FLAG_END	(128) // 0x80

typedef enum {
	RECV_NONE = 0,
	RECV_REQUEST_IDENTITY,
	RECV_REQUEST_MD5_CHALLENGE,
	RECV_TO_START_HANDSHAKE_TLS,
	RECV_TO_START_HANDSHAKE_PEAP,
	RECV_SERVER_HELLO,
	RECV_CHANGE_CIPHERSPEC,
	RECV_APPLICATION_DATA
} EapolRecvState;

struct eap_body {
	unsigned char  eap_code;
	unsigned char  eap_id;
	unsigned short eap_length;
	unsigned char  eap_type;
	//eap_identity[0] is flag, eap_identity[1-5] is data_length
	unsigned char  eap_identity[];
};

struct ethernet_eapol_hdr_data
{
	unsigned char		daddr[ETH_ALEN];
	unsigned char		saddr[ETH_ALEN];
	unsigned short		type;
	unsigned char		protocol_version;
	unsigned char		packet_type;
	unsigned short		packet_body_length;
	struct eap_body		eap_body;
};


#pragma pack(push, 1)
struct mschapv2_challenge_data
{
	unsigned char		eap_type;
	unsigned char		opcode;
 	unsigned char		id;
	unsigned short		packet_length;
	unsigned char		value_size;
	unsigned char		peer_challenge[16];
	unsigned char		reserved[8];
	unsigned char		nt_response[24];
	unsigned char		flag;
	char				identity[];
};

struct eap_tlv
{
	unsigned char		code;
	unsigned char		id;
	unsigned short		packet_length;
	unsigned char		type;
	unsigned char		tlv_flag;
	unsigned char		tlv_type;
	unsigned short		tlv_length;
	unsigned short		tlv_value;
};
#pragma pack(pop)


#endif //USBH_CDC_ECM_8021X_LAN