#include "USBHIDDevice.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "rtl8721d_usb.h"
#include "usbd.h"
#include "usbd_hid.h"

#ifdef __cplusplus
}
#endif

USBHIDDevice USBHIDDev;

uint8_t* USBHIDDevice::_pReportDesc = nullptr;
uint16_t USBHIDDevice::_reportDescSize = 0;
uint8_t USBHIDDevice::_usbEPMPS = 9;
uint8_t USBHIDDevice::_usbEPInterval = 1;
uint16_t USBHIDDevice::_usbVID = 0x0BDA;
uint16_t USBHIDDevice::_usbPID = 0x4042;
uint8_t* USBHIDDevice::_pManufacturerStr = nullptr;
uint8_t USBHIDDevice::_manufacturerStrLen = 0;
uint8_t* USBHIDDevice::_pModelStr = nullptr;
uint8_t USBHIDDevice::_modelStrLen = 0;
uint8_t* USBHIDDevice::_pSerialStr = nullptr;
uint8_t USBHIDDevice::_serialStrLen = 0;
uint8_t USBHIDDevice::_usbStatus = USBD_ATTACH_STATUS_INIT;
uint8_t USBHIDDevice::_oldUsbStatus = USBD_ATTACH_STATUS_INIT;
struct task_struct USBHIDDevice::_usbDetectTask;

usbd_config_t USBHIDDevice::_hidCfg = {
    .max_ep_num = 4U,
    .rx_fifo_size = 512U,
    .nptx_fifo_size = 256U,
    .ptx_fifo_size = 64U,
    .intr_use_ptx_fifo = FALSE,
    .speed = USBD_SPEED_FULL,
    .dma_enable = FALSE,
    .self_powered = HID_SELF_POWERED,
    .isr_priority = 4U
};

usbd_class_driver_t USBHIDDevice::_usbdHIDDriver = {
    .get_descriptor = usbdHIDGetDescriptor,
    .set_config = usbdHIDSetConfig,
    .clear_config = usbdHIDClearConfig,
    .setup = usbdHIDSetup,
    .sof = NULL,
    .suspend = NULL,
    .resume = NULL,
    .ep0_data_in = NULL,
    .ep0_data_out = NULL,
    .ep_data_in = usbdHIDHandleEPDataIn,
    .ep_data_out = NULL
};

static struct usb_hid_device_t hid_device;

// USB Standard Device Descriptor
static uint8_t usbd_hid_dev_desc[USB_LEN_DEV_DESC] = {
    USB_LEN_DEV_DESC,                   //bLength
    USB_DESC_TYPE_DEVICE,               //bDescriptorType
    0x00,                               //bcdUSB
    0x02,
    0x00,                               //bDeviceClass
    0x00,                               //bDeviceSubClass
    0x00,                               //bDeviceProtocol
    USB_MAX_EP0_SIZE,                   //bMaxPacketSize
    USB_LOW_BYTE(HID_VID),              //idVendor
    USB_HIGH_BYTE(HID_VID),
    USB_LOW_BYTE(HID_PID),              //idProduct
    USB_HIGH_BYTE(HID_PID),
    0x00,                               //bcdDevice
    0x02,
    USBD_HID_STRING_MANUFACTURER,       //iManufacturer
    USBD_HID_STRING_PRODUCT,            //iProduct
    USBD_HID_STRING_SERIALNUMBER,       //iSerialNumber
    0x01                                //bNumConfigurations
};

// USB Standard String Descriptor 0
static uint8_t usbd_hid_lang_id_desc[USB_LEN_LANGID_STR_DESC] = {
    USB_LEN_LANGID_STR_DESC,            //bLength
    USB_DESC_TYPE_STRING,               //bDescriptorType
    USB_LOW_BYTE(HID_LANGID_STRING),    //wLANGID
    USB_HIGH_BYTE(HID_LANGID_STRING)
};

