#ifndef TREE_H
#define TREE_H


#define MACADDRLEN 6
#define MAX_MP_NUM 5
#define MAX_CONNECTION 100
#define MAX_TIMER_INSTANCE 7

#define PORTAL_ANNOUNCEMENT_INTERVAL 3
#define PORTAL_ANNOUNCEMENT_TIMEOUT 17 // must be larger than PORTAL_ANNOUNCEMENT_INTERVAL
#define PORTAL_PROPAGATION_DELAY 1

#define PORTAL_ARBITRATION_TIME 20

#define ROOT_ANNOUNCEMENT_INTERVAL 1
#define ROOT_ANNOUNCEMENT_TIMEOUT 17 // must be larger than ROOT_ANNOUNCEMENT_INTERVAL
#define ROOT_PROPAGATION_DELAY 1
#define HWMP_ROUTE_LOSS_THRESHOLD 5

#define INIT_TIMER_INSTANCE() {{PORTAL_ANNOUNCEMENT_INTERVAL, PORTAL_ANNOUNCEMENT_INTERVAL, gen_pann},  \
                               {1, 1, retx_pann}, {1, 1, tbl_pann},                                     \
                               {ROOT_ANNOUNCEMENT_INTERVAL, ROOT_ANNOUNCEMENT_INTERVAL, gen_rann},      \
                               {1, 1, retx_rann}, {1, 1, tbl_rann},                                     \
                               {PORTAL_ARBITRATION_TIME, PORTAL_ARBITRATION_TIME, root_decision}}

void gen_pann();
void retx_pann();

void gen_rann();
void retx_rann();
void tbl_rann();
void root_decision();

#pragma pack(1)
struct rann {
    unsigned char category;
    unsigned char action;
    unsigned char Id;
    unsigned char length;
    unsigned char flag;
    unsigned char hopCount;
    unsigned char ttl;
    unsigned char origAddr[MACADDRLEN];
    unsigned int seqNum;
    unsigned int lifetime;
    unsigned int metric;
};
#pragma pack()

struct _rann_retx_entry_{ // RANN retransmit table, record what time shoude RANN be relayed.
    struct rann rann;
    unsigned int timeout;
    short int flag;
};


//struct _rann_retx_entry_ rann_retx_entry;


struct _rann_root_entry_{ // root info table.
    unsigned char mac[MACADDRLEN];
    unsigned int timeout;
    unsigned int seqNum;
    short int flag;
};


//struct _rann_root_entry_ rann_root_entry;
 

extern void pann_handler();
#endif /* tree.h */
