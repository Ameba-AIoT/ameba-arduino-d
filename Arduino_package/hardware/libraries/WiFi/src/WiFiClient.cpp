#include "WiFi.h"

extern "C" {
    #include "wl_definitions.h"
    #include "wl_types.h"
    #include "string.h"
    #include "errno.h"
    #include "update.h"
}

#include "WiFiClient.h"
#include "WiFiServer.h"
#include "server_drv.h"

WiFiClient::WiFiClient() : _sock(MAX_SOCK_NUM) {
    _is_connected = false;
    recvTimeout = 3000;
}

WiFiClient::WiFiClient(uint8_t sock) {
    _sock = sock;
    //if ((sock >= 0) && (sock != 0xFF)) {
    if (sock != 0xFF) {
        _is_connected = true;
    }
    recvTimeout = 3000;
}

uint8_t WiFiClient::connected() {
    if ((_sock < 0) || (_sock == 0xFF)) {
        _is_connected = false;
        return 0;
    } else {
        if (_is_connected) {
            return 1;
        } else {
            stop();
            return 0;
        }
    }
}

int WiFiClient::available() {
    int ret = 0;
    int err;

    if (!_is_connected) {
        return 0;
    }
    if (_sock >= 0) {
try_again:
        ret = clientdrv.availData(_sock);
        if (ret > 0) {
            return 1;
        } else {
            err = clientdrv.getLastErrno(_sock);
            if (err == EAGAIN) goto try_again;
            if (err != 0) {
                _is_connected = false;
            }
            return 0;
        }
    }

    return 0;
}

int WiFiClient::read() {
    int ret;
    int err;
    uint8_t b[1];

    if (!available()) {
        return -1;
    }

    ret = clientdrv.getData(_sock, b);
    if (ret > 0) {
        return b[0];
    } else {
        err = clientdrv.getLastErrno(_sock);
        if (err != EAGAIN) {
            _is_connected = false;
        }
    }

    return ret;
}

int WiFiClient::read(uint8_t* buf, size_t size) {
    uint16_t _size = size;
    int ret;
    int err;

    ret = clientdrv.getDataBuf(_sock, buf, _size);
    if (ret <= 0) {
        err = clientdrv.getLastErrno(_sock);
        if (err != EAGAIN) {
            _is_connected = false;
        }
    }
    return ret;
}

int WiFiClient::recv(uint8_t* buf, size_t size) {
    uint16_t _size = size;
    int ret;
    int err;

    ret = clientdrv.recvData(_sock, buf, _size);
    if (ret <= 0) {
        err = clientdrv.getLastErrno(_sock);
        if (err != EAGAIN) {
            _is_connected = false;
        }
    }
    return ret;
}

void WiFiClient::stop() {
    if (_sock < 0) {
        return;
    }
    clientdrv.stopSocket(_sock);
    _is_connected = false;
    _sock = -1;
}

size_t WiFiClient::write(uint8_t b) {
    return write(&b, 1);
}

size_t WiFiClient::write(const uint8_t *buf, size_t size) {
    if (_sock < 0) {
        setWriteError();
        return 0;
    }
    if (size == 0) {
        setWriteError();
        return 0;
    }

    if (!clientdrv.sendData(_sock, buf, size)) {
        setWriteError();
        _is_connected = false;
        return 0;
    }

    return size;
}

WiFiClient::operator bool() {
    return _sock >= 0;
}

int WiFiClient::connect(const char* host, uint16_t port) {
    IPAddress remote_addr;
    IPv6Address remote_addr_v6;

    if (getIPv6Status() == 0) {
        if (WiFi.hostByName(host, remote_addr)) {
            return connect(remote_addr, port);
        }
    } else {
        //printf("\n\r[INFO]WiFiClient.cpp: connect hostByNameV6() \n\r");
        if (WiFi.hostByNamev6(host, remote_addr_v6)) {
            //printf("[INFO]WiFiClient.cpp: connect ipv6: %s\n\r", host);
            _sock = clientdrv.startClientV6(host, port, TCP_MODE);
        } else {
        }
        // whether sock is connected
        if (_sock < 0) {
            _is_connected = false;
            return 0;
        } else {
            _is_connected = true;
            clientdrv.setSockRecvTimeout(_sock, recvTimeout);
        }
        return 1;
    }
    return 0;
}

int WiFiClient::connect(IPAddress ip, uint16_t port) {
    _is_connected = false;
    _sock = clientdrv.startClient(ip, port);
    // whether sock is connected
    if (_sock < 0) {
        _is_connected = false;
        return 0;
    } else {
        _is_connected = true;
        clientdrv.setSockRecvTimeout(_sock, recvTimeout);
    }
    return 1;
}

int WiFiClient::connectv6(IPv6Address ipv6, uint16_t port) {
    _is_connected = false;
    _sock = clientdrv.startClientv6(ipv6, port);
    //printf("[INFO]wifiClient.cpp: connectv6 sock value: %x\n\r", _sock);
    if (_sock < 0) {
        _is_connected = false;
        //printf("[INFO]wifiClient.cpp: connectv6 not connected\n\r");
        return 0;
    } else {
        _is_connected = true;
        //printf("[INFO]wifiClient.cpp: connectv6 connected\n\r");
        clientdrv.setSockRecvTimeout(_sock, recvTimeout);
    }
    return 1;
}

int WiFiClient::peek() {
    uint8_t b;
    if (!available()) {
        return -1;
    }
    clientdrv.getData(_sock, &b, 1);

    return b;
}

void WiFiClient::flush() {
    while (available()) {
        read();
    }
}

// extend API from RTK

int WiFiClient::setRecvTimeout(int timeout) {
    if (connected()) {
        recvTimeout = timeout;
        clientdrv.setSockRecvTimeout(_sock, recvTimeout);
    }

    return 0;
}

int WiFiClient::read(char *buf, size_t size) {
    read(((uint8_t *)buf), size);

    return 0;
}

int WiFiClient::enableIPv6() {
    return clientdrv.enableIPv6();
}

int WiFiClient::getIPv6Status() {
    return clientdrv.getIPv6Status();
}
