/**
 * @file
 * repeater handle
 */

#include "netif/bridgeif.h"
#include "lwip/netif.h"
#include "lwip/sys.h"
#include "lwip/ip.h"
#include "lwip/etharp.h"
#include "lwip/ethip6.h"
#include "lwip/udp.h"
#include "lwip/prot/dhcp.h"



#include "lwip/dhcp.h"
#include "lwip/snmp.h"
#include "lwip/timeouts.h"
#include <string.h>
#include <osdep_service.h>



#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
/** minimum set of fields of any DHCP message */
struct rpt_dhcp_msg
{
  PACK_STRUCT_FLD_8(u8_t op);
  PACK_STRUCT_FLD_8(u8_t htype);
  PACK_STRUCT_FLD_8(u8_t hlen);
  PACK_STRUCT_FLD_8(u8_t hops);
  PACK_STRUCT_FIELD(u32_t xid);
  PACK_STRUCT_FIELD(u16_t secs);
  PACK_STRUCT_FIELD(u16_t flags);
  PACK_STRUCT_FLD_S(ip4_addr_p_t ciaddr);
  PACK_STRUCT_FLD_S(ip4_addr_p_t yiaddr);
  PACK_STRUCT_FLD_S(ip4_addr_p_t siaddr);
  PACK_STRUCT_FLD_S(ip4_addr_p_t giaddr);
  PACK_STRUCT_FLD_8(u8_t chaddr[16]);
  PACK_STRUCT_FLD_8(u8_t sname[64]);
  PACK_STRUCT_FLD_8(u8_t file[128]);
  PACK_STRUCT_FIELD(u32_t cookie);
  PACK_STRUCT_FLD_8(u8_t options[308]);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/epstruct.h"
#endif



/** This is the aligned version of ip6_addr_t,
    used as local variable, on the stack, etc. */
struct br_rpt_ip6_addr {
  u32_t addr[4];
};

/** IPv6 address */
typedef struct br_rpt_ip6_addr br_rpt_ip6_addr_t;

#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
struct br_rpt_ip6_hdr {
  /** version / traffic class / flow label */
  PACK_STRUCT_FIELD(u32_t _v_tc_fl);
  /** payload length */
  PACK_STRUCT_FIELD(u16_t _plen);
  /** next header */
  PACK_STRUCT_FLD_8(u8_t _nexth);
  /** hop limit */
  PACK_STRUCT_FLD_8(u8_t _hoplim);
  /** source and destination IP addresses */
  PACK_STRUCT_FLD_S(struct br_rpt_ip6_addr src);
  PACK_STRUCT_FLD_S(struct br_rpt_ip6_addr dest);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/epstruct.h"
#endif

/** This is the standard ICMP6 header. */
#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
struct br_rpt_icmp6_hdr {
  PACK_STRUCT_FLD_8(u8_t type);
  PACK_STRUCT_FLD_8(u8_t code);
  PACK_STRUCT_FIELD(u16_t chksum);
  PACK_STRUCT_FIELD(u32_t data);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/epstruct.h"
#endif

/** Access address in 16-bit block */
#define IP6_ADDR_BLOCK1(ip6addr) ((u16_t)((lwip_htonl((ip6addr)->addr[0]) >> 16) & 0xffff))
/** Access address in 16-bit block */
#define IP6_ADDR_BLOCK2(ip6addr) ((u16_t)((lwip_htonl((ip6addr)->addr[0])) & 0xffff))
/** Access address in 16-bit block */
#define IP6_ADDR_BLOCK3(ip6addr) ((u16_t)((lwip_htonl((ip6addr)->addr[1]) >> 16) & 0xffff))
/** Access address in 16-bit block */
#define IP6_ADDR_BLOCK4(ip6addr) ((u16_t)((lwip_htonl((ip6addr)->addr[1])) & 0xffff))
/** Access address in 16-bit block */
#define IP6_ADDR_BLOCK5(ip6addr) ((u16_t)((lwip_htonl((ip6addr)->addr[2]) >> 16) & 0xffff))
/** Access address in 16-bit block */
#define IP6_ADDR_BLOCK6(ip6addr) ((u16_t)((lwip_htonl((ip6addr)->addr[2])) & 0xffff))
/** Access address in 16-bit block */
#define IP6_ADDR_BLOCK7(ip6addr) ((u16_t)((lwip_htonl((ip6addr)->addr[3]) >> 16) & 0xffff))
/** Access address in 16-bit block */
#define IP6_ADDR_BLOCK8(ip6addr) ((u16_t)((lwip_htonl((ip6addr)->addr[3])) & 0xffff))


extern u16_t inet_chksum(const void *dataptr, u16_t len);

#define ETH_ALEN	6		/* Octets in one ethernet addr	 */
#define ETH_HLEN	14		/* Total octets in header.	 */
#define ETH_P_IP	0x0800		/* Internet Protocol packet	*/
#define ETH_P_ARP	0x0806		/* Address Resolution packet	*/
#define ETH_P_IPV6	0x86DD		/* IPv6 over bluebook		*/
#define NAT25_HASH_BITS		4
#define NAT25_HASH_SIZE		(1 << NAT25_HASH_BITS)
#define NAT25_AGEING_TIME	300

#define IPPROTO_ICMPV6		58	/* ICMPv6*/	
#define MAX_NETWORK_ADDR_LEN	17
#define NAT25_IPV4		01
#define NAT25_IPV6		02
/*
 *	ICMP codes for neighbour discovery messages
 */

#define NDISC_ROUTER_SOLICITATION	133
#define NDISC_ROUTER_ADVERTISEMENT	134
#define NDISC_NEIGHBOUR_SOLICITATION	135
#define NDISC_NEIGHBOUR_ADVERTISEMENT	136
#define NDISC_REDIRECT			137


struct nat25_network_db_entry
{
  struct nat25_network_db_entry	*next_hash;
  struct nat25_network_db_entry	**pprev_hash;
  atomic_t	use_count;
  unsigned char	macAddr[6];
  unsigned long	ageing_timer;
  unsigned char   networkAddr[MAX_NETWORK_ADDR_LEN];
};

enum NAT25_METHOD {
  NAT25_MIN,
  NAT25_CHECK,
  NAT25_INSERT,
  NAT25_LOOKUP,
  NAT25_PARSE,
  NAT25_MAX
};
struct nat25_network_db_entry	*rpt_nethash[NAT25_HASH_SIZE];
unsigned char rpt_br_ip[4];
unsigned char rpt_br_mac[ETH_ALEN];
unsigned char rpt_ukpro_mac[ETH_ALEN];	// mac address of unknown protocol
unsigned char rpt_ukpro_mac_valid;		// if the above entry is valid
struct nat25_network_db_entry *rpt_nethash_handle=NULL;


void dhcp_dst_bcast(struct pbuf *pd);
#define FOLD_U32T(u)          (((u) >> 16) + ((u) & 0x0000ffffUL))
#define SWAP_BYTES_IN_WORD(w) (((w) & 0xff) << 8) | (((w) & 0xff00) >> 8)
extern u16_t lwip_standard_chksum(const void *dataptr, int len);
static u16_t cksum_pseudo_base(uint8_t *payload, u8_t proto, u16_t proto_len, u32_t acc)
{
 
  u8_t swapped = 0;
  acc += lwip_standard_chksum(payload, proto_len);
  acc = FOLD_U32T(acc);
  if (proto_len % 2 != 0) {
      swapped = 1 - swapped;
      acc = SWAP_BYTES_IN_WORD(acc);
  }
  
  if (swapped) {
    acc = SWAP_BYTES_IN_WORD(acc);
  }

  acc += (u32_t)lwip_htons((u16_t)proto);
  acc += (u32_t)lwip_htons(proto_len);

  /* Fold 32-bit sum to 16 bits
     calling this twice is probably faster than if statements... */
  acc = FOLD_U32T(acc);
  acc = FOLD_U32T(acc);
  //LWIP_DEBUGF(INET_DEBUG, ("inet_chksum_pseudo(): pbuf chain lwip_chksum()=%"X32_F"\n", acc));
  return (u16_t)~(acc & 0xffffUL);
}

u16_t ipcmp6_chksum_pseudo(uint8_t *payload, u8_t proto, u16_t proto_len,u32_t *src, u32_t *dest)
{
  u32_t acc = 0;
  u32_t addr;
  u8_t addr_part;

  for (addr_part = 0; addr_part < 4; addr_part++) {
    addr = src[addr_part];
    acc += (addr & 0xffffUL);
    acc += ((addr >> 16) & 0xffffUL);
    addr = dest[addr_part];
    acc += (addr & 0xffffUL);
    acc += ((addr >> 16) & 0xffffUL);
  }
  /* fold down to 16 bits */
  acc = FOLD_U32T(acc);
  acc = FOLD_U32T(acc);

  return cksum_pseudo_base(payload, proto, proto_len, acc);
}

static uint32_t checksum32(uint32_t start_value, uint8_t *data, size_t len)
{
    uint32_t checksum32 = start_value;
    uint16_t data16 = 0;
    int i;

    for(i = 0; i < (len / 2 * 2); i += 2) {
            data16 = (data[i] << 8) | data[i + 1];
            checksum32 += data16;
    }

    if(len % 2) {
            data16 = data[len - 1] << 8;
            checksum32 += data16;
    }

    return checksum32;
}

static uint16_t checksum32to16(uint32_t checksum32)
{
  uint16_t checksum16 = 0;

  checksum32 = (checksum32 >> 16) + (checksum32 & 0x0000ffff);
  checksum32 = (checksum32 >> 16) + (checksum32 & 0x0000ffff);
  checksum16 = (uint16_t) ~(checksum32 & 0xffff);

  return checksum16;
}
u16_t calculate_chksum_pseudo(uint8_t *payload, struct udp_hdr *uhdr, uint8_t protocol, u16_t ulen,u32_t *src, u32_t *dest)
{
  uint32_t udp_checksum32 = 0;
  uint16_t udp_checksum16 = 0;
  
  uint8_t *uhdr_data=NULL;
  // pseudo header 
  uint8_t pseudo_header[12] = {/*srcip*/ 0x00, 0x00, 0x00, 0x00 /*srcip*/, /*dstip*/ 0x00, 0x00, 0x00, 0x00 /*dstip*/, 0x00, /*protocol*/ 0x00 /*protocol*/, /*l4len*/ 0x00, 0x00 /*l4len*/};
  uhdr_data = (uint8_t *)uhdr;
  memcpy(&pseudo_header[0], src, 4);
  memcpy(&pseudo_header[4], dest, 4);
  pseudo_header[8] = 0x00;
  pseudo_header[9] = protocol;
  
  pseudo_header[10] = (uint8_t) (ulen >> 8);
  pseudo_header[11] = (uint8_t) (ulen & 0xff);
  
  udp_checksum32 = checksum32(udp_checksum32, pseudo_header, sizeof(pseudo_header));
  udp_checksum32 = checksum32(udp_checksum32, uhdr_data, sizeof(struct udp_hdr));
  udp_checksum32 = checksum32(udp_checksum32, payload, (ulen-8));
  udp_checksum16 = checksum32to16(udp_checksum32);
  return udp_checksum16;
}

#if 0
u16_t calculate_icmpv6_chksum_pseudo(uint8_t *payload, struct br_rpt_icmp6_hdr *icmp6, uint8_t protocol, u16_t ulen,u32_t *src, u32_t *dest)
{
  uint32_t icmpv6_checksum32 = 0;
  uint16_t icmpv6_checksum16 = 0;
  uint32_t uprotocol_len = ulen;
  uint32_t uprotocol = protocol;
  
  // pseudo header 
  uint8_t pseudo_header[40] = {0};
  
  memcpy(&pseudo_header[0], src, 16);
  memcpy(&pseudo_header[16], dest, 16);
  //pseudo_header[32] = (uint8_t) (ulen & 0xff);
  //pseudo_header[33] = (uint8_t) (ulen >> 8);
  printf("\n\r uprotocol_len=%08X ulen=%04X, %08X", uprotocol_len, ulen, lwip_htonl(uprotocol_len));
  pseudo_header[32] = 0;
  pseudo_header[33] = 0;
  pseudo_header[35] = (uint8_t) (ulen >> 8);
  pseudo_header[34] =  (uint8_t) (ulen & 0xff); 
  //uprotocol_len = lwip_htonl(uprotocol_len);
  //memcpy(&pseudo_header[32], &uprotocol_len, 4);
  //pseudo_header[36] = 0;
  //pseudo_header[37] = 0;
  //pseudo_header[38] = 0;
  //pseudo_header[39] = protocol;
  //uprotocol = lwip_htonl(uprotocol);
  //memcpy(&pseudo_header[36], &uprotocol, 4);
  pseudo_header[36] = protocol;
  
  icmpv6_checksum32 = checksum32(icmpv6_checksum32, pseudo_header, sizeof(pseudo_header));
  
  icmpv6_checksum32 = checksum32(icmpv6_checksum32, payload, (ulen));
  icmpv6_checksum16 = checksum32to16(icmpv6_checksum32);
  return icmpv6_checksum16;
}
#endif
#if 0 //for debug only
static void __nat25_db_print(void)
{
    int i, j;
    struct nat25_network_db_entry *db;
    for(i=0, j=0; i<NAT25_HASH_SIZE; i++)
    {
      db = br_priv->nethash[i];

        while (db != NULL)
        {
            printf("\n\rNAT25: DB(%d) H(%02d) "
            "M:%02x%02x%02x%02x%02x%02x "
            "N:%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
            j,
            i,
            db->macAddr[0],
            db->macAddr[1],
            db->macAddr[2],
            db->macAddr[3],
            db->macAddr[4],
            db->macAddr[5],
            db->networkAddr[0],
            db->networkAddr[1],
            db->networkAddr[2],
            db->networkAddr[3],
            db->networkAddr[4],
            db->networkAddr[5],
            db->networkAddr[6],
            db->networkAddr[7],
            db->networkAddr[8],
            db->networkAddr[9],
            db->networkAddr[10],
            db->networkAddr[11],
            db->networkAddr[12],
            db->networkAddr[13],
            db->networkAddr[14],
            db->networkAddr[15],
            db->networkAddr[16]);
            j++;
            db = db->next_hash;
        }
    }
}

#endif

int IS_MCAST(unsigned char *da)
{
  if ((*da) & 0x01)
    return TRUE;
  else
    return FALSE;
}
static void __network_hash_link(struct nat25_network_db_entry *ent, int hash)
{
  ent->next_hash = rpt_nethash[hash];
  if(ent->next_hash != NULL)
          ent->next_hash->pprev_hash = &ent->next_hash;
  rpt_nethash[hash] = ent;
  ent->pprev_hash = &rpt_nethash[hash];
}


static void __network_hash_unlink(struct nat25_network_db_entry *ent)
{
  *(ent->pprev_hash) = ent->next_hash;
  if(ent->next_hash != NULL)
          ent->next_hash->pprev_hash = ent->pprev_hash;
  ent->next_hash = NULL;
  ent->pprev_hash = NULL;
}


static int __nat25_network_hash(unsigned char *networkAddr)
{
    if(networkAddr[0] == NAT25_IPV4)
    {
        unsigned long x;

        x = networkAddr[7] ^ networkAddr[8] ^ networkAddr[9] ^ networkAddr[10];

        return x & (NAT25_HASH_SIZE - 1);
    }
    else if(networkAddr[0] == NAT25_IPV6)
    {
        unsigned long x;

        x = networkAddr[1] ^ networkAddr[2] ^ networkAddr[3] ^ networkAddr[4] ^ networkAddr[5] ^
                networkAddr[6] ^ networkAddr[7] ^ networkAddr[8] ^ networkAddr[9] ^ networkAddr[10] ^
                networkAddr[11] ^ networkAddr[12] ^ networkAddr[13] ^ networkAddr[14] ^ networkAddr[15] ^
                networkAddr[16];

        return x & (NAT25_HASH_SIZE - 1);
    }

    else
    {
        unsigned long x = 0;
        int i;

        for (i=0; i<MAX_NETWORK_ADDR_LEN; i++)
                x ^= networkAddr[i];

        return x & (NAT25_HASH_SIZE - 1);
    }
}

static void __nat25_generate_ipv4_network_addr(unsigned char *networkAddr,unsigned int *ipAddr)
{
  memset(networkAddr, 0, MAX_NETWORK_ADDR_LEN);

  networkAddr[0] = NAT25_IPV4;
  memcpy(networkAddr+7, (unsigned char *)ipAddr, 4);
}


static  void __nat25_generate_ipv6_network_addr(unsigned char *networkAddr,unsigned int *ipAddr)
{
  memset(networkAddr, 0, MAX_NETWORK_ADDR_LEN);

  networkAddr[0] = NAT25_IPV6;
  memcpy(networkAddr+1, (unsigned char *)ipAddr, 16);
}

static unsigned char *scan_tlv(unsigned char *data, int len, unsigned char tag, unsigned char len8b)
{
    while (len > 0) {
      if (*data == tag && *(data+1) == len8b && len >= len8b*8)
        return data+2;

      len -= (*(data+1))*8;
      data += (*(data+1))*8;
    }
    return NULL;
}



int update_nd_link_layer_addr(unsigned char *data, int len, unsigned char *replace_mac)

{
  struct br_rpt_icmp6_hdr *icmphdr = (struct br_rpt_icmp6_hdr *)data;
  unsigned char *mac;

  if (icmphdr->type == NDISC_ROUTER_SOLICITATION) {
    if (len >= 8) {
      mac = scan_tlv(&data[8], len-8, 1, 1);
      if (mac) {
#if 0        
        printf("\n\rRouter Solicitation, "
        "replace MAC From: %02x:%02x:%02x:%02x:%02x:%02x, "
        "To: %02x:%02x:%02x:%02x:%02x:%02x",
        mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],
        replace_mac[0],replace_mac[1],replace_mac[2],replace_mac[3],replace_mac[4],replace_mac[5]);
#endif        
        memcpy(mac, replace_mac, 6);
        return 1;
      }
    }
  }
  else if (icmphdr->type == NDISC_ROUTER_ADVERTISEMENT) {
    if (len >= 16) {
      mac = scan_tlv(&data[16], len-16, 1, 1);
      if (mac) {
#if 0        
        printf("\n\rRouter Advertisement, "
        "replace MAC From: %02x:%02x:%02x:%02x:%02x:%02x, "
        "To: %02x:%02x:%02x:%02x:%02x:%02x",
        mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],
        replace_mac[0],replace_mac[1],replace_mac[2],replace_mac[3],replace_mac[4],replace_mac[5]);
#endif
        memcpy(mac, replace_mac, 6);
        return 1;
      }
    }
  }
  else if (icmphdr->type == NDISC_NEIGHBOUR_SOLICITATION) {
    if (len >= 24) {
      mac = scan_tlv(&data[24], len-24, 1, 1);
      if (mac) {
#if 0        
        printf("\n\rNeighbor Solicitation, "
        "replace MAC From: %02x:%02x:%02x:%02x:%02x:%02x, "
        "To: %02x:%02x:%02x:%02x:%02x:%02x\n",
        mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],
        replace_mac[0],replace_mac[1],replace_mac[2],replace_mac[3],replace_mac[4],replace_mac[5]);
#endif
        memcpy(mac, replace_mac, 6);
        return 1;
      }
    }
  }
  else if (icmphdr->type == NDISC_NEIGHBOUR_ADVERTISEMENT) {
    if (len >= 24) {
      mac = scan_tlv(&data[24], len-24, 2, 1);
      if (mac) {
#if 0        
        printk("\n\rNeighbor Advertisement, "
        "replace MAC From: %02x:%02x:%02x:%02x:%02x:%02x, "
        "To: %02x:%02x:%02x:%02x:%02x:%02x",
        mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],
        replace_mac[0],replace_mac[1],replace_mac[2],replace_mac[3],replace_mac[4],replace_mac[5]);
#endif
        memcpy(mac, replace_mac, 6);
        return 1;
      }
    }
  }
  else if (icmphdr->type == NDISC_REDIRECT) {
    if (len >= 40) {
      mac = scan_tlv(&data[40], len-40, 2, 1);
      if (mac) {
#if 0        
        printf("\n\rRedirect,  "
        "replace MAC From: %02x:%02x:%02x:%02x:%02x:%02x, "
        "To: %02x:%02x:%02x:%02x:%02x:%02x",
        mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],
        replace_mac[0],replace_mac[1],replace_mac[2],replace_mac[3],replace_mac[4],replace_mac[5]);
#endif
        memcpy(mac, replace_mac, 6);
        return 1;
      }
    }
  }
  return 0;
}
void convert_ipv6_mac_to_mc(struct pbuf *pd)
{
  unsigned char *data = pd->payload;
  struct br_rpt_ip6_hdr *ip6h = (struct br_rpt_ip6_hdr *)(data + ETH_HLEN);
  unsigned char *dst_mac = data;
  unsigned int addr3 = ip6h->src.addr[3];
      
  dst_mac[0] = 0x33;
  dst_mac[1] = 0x33;
  memcpy(&dst_mac[2], &addr3, 4);

}



