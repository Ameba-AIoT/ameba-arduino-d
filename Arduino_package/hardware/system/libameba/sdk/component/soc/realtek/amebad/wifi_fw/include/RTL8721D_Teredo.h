#ifndef __WLAN_TEREDO_RAM_H__
#define __WLAN_TEREDO_RAM_H__


//#if CONFIG_TEREDO_REALWOWL

/*--------------------Define --------------------------------------------*/


// below is UDP header offset 
#define NONCE_VALUE_LENGTH      16
#define UDP_SRC_PORT_OFFSET     0
#define UDP_DES_PORT_OFFSET     2
#define UDP_NONCE_OFFSET        4
#define UDP_LENGTH_OFFSET       4
#define UDP_CHECK_SUM_OFFSET    6
#define UDP_HEADER              8

// below is UDP Auth payload offset
#define AUTH_PAYLOAD_BASIC_LEN  13
#define CIDLEN_OFFSET           2
#define AULEN_OFFSET            3
#define NONCE_VALUE_OFFSET      4

// below is UDP Origin payload offset
#define ORIGIN_PAYLOAD_LEN        8
#define ORIGIN_IPv4_ADRESS_OFFSET 4


// below is IPv6 header offset 
#define IPv6_ALEN               16
#define IPv6_NEXT_HEADER_OFFSET 6
#define IPv6_SRC_ADDRESS_OFFSET 8
#define CLIENT_PORT_IN_SRC_IPv6_OFFSET (IPv6_SRC_ADDRESS_OFFSET+10)
#define CLIENT_IPv4_IN_SRC_IPv6_OFFSET (IPv6_SRC_ADDRESS_OFFSET+12)
#define IPv6_DES_ADDRESS_OFFSET 24
#define IPv6_HEADER             40

// below is IPv6 header offset 
#define ICMPV6_TYPE_OFFSET             0
#define ICMPV6_CHECKSUM_OFFSET         2
#define ICMPV6_TARGETADDRESS_OFFSET        8
#define ICMPV6_OPTION_TYPE_OFFSET      24
#define ICMPV6_OPTION_LENGTH_OFFSET    25
#define ICMPV6_OPTION_TARGET_ADDR_OFFSET    26
#define ICMPV6_HEADER           32
#define ICMP_CHECKSUM_SRC_ADDR_OFFSET    0
#define ICMP_CHECKSUM_DES_ADDR_OFFSET    72


#define START_RS_TIMEOUT        500  // 5secs
#define ISSUE_RS_TIMEOUT        3000 // 30secs  
#define CHECK_RA_TIMEOUT        400 //4sesc
#define BLOCK_RX_DMA            2000

#define RS_RETRY_LIMIT          4

#define IP_TYPE                 0x45
#define IPv6_TYPE               0x60
#define IPv6_NO_NEXT_HEADER     0x3b
#define OBFUSCATED_VALUE_DECODE 0xFF
#define WAKE_UP_UDP_DES_PORT    0x8618
#define WAKE_UP_TYPE            0x0100

/*--------------------Define Enum---------------------------------------*/
/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/

typedef struct _NETWORK_INFO_ {
    u8 IPv4[4];
    u16 UDPport;
    u8 IPv6[16];     
}NETWORK_INFO,*PNETWORK_INFO;

typedef struct _TEREDO_SERVER_ {
    NETWORK_INFO teredo_info;
}TEREDO_SERVER,*PTEREDO_SERVER;

typedef struct _NIC_CLIENT_ {
    NETWORK_INFO NIC_info;
    u8 publicIP[4];
}NIC_CLIENT,*PNIC_CLIENT;

typedef struct _REALWOWLAN_ENABLE_ {
    u8 fun_En:1;
    u8 rsvd:7;
}REALWOWLAN_ENABLE,*PREALWOWLAN_ENABLE;


typedef struct _REALWOWLAN_ {
    REALWOWLAN_ENABLE   enable;
    u8       loc_router_solicitation ;
    u8       loc_bubble_packet;
    u8       loc_teredo_info;
}REALWOWLAN,*PREALWOWLAN;


typedef struct _TEREDO_INFO_ {
    u8 verifyPattern[16];  // verifyPattern
    u32 DHCPExpireTime; // 4byte
    u8 nonceValue[8]; // nonce value    
    u16 IPIdentifier;
    TEREDO_SERVER teredoServer;
    NIC_CLIENT    NIC;
    u8  RSretryCount;
    u8  RAvalid;
}TEREDO_INFO,*PTEREDO_INFO;

typedef enum _RX_BUBBLE_PACKET_MODE_ {
    INDIRECT_BUBBLE    = 0,
    DIRECT_BUBBLE      = 1,    
}RX_BUBBLE_PACKET_MODE,*PRX_BUBBLE_PACKET_MODE;

/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/


void 
InitRealWoWLAN(
    void
) ;

void 
parseRSPktInfo(
    void
) ;

void 
initBubblePacket(
    void
) ;

void 
updateIPID(
    IN u16 offset
) ;

u8 
checkNICIPv4(
    IN u16 offset
) ;

u8 
checkNICUDPPort(
    IN u16 offset
) ;

u8 
checkTeredoIPv4(
    IN u16 offset
) ;

u8 
checkTeredoUDPPort(
    IN u16 offset
) ;

void 
updateCheckSum(
    IN u8 mode,
    IN u16 offset,
    IN u8 loc_rsvd
) ;

void 
fillBubblePacket(
    IN u8 bDirectBubble,
    IN u16 rx_offset
) ;

u8 
isOriginIndication(
    IN u16 offset
) ;

void 
getTeredoInfo(
    void
) ;

extern u16 
getCheckSum(
    IN u8 mode,
    IN u16 pktLen,
    IN u8 *src_addr,
    IN u8 *dest_addr,
    IN u32 input
) ;

extern u32 
getSumValue(
    IN u8 *buff,
    IN u16 pktLen
) ;

extern u8 
OnRouterAderisement(
    IN u8 mac_header_offset,
    IN u8 macHeaderLen,
    IN u8 rxPageHighAddr
) ;

extern u8 
OnWakeUpPkt(
    IN u8 mac_header_offset,
    IN u8 macHeaderLen,
    IN u8 rxPageHighAddr
) ;

extern u8 
OnBubblePkt(
    IN u8 mac_header_offset,
    IN u8 macHeaderLen,
    IN u8 rxPageHighAddr
) ;

extern void 
issueBubblePkt(
    void
) ;

extern void 
issueRSPkt(
    IN u8 isRetry
) ;

extern void 
enableRealWowLan(
    void
) ;

extern void 
disableRealWowLan(
    void
) ;

//#endif //#if CONFIG_TEREDO_REALWOWL 

#endif //#ifndef __WLAN_TEREDO_RAM_H__

