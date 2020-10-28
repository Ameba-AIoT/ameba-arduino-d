#ifndef _MESH_11KV_H_
#define _MESH_11KV_H_

#ifdef CONFIG_RTL8192CD
#include "../8192cd.h"
#else
#include "../rtl8190/8190n.h"
#endif

extern void issue_11kv_LinkMeasureReq(DRV_PRIV * priv, struct stat_info * pstat, UINT16 lenTst, UINT16 cntTst, UINT8 prioTst);
extern void issue_11kv_LinkMeasureRepAck(DRV_PRIV * priv, struct stat_info * pstat);
extern void issue_11kv_LinkMeasureRepRep(DRV_PRIV * priv, struct stat_info * pstat);
extern unsigned int On11kvLinkMeasureReq(DRV_PRIV *priv, struct rx_frinfo *pfrinfo);
extern unsigned int On11kvLinkMeasureRep(DRV_PRIV *priv, struct rx_frinfo *pfrinfo);
extern UINT32 computeMetric(DRV_PRIV * priv, struct stat_info * pstat, UINT8 rate, UINT8 priority,
			UINT16 lenTotalSnd, UINT16 cntSnd, UINT16 lenTotalRcv, UINT16 cntRcv);
extern UINT32 getMetric(DRV_PRIV * priv, struct stat_info * pstat);
extern void metric_update(DRV_PRIV *priv);

#endif // __MESH_11KV_H_
