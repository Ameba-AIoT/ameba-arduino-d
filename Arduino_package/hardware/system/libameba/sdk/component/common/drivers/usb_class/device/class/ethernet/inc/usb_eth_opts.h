#ifndef _USB_ETH_OPTS_H
#define _USB_ETH_OPTS_H

#define CONFIG_WLAN2ETH_THREAD 		0
#define CONFIG_ETH2WLAN_THREAD 		0

#define CONFIG_RX_AGGREGATION
#define CONFIG_TX_AGGREGATION
// RX aggregation
#ifdef CONFIG_RX_AGGREGATION
	#define NR_RX_SKB_PERBUF		3
	#define CONFIG_RX_TIMER			1
	#define CONFIG_USE_GDMA			0    // use multi-block gdma to do aggregation
	
	#if	CONFIG_RX_TIMER
		#define CONFIG_RX_SW_TIMER	0
		#define CONFIG_RX_HW_TIMER  1
	#endif
#else
	#define NR_RX_SKB_PERBUF		1
#endif

// TX aggregation
#ifdef CONFIG_TX_AGGREGATION
	#define NR_TX_SKB_PERBUF	3
#else
	#define NR_TX_SKB_PERBUF	1
#endif

#endif //_USB_ETH_OPTS_H