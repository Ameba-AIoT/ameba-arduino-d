//----------------------------------------------------------------------------//
#ifndef __WLAN_SOCKET_TEST_H
#define __WLAN_SOCKET_TEST_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Exported test functions ------------------------------------------------------- */
void reset_socket_test_cb();
void do_socket_test(int task_num, int rw_num, int non_stop);
void reset_socket_test_status();
void do_socket_test(int task_num, int rw_num, int non_stop);
void do_socket_connect(void);
#ifdef __cplusplus
  }
#endif

#endif // __WLAN_SOCKET_TEST_H

//----------------------------------------------------------------------------//
