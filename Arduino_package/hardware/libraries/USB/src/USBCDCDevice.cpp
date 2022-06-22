#include "USBCDCDevice.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "rtl8721d_usb.h"
#include "usbd.h"
#include "usbd_cdc_acm.h"

#ifdef __cplusplus
}
#endif

USBCDCDevice SerialUSB;

uint16_t USBCDCDevice::_usbVID = 0x0BDA;
uint16_t USBCDCDevice::_usbPID = 0x4042;
uint8_t* USBCDCDevice::_pManufacturerStr = nullptr;
uint8_t USBCDCDevice::_manufacturerStrLen = 0;
uint8_t* USBCDCDevice::_pModelStr = nullptr;
uint8_t USBCDCDevice::_modelStrLen = 0;
uint8_t* USBCDCDevice::_pSerialStr = nullptr;
uint8_t USBCDCDevice::_serialStrLen = 0;
uint8_t USBCDCDevice::_usbStatus = USBD_ATTACH_STATUS_INIT;
uint8_t USBCDCDevice::_oldUsbStatus = USBD_ATTACH_STATUS_INIT;
struct task_struct USBCDCDevice::_usbDetectTask;
usbd_cdc_acm_line_coding_t USBCDCDevice::_cdcACMLineCoding;
uint8_t USBCDCDevice::_dtr = 0;
uint8_t USBCDCDevice::_rts = 0;

struct ring_buffer
{
    uint8_t buffer[CDC_ACM_HS_BULK_MAX_PACKET_SIZE];
    volatile uint32_t _iHead;
    volatile uint32_t _iTail;
};
static ring_buffer rx_buffer;

usbd_config_t USBCDCDevice::_cdcCfg = {
    .max_ep_num = 4U,
    .rx_fifo_size = 512U,
    .nptx_fifo_size = 256U,
    .ptx_fifo_size = 64U,
    .intr_use_ptx_fifo = TRUE,
    .speed = USBD_SPEED_HIGH,
    .dma_enable = FALSE,
    .self_powered = CDC_ACM_SELF_POWERED,
    .isr_priority = 4U,
};

usbd_class_driver_t USBCDCDevice::_usbdCDCDriver = {
    .get_descriptor = usbdCDCGetDescriptor,
    .set_config = usbdCDCSetConfig,
    .clear_config = usbdCDCClearConfig,
    .setup = usbdCDCSetup,
    .sof = NULL,
    .suspend = NULL,
    .resume = NULL,
    .ep0_data_in = NULL,
    .ep0_data_out = usbdCDCHandleEP0DataOut,
    .ep_data_in = usbdCDCHandleEPDataIn,
    .ep_data_out = usbdCDCHandleEPDataOut
};

static usbd_cdc_acm_dev_t usbd_cdc_acm_dev;

// USB Standard Device Descriptor
static uint8_t usbd_cdc_acm_dev_desc[USB_LEN_DEV_DESC] USB_DMA_ALIGNED = {
    USB_LEN_DEV_DESC,                               //bLength
    USB_DESC_TYPE_DEVICE,                           //bDescriptorType
    0x00,                                           //bcdUSB
    0x02,
    0x02,                                           //bDeviceClass
    0x02,                                           //bDeviceSubClass
    0x00,                                           //bDeviceProtocol
    USB_MAX_EP0_SIZE,                               //bMaxPacketSize
    USB_LOW_BYTE(CDC_ACM_VID),                      //idVendor
    USB_HIGH_BYTE(CDC_ACM_VID),
    USB_LOW_BYTE(CDC_ACM_PID),                      //idProduct
    USB_HIGH_BYTE(CDC_ACM_PID),
    0x00,                                           //bcdDevice
    0x02,
    USBD_IDX_MFC_STR,                               //iManufacturer
    USBD_IDX_PRODUCT_STR,                           //iProduct
    USBD_IDX_SERIAL_STR,                            //iSerialNumber
    0x01                                            //bNumConfigurations
};

// USB Standard String Descriptor 0
static uint8_t usbd_cdc_acm_lang_id_desc[USB_LEN_LANGID_STR_DESC] USB_DMA_ALIGNED = {
    USB_LEN_LANGID_STR_DESC,                        //bLength
    USB_DESC_TYPE_STRING,                           //bDescriptorType
    USB_LOW_BYTE(CDC_ACM_LANGID_STRING),            //wLANGID
    USB_HIGH_BYTE(CDC_ACM_LANGID_STRING)
};