static  int  __nat25_has_expired(struct nat25_network_db_entry *fdb)
{
  int check_use_count=0;
  check_use_count = ATOMIC_READ(&fdb->use_count);
  
  if (check_use_count > 0 )
    return 0;
  else 
    return 1;
}


void nat25_db_expire(void)
{
    int i;
    for (i=0; i<NAT25_HASH_SIZE; i++)
    {
      struct nat25_network_db_entry *f;
      f = rpt_nethash[i];

        while (f != NULL)
        {
            struct nat25_network_db_entry *g;
            g = f->next_hash;

            if(!__nat25_has_expired(f))
            {
                    if(ATOMIC_DEC_AND_TEST(&f->use_count))
                    {
#if 0
                      printk("\n\rNAT25 Expire timeout H(%02d) "
                      "M:%02x%02x%02x%02x%02x%02x "
                      "N:%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
                      i,
                      f->macAddr[0],
                      f->macAddr[1],
                      f->macAddr[2],
                      f->macAddr[3],
                      f->macAddr[4],
                      f->macAddr[5],
                      f->networkAddr[0],
                      f->networkAddr[1],
                      f->networkAddr[2],
                      f->networkAddr[3],
                      f->networkAddr[4],
                      f->networkAddr[5],
                      f->networkAddr[6],
                      f->networkAddr[7],
                      f->networkAddr[8],
                      f->networkAddr[9],
                      f->networkAddr[10],
                      f->networkAddr[11],
                      f->networkAddr[12],
                      f->networkAddr[13],
                      f->networkAddr[14],
                      f->networkAddr[15],
                      f->networkAddr[16]);
#endif                      
                      __network_hash_unlink(f);
                      mem_free(f);
                      f = NULL;
                  }
            }
          f = g;
        }
    }
}


