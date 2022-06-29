#ifndef _USB_CDCDEVICE_H_
#define _USB_CDCDEVICE_H_

#ifdef BOARD_RTL8720DN_BW16
    #error "USB functionality is not supported on BW16 development board"
#endif

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "usbd.h"
#include "usbd_cdc_acm.h"

#ifdef __cplusplus
}
#endif

class USBCDCDevice : public HardwareSerial{
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
        void begin(uint32_t baud = 0);
        void end();
        int available();
        int peek();
        int read();
        void flush();
        size_t write(uint8_t data);
        size_t write(const uint8_t *buffer, size_t size);
        using Print::write; // pull in overloaded write functions from Print which are not overridden here
        operator bool();

    private:
        static void usbConnectDetectTask(void* param);
        static uint8_t usbdCDCInit(uint16_t rx_buf_len, uint16_t tx_buf_len);
        static uint8_t usbdCDCDeinit(void);
        static size_t usbdCDCTransmit(const uint8_t* data, size_t len);
        static uint8_t usbdCDCReceive(void);

        //----------------------------------- USB Driver Functions -----------------------------------//
        static uint8_t* usbdCDCGetDescriptor(usb_setup_req_t* req, usbd_speed_type_t speed, uint16_t *len);
        static uint8_t usbdCDCSetConfig(usb_dev_t* dev, uint8_t config);
        static uint8_t usbdCDCClearConfig(usb_dev_t* dev, uint8_t config);
        static uint8_t usbdCDCSetup(usb_dev_t* dev, usb_setup_req_t* req);
        static uint8_t usbdCDCHandleEP0DataOut(usb_dev_t* dev);
        static uint8_t usbdCDCHandleEPDataIn(usb_dev_t* dev, uint8_t ep_num);
        static uint8_t usbdCDCHandleEPDataOut(usb_dev_t* dev, uint8_t ep_num, uint16_t len);

        static uint8_t cdcACMConfigCoding(uint8_t cmd, uint8_t *pbuf, uint16_t len, uint16_t value);

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
        static uint8_t _oldUsbStatus;
        static struct task_struct _usbDetectTask;

        static usbd_cdc_acm_line_coding_t _cdcACMLineCoding;
        static uint8_t _dtr;
        static uint8_t _rts;
};

extern USBCDCDevice SerialUSB;

#endif