// USB Standard Device Qualifier Descriptor
static uint8_t usbd_cdc_acm_device_qualifier_desc[USB_LEN_DEV_QUALIFIER_DESC] USB_DMA_ALIGNED = {
    USB_LEN_DEV_QUALIFIER_DESC,                     //bLength
    USB_DESC_TYPE_DEVICE_QUALIFIER,                 //bDescriptorType
    0x00,                                           //bcdUSB
    0x02,
    0x00,                                           //bDeviceClass
    0x00,                                           //bDeviceSubClass
    0x00,                                           //bDeviceProtocol
    0x40,                                           //bMaxPacketSize
    0x01,                                           //bNumConfigurations
    0x00                                            //Reserved
};

// USB CDC ACM Device High Speed Configuration Descriptor
static uint8_t usbd_cdc_acm_hs_config_desc[CDC_ACM_CONFIG_DESC_SIZE] USB_DMA_ALIGNED = {
    // USB Standard Configuration Descriptor
    USB_LEN_CFG_DESC,                               //bLength
    USB_DESC_TYPE_CONFIGURATION,                    //bDescriptorType
    CDC_ACM_CONFIG_DESC_SIZE,                       //wTotalLength
    0x00,
    0x02,                                           //bNumInterfaces
    0x01,                                           //bConfigurationValue
    0x00,                                           //iConfiguration
    0xC0,                                           //bmAttributes: self powered
    0x32,                                           //bMaxPower

    // CDC Communication Interface Descriptor
    USB_LEN_IF_DESC,                                //bLength
    USB_DESC_TYPE_INTERFACE,                        //bDescriptorType
    0x00,                                           //bInterfaceNumber
    0x00,                                           //bAlternateSetting
    0x01,                                           //bNumEndpoints
    0x02,                                           //bInterfaceClass: CDC
    0x02,                                           //bInterfaceSubClass: Abstract Control Model
    0x01,                                           //bInterfaceProtocol: Common AT commands
    0x00,                                           //iInterface

    // CDC Header Functional Descriptor
    0x05,                                           //bLength
    0x24,                                           //bDescriptorType: CS_INTERFACE
    0x00,                                           //bDescriptorSubtype: Header Functional Descriptor
    0x10,                                           //bcdCDC
    0x01,

    // CDC Call Management Functional Descriptor
    0x05,                                           //bFunctionLength
    0x24,                                           //bDescriptorType: CS_INTERFACE
    0x01,                                           //bDescriptorSubtype: Call Management Functional Descriptor
    0x00,                                           //bmCapabilities: D0+D1
    0x01,                                           //bDataInterface

    // CDC ACM Functional Descriptor
    0x04,                                           //bFunctionLength
    0x24,                                           //bDescriptorType: CS_INTERFACE
    0x02,                                           //bDescriptorSubtype: ACM Functional Descriptor
    0x02,                                           //bmCapabilities

    // CDC Union Functional Descriptor
    0x05,                                           //bFunctionLength
    0x24,                                           //bDescriptorType: CS_INTERFACE
    0x06,                                           //bDescriptorSubtype: Union Functional Descriptor
    0x00,                                           //bMasterInterface: Communication Class Interface
    0x01,                                           //bSlaveInterface0: Data Class Interface

    // INTR IN Endpoint Descriptor
    USB_LEN_EP_DESC,                                //bLength
    USB_DESC_TYPE_ENDPOINT,                         //bDescriptorType
    CDC_ACM_INTR_IN_EP,                             //bEndpointAddress
    0x03,                                           //bmAttributes: INTR
    USB_LOW_BYTE(CDC_ACM_INTR_IN_PACKET_SIZE),      //wMaxPacketSize
    USB_HIGH_BYTE(CDC_ACM_INTR_IN_PACKET_SIZE),
    CDC_ACM_HS_INTR_IN_INTERVAL,                    //bInterval:

    // CDC Data Interface Descriptor
    USB_LEN_IF_DESC,                                //bLength
    USB_DESC_TYPE_INTERFACE,                        //bDescriptorType:
    0x01,                                           //bInterfaceNumber
    0x00,                                           //bAlternateSetting
    0x02,                                           //bNumEndpoints
    0x0A,                                           //bInterfaceClass: CDC
    0x00,                                           //bInterfaceSubClass
    0x00,                                           //bInterfaceProtocol
    0x00,                                           //iInterface

    // BULK OUT Endpoint Descriptor
    USB_LEN_EP_DESC,                                //bLength
    USB_DESC_TYPE_ENDPOINT,                         //bDescriptorType
    CDC_ACM_BULK_OUT_EP,                            //bEndpointAddress
    0x02,                                           //bmAttributes: BULK
    USB_LOW_BYTE(CDC_ACM_HS_BULK_MAX_PACKET_SIZE),  //wMaxPacketSize:
    USB_HIGH_BYTE(CDC_ACM_HS_BULK_MAX_PACKET_SIZE),
    0x00,                                           //bInterval

    // BULK IN Endpoint Descriptor
    USB_LEN_EP_DESC,                                //bLength
    USB_DESC_TYPE_ENDPOINT,                         //bDescriptorType
    CDC_ACM_BULK_IN_EP,                             //bEndpointAddress
    0x02,                                           //bmAttributes: BULK
    USB_LOW_BYTE(CDC_ACM_HS_BULK_MAX_PACKET_SIZE),  //wMaxPacketSize:
    USB_HIGH_BYTE(CDC_ACM_HS_BULK_MAX_PACKET_SIZE),
    0x00                                            //bInterval
};

