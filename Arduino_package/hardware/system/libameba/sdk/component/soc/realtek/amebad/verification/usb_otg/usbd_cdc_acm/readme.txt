##################################################################################
#                                                                                #
#                          Example example_usbd_cdc_acm_tp(rp)                    #
#                                                                                #
##################################################################################

Date: 2019-04-03

Table of Contents
~~~~~~~~~~~~~~~~~
 - Description
 - Setup Guide
 - Parameter Setting and Configuration
 - Result description
 - Other Reference
 - Supported List

 
Description
~~~~~~~~~~~
        With USB device v2.0 interface, Ameba can be designed to a USB CDC ACM class device.
        In this application, Ameba boots up as a USB VCOM device, USB host can recognize Ameba and
        communicates with it via USB interface.
        NOTE: Currently only support Win10 host natively, for other host system, please modify the CDC ACM class
        driver in SDK or develop the corresponding host driver.
    
Setup Guide
~~~~~~~~~~~
        In order to run this application successfully, the hardware setup should be confirm before moving on.
        1. Connect Ameba to USB host end with a micro USB cable. Ameba DEV board has designed a micro USB
           connector on board.
        2. For GCC environmnet, type command "make menuconfig" and choose choose “Config USB” => “Enable USB” => 
           “USB OTG Type = USB_OTG_DEVICE_MODE” and “USB Device Type = USB_DEVICE_CDC_ACM”, then
           “CONFIG FUNCTION TEST” => “Enable Peripheral Test” => “USB_Verify_Mode” => 
		   choose USB CDC ACM Test “CDC_TX_Performance_Test” or “CDC_RX_Performance_Test”
           which will auto-generate following lines in platform_autoconf.h:
		   for USB_DEVICE_CDC_ACM_TP:
               #define CONFIG_USB_OTG_EN              1
               #define CONFIG_USB_DEVICE_EN           1
               #define CONFIG_USBD_CDC_ACM            1
			   #define CONFIG_USB_VERIFY 1
			   #define CONFIG_USBD_CDC_ACM_TP		  1
		   for USB_DEVICE_CDC_ACM_RP:
               #define CONFIG_USB_OTG_EN              1
               #define CONFIG_USB_DEVICE_EN           1
               #define CONFIG_USBD_CDC_ACM            1
			   #define CONFIG_USB_VERIFY 1
			   #define CONFIG_USBD_CDC_ACM_RP		  1
           And for IAR environment, manually edit platform_autoconf.h as above.
        3. Make sure the USB CDC ACM example is enabled (default) in platform_opts.h:
               #define CONFIG_EXAMPLE_USBD_CDC_ACM_TP     1 or
			   #define CONFIG_EXAMPLE_USBD_CDC_ACM_RP     1
        4. Rebuild the project and download firmware to DEV board.

Parameter Setting and Configuration
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        None
    
Result description
~~~~~~~~~~~~~~~~~~
        On the serial console, USB CDC ACM loading log will be printed, make sure there is no error reported.
        After the CDC ACM driver is successfully loaded, USB host end will recognize Ameba as a COM port. 
        Now user can operate the COM port via common COM tools, and Ameba send data or receive messages.

Other Reference
~~~~~~~~~~~~~~~
        None

Supported List
~~~~~~~~~~~~~~
[Supported List]
        Supported : Ameba-D
        Source code not in project: Ameba-1, Ameba-z, Ameba-pro