static int __nat25_db_network_lookup_and_replace(struct pbuf *pd, unsigned char *networkAddr)
{
    struct nat25_network_db_entry *db;
    unsigned char *data = pd->payload;
    db = rpt_nethash[__nat25_network_hash(networkAddr)];
    while (db != NULL)
    {
      if(!memcmp(db->networkAddr, networkAddr, MAX_NETWORK_ADDR_LEN))
      {
          if(!__nat25_has_expired(db))
          {
                              
            // replace the destination mac address
            memcpy(data, db->macAddr, ETH_ALEN);
            ATOMIC_SET(&db->use_count, NAT25_AGEING_TIME);
#if 0 //for debug only
            printf("\n\rNAT25: Lookup DB M:%02x%02x%02x%02x%02x%02x "
              "N:%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
              db->macAddr[0],
              db->macAddr[1],
              db->macAddr[2],
              db->macAddr[3],
              db->macAddr[4],
              db->macAddr[5],
              db->networkAddr[0],
              db->networkAddr[1],
              db->networkAddr[2],
              db->networkAddr[3],
              db->networkAddr[4],
              db->networkAddr[5],
              db->networkAddr[6],
              db->networkAddr[7],
              db->networkAddr[8],
              db->networkAddr[9],
              db->networkAddr[10],
              db->networkAddr[11],
              db->networkAddr[12],
              db->networkAddr[13],
              db->networkAddr[14],
              db->networkAddr[15],
              db->networkAddr[16]);
#endif                                
        }
        return 1;
    }else {
        //printf("\n\rNot Found match networkaddress");
    }

    db = db->next_hash;
    }
#if 0 //for debug only
    if (db ==NULL) {
      printf("\n\rNAT25: DB NOT FOUND "
      "N:%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x ",
        networkAddr[0],
        networkAddr[1],
        networkAddr[2],
        networkAddr[3],
        networkAddr[4],
        networkAddr[5],
        networkAddr[6],
        networkAddr[7],
        networkAddr[8],
        networkAddr[9],
        networkAddr[10],
        networkAddr[11],
        networkAddr[12],
        networkAddr[13],
        networkAddr[14],
        networkAddr[15],
        networkAddr[16]);
      //__nat25_db_print();
    }
#endif        
	return 0;
}