// USB CDC ACM Device Full Speed Configuration Descriptor
static uint8_t usbd_cdc_acm_fs_config_desc[CDC_ACM_CONFIG_DESC_SIZE] USB_DMA_ALIGNED = {
    // USB Standard Configuration Descriptor
    USB_LEN_CFG_DESC,                               //bLength
    USB_DESC_TYPE_CONFIGURATION,                    //bDescriptorType
    CDC_ACM_CONFIG_DESC_SIZE,                       //wTotalLength
    0x00,
    0x02,                                           //bNumInterfaces
    0x01,                                           //bConfigurationValue
    0x00,                                           //iConfiguration
    0xC0,                                           //bmAttributes: self powered
    0x32,                                           //bMaxPower

    // CDC Communication Interface Descriptor
    USB_LEN_IF_DESC,                                //bLength
    USB_DESC_TYPE_INTERFACE,                        //bDescriptorType
    0x00,                                           //bInterfaceNumber
    0x00,                                           //bAlternateSetting
    0x01,                                           //bNumEndpoints
    0x02,                                           //bInterfaceClass: CDC
    0x02,                                           //bInterfaceSubClass: Abstract Control Model
    0x01,                                           //bInterfaceProtocol: Common AT commands
    0x00,                                           //iInterface

    // CDC Header Functional Descriptor
    0x05,                                           //bLength
    0x24,                                           //bDescriptorType: CS_INTERFACE
    0x00,                                           //bDescriptorSubtype: Header Functional Descriptor
    0x10,                                           //bcdCDC
    0x01,

    // CDC Call Management Functional Descriptor
    0x05,                                           //bFunctionLength
    0x24,                                           //bDescriptorType: CS_INTERFACE
    0x01,                                           //bDescriptorSubtype: Call Management Functional Descriptor
    0x00,                                           //bmCapabilities: D0+D1
    0x01,                                           //bDataInterface

    // CDC ACM Functional Descriptor
    0x04,                                           //bFunctionLength
    0x24,                                           //bDescriptorType: CS_INTERFACE
    0x02,                                           //bDescriptorSubtype: ACM Functional Descriptor
    0x02,                                           //bmCapabilities

    // CDC Union Functional Descriptor
    0x05,                                           //bFunctionLength
    0x24,                                           //bDescriptorType: CS_INTERFACE
    0x06,                                           //bDescriptorSubtype: Union Functional Descriptor
    0x00,                                           //bMasterInterface: Communication Class Interface
    0x01,                                           //bSlaveInterface0: Data Class Interface

    // INTR IN Endpoint Descriptor
    USB_LEN_EP_DESC,                                //bLength
    USB_DESC_TYPE_ENDPOINT,                         //bDescriptorType
    CDC_ACM_INTR_IN_EP,                             //bEndpointAddress
    0x03,                                           //bmAttributes: INTR
    USB_LOW_BYTE(CDC_ACM_INTR_IN_PACKET_SIZE),      //wMaxPacketSize
    USB_HIGH_BYTE(CDC_ACM_INTR_IN_PACKET_SIZE),
    CDC_ACM_FS_INTR_IN_INTERVAL,                    //bInterval:

    // CDC Data Interface Descriptor
    USB_LEN_IF_DESC,                                //bLength
    USB_DESC_TYPE_INTERFACE,                        //bDescriptorType:
    0x01,                                           //bInterfaceNumber
    0x00,                                           //bAlternateSetting
    0x02,                                           //bNumEndpoints
    0x0A,                                           //bInterfaceClass: CDC
    0x00,                                           //bInterfaceSubClass
    0x00,                                           //bInterfaceProtocol
    0x00,                                           //iInterface

    // BULK OUT Endpoint Descriptor
    USB_LEN_EP_DESC,                                //bLength
    USB_DESC_TYPE_ENDPOINT,                         //bDescriptorType
    CDC_ACM_BULK_OUT_EP,                            //bEndpointAddress
    0x02,                                           //bmAttributes: BULK
    USB_LOW_BYTE(CDC_ACM_FS_BULK_MAX_PACKET_SIZE),  //wMaxPacketSize:
    USB_HIGH_BYTE(CDC_ACM_FS_BULK_MAX_PACKET_SIZE),
    0x00,                                           //bInterval

    // BULK IN Endpoint Descriptor
    USB_LEN_EP_DESC,                                //bLength
    USB_DESC_TYPE_ENDPOINT,                         //bDescriptorType
    CDC_ACM_BULK_IN_EP,                             //bEndpointAddress
    0x02,                                           //bmAttributes: BULK
    USB_LOW_BYTE(CDC_ACM_FS_BULK_MAX_PACKET_SIZE),  //wMaxPacketSize:
    USB_HIGH_BYTE(CDC_ACM_FS_BULK_MAX_PACKET_SIZE),
    0x00                                            //bInterval
};

