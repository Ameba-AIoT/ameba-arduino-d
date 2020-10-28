The files under wpa_supplicant-2.0 folder are be placed 

as the origonal wpa_supplicant-2.0 package and had already removed 

all non-used files.

the names of the files under wpa_supplicant-2.0 folder are the same as origonal 

wpa_supplicant-2.0 package except the following two files.

sha256.c/h -> sha_256.c/h  (conflict with wifi driver security define)
bignum.c/h -> big_num.c/h  (conflict with polarSSL define)



Other files are add by our platform(i.e FreeRTOS).

wifi_wps_config:
the user space test program which can test the wps pin/pbc function

wps_protocol_handler:
the code to handle received eap packet(i.e WSC MSG) during the WPS process.
In order to separate WPS related code from rtw_psk.c,
I move the WPS related define and handler functions to this C and header files.  

os_freertos
add this wrap file to support some OS-dependence(i.e freeRTOS) API in WPS process. 
ex: WPS_realloc/WPS_strdup¡Bos_zalloc¡Bos_get_random.
   