static void __nat25_db_network_insert(unsigned char *macAddr, unsigned char *networkAddr)
{
  struct nat25_network_db_entry *db;
  int hash;

  hash = __nat25_network_hash(networkAddr);
  db = rpt_nethash[hash];
  while (db != NULL)
  {
    if(!memcmp(db->networkAddr, networkAddr, MAX_NETWORK_ADDR_LEN))
    {
      memcpy(db->macAddr, macAddr, ETH_ALEN);
      db->ageing_timer = sys_jiffies();
      ATOMIC_SET(&db->use_count, NAT25_AGEING_TIME);
#if 0 // for debug only
      printf("\n\rNAT25: Found insert DB M:%02x%02x%02x%02x%02x%02x "
        "N:%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
        db->macAddr[0],
        db->macAddr[1],
        db->macAddr[2],
        db->macAddr[3],
        db->macAddr[4],
        db->macAddr[5],
        db->networkAddr[0],
        db->networkAddr[1],
        db->networkAddr[2],
        db->networkAddr[3],
        db->networkAddr[4],
        db->networkAddr[5],
        db->networkAddr[6],
        db->networkAddr[7],
        db->networkAddr[8],
        db->networkAddr[9],
        db->networkAddr[10],
        db->networkAddr[11],
        db->networkAddr[12],
        db->networkAddr[13],
        db->networkAddr[14],
        db->networkAddr[15],
        db->networkAddr[16]);
#endif                        
    return;
  }

    db = db->next_hash;
  }

  db = mem_calloc(1, sizeof(struct nat25_network_db_entry));
            
  if(db == NULL) {
      printf("\n\rrun out  nat25  table!");
      return;
  }

  memcpy(db->networkAddr, networkAddr, MAX_NETWORK_ADDR_LEN);
  memcpy(db->macAddr, macAddr, ETH_ALEN);
  ATOMIC_SET(&db->use_count, NAT25_AGEING_TIME);
  db->ageing_timer = sys_jiffies();
#if 0//for debug only
  printf("\n\rNAT25: NEW insert DB M:%02x%02x%02x%02x%02x%02x "
          "N:%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
          db->macAddr[0],
          db->macAddr[1],
          db->macAddr[2],
          db->macAddr[3],
          db->macAddr[4],
          db->macAddr[5],
          db->networkAddr[0],
          db->networkAddr[1],
          db->networkAddr[2],
          db->networkAddr[3],
          db->networkAddr[4],
          db->networkAddr[5],
          db->networkAddr[6],
          db->networkAddr[7],
          db->networkAddr[8],
          db->networkAddr[9],
          db->networkAddr[10],
          db->networkAddr[11],
          db->networkAddr[12],
          db->networkAddr[13],
          db->networkAddr[14],
          db->networkAddr[15],
          db->networkAddr[16]);
#endif        
  __network_hash_link(db, hash);
}



