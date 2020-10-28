#ifndef __PATHSEL_DAEMON_UTIL_H__
#define __PATHSEL_DAEMON_UTIL_H__

extern int updatePathByFreshness(unsigned char *src,unsigned int dsn,
               unsigned int metric, unsigned char hopcount, unsigned char *My_Prehop,
               int My_interFace);
extern int query_table(unsigned char *destaddr, struct path_sel_entry *pEntry_out);
extern int remove_table(unsigned char* invalid_addr);
extern void send_packet_with_6addr( unsigned char *RBody ,
							 unsigned char *self_mac_addr ,unsigned char *dest_mac_addr ,
							 unsigned char *src_mac_addr ,
							 int flag,unsigned char TTL,
							 unsigned short SeqNum,
							 unsigned char *addr5 ,
							 unsigned char *addr6);
extern void send_packet( unsigned char *RBody ,unsigned char *self_mac_addr, unsigned char *dest_mac_addr,
                             unsigned char *src_mac_addr,
                             int flag, unsigned char TTL, unsigned short SeqNum);
#ifdef CONFIG_RTK_MESH_METRIC_REFINE
extern int calculateMetric(unsigned char *My_Add, unsigned int metric, unsigned int rssiMetric,unsigned int cuMetric,unsigned int noiseMetric);
extern int updateMetric(unsigned char *My_Add, unsigned char *My_Prehop, unsigned int *metric); 
#endif

#endif

