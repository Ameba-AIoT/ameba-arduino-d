#ifndef _USB_CDCDEVICE_H_
#define _USB_CDCDEVICE_H_

#ifdef BOARD_AITHINKER_BW16
// error "USB functionality is not supported on BW16 development board"
#else

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

// Prevent double definition warnings
#ifdef USB_VID
#undef USB_VID
#endif

#ifdef USB_PID
#undef USB_PID
#endif

#include "rtl8721d_usb.h"
#include "usbd.h"
#include "usbd_cdc_acm.h"

#ifdef __cplusplus
}
#endif

// Do not change the settings unless indeed necessary
#define CONFIG_CDC_ACM_BULK_IN_XFER_SIZE       2048U
#define CONFIG_CDC_ACM_BULK_OUT_XFER_SIZE      2048U

// Thread priorities
#define CONFIG_CDC_ACM_INIT_THREAD_PRIORITY    5
#define CONFIG_CDC_ACM_ISR_THREAD_PRIORITY     7
#define CONFIG_CDC_ACM_HOTPLUG_THREAD_PRIORITY 8    // Should be higher than CONFIG_CDC_ACM_ISR_THREAD_PRIORITY
#define CONFIG_CDC_ACM_XFER_THREAD_PRIORITY    5

class USBCDCDevice: public HardwareSerial {
public:
    USBCDCDevice();
    ~USBCDCDevice();

    void setVID(uint16_t vid);
    void setPID(uint16_t pid);
    void setManufacturerString(const char* manufacturer);
    void setModelString(const char* model);
    void setSerialString(const char* serial);

    uint8_t USBconnected();
    uint8_t connected();
    uint8_t dtr();
    uint8_t rts();
    void begin(uint32_t baud = 0, uint8_t config = 0);
    void end();
    int available();
    int peek();
    int read();
    void flush();
    size_t write(uint8_t data);
    size_t write(const uint8_t* buffer, size_t size);
    using Print::write;    // pull in overloaded write functions from Print which are not overridden here
    operator bool();
private:
    static void usbCDCStatusChanged(usb_dev_t* dev, uint8_t status);
    static void usbConnectDetectTask(void* param);
    static uint8_t usbdCDCInit(uint16_t rx_buf_len, uint16_t tx_buf_len, usbd_cdc_acm_cb_t* cb = NULL);
    static uint8_t usbdCDCDeinit(void);
    static uint8_t usbdCDCTransmit(const uint8_t* data, size_t len);
    static uint8_t usbdCDCReceive(void);
    //----------------------------------- USB Driver Functions -----------------------------------//
    static uint8_t* usbdCDCGetDescriptor(usb_dev_t* dev, usb_setup_req_t* req, usb_speed_type_t speed, uint16_t* len);
    static uint8_t usbdCDCSetConfig(usb_dev_t* dev, uint8_t config);
    static uint8_t usbdCDCClearConfig(usb_dev_t* dev, uint8_t config);
    static uint8_t usbdCDCSetup(usb_dev_t* dev, usb_setup_req_t* req);
    static uint8_t usbdCDCHandleEP0DataOut(usb_dev_t* dev);
    static uint8_t usbdCDCHandleEPDataIn(usb_dev_t* dev, uint8_t ep_num, uint8_t status);
    static uint8_t usbdCDCHandleEPDataOut(usb_dev_t* dev, uint8_t ep_num, uint16_t len);
    static uint8_t cdcACMTransmitZlp(void);
    static uint8_t cdcACMConfigCoding(uint8_t cmd, uint8_t* pbuf, uint16_t len, uint16_t value);

    static uint8_t* _pReportDesc;
    static uint16_t _reportDescSize;
    static usbd_config_t _cdcCfg;
    static usbd_class_driver_t _usbdCDCDriver;

    static uint16_t _usbVID;
    static uint16_t _usbPID;
    static uint8_t* _pManufacturerStr;
    static uint8_t _manufacturerStrLen;
    static uint8_t* _pModelStr;
    static uint8_t _modelStrLen;
    static uint8_t* _pSerialStr;
    static uint8_t _serialStrLen;

    static uint8_t _usbStatus;
    static struct task_struct _usbDetectTask;

    static usbd_cdc_acm_line_coding_t _cdcACMLineCoding;
    static uint16_t _cdcACMCtrlLineState;
    static uint8_t _dtr;
    static uint8_t _rts;

    // sema
    static _sema cdc_acm_attach_status_changed_sema;
};

extern USBCDCDevice SerialUSB;

#endif

#endif