int nat25_db_handle(struct netif *br, struct pbuf *pd, int method)
{
        
  u32_t br_ipaddr=0, target_ipaddr=0;
  unsigned short protocol;
  unsigned char networkAddr[MAX_NETWORK_ADDR_LEN];
  int macclone_da=0;
  u32_t addr=0;
  unsigned char *data=NULL;
  struct ip_hdr  *iph=NULL;
  //uint8_t ipver=0;
  uint8_t iphl=0;

       
  if((method <= NAT25_MIN) || (method >= NAT25_MAX))
    return -1;
  data = pd->payload;

  protocol = *((unsigned short *)(data + 2 * ETH_ALEN));

  /*---------------------------------------------------*/
  /*                 Handle IP frame                   */
  /*---------------------------------------------------*/
  if(protocol == lwip_htons(ETH_P_IP))
  {
    iph = (struct ip_hdr *)(data + ETH_HLEN);
    //ipver = (iph->_v_hl & 0xf0);
    iphl = (iph->_v_hl & 0x0f);
    if(((unsigned char*)(iph) + (iphl<<2)) >= (data + ETH_HLEN + pd->len))
    {
      printf("\n\rNAT25: malformed IP packet !");
      return -1;
    }

    switch(method)
    {
    case NAT25_CHECK:
      if (IS_MCAST(pd->payload))
           return 0;
      else
           return -1;

    case NAT25_INSERT:
      {
          //printf("\n\rNAT25: Insert IP, SA=%08x, DA=%08x\n", lwip_ntohl(iph->src.addr), lwip_ntohl(iph->dest.addr));
          addr= iph->src.addr;
          __nat25_generate_ipv4_network_addr(networkAddr, &(addr));

          __nat25_db_network_insert(data+ETH_ALEN, networkAddr);

          //__nat25_db_print();
      }
      return 0;

    case NAT25_LOOKUP:
      {
        //printf("\n\rNAT25: Lookup IP, SA=%08x, DA=%08x\n", lwip_ntohl(iph->src.addr), lwip_ntohl(iph->dest.addr));

        addr= iph->dest.addr;
        __nat25_generate_ipv4_network_addr(networkAddr, &(addr));
        if (!__nat25_db_network_lookup_and_replace(pd, networkAddr)) {
          if (*((unsigned char *)&iph->dest + 3) == 0xff) {
            // L2 is unicast but L3 is broadcast, make L2 bacome broadcast
            printf("\n\rNAT25: Set DA as boardcast");
            memset(pd->payload, 0xff, ETH_ALEN);
          }
          else {
            if(macclone_da){
              /*RX path ,the maccloned packet don't Replace it's DA with BR's MAC*/
            }
            else
            {	        
            // NAT25 only (maclone is disabled)
            //printf("\n\rNAT25: Replace DA with BR's MAC:%02X-%02X-%02X-%02X-%02X-%02X",br->hwaddr[0],br->hwaddr[1],br->hwaddr[2],br->hwaddr[3],br->hwaddr[4],br->hwaddr[5] );
            memcpy(data, br->hwaddr, ETH_ALEN);
            }
          }
        }

        //printf("\n\rNAT25: DHCP dst bcst skip NOW\n");
        //dhcp_dst_bcast(pd);
      }
      return 0;

    default:
    return -1;
    }
  }
  /*---------------------------------------------------*/
  /*                 Handle ARP frame                  */
  /*---------------------------------------------------*/
  else if(protocol == lwip_htons(ETH_P_ARP))
  {
    struct etharp_hdr *arp = (struct etharp_hdr *)(data + ETH_HLEN);
    unsigned char *arp_ptr = (unsigned char *)(arp);
    unsigned int senderaddr = 0, targetaddr = 0;
    unsigned int *sender=&senderaddr, *target=&targetaddr;
    
    if(arp->proto != lwip_htons(ETH_P_IP))
    {
        printf("\n\rNAT25 ARP : arp protocol unknown (%04x)!", htons(arp->proto));
        return -1;
    }

    switch(method)
    {
      case NAT25_CHECK:
        return 0;	// skb_copy for all ARP frame

      case NAT25_INSERT:
      {
        //printf("\n\rNAT25 ARP : Insert ARP, MAC=%02x%02x%02x%02x%02x%02x\n", arp_ptr[0+8],arp_ptr[1+8], arp_ptr[2+8], arp_ptr[3+8], arp_ptr[4+8], arp_ptr[5+8]);
        //printf("\n\rNAT25 ARP: Insert ARP Update arp sender %04X %04X\n",arp->sipaddr.addrw[0], arp->sipaddr.addrw[1]);
        //memcpy(sender, &arp->sipaddr.addrw, 4);
        
        senderaddr = arp->sipaddr.addrw[1];
        senderaddr = (senderaddr << 16 ) | arp->sipaddr.addrw[0];
        __nat25_generate_ipv4_network_addr(networkAddr, sender);
        __nat25_db_network_insert(data+ETH_ALEN, networkAddr);

#if LWIP_VERSION_MAJOR >= 2
		br_ipaddr = ip_addr_get_ip4_u32(netif_ip_addr4(br));
#else
		br_ipaddr = br->ip_addr.addr;
#endif

        //br_ipaddr = br->ip_addr.addr;
        target_ipaddr = lwip_ntohs(arp->dipaddr.addrw[0]);
        target_ipaddr = (target_ipaddr << 16) | lwip_ntohs(arp->dipaddr.addrw[1]);
        //printf("\n\rbr_ipaddr=%X", br_ipaddr);
        //__nat25_db_print(br);
        // change to ARP sender mac address to wlan STA address
        if (lwip_ntohs(arp->opcode) == 0x0001 && (lwip_ntohl(target_ipaddr) == br_ipaddr)) {
          //printf("\n\rDONT update arp req sender hw address");
        }else
          memcpy(arp_ptr+8, br->hwaddr, ETH_ALEN);
      }
      return 0;

      case NAT25_LOOKUP:
      {
        //printf("\n\rNAT25 ARP: Lookup ARP");
        targetaddr = arp->dipaddr.addrw[1];
        targetaddr = (targetaddr << 16) | arp->dipaddr.addrw[0];
        
        __nat25_generate_ipv4_network_addr(networkAddr, target);

        if (!__nat25_db_network_lookup_and_replace(pd, networkAddr)) {
            //printf("\n\r NOT Found entry from NAT25 DB");
        }
        //printf("\n\rNAT25 ARP %d: Lookup ARP update arp_ptr target\n", __LINE__);
        // change to ARP target mac address to Lookup result
        arp_ptr = (unsigned char *)(arp);
        arp_ptr += (8+arp->hwlen + arp->protolen);
        memcpy(arp_ptr, pd->payload, ETH_ALEN);
      }
      return 0;
      
      default:
        printf("\n\rNAT25 ARP: default -1");
        return -1;
    }
  }

  /*---------------------------------------------------*/
  /*                 Handle EAP frame                  */
  /*---------------------------------------------------*/
  else if(protocol == lwip_htons(0x888e))
  {
      switch(method)
      {
          case NAT25_CHECK:
                  return -1;

          case NAT25_INSERT:
                  return 0;

          case NAT25_LOOKUP:
                  return 0;

          default:
                  return -1;
      }
  }

  /*---------------------------------------------------*/
  /*                 Handle LLTD frame                 */
  /*---------------------------------------------------*/
  else if(protocol == lwip_htons(0x88d9))
  {
      switch(method)
      {
            case NAT25_CHECK:
                    return -1;

            case NAT25_INSERT:
                    return 0;

            case NAT25_LOOKUP:
                    return 0;

            default:
                    return -1;
      }
  }
  /*---------------------------------------------------*/
  /*                 Handle IPV6 frame                 */
  /*---------------------------------------------------*/
  else if(protocol == lwip_htons(ETH_P_IPV6))
  {
    unsigned char addr;
    struct br_rpt_ip6_addr ip6_address_src,ip6_address_dest;
    struct br_rpt_ip6_addr *pip6_address_src=&ip6_address_src, *pip6_address_dest= &ip6_address_dest;
    struct br_rpt_ip6_hdr  *ip6h = (struct br_rpt_ip6_hdr *)(data + ETH_HLEN);
    if (sizeof(struct br_rpt_ip6_hdr) >= (pd->len - ETH_HLEN))
    {
      printf("\n\rNAT25: malformed IPv6 packet !");
      return -1;
    }

    switch(method)
    {
      case NAT25_CHECK:
        if (IS_MCAST(pd->payload))
          return 0;
        else
          return -1;

      case NAT25_INSERT:
      {
        ip6_address_src.addr[0]= ip6h->src.addr[0];
        ip6_address_src.addr[1]= ip6h->src.addr[1];
        ip6_address_src.addr[2]= ip6h->src.addr[2];
        ip6_address_src.addr[3]= ip6h->src.addr[3];
        
        ip6_address_dest.addr[0]= ip6h->dest.addr[0];
        ip6_address_dest.addr[1]= ip6h->dest.addr[1];
        ip6_address_dest.addr[2]= ip6h->dest.addr[2];
        ip6_address_dest.addr[3]= ip6h->dest.addr[3];

        if (memcmp(pip6_address_src, "\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0", 16)) {
#if 0                
        printf("\n\rNAT25: Insert IPv6 %X, SA=%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x,"
          " DA=%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x",ip6h->_nexth,
          IP6_ADDR_BLOCK1(pip6_address_src),IP6_ADDR_BLOCK2(pip6_address_src),IP6_ADDR_BLOCK3(pip6_address_src),IP6_ADDR_BLOCK4(pip6_address_src),
          IP6_ADDR_BLOCK5(pip6_address_src),IP6_ADDR_BLOCK6(pip6_address_src),IP6_ADDR_BLOCK7(pip6_address_src),IP6_ADDR_BLOCK8(pip6_address_src),
          IP6_ADDR_BLOCK1(pip6_address_dest),IP6_ADDR_BLOCK2(pip6_address_dest),IP6_ADDR_BLOCK3(pip6_address_dest),IP6_ADDR_BLOCK4(pip6_address_dest),
          IP6_ADDR_BLOCK5(pip6_address_dest),IP6_ADDR_BLOCK6(pip6_address_dest),IP6_ADDR_BLOCK7(pip6_address_dest),IP6_ADDR_BLOCK8(pip6_address_dest));
#endif                
          __nat25_generate_ipv6_network_addr(networkAddr, (unsigned int *)pip6_address_src);
          __nat25_db_network_insert(data+ETH_ALEN, networkAddr);
          //__nat25_db_print(priv);

          if (ip6h->_nexth == IPPROTO_ICMPV6 && (pd->len > ((ETH_HLEN +  sizeof(struct br_rpt_ip6_hdr) + 4)))) {
           //int i,j;
           unsigned char *icmpv6_data =data + ETH_HLEN + sizeof(struct br_rpt_ip6_hdr); 
           int data_len =  pd->len - ETH_HLEN - sizeof(struct br_rpt_ip6_hdr);
           unsigned char myMacaddr[ETH_ALEN];
           memcpy(myMacaddr, br->hwaddr, ETH_ALEN); 
                      
            if (update_nd_link_layer_addr(icmpv6_data,data_len, myMacaddr)) {
              struct br_rpt_icmp6_hdr  *hdr = (struct br_rpt_icmp6_hdr *)(icmpv6_data);
              u32_t src_addr[4], dest_addr[4];  
              unsigned short payload_len = lwip_ntohs(ip6h->_plen);
              unsigned char *icmpv6_payload = icmpv6_data;
              
              //printf("\n\rplen=%04X, payload_len=%04X, total_len=%X",ip6h->_plen,payload_len, pd->tot_len);
              
              
              src_addr[0] =  ip6_address_src.addr[0];
              src_addr[1] =  ip6_address_src.addr[1];
              src_addr[2] =  ip6_address_src.addr[2];
              src_addr[3] =  ip6_address_src.addr[3];
              dest_addr[0] = ip6_address_dest.addr[0];
              dest_addr[1] = ip6_address_dest.addr[1];
              dest_addr[2] = ip6_address_dest.addr[2];
              dest_addr[3] = ip6_address_dest.addr[3];
              hdr->chksum = 0;
              hdr->chksum = ipcmp6_chksum_pseudo(icmpv6_payload, IPPROTO_ICMPV6,payload_len,src_addr, dest_addr);
            }
          } 
         
        }
   
      }
      return 0;

    case NAT25_LOOKUP:
    
      ip6_address_src.addr[0]= ip6h->src.addr[0];
      ip6_address_src.addr[1]= ip6h->src.addr[1];
      ip6_address_src.addr[2]= ip6h->src.addr[2];
      ip6_address_src.addr[3]= ip6h->src.addr[3];
      ip6_address_dest.addr[0]= ip6h->dest.addr[0];
      ip6_address_dest.addr[1]= ip6h->dest.addr[1];
      ip6_address_dest.addr[2]= ip6h->dest.addr[2];
      ip6_address_dest.addr[3]= ip6h->dest.addr[3];
#if 0             
      printf("\n\rNAT25: Lookup IP, SA=%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x,"
        " DA=%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x",
        IP6_ADDR_BLOCK1(pip6_address_src),IP6_ADDR_BLOCK2(pip6_address_src),IP6_ADDR_BLOCK3(pip6_address_src),IP6_ADDR_BLOCK4(pip6_address_src),
        IP6_ADDR_BLOCK5(pip6_address_src),IP6_ADDR_BLOCK6(pip6_address_src),IP6_ADDR_BLOCK7(pip6_address_src),IP6_ADDR_BLOCK8(pip6_address_src),
        IP6_ADDR_BLOCK1(pip6_address_dest),IP6_ADDR_BLOCK2(pip6_address_dest),IP6_ADDR_BLOCK3(pip6_address_dest),IP6_ADDR_BLOCK4(pip6_address_dest),
        IP6_ADDR_BLOCK5(pip6_address_dest),IP6_ADDR_BLOCK6(pip6_address_dest),IP6_ADDR_BLOCK7(pip6_address_dest),IP6_ADDR_BLOCK8(pip6_address_dest));
#endif            
      __nat25_generate_ipv6_network_addr(networkAddr, (unsigned int *)pip6_address_dest);
     
      if (!__nat25_db_network_lookup_and_replace(pd, networkAddr)) {
        //printf("\n\r NOT Found entry from NAT25 DB %08X %08X %08X %08X",ip6h->dest.addr[0], ip6h->dest.addr[1], ip6h->dest.addr[2], ip6h->dest.addr[3] );
        addr = (unsigned char )(ip6h->dest.addr[0] & 0xFF);
        if (addr == 0xff) {
          convert_ipv6_mac_to_mc(pd);
        }
      }
      return 0;
    default:
       return -1;
   }
  }


  /*---------------------------------------------------*/
  /*         Handle all other unknown format           */
  /*---------------------------------------------------*/
  else
  {
    printf("\n\rNAT25: Unknown protocol: 0x%04x\n", ntohs(protocol));
    switch(method)
    {
      case NAT25_CHECK:
        if (IS_MCAST(data))
          return 0;
        else
          return -1;

      case NAT25_INSERT:
        if (memcmp(data+ETH_ALEN, br->hwaddr, ETH_ALEN))
        {
            if (memcmp(data+ETH_ALEN, rpt_ukpro_mac, ETH_ALEN)) {
                    memcpy(rpt_ukpro_mac, data+ETH_ALEN, ETH_ALEN);
                    rpt_ukpro_mac_valid = 1;
                    printf("\n\rNAT25: Insert unknown protocol, MAC=%02x%02x%02x%02x%02x%02x\n",
                                    rpt_ukpro_mac[0], rpt_ukpro_mac[1], rpt_ukpro_mac[2],
                                    rpt_ukpro_mac[3], rpt_ukpro_mac[4], rpt_ukpro_mac[5]);
            }
        }
        return 0;

      case NAT25_LOOKUP:
        // replace the destination mac address
        if (rpt_ukpro_mac_valid) {
                memcpy(data, rpt_ukpro_mac, ETH_ALEN);
                printf("\n\rNAT25: Lookup unknown protocol, MAC=%02x%02x%02x%02x%02x%02x",
                                rpt_ukpro_mac[0], rpt_ukpro_mac[1], rpt_ukpro_mac[2],
                                rpt_ukpro_mac[3], rpt_ukpro_mac[4], rpt_ukpro_mac[5]);
        }
        return 0;
      default:
          return -1;
      }
  }
}


