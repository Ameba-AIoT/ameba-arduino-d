#ifndef _MESH_CFG_H_
#define _MESH_CFG_H_

#ifdef CONFIG_RTK_MESH 

//#define PU_STANDARD // use proxy update protocol

#ifdef PU_STANDARD
//#define PU_A4A6 // use A4/A6 to update proxy table
//#define PU_STANDARD_RX // send proxy update message when receving a data frame
#define PU_STANDARD_SME // send proxy update message when assoc
#endif

#ifdef MESH_USE_METRICOP
#define MESH_METRIC_PERIOD_UPDATE 60*HZ // 1 mins (jiffies)
#define MESH_METRIC_TIMEOUT_FRAME 1000 // jiffies
#define MESH_METRIC_MAX_RETRY  3
#define MESH_METRIC_UNSPECIFIED 20000 // if metric = 0, response this value; if exceeding MAX_RETRY, update with this value
#endif

//#define MESH_ROUTE_MAINTENANCE


//#define SIMPLE_CH_UNI_PROTOCOL
 
#ifdef SIMPLE_CH_UNI_PROTOCOL
#define _11S_SS_COUNT1 3
#define _11S_SS_COUNT2 10
#endif

//#define D_ACL 	//tsananiu

#if defined(CONFIG_RTL8192SE)
#define SIGNINTX rtl8192SE_signin_txdesc
#else
#define SIGNINTX signin_txdesc
#endif

#define RTK_MESH_AODV_STANDALONE_TIMER
#if defined(RTK_MESH_AODV_STANDALONE_TIMER)
#define MESH_AODV_EXPIRE_TO	rtw_ms_to_systime(100)
#endif
#define RTK_MESH_REMOVE_PATH_AFTER_AODV_TIMEOUT


#endif
#endif	// _MESH_CFG_H