// USB HID Configuration Descriptor
static uint8_t usbd_hid_config_desc[HID_CONFIG_DESC_SIZ] = {
    // USB Standard Configuration Descriptor
    USB_LEN_CFG_DESC,                   //bLength
    USB_DESC_TYPE_CONFIGURATION,        //bDescriptorType: Configuration
    HID_CONFIG_DESC_SIZ,                //wTotalLength
    0x00,
    0x01,                               //bNumInterfaces
    0x01,                               //bConfigurationValue
    0x00,                               //iConfiguration
    0x80,                               //bmAttributes
    0x32,                               //MaxPower 100 mA

    // HID Interface Descriptor
    USB_LEN_IF_DESC,                    //bLength
    USB_DESC_TYPE_INTERFACE,            //bDescriptorType
    0x00,                               //bInterfaceNumber
    0x00,                               //bAlternateSetting
    0x01,                               //bNumEndpoints
    0x03,                               //bInterfaceClass: HID
    0x00,                               //bInterfaceSubClass: 1=BOOT, 0=no boot
    0x00,                               //nInterfaceProtocol: 0=none, 1=keyboard, 2=mouse
    0,                                  //iInterface

    //HID Descriptor
    0x09,                               //bLength
    0x21,                               //bDescriptorType
    0x11,                               //bcdHID
    0x01,
    0x00,                               //bCountryCode
    0x01,                               //bNumDescriptors
    0x22,                               //bDescriptorType
    HID_MOUSE_REPORT_DESC_SIZE,         //wItemLength
    0x00,

    // Endpoint Descriptor
    USB_LEN_EP_DESC,                    //bLength
    USB_DESC_TYPE_ENDPOINT,             //bDescriptorType:
    HID_INTERRUPT_IN_EP_ADDRESS,        //bEndpointAddress
    0x03,                               //bmAttributes
    HID_DEFAULT_INT_IN_XFER_SIZE,       //wMaxPacketSize
    0x00,
    0x1                                 //bInterval
};

uint8_t usb_hid_report_descriptor[] = {
    TUD_HID_REPORT_DESC_KEYBOARD( HID_REPORT_ID(REPORT_ID_KEYBOARD) ),
    TUD_HID_REPORT_DESC_CONSUMER( HID_REPORT_ID(REPORT_ID_CONSUMER_CONTROL) ),
    TUD_HID_REPORT_DESC_MOUSE   ( HID_REPORT_ID(REPORT_ID_MOUSE) )
};

USBHIDDevice::USBHIDDevice() {
    setReportDescriptor(usb_hid_report_descriptor, sizeof(usb_hid_report_descriptor));
    setManufacturerString("Realtek");
    setModelString("Realtek USB HID");
    setSerialString("0123456789");
}

USBHIDDevice::~USBHIDDevice() {
    free(_pManufacturerStr);
    free(_pModelStr);
    free(_pSerialStr);
}

void USBHIDDevice::setReportDescriptor(uint8_t* report_desc, uint16_t len) {
    _pReportDesc = report_desc;
    _reportDescSize = len;
}

void USBHIDDevice::setUSBEndpointMPS(uint8_t max_packet_size) {
    if (max_packet_size <= 64) {
        _usbEPMPS = max_packet_size;
    }
}

void USBHIDDevice::setUSBEndpointInterval(uint8_t poll_interval) {
    _usbEPInterval = poll_interval;
}

void USBHIDDevice::setVID(uint16_t vid) {
    _usbVID = vid;
}

void USBHIDDevice::setPID(uint16_t pid) {
    _usbPID = pid;
}

