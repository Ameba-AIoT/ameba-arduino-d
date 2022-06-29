#ifndef _USB_HIDDEVICE_H_
#define _USB_HIDDEVICE_H_

#ifdef BOARD_RTL8720DN_BW16
    #error "USB functionality is not supported on BW16 development board"
#endif

#include <Arduino.h>
#include "HID.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "usbd.h"
#include "usbd_hid.h"

#ifdef __cplusplus
}
#endif

// Endpoint address
#define HID_INTERRUPT_IN_EP_ADDRESS		0x81

// Report ID should start with 1
#define REPORT_ID_KEYBOARD          1
#define REPORT_ID_CONSUMER_CONTROL  2
#define REPORT_ID_MOUSE             3

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
        static void usbConnectDetectTask(void* param);
        static uint8_t usbdHIDInit(uint16_t tx_buf_len);
        static uint8_t usbdHIDDeinit(void);
        static uint8_t usbdHIDSendData(uint8_t* data, uint16_t len);

        //----------------------------------- USB Driver Functions -----------------------------------//
        static uint8_t* usbdHIDGetDescriptor(usb_setup_req_t* req, usbd_speed_type_t speed, uint16_t *len);
        static uint8_t usbdHIDSetConfig(usb_dev_t* dev, uint8_t config);
        static uint8_t usbdHIDClearConfig(usb_dev_t* dev, uint8_t config);
        static uint8_t usbdHIDSetup(usb_dev_t* dev, usb_setup_req_t* req);
        static uint8_t usbdHIDHandleEPDataIn(usb_dev_t* dev, uint8_t ep_num);

        static uint8_t* _pReportDesc;
        static uint16_t _reportDescSize;
        static uint8_t _usbEPMPS;
        static uint8_t _usbEPInterval;
        static usbd_config_t _hidCfg;
        static usbd_class_driver_t _usbdHIDDriver;

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

};

extern USBHIDDevice USBHIDDev;

#endif