USBCDCDevice::USBCDCDevice() {
    setManufacturerString("Realtek");
    setModelString("Realtek USB VCP");
    setSerialString("0123456789");
}

USBCDCDevice::~USBCDCDevice() {
    free(_pManufacturerStr);
    free(_pModelStr);
    free(_pSerialStr);
}

void USBCDCDevice::setVID(uint16_t vid) {
    _usbVID = vid;
}

void USBCDCDevice::setPID(uint16_t pid) {
    _usbPID = pid;
}

void USBCDCDevice::setManufacturerString(const char* manufacturer) {
    // Calculate memory space required for USB descriptor headers + string
    uint8_t len = 2 + 2 * strlen(manufacturer);
    // Allocate memory space and set to zero
    _pManufacturerStr = (uint8_t*) realloc(_pManufacturerStr, len);
    if (_pManufacturerStr == NULL) {
        printf("Error: Not enough memory to allocate for USB CDC string\n");
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

void USBCDCDevice::setModelString(const char* model) {
    // Calculate memory space required for USB descriptor headers + string
    uint8_t len = 2 + 2 * strlen(model);
    // Allocate memory space and set to zero
    _pModelStr = (uint8_t*) realloc(_pModelStr, len);
    if (_pModelStr == NULL) {
        printf("Error: Not enough memory to allocate for USB CDC string\n");
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

void USBCDCDevice::setSerialString(const char* serial) {
    // Calculate memory space required for USB descriptor headers + string
    uint8_t len = 2 + 2 * strlen(serial);
    // Allocate memory space and set to zero
    _pSerialStr = (uint8_t*) realloc(_pSerialStr, len);
    if (_pSerialStr == NULL) {
        printf("Error: Not enough memory to allocate for USB CDC string\n");
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

uint8_t USBCDCDevice::USBconnected() {
    if (_usbStatus == USBD_ATTACH_STATUS_ATTACHED) {
        return 1;
    } else {
        return 0;
    }
}

uint8_t USBCDCDevice::connected() {
    if (_usbStatus == USBD_ATTACH_STATUS_ATTACHED) {
        return (_dtr && _rts);
    } else {
        return 0;
    }
}

uint8_t USBCDCDevice::dtr() {
    if (_usbStatus == USBD_ATTACH_STATUS_ATTACHED) {
        return _dtr;
    } else {
        return 0;
    }
}

uint8_t USBCDCDevice::rts() {
    if (_usbStatus == USBD_ATTACH_STATUS_ATTACHED) {
        return _rts;
    } else {
        return 0;
    }
}

void USBCDCDevice::begin(uint32_t baud) {
    (void)baud;

    // Zero out RX buffer
    memset(rx_buffer.buffer, 0, CDC_ACM_HS_BULK_MAX_PACKET_SIZE);
    rx_buffer._iHead = 0;
    rx_buffer._iTail = 0;;

    // Update USB device descriptor with new VID & PID
    usbd_cdc_acm_dev_desc[8] = USB_LOW_BYTE(_usbVID);
    usbd_cdc_acm_dev_desc[9] = USB_HIGH_BYTE(_usbVID);
    usbd_cdc_acm_dev_desc[10] = USB_LOW_BYTE(_usbPID);
    usbd_cdc_acm_dev_desc[11] = USB_HIGH_BYTE(_usbPID);

    // Start USB device stack
    int ret = 0;
    ret = usbd_init(&_cdcCfg);
    if (ret != 0) {
        printf("Error: Failed to init USBD controller\n");
        return;
    }
    ret = usbdCDCInit(CDC_ACM_HS_BULK_MAX_PACKET_SIZE, CDC_ACM_HS_BULK_MAX_PACKET_SIZE);
    if (ret != 0) {
        printf("Error: Failed to init USB CDC class\n");
        usbd_deinit();
        return;
    }
    ret = rtw_create_task(&_usbDetectTask, "USBD_CDC_Connect_Detection_Task", 512, tskIDLE_PRIORITY + 2, usbConnectDetectTask, NULL);
    if (ret != pdPASS) {
        printf("Fail to create USB connection detection thread\n");
        usbdCDCDeinit();
        usbd_deinit();
        return;
    }
    printf("USB CDC Device started\n");
}

void USBCDCDevice::end() {
    rtw_delete_task(&_usbDetectTask);
    usbdCDCDeinit();
    usbd_deinit();
    _dtr = 0;
    _rts = 0;
    printf("USB CDC Device stopped\n");
}

int USBCDCDevice::available() {
    return ((uint32_t)(CDC_ACM_HS_BULK_MAX_PACKET_SIZE + rx_buffer._iHead - rx_buffer._iTail) % CDC_ACM_HS_BULK_MAX_PACKET_SIZE);
}

int USBCDCDevice::peek() {
    if (rx_buffer._iHead == rx_buffer._iTail) {
        return -1;
    }
    return rx_buffer.buffer[rx_buffer._iTail];
}

int USBCDCDevice::read() {
    // if the head isn't ahead of the tail, no data is available
    if (rx_buffer._iHead == rx_buffer._iTail) {
        return -1;
    }
    uint8_t uc = rx_buffer.buffer[rx_buffer._iTail];
    rx_buffer._iTail = (uint32_t)(rx_buffer._iTail + 1) % CDC_ACM_HS_BULK_MAX_PACKET_SIZE;
    return uc;
}

void USBCDCDevice::flush() {
    if (!connected()) {
        return;
    }
    usbd_cdc_acm_dev_t *cdev = &usbd_cdc_acm_dev;
    while (cdev->bulk_in_state) {
        // Wait for previous USB transmission to finish
        delay(1);
    }
}

size_t USBCDCDevice::write(uint8_t data) {
    return write(&data, 1);
}

size_t USBCDCDevice::write(const uint8_t *buffer, size_t size) {
    return usbdCDCTransmit(buffer, size);
}

USBCDCDevice::operator bool() {
    return connected();
}

void USBCDCDevice::usbConnectDetectTask(void* param) {
    (void)param;
    int ret = 0;

    for (;;) {
        rtw_mdelay_os(100);
        _usbStatus = usbd_get_attach_status();
        if (_oldUsbStatus != _usbStatus) {
            _oldUsbStatus = _usbStatus;
            if (_usbStatus == USBD_ATTACH_STATUS_DETACHED) {
                printf("USB Disconnected\n");
                usbdCDCDeinit();
                usbd_deinit();

                rtw_mdelay_os(100);
                ret = usbd_init(&_cdcCfg);
                if (ret != 0) {
                    printf("Error: Failed to re-init USBD controller\n");
                    break;
                }
                ret = usbdCDCInit(CDC_ACM_HS_BULK_MAX_PACKET_SIZE, CDC_ACM_HS_BULK_MAX_PACKET_SIZE);
                if (ret != 0) {
                    printf("Error: Failed to re-init USB CDC class\n");
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

uint8_t USBCDCDevice::usbdCDCInit(uint16_t rx_buf_len, uint16_t tx_buf_len) {
    uint8_t ret = HAL_OK;
    usbd_cdc_acm_dev_t *cdev = &usbd_cdc_acm_dev;

    // Memory allocation for TX RX buffers
    cdev->bulk_out_buf_size = rx_buf_len;
    cdev->bulk_out_buf = rtw_zmalloc(rx_buf_len);
    if (cdev->bulk_out_buf == NULL) {
        ret = HAL_ERR_MEM;
        return ret;
    }
    cdev->bulk_in_buf_size = tx_buf_len;
    cdev->bulk_in_buf = rtw_zmalloc(tx_buf_len);
    if (cdev->bulk_in_buf == NULL) {
        ret = HAL_ERR_MEM;
        rtw_free(cdev->bulk_out_buf);
        cdev->bulk_out_buf = NULL;
        return ret;
    }
    cdev->ctrl_buf = rtw_zmalloc(CDC_ACM_CTRL_BUF_SIZE);
    if (cdev->ctrl_buf == NULL) {
        ret = HAL_ERR_MEM;
        rtw_free(cdev->bulk_in_buf);
        cdev->bulk_in_buf = NULL;
        rtw_free(cdev->bulk_out_buf);
        cdev->bulk_out_buf = NULL;
        return ret;
    }

    // Set initial line coding config
    usbd_cdc_acm_line_coding_t *lc = &_cdcACMLineCoding;
    lc->bitrate = 115200;
    lc->format = 0x00;
    lc->parity_type = 0x00;
    lc->data_type = 0x08;

    usbd_register_class(&_usbdCDCDriver);
    return ret;
}

uint8_t USBCDCDevice::usbdCDCDeinit(void) {
    usbd_cdc_acm_dev_t *cdev = &usbd_cdc_acm_dev;

    usbd_unregister_class();
    if (cdev->ctrl_buf != NULL) {
        rtw_free(cdev->ctrl_buf);
        cdev->ctrl_buf = NULL;
    }
    if (cdev->bulk_in_buf != NULL) {
        rtw_free(cdev->bulk_in_buf);
        cdev->bulk_in_buf = NULL;
    }
    if (cdev->bulk_out_buf != NULL) {
        rtw_free(cdev->bulk_out_buf);
        cdev->bulk_out_buf = NULL;
    }
    return HAL_OK;
}

size_t USBCDCDevice::usbdCDCTransmit(const uint8_t* data, size_t len) {
    // Transmit BULK IN packet
    if (_usbStatus != USBD_ATTACH_STATUS_ATTACHED) {
        return 0;
    }
    usbd_cdc_acm_dev_t *cdev = &usbd_cdc_acm_dev;
    usb_dev_t *dev = cdev->dev;

    size_t remaining = len;
    size_t txcount = 0;
    while (remaining && _dtr && (_usbStatus == USBD_ATTACH_STATUS_ATTACHED)) {
        while (cdev->bulk_in_state) {
            // Wait for previous USB transmission to finish
            delay(1);
        }
        if (remaining > cdev->bulk_in_buf_size) {
            txcount = cdev->bulk_in_buf_size;
        } else {
            txcount = remaining;
        }

        cdev->bulk_in_state = 1U;
        rtw_memcpy((void *)cdev->bulk_in_buf, (void *)data, txcount);
        usbd_ep_transmit(dev, CDC_ACM_BULK_IN_EP, cdev->bulk_in_buf, txcount);
        remaining -= txcount;
        data += txcount;
    }
    return (len - remaining);
}

uint8_t USBCDCDevice::usbdCDCReceive(void) {
    // Prepare to receive BULK OUT packet
    usbd_cdc_acm_dev_t *cdev = &usbd_cdc_acm_dev;
    usbd_ep_receive(cdev->dev, CDC_ACM_BULK_OUT_EP, cdev->bulk_out_buf, cdev->bulk_out_buf_size);
    return HAL_OK;
}

//----------------------------------- USB Driver Functions -----------------------------------//

uint8_t* USBCDCDevice::usbdCDCGetDescriptor(usb_setup_req_t* req, usbd_speed_type_t speed, uint16_t *len) {
    uint8_t *buf = NULL;

    switch ((req->wValue >> 8) & 0xFF) {
        case USB_DESC_TYPE_DEVICE: {
            buf = usbd_cdc_acm_dev_desc;
            *len = sizeof(usbd_cdc_acm_dev_desc);
            break;
        }
        case USB_DESC_TYPE_CONFIGURATION: {
            if (speed == USBD_SPEED_HIGH) {
                usbd_cdc_acm_hs_config_desc[1] = USB_DESC_TYPE_CONFIGURATION;
                buf = usbd_cdc_acm_hs_config_desc;
                *len = sizeof(usbd_cdc_acm_hs_config_desc);
            } else {
                usbd_cdc_acm_fs_config_desc[1] = USB_DESC_TYPE_CONFIGURATION;
                buf = usbd_cdc_acm_fs_config_desc;
                *len = sizeof(usbd_cdc_acm_fs_config_desc);
            }
            break;
        }
        case USB_DESC_TYPE_DEVICE_QUALIFIER: {
            if (speed == USBD_SPEED_HIGH) {
                buf = usbd_cdc_acm_device_qualifier_desc;
                *len = sizeof(usbd_cdc_acm_device_qualifier_desc);
            }
            break;
        }
        case USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION: {
            if (speed == USBD_SPEED_HIGH) {
                usbd_cdc_acm_fs_config_desc[1] = USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION;
                buf = usbd_cdc_acm_fs_config_desc;
                *len = sizeof(usbd_cdc_acm_fs_config_desc);
            }
            break;
        }
        case USB_DESC_TYPE_STRING: {
            switch (req->wValue & 0xFF) {
                case USBD_IDX_LANGID_STR: {
                    buf = usbd_cdc_acm_lang_id_desc;
                    *len = sizeof(usbd_cdc_acm_lang_id_desc);
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
//                  printf("Get descriptor failed, invalid string index %d\n", req->wValue & 0xFF);
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

uint8_t USBCDCDevice::usbdCDCSetConfig(usb_dev_t* dev, uint8_t config) {
    (void)config;

    uint16_t ep_mps;
    uint8_t ret = HAL_OK;
    usbd_cdc_acm_dev_t *cdev = &usbd_cdc_acm_dev;

    cdev->dev = dev;
    /* Init BULK IN state */
    cdev->bulk_in_state = 0;
    /* Init BULK IN EP */
    ep_mps = (dev->dev_speed == USBD_SPEED_HIGH) ? CDC_ACM_HS_BULK_IN_PACKET_SIZE : CDC_ACM_FS_BULK_IN_PACKET_SIZE;
    usbd_ep_init(dev, CDC_ACM_BULK_IN_EP, USB_CH_EP_TYPE_BULK, ep_mps);
    /* Init BULK OUT EP */
    ep_mps = (dev->dev_speed == USBD_SPEED_HIGH) ? CDC_ACM_HS_BULK_OUT_PACKET_SIZE : CDC_ACM_FS_BULK_OUT_PACKET_SIZE;
    usbd_ep_init(dev, CDC_ACM_BULK_OUT_EP, USB_CH_EP_TYPE_BULK, ep_mps);
    /* Init INTR IN EP */
    usbd_ep_init(dev, CDC_ACM_INTR_IN_EP, USB_CH_EP_TYPE_INTR, CDC_ACM_INTR_IN_PACKET_SIZE);
    /* Prepare to receive next BULK OUT packet */
    usbd_ep_receive(dev, CDC_ACM_BULK_OUT_EP, cdev->bulk_out_buf, cdev->bulk_out_buf_size);
    return ret;
}

uint8_t USBCDCDevice::usbdCDCClearConfig(usb_dev_t* dev, uint8_t config) {
    (void)config;

    uint8_t ret = 0;
    /* DeInit BULK IN EP */
    usbd_ep_deinit(dev, CDC_ACM_BULK_IN_EP);
    /* DeInit BULK OUT EP */
    usbd_ep_deinit(dev, CDC_ACM_BULK_OUT_EP);
    /* DeInit INTR IN EP */
    usbd_ep_deinit(dev, CDC_ACM_INTR_IN_EP);
    return ret;
}

uint8_t USBCDCDevice::usbdCDCSetup(usb_dev_t* dev, usb_setup_req_t* req) {
    usbd_cdc_acm_dev_t *cdev = &usbd_cdc_acm_dev;
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
                        cdev->ctrl_buf[0] = 0U;
                        usbd_ep0_transmit(dev, cdev->ctrl_buf, 1U);
                    } else {
                        ret = HAL_ERR_HW;
                    }
                    break;
                }
                case USB_REQ_GET_STATUS: {
                    if (dev->dev_state == USBD_STATE_CONFIGURED) {
                        cdev->ctrl_buf[0] = 0U;
                        cdev->ctrl_buf[1] = 0U;
                        usbd_ep0_transmit(dev, cdev->ctrl_buf, 2U);
                    } else {
                        ret = HAL_ERR_HW;
                    }
                    break;
                }
                default: {
                    ret = HAL_ERR_HW;
                    break;
                }
            }
            break;
        }
        case USB_REQ_TYPE_CLASS: {
            if (req->wLength) {
                if (req->bmRequestType & 0x80U) {
                    cdcACMConfigCoding(req->bRequest, cdev->ctrl_buf, req->wLength, req->wValue);
                    usbd_ep0_transmit(dev, cdev->ctrl_buf, req->wLength);
                } else {
                    cdev->ctrl_req = req->bRequest;
                    cdev->ctrl_data_len = (uint8_t)req->wLength;
                    usbd_ep0_receive(dev, cdev->ctrl_buf, req->wLength);
                }
            } else {
                cdcACMConfigCoding(req->bRequest, cdev->ctrl_buf, 0, req->wValue);
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

uint8_t USBCDCDevice::usbdCDCHandleEP0DataOut(usb_dev_t* dev) {
    (void)dev;

    uint8_t ret = HAL_ERR_HW;
    usbd_cdc_acm_dev_t *cdev = &usbd_cdc_acm_dev;

    if (cdev->ctrl_req != 0xFFU) {
        cdcACMConfigCoding(cdev->ctrl_req, cdev->ctrl_buf, cdev->ctrl_data_len, 0);
        cdev->ctrl_req = 0xFFU;
        ret = HAL_OK;
    }
    return ret;
}

uint8_t USBCDCDevice::usbdCDCHandleEPDataIn(usb_dev_t* dev, uint8_t ep_num) {
    (void)dev;
    (void)ep_num;

    usbd_cdc_acm_dev_t *cdev = &usbd_cdc_acm_dev;
    // Previous TX done, mark as available
    cdev->bulk_in_state = 0U;
    return HAL_OK;
}

uint8_t USBCDCDevice::usbdCDCHandleEPDataOut(usb_dev_t* dev, uint8_t ep_num, uint16_t len) {
    (void)dev;
    (void)ep_num;

    usbd_cdc_acm_dev_t *cdev = &usbd_cdc_acm_dev;

    uint8_t* buf = cdev->bulk_out_buf;
    for (uint16_t i = 0; i < len; i++) {
        uint32_t idx = (uint32_t)(rx_buffer._iHead + 1) % CDC_ACM_HS_BULK_MAX_PACKET_SIZE;
        if (idx != rx_buffer._iTail) {
            // Add newly received data into ring buffer
            rx_buffer.buffer[rx_buffer._iHead] = buf[i];
            rx_buffer._iHead = idx;
        } else {
            // New head position is the same as current tail position, indicating that buffer is full
            // Discard newly received data
            break;
        }
    }
    // Prepare to receive next USB BULK_OUT packet
    return usbdCDCReceive();
}

uint8_t USBCDCDevice::cdcACMConfigCoding(uint8_t cmd, uint8_t *pbuf, uint16_t len, uint16_t value) {
    // Handle the CDC class control requests
    usbd_cdc_acm_line_coding_t *lc = &_cdcACMLineCoding;

    switch (cmd) {
        case CDC_SET_LINE_CODING: {
            if (len == CDC_ACM_LINE_CODING_SIZE) {
                lc->bitrate = (uint32_t)(pbuf[0] | (pbuf[1] << 8) | (pbuf[2] << 16) | (pbuf[3] << 24));
                lc->format = pbuf[4];
                lc->parity_type = pbuf[5];
                lc->data_type = pbuf[6];
            }
            break;
        }
        case CDC_GET_LINE_CODING: {
            pbuf[0] = (uint8_t)(lc->bitrate & 0xFF);
            pbuf[1] = (uint8_t)((lc->bitrate >> 8) & 0xFF);
            pbuf[2] = (uint8_t)((lc->bitrate >> 16) & 0xFF);
            pbuf[3] = (uint8_t)((lc->bitrate >> 24) & 0xFF);
            pbuf[4] = lc->format;
            pbuf[5] = lc->parity_type;
            pbuf[6] = lc->data_type;
            break;
        }
        case CDC_SET_CONTROL_LINE_STATE: {
            // wValue: Control Signal Bitmap
            // D2-15:  Reserved, 0
            // D1: RTS, 0 - Deactivate, 1 - Activate
            // D0: DTR, 0 - Not Present, 1 - Present
            _rts = (value & 0x02);
            _dtr = (value & 0x01);
            break;
        }
        default: {
            break;
        }
    }
    return HAL_OK;
}