void USBHIDDevice::setManufacturerString(const char* manufacturer) {
    // Calculate memory space required for USB descriptor headers + string
    uint8_t len = 2 + 2 * strlen(manufacturer);
    // Allocate memory space and set to zero
    _pManufacturerStr = (uint8_t*) realloc(_pManufacturerStr, len);
    if (_pManufacturerStr == NULL) {
        printf("Error: Not enough memory to allocate for USB HID string\n");
        _manufacturerStrLen = 0;
        return;
    }
    memset(_pManufacturerStr, 0, len);
    // Fill in headers and string data
    _pManufacturerStr[0] = len;
    _pManufacturerStr[1] = USB_DESC_TYPE_STRING;
    for (uint8_t i = 0; i < strlen(manufacturer); i++) {
        _pManufacturerStr[2 + 2 * i] = manufacturer[i];
    }
    _manufacturerStrLen = len;
}

void USBHIDDevice::setModelString(const char* model) {
    // Calculate memory space required for USB descriptor headers + string
    uint8_t len = 2 + 2 * strlen(model);
    // Allocate memory space and set to zero
    _pModelStr = (uint8_t*) realloc(_pModelStr, len);
    if (_pModelStr == NULL) {
        printf("Error: Not enough memory to allocate for USB HID string\n");
        _modelStrLen = 0;
        return;
    }
    memset(_pModelStr, 0, len);
    // Fill in headers and string data
    _pModelStr[0] = len;
    _pModelStr[1] = USB_DESC_TYPE_STRING;
    for (uint8_t i = 0; i < strlen(model); i++) {
        _pModelStr[2 + 2 * i] = model[i];
    }
    _modelStrLen = len;
}

void USBHIDDevice::setSerialString(const char* serial) {
    // Calculate memory space required for USB descriptor headers + string
    uint8_t len = 2 + 2 * strlen(serial);
    // Allocate memory space and set to zero
    _pSerialStr = (uint8_t*) realloc(_pSerialStr, len);
    if (_pSerialStr == NULL) {
        printf("Error: Not enough memory to allocate for USB HID string\n");
        _serialStrLen = 0;
        return;
    }
    memset(_pSerialStr, 0, len);
    // Fill in headers and string data
    _pSerialStr[0] = len;
    _pSerialStr[1] = USB_DESC_TYPE_STRING;
    for (uint8_t i = 0; i < strlen(serial); i++) {
        _pSerialStr[2 + 2 * i] = serial[i];
    }
    _serialStrLen = len;
}

void USBHIDDevice::begin() {
    // Update USB device descriptor with new VID & PID
    usbd_hid_dev_desc[8] = USB_LOW_BYTE(_usbVID);
    usbd_hid_dev_desc[9] = USB_HIGH_BYTE(_usbVID);
    usbd_hid_dev_desc[10] = USB_LOW_BYTE(_usbPID);
    usbd_hid_dev_desc[11] = USB_HIGH_BYTE(_usbPID);

    // Update USB config descriptor with new HID report desc size, EP MPS, update interval
    usbd_hid_config_desc[25] = USB_LOW_BYTE(_reportDescSize);
    usbd_hid_config_desc[26] = USB_HIGH_BYTE(_reportDescSize);
    usbd_hid_config_desc[31] = _usbEPMPS;
    usbd_hid_config_desc[33] = _usbEPInterval;

    // Start USB device stack
    int ret = 0;
    ret = usbd_init(&_hidCfg);
    if (ret != 0) {
        printf("Error: Failed to init USBD controller\n");
        return;
    }
    ret = usbdHIDInit(512);
    if (ret != 0) {
        printf("Error: Failed to init USB HID class\n");
        usbd_deinit();
        return;
    }
    ret = rtw_create_task(&_usbDetectTask, "USBD_HID_Connect_Detection_Task", 512, tskIDLE_PRIORITY + 2, usbConnectDetectTask, NULL);
    if (ret != pdPASS) {
        printf("Fail to create USB connection detection thread\n");
        usbdHIDDeinit();
        usbd_deinit();
        return;
    }
    printf("USB HID Device started\n");
}