int br_rpt_handle_frame(struct netif *br, struct pbuf *pd)
{
  int retval=0;
  unsigned char *data=NULL;
  data = pd->payload;
  if(!(data[0] & 1))
  {
#if 0 //for debug only
    printf("\n\rNAT25: Input Frame: DA=%02x%02x%02x%02x%02x%02x "
      "SA=%02x%02x%02x%02x%02x%02x",
      data[0],
      data[1],
      data[2],
      data[3],
      data[4],
      data[5],
      data[6],
      data[7],
      data[8],
      data[9],
      data[10],
      data[11]);
#endif               
  }
  if(!(data[0] & 1))
  {
    retval = nat25_db_handle(br, pd, NAT25_LOOKUP);
    
    if(retval == -1) {
      printf("\n\rNAT25: Lookup fail!");
      return -1;
    }
  }
  return 0;
}



#define SERVER_PORT		67
#define CLIENT_PORT		68
#define DHCP_MAGIC		0x63825363
#define BROADCAST_FLAG		0x8000
#define IPPROTO_UDP     17
#define OPT_CODE 	0
#define OPT_LEN 	1
#define OPT_DATA 	2
#define OPTION_FIELD	0
#define FILE_FIELD	1
#define SNAME_FIELD	2

/* DHCP option codes (partial list) */
#define DHCP_PADDING		0x00
#define DHCP_REQUESTED_IP	0x32
#define DHCP_OPTION_OVER	0x34
#define DHCP_CLIENT_ID		0x3d
#define DHCP_END		0xFF


