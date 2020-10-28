#ifndef _MESH_SECURITY_H_
#define _MESH_SECURITY_H_

#include "./mesh.h"

#define DOT11_IsEmptyQueue(q) (q->NumItem==0 ? 1:0)
#define DOT11_IsFullQueue(q) (q->NumItem==q->MaxItem? 1:0)
#define DOT11_NumItemQueue(q) q->NumItem

#define E_DOT11_2LARGE	-1
#define E_DOT11_QFULL	-2
#define E_DOT11_QEMPTY	-3

// enough for DOT11s_RECV_PACKET
#define MAXDATALEN2		256
// 6 entries should be enough
#define MAXQUEUESIZE2	6

typedef struct _DOT11_QUEUE_NODE2
{
        unsigned short  ItemSize;
        unsigned char   Item[MAXDATALEN2];
}DOT11_QUEUE_NODE2;

typedef struct _DOT11_QUEUE2
{
        int     Head;
        int     Tail;
        int     NumItem;
        int     MaxItem;
        DOT11_QUEUE_NODE2        ItemArray[MAXQUEUESIZE2];
	int	MaxData;
}DOT11_QUEUE2;

#endif //_MESH_SECURITY_H_