void USBHIDDevice::end() {
    rtw_delete_task(&_usbDetectTask);
    usbdHIDDeinit();
    usbd_deinit();
    printf("USB HID Device stopped\n");
}

uint8_t USBHIDDevice::USBconnected() {
    if (_usbStatus == USBD_ATTACH_STATUS_ATTACHED) {
        return (1);
    } else {
        return (0);
    }
}

void USBHIDDevice::inputReport(uint8_t reportID, uint8_t* data, uint16_t len) {
    uint8_t report[64] = {0};

    // If reportID == 0, not a valid reportID, send HID data without reportID
    if (reportID) {
        report[0] = reportID;
        memcpy(&(report[1]), data, len);
    } else {
        memcpy(&(report[0]), data, len);
    }
    // Send report over USB endpoint
    usbdHIDSendData(report, _usbEPMPS);
}

void USBHIDDevice::usbConnectDetectTask(void *param) {
    (void)param;
    int ret = 0;

    for (;;) {
        rtw_mdelay_os(100);
        _usbStatus = usbd_get_attach_status();
        if (_oldUsbStatus != _usbStatus) {
            _oldUsbStatus = _usbStatus;
            if (_usbStatus == USBD_ATTACH_STATUS_DETACHED) {
                printf("USB Disconnected\n");
                usbdHIDDeinit();
                usbd_deinit();

                rtw_mdelay_os(100);
                ret = usbd_init(&_hidCfg);
                if (ret != 0) {
                    printf("Error: Failed to re-init USBD controller\n");
                    break;
                }
                ret = usbdHIDInit(512);
                if (ret != 0) {
                    printf("Error: Failed to re-init USB HID class\n");
                    usbd_deinit();
                    break;
                }
            }else if (_usbStatus == USBD_ATTACH_STATUS_ATTACHED) {
                printf("USB Connected\n");
            } else {
                printf("USB Initialized\n");
            }
        }
    }
    rtw_thread_exit();
}

uint8_t USBHIDDevice::usbdHIDInit(uint16_t tx_buf_len) {
    uint8_t ret = HAL_OK;
    struct usb_hid_device_t *hid = &hid_device;

    hid->ctrl_buf = rtw_zmalloc(HID_CTRL_BUF_SIZE);
    if (hid->ctrl_buf == NULL) {
        ret = HAL_ERR_MEM;
        return ret;
    }
    if (tx_buf_len > 0) {
        hid->intr_in_buf = rtw_zmalloc(tx_buf_len);
    } else {
        hid->intr_in_buf = rtw_zmalloc(HID_INTR_IN_BUF_SIZE);
    }
    if (hid->intr_in_buf == NULL) {
        ret = HAL_ERR_MEM;
        rtw_free(hid->ctrl_buf);
        hid->ctrl_buf = NULL;
        return ret;
    }

    usbd_register_class(&_usbdHIDDriver);
    return ret;
}

uint8_t USBHIDDevice::usbdHIDDeinit(void) {
    struct usb_hid_device_t *hid = &hid_device;

    usbd_unregister_class();
    if (hid->ctrl_buf != NULL) {
        rtw_free(hid->ctrl_buf);
        hid->ctrl_buf = NULL;
    }
    if (hid->intr_in_buf != NULL) {
        rtw_free(hid->intr_in_buf);
        hid->intr_in_buf = NULL;
    }
    return HAL_OK;
}

uint8_t USBHIDDevice::usbdHIDSendData(uint8_t* data, uint16_t len) {
    uint8_t ret = HAL_ERR_HW;
    if (_usbStatus != USBD_ATTACH_STATUS_ATTACHED) {
        return ret;
    }
    struct usb_hid_device_t *hid = &hid_device;

    while (hid->intr_in_state) {
        // Wait for previous USB transmission to finish
        delay(1);
    }
    hid->intr_in_state = 1U;
    rtw_memcpy((void *)hid->intr_in_buf, (void *)data, len);
    usbd_ep_transmit(hid->dev, HID_INTERRUPT_IN_EP_ADDRESS, hid->intr_in_buf, len);
    ret = HAL_OK;
    return ret;
}

