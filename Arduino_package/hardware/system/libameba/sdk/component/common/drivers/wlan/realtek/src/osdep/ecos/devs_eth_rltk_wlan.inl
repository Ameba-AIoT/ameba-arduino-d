static Rltk_wlan_t rltk_wlan0_priv_data = {
  0, 0
};

ETH_DRV_SC(rltk_wlan_sc0,
           &rltk_wlan0_priv_data,
           RTLDAT_DEVS_ETH_RLTK_WLAN_WLAN0_NAME,
           rltk_wlan_start,
           rltk_wlan_stop,
           rltk_wlan_control,
           rltk_wlan_can_send,
           rltk_wlan_send,
           rltk_wlan_recv,
           rltk_wlan_deliver,
           rltk_wlan_poll,
           rltk_wlan_int_vector
           );

NETDEVTAB_ENTRY(rltk_wlan_netdev0,
                "rltk_" RTLDAT_DEVS_ETH_RLTK_WLAN_WLAN0_NAME,
                rltk_wlan_init,
                &rltk_wlan_sc0);

