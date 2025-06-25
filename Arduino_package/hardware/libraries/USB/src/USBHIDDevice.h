#ifndef _USB_HIDDEVICE_H_
#define _USB_HIDDEVICE_H_

#ifdef BOARD_AITHINKER_BW16
// #error "USB functionality is not supported on BW16 development board"
#else

#include <Arduino.h>
#include "HID.h"

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
#include "usbd_hid.h"

#ifdef __cplusplus
}
#endif

// Endpoint address
#define HID_INTERRUPT_IN_EP_ADDRESS             0x81

// Report ID should start with 1
#define REPORT_ID_KEYBOARD                      1
#define REPORT_ID_CONSUMER_CONTROL              2
#define REPORT_ID_MOUSE                         3

// Thread priorities
#define CONFIG_USBD_HID_INIT_THREAD_PRIORITY    5
#define CONFIG_USBD_HID_ISR_THREAD_PRIORITY     7
#define CONFIG_USBD_HID_HOTPLUG_THREAD_PRIORITY 8    // Should be higher than CONFIG_USBD_HID_ISR_THREAD_PRIORITY

#define USBD_HID_CONFIG_DESC_SIZ      34
#define USBD_HID_DEV_REPORT_DESC_SIZE 52

class USBHIDDevice {
public:
    USBHIDDevice();
    ~USBHIDDevice();

    void setReportDescriptor(uint8_t* report_desc, uint16_t len);
    void setUSBEndpointMPS(uint8_t max_packet_size);
    void setUSBEndpointInterval(uint8_t poll_interval);
    void setVID(uint16_t vid);
    void setPID(uint16_t pid);
    void setManufacturerString(const char* manufacturer);
    void setModelString(const char* model);
    void setSerialString(const char* serial);

    uint8_t USBconnected();
    void begin();
    void end();
    void inputReport(uint8_t reportID, uint8_t* data, uint16_t len);

private:
    static void usbdHIDStatusChanged(usb_dev_t* dev, uint8_t status);
    static void usbConnectDetectTask(void* param);
    static uint8_t usbdHIDInit(uint16_t tx_buf_len, usbd_hid_usr_cb_t* cb = NULL);
    static uint8_t usbdHIDDeinit(void);
    static uint8_t usbdHIDSendData(uint8_t* data, uint16_t len);

    //----------------------------------- USB Driver Functions -----------------------------------//
    static uint8_t* usbdHIDGetDescriptor(usb_dev_t* dev, usb_setup_req_t* req, usb_speed_type_t speed, uint16_t* len);
    static uint8_t usbdHIDSetConfig(usb_dev_t* dev, uint8_t config);
    static uint8_t usbdHIDClearConfig(usb_dev_t* dev, uint8_t config);
    static uint8_t usbdHIDSetup(usb_dev_t* dev, usb_setup_req_t* req);
    static uint8_t usbdHIDHandleEPDataIn(usb_dev_t* dev, uint8_t ep_num, uint8_t status);

    static uint8_t* _pReportDesc;
    static uint16_t _reportDescSize;
    static uint8_t _usbEPMPS;
    static uint8_t _usbEPInterval;
    static usbd_config_t _hidCfg;
    static usbd_class_driver_t _usbdHIDDriver;
    static usbd_hid_usr_cb_t _usbdHIDUsrCallback;

    static uint16_t _usbVID;
    static uint16_t _usbPID;
    static uint8_t* _pManufacturerStr;
    static uint8_t _manufacturerStrLen;
    static uint8_t* _pModelStr;
    static uint8_t _modelStrLen;
    static uint8_t* _pSerialStr;
    static uint8_t _serialStrLen;

    static uint8_t _usbStatus;
    static uint8_t _oldUsbStatus;
    static struct task_struct _usbDetectTask;

    // sema
    static _sema hid_connect_sema;
    static _sema hid_transmit_sema;
    static _sema hid_attach_status_changed_sema;

    static bool hid_connected;
};

extern USBHIDDevice USBHIDDev;

#endif

#endif