//----------------------------------- USB Driver Functions -----------------------------------//

uint8_t* USBHIDDevice::usbdHIDGetDescriptor(usb_setup_req_t* req, usbd_speed_type_t speed, uint16_t* len) {
    (void)speed;
    uint8_t* buf = NULL;

//    printf("hid_get_descriptor | bmRequestType=0x%02X | bRequest=0x%02X | wValue=0x%04x | wIndex=0x%04x | wLength=0x%04X\n",
//        req->bmRequestType, req->bRequest, req->wValue, req->wIndex, req->wLength);

    switch ((req->wValue >> 8) & 0xFF) {
        case USB_DESC_TYPE_DEVICE: {
            buf = usbd_hid_dev_desc;
            *len = sizeof(usbd_hid_dev_desc);
            break;
        }
        case USB_DESC_TYPE_CONFIGURATION: {
            buf = usbd_hid_config_desc;
            *len = sizeof(usbd_hid_config_desc);
            break;
        }
        case USB_DESC_TYPE_DEVICE_QUALIFIER: {
            // Ignore if USB high speed not supported
            break;
        }
        case USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION: {
            // Ignore if USB high speed not supported
            break;
        }
        case USB_DESC_TYPE_STRING: {
            switch (req->wValue & 0xFF) {
                case USBD_IDX_LANGID_STR: {
                    buf = usbd_hid_lang_id_desc;
                    *len = sizeof(usbd_hid_lang_id_desc);
                    break;
                }
                case USBD_IDX_MFC_STR: {
                    buf = _pManufacturerStr;
                    *len = _manufacturerStrLen;
                    break;
                }
                case USBD_IDX_PRODUCT_STR: {
                    buf = _pModelStr;
                    *len = _modelStrLen;
                    break;
                }
                case USBD_IDX_SERIAL_STR: {
                    buf = _pSerialStr;
                    *len = _serialStrLen;
                    break;
                }
                default: {
//                    printf("Get descriptor failed, invalid string index %d\n", req->wValue & 0xFF);
                    break;
                }
            }
            break;
        }
        default: {
            break;
        }
    }
    return buf;
}

uint8_t USBHIDDevice::usbdHIDSetConfig(usb_dev_t* dev, uint8_t config) {
    (void)config;

    struct usb_hid_device_t *hid = &hid_device;

    hid->dev = dev;
    hid->intr_in_state = 0;
    /* Init INTR IN EP */
    usbd_ep_init(dev, HID_INTERRUPT_IN_EP_ADDRESS, USB_CH_EP_TYPE_INTR, _usbEPMPS);
    return HAL_OK;
}

uint8_t USBHIDDevice::usbdHIDClearConfig(usb_dev_t* dev, uint8_t config) {
    (void)config;

    /* DeInit INTR IN EP */
    usbd_ep_deinit(dev, HID_INTERRUPT_IN_EP_ADDRESS);
    return HAL_OK;
}