static int end_option(unsigned char *optionptr)
{
    int i = 0;

    while (optionptr[i] != DHCP_END) {
            if (optionptr[i] == DHCP_PADDING) i++;
            else i += optionptr[i + OPT_LEN] + 2;
    }
    return i;
}
#if 0
void dhcp_add_reqip_option(struct pbuf *pd)
{
    unsigned char opt[6] = {0};
    unsigned char *data=NULL;
    struct ip_hdr  *iph=NULL;
    struct udp_hdr *udph=NULL;
    struct rpt_dhcp_msg *dhcph=NULL;
    unsigned short protocol;
    unsigned char *opt_end;
    unsigned int end_offset;
    struct pbuf *p, *q;
    uint8_t iphl=0;
    uint8_t *udp_data=NULL;
    u32_t udp_offset=0, dhcp_offset=0;
    err_t copy_res=0;
    
    p = pbuf_alloc(PBUF_RAW, (pd->tot_len+6), PBUF_POOL);
    if (p == NULL) {
      printf("\n\rCannot allocate pbuf to receive packet");
      return;
    }
    
    copy_res = pbuf_copy(p, pd);
    
    if (copy_res == ERR_OK) {
      data = p->payload;
      protocol = *((unsigned short *)(data + 2 * ETH_ALEN));
      iph = (struct ip_hdr *)(data + ETH_HLEN);
      iphl = (iph->_v_hl & 0x0f);
    
      udp_offset = ETH_HLEN + (iphl << 2);
      udph = (struct udp_hdr *)(data + udp_offset);
    
      dhcp_offset = udp_offset + sizeof(struct udp_hdr);
      dhcph = (struct rpt_dhcp_msg *)(data + dhcp_offset);
    
      opt[0] = (unsigned char)DHCP_REQUESTED_IP;
      opt[1] = (unsigned char)4;
      memcpy(&opt[2], &(dhcph->ciaddr), 4);
      dhcph->ciaddr.addr=0;
    
      printf("\n\rdhcp_add_reqip_option pd total len=%d pd len=%d", pd->tot_len, pd->len);
      printf("\n\rdhcp_add_reqip_option new pd total len=%d pd len=%d", p->tot_len, p->len);
    //skb_put(skb, 6);

      end_offset = end_option(dhcph->options);
      if (end_offset + dhcph->options[OPT_LEN] + 2 + 1 >= 308) {
          printf("\n\r*** %s add option error!!! ***\n", __FUNCTION__);
          return;
      }

      opt_end = (unsigned char *)&(dhcph->options) + end_offset;
      printf("\n\rdhcp_add_reqip_option end_offset=%u option len=%X", end_offset, dhcph->options[OPT_LEN]);
      memcpy(opt_end, opt, 6);
      dhcph->options[end_offset+6] = DHCP_END;
      printf("\n\r %x udp len=%x %x",udph->len, lwip_ntohs(udph->len), lwip_htons(lwip_ntohs(udph->len) + 6));
      printf("\n\r%x ip len=%x %x",iph->_len, lwip_ntohs(iph->_len), lwip_htons(lwip_ntohs(iph->_len) + 6));
      udph->len = lwip_htons(lwip_ntohs(udph->len) + 6);
      iph->_len = lwip_htons(lwip_ntohs(iph->_len) + 6);
      pbuf_free(pd);
      pd = p;
    }
    
    
        
}


#endif
 
unsigned char *get_dhcp_option(struct rpt_dhcp_msg *packet, int code)
{

  int i, length;
  unsigned char *optionptr=NULL;
  int over = 0, done = 0, curr = OPTION_FIELD;

  optionptr = packet->options;
  i = 0;
  length = 308;
  while (!done) {
    if (i >= length) {
            return NULL;
    }

    if (optionptr[i + OPT_CODE] == code) {
            if (i + 1 + optionptr[i + OPT_LEN] >= length) {
                    return NULL;
            }
            return optionptr + i + 2;
    }

    switch (optionptr[i + OPT_CODE]) {
    case DHCP_PADDING:
            i++;
            break;
    case DHCP_OPTION_OVER:
            if (i + 1 + optionptr[i + OPT_LEN] >= length) {
                    return NULL;
            }
            over = optionptr[i + 3];
            i += optionptr[OPT_LEN] + 2;
            break;
    case DHCP_END:
            if (curr == OPTION_FIELD && over & FILE_FIELD) {
                optionptr = packet->file;
                i = 0;
                length = 128;
                curr = FILE_FIELD;
            } else if (curr == FILE_FIELD && over & SNAME_FIELD) {
                optionptr = packet->sname;
                i = 0;
                length = 64;
                curr = SNAME_FIELD;
            } else done = 1;
            break;
    default:
            i += optionptr[OPT_LEN + i] + 2;
    }
  }
  return NULL;

}
 

void dhcp_flag_bcast(struct netif *br, struct pbuf *pd)
{
  unsigned int ip_check_recalc = 0;
  unsigned int udp_check_recalc = 0;
  struct ip_hdr  *iph=NULL;
  struct udp_hdr *udph=NULL;
  struct rpt_dhcp_msg *dhcph=NULL;
  unsigned char *data=NULL;
  u32_t src_addr=0;
  u32_t dest_addr=0;
  //uint8_t ipver=0;
  uint8_t iphl=0;
  uint8_t *udp_data=NULL;
  u32_t udp_offset=0, dhcp_offset=0;
  
  data = pd->payload;
  unsigned short protocol = *((unsigned short *)(data + 2 * ETH_ALEN));
	

  if(protocol == lwip_htons(ETH_P_IP)) {
    iph = (struct ip_hdr *)(data + ETH_HLEN);
    if(iph->_proto == IPPROTO_UDP) {
      //ipver = (iph->_v_hl & 0xf0);
      iphl = (iph->_v_hl & 0x0f);
      
      udp_offset = ETH_HLEN + (iphl << 2);
      
      udph = (struct udp_hdr *)(data + udp_offset);
       
      if((udph->src == lwip_htons(CLIENT_PORT)) && (udph->dest == lwip_htons(SERVER_PORT))) {// DHCP request
        dhcp_offset = udp_offset + sizeof(struct udp_hdr);
        dhcph = (struct rpt_dhcp_msg *)(data + dhcp_offset);
        
        //printf("\n\r DHCP chaddr=%02X-%02X-%02X-%02X-%02X-%02X",dhcph->chaddr[0],dhcph->chaddr[1], dhcph->chaddr[2], dhcph->chaddr[3], dhcph->chaddr[4], dhcph->chaddr[5]);
         //printf("\n\r DHCP cookie=%X, %X", dhcph->cookie, lwip_htonl(dhcph->cookie));
          if(dhcph->cookie == lwip_htonl(DHCP_MAGIC))
          {
#if 0
            unsigned char *option_value = get_dhcp_option(dhcph, DHCP_OPTION_MESSAGE_TYPE);
            if (option_value) {
              int opt_len = *(unsigned char*)(option_value - OPT_LEN);;
              char tmp_str[256] = {0};
              memcpy(tmp_str, option_value, opt_len);
              tmp_str[opt_len] = '\0';
              printf("\n\rDHCP msg type=%02X", tmp_str[0]);
            }
#endif
            if(!(dhcph->flags & lwip_htons(BROADCAST_FLAG))) {
                //printf("\n\rDHCP: change flag of DHCP request to broadcast.");
                dhcph->flags |= lwip_htons(BROADCAST_FLAG);
                udp_check_recalc++;
            }

            if (dhcph->ciaddr.addr != 0 && memcmp(dhcph->chaddr, br->hwaddr, 6))
            {
              if (!get_dhcp_option(dhcph, DHCP_OPTION_REQUESTED_IP)) {
                //printf("\n\rDHCP Client set ciaddr && no requested!");

                if ((pd->tot_len == pd->len) && (pd->len+6) < PBUF_POOL_BUFSIZE) {
                  unsigned char *opt_end;
                  unsigned int end_offset;
                  unsigned char opt[6] = {0};
                  
                  opt[0] = (unsigned char)DHCP_REQUESTED_IP;
                  opt[1] = (unsigned char)4;
                  memcpy(&opt[2], &(dhcph->ciaddr), 4);
                  dhcph->ciaddr.addr=0;
                  end_offset = end_option(dhcph->options);
                  if (end_offset + dhcph->options[OPT_LEN] + 2 + 1 >= 308) {
                      printf("\n\r*** %s add option error!!! ***\n", __FUNCTION__);
                      return;
                  }

                  opt_end = (unsigned char *)&(dhcph->options) + end_offset;
                  memcpy(opt_end, opt, 6);
                  dhcph->options[end_offset+6] = DHCP_END;
                  
                  
                  udph->len = lwip_htons(lwip_ntohs(udph->len) + 6);
                  iph->_len = lwip_htons(lwip_ntohs(iph->_len) + 6);
                    
                  pd->len = pd->len+6;
                  pd->tot_len = pd->tot_len+6;
                  
                  udp_check_recalc++;
                  ip_check_recalc++;
                }

              } else {
                //printf("\n\rDHCP Client set ciaddr=0");
                dhcph->ciaddr.addr = 0;
                udp_check_recalc++;
              }
            }
          }
      }

      if (udph->chksum && udp_check_recalc) {
        //printf("\n\rUpdate UDP cksum");
        udph->chksum = 0;
        udp_data = (uint8_t *)(dhcph); 
        src_addr = iph->src.addr;
        dest_addr= iph->dest.addr;
        udph->chksum = calculate_chksum_pseudo(udp_data, udph, 0x11, lwip_ntohs(udph->len), &src_addr, &dest_addr);
        udph->chksum = lwip_htons(udph->chksum);
       }
    }
  
    if (ip_check_recalc) {
      //printf("\n\rUpdate IP cksum");
      IPH_CHKSUM_SET(iph, 0);
      IPH_CHKSUM_SET(iph, inet_chksum(iph, (iphl<<2))); 
    }
  }
}


