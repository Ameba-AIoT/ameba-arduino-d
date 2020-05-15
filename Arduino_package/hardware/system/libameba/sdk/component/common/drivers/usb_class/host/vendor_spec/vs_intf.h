/*
header file for vendor spec driver
*/

//#define ctrl_test
//#define bulk_test
#define iso_test_in
#define iso_test_out
#define IN_STORAGE_SIZE 512
#define IN_BUFFER_SIZE  128
#define OUT_BUFFER_SIZE 128
#define USB_IN_CNT      100
//#define ctrl_test


int vs_init(void); //entry function to start vs
void vs_free(void);