uint8_t USBHIDDevice::usbdHIDSetup(usb_dev_t* dev, usb_setup_req_t* req) {
    struct usb_hid_device_t *hid = &hid_device;
    uint16_t len = 0;
    uint8_t *pbuf = NULL;
    uint8_t ret = HAL_OK;

//    printf("hid_setup | bmRequestType=0x%02X | bRequest=0x%02X | wValue=0x%04x | wIndex=0x%04x | wLength=0x%04X\n",
//        req->bmRequestType, req->bRequest, req->wValue, req->wIndex, req->wLength);

    switch (req->bmRequestType & USB_REQ_TYPE_MASK) {
        case USB_REQ_TYPE_STANDARD: {
            switch (req->bRequest) {
                case USB_REQ_SET_INTERFACE: {
                    if (dev->dev_state != USBD_STATE_CONFIGURED) {
                        ret = HAL_ERR_HW;
                    }
                    break;
                }
                case USB_REQ_GET_INTERFACE: {
                    if (dev->dev_state == USBD_STATE_CONFIGURED) {
                        hid->ctrl_buf[0] = 0U;
                        usbd_ep0_transmit(dev, hid->ctrl_buf, 1);
                    } else {
                        ret = HAL_ERR_HW;
                    }
                    break;
                }
                case USB_REQ_GET_STATUS: {
                    if (dev->dev_state == USBD_STATE_CONFIGURED) {
                        hid->ctrl_buf[0] = 0U;
                        hid->ctrl_buf[1] = 0U;
                        usbd_ep0_transmit(dev, hid->ctrl_buf, 2);
                    } else {
                        ret = HAL_ERR_HW;
                    }
                    break;
                }
                case USB_REQ_GET_DESCRIPTOR: {
                    if (req->wValue >> 8 == HID_REPORT_DESC) {
                        len = MIN(_reportDescSize, req->wLength);
                        pbuf = _pReportDesc;
                    } else {
                        ret = HAL_ERR_HW;
                        break;
                    }
                    usbd_ep0_transmit(dev, pbuf, len);
                    break;
                }
                default: {
                    ret = HAL_ERR_HW;
//                    printf("hid_setup REQ STANDARD unhandled | bRequest=0x%02X\n", req->bRequest);
                    break;
                }
            }
            break;
        }
        case USB_REQ_TYPE_CLASS: {
            switch (req->bRequest) {
                case USBD_HID_GET_REPORT: {
//                    uint8_t report_type = (req->wValue)>>8;
//                    uint8_t report_ID = (req->wValue)&0xff;
//                    uint16_t report_interface = req->wIndex;
//                    uint16_t report_length = req->wLength;
//                    printf("hid_setup REQ CLASS GET REPORT | type=%d | ID=%d | interface=%d | length=%d\n",
//                        report_type, report_ID, report_interface, report_length);
                    ret = HAL_ERR_HW;
                    break;
                }
                case USBD_HID_GET_IDLE: {
                    usbd_ep0_transmit(dev, (u8 *) &hid->IdleState, 1);
                    break;
                }
                case USBD_HID_GET_PROTOCOL: {
                    usbd_ep0_transmit(dev, (u8 *) &hid->Protocol, 1);
                    break;
                }
                case USBD_HID_SET_REPORT: {
//                    uint8_t report_type = (req->wValue)>>8;
//                    uint8_t report_ID = (req->wValue)&0xff;
//                    uint16_t report_interface = req->wIndex;
//                    uint16_t report_length = req->wLength;
//                    printf("hid_setup REQ CLASS SET REPORT | type=%d | ID=%d | interface=%d | length=%d\n",
//                        report_type, report_ID, report_interface, report_length);
                    ret = HAL_ERR_HW;
                    break;
                }
                case USBD_HID_SET_IDLE: {
                    hid->IdleState = req->wValue >> 8;
                    break;
                }
                case USBD_HID_SET_PROTOCOL: {
                    hid->Protocol = req->wValue;
                    break;
                }
                default: {
                    ret = HAL_ERR_HW;
//                    printf("hid_setup REQ CLASS unhandled | bRequest=0x%02X\n", req->bRequest);
                    break;
                }
            }
            break;
        }
        default: {
            ret = HAL_ERR_HW;
            break;
        }
    }
    return ret;
}

uint8_t USBHIDDevice::usbdHIDHandleEPDataIn(usb_dev_t* dev, uint8_t ep_num) {
    (void)dev;
    (void)ep_num;

    struct usb_hid_device_t *hid = &hid_device;
    // Previous TX done, mark as avaliable
    hid->intr_in_state = 0U;
    return HAL_OK;
}