void dhcp_dst_bcast(struct pbuf *pd)
{
  unsigned int ip_check_recalc = 0;
  unsigned int udp_check_recalc = 0;
  struct ip_hdr  *iph=NULL;
  struct udp_hdr *udph=NULL;
  struct rpt_dhcp_msg *dhcph=NULL;
  unsigned char *data=NULL;
  unsigned short protocol;
  u32_t udp_offset=0, dhcp_offset=0;
  u32_t src_addr=0;
  u32_t dest_addr=0;
  //uint8_t ipver=0;
  uint8_t iphl=0;
  uint8_t *udp_data=NULL;

  data = pd->payload;
  protocol = *((unsigned short *)(data + 2 * ETH_ALEN));
  if(protocol == lwip_htons(ETH_P_IP) ) {
      iph = (struct ip_hdr *)(data + ETH_HLEN);

      if(iph->_proto == IPPROTO_UDP) {
        //ipver = (iph->_v_hl & 0xf0);
        iphl = (iph->_v_hl & 0x0f);
        //printf("\n\r IP ver=%X, IP hl=%X", ipver, iphl);
        udp_offset = ETH_HLEN + (iphl << 2);
        udph = (struct udp_hdr *)(data + udp_offset);

        if((udph->src == lwip_htons(SERVER_PORT)) && (udph->dest == lwip_htons(CLIENT_PORT))) {// DHCP request
          dhcp_offset = udp_offset + sizeof(struct udp_hdr);
          dhcph = (struct rpt_dhcp_msg *)(data + dhcp_offset);
          if(dhcph->cookie == lwip_htonl(DHCP_MAGIC))
          {
            unsigned char *daddr = (unsigned char *)&(iph->dest.addr);
            if((daddr[0] != 0xff))
            {
                //we set bootp flags to broadcast, but the DHCP server reply a unicast packets, so we reset the dst to broadcast
                daddr[0] = 0xff;
                daddr[1] = 0xff;
                daddr[2] = 0xff;
                daddr[3] = 0xff;
                if(data[0] != 0xff)
                {
                  data[0] = 0xff;
                  data[1] = 0xff;
                  data[2] = 0xff;
                  data[3] = 0xff;
                  data[4] = 0xff;
                  data[5] = 0xff;
                  
                }
                ip_check_recalc++;
                udp_check_recalc++;
            }
          }
        }

        if (udph->chksum && udp_check_recalc) {
          udph->chksum = 0;
          udp_data = (uint8_t *)(dhcph); 
          src_addr = iph->src.addr;
          dest_addr= iph->dest.addr;
          udph->chksum = calculate_chksum_pseudo(udp_data, udph, 0x11, lwip_ntohs(udph->len), &src_addr, &dest_addr);
          udph->chksum = lwip_htons(udph->chksum);
        }
        if (ip_check_recalc) {
            //printf("\n\rRe-Cal IPcheck\n");
            IPH_CHKSUM_SET(iph, 0);
            IPH_CHKSUM_SET(iph, inet_chksum(iph, iphl));
        }
      }
  }
}

err_t br_rpt_handle(struct netif *br, struct pbuf *p, u8_t portif_idex)
{
  int res=0;
  int lltd_flag = 0, do_nat25=1;
  unsigned char *data=NULL;
  data = p->payload;
//STA Client port TX
  if (p->if_idx == 2 && portif_idex ==1) {
    if (*((unsigned short *)(data + ETH_ALEN * 2)) == lwip_htons(0x88d9)) {
      if (data[0] & 0x1) {
        do_nat25 = 0;
        lltd_flag = 1;
      }
    }

    if (do_nat25 ==1) {
      if (nat25_db_handle(br, p, NAT25_CHECK) == 0) {
      }
      res = nat25_db_handle(br, p, NAT25_INSERT);
      if (res < 0) {
        if (res == -2) {
          printf("\n\rTX DROP: nat25_db_handle fail!");
        }
        // we just print warning message and let it go
        printf("\n\rnat25_db_handle INSERT fail!");
      }
    }
    if (lltd_flag != 1) {
      memcpy(data + ETH_ALEN, br->hwaddr, ETH_ALEN);
    }

      dhcp_flag_bcast(br, p);
  }else if (p->if_idx == 1 && portif_idex ==2) {
      //STA Client port RX
      data = p->payload;
   }else {
      if (p->if_idx == 0 && portif_idex > 0) {
        if (nat25_db_handle(br, p, NAT25_CHECK) == 0) {
        }
        res = nat25_db_handle(br, p, NAT25_INSERT);
        if (res < 0) {
          if (res == -2) {
             printf("\n\rTX DROP: nat25_db_handle fail!\n");
          }
          // we just print warning message and let it go
          printf("\n\rnat25_db_handle INSERT fail!\n");
        }
      }
  }
  return ERR_OK;
}

/** Timer callback for nat25 fdb aging, called once per second */
static void rptdb_ageing_tmr(void *arg)
{
  nat25_db_expire();
  sys_timeout(1000, rptdb_ageing_tmr, arg);
}


int rpt_handle_init(unsigned char *br_mac)
{
  int i;
  for(i=0;i<NAT25_HASH_SIZE;i++) {
      rpt_nethash[i]=NULL;
  }
  memset(rpt_br_ip, 0x00, 4);
  rpt_ukpro_mac_valid = 0;
  memset(rpt_ukpro_mac, 0x00, ETH_ALEN);
  memcpy(rpt_br_mac, br_mac, ETH_ALEN);
  
  sys_timeout(1000, rptdb_ageing_tmr, NULL);
  return 0;
}


