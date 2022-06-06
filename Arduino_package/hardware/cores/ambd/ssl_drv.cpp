#include "ssl_drv.h"
#ifdef __cplusplus
extern "C" {
#include "platform_stdlib.h"
}
#endif

uint16_t SSLDrv::availData(sslclient_context *ssl_client)
{
    //int ret;

    if (ssl_client->socket < 0) {
        return 0;
    }

    if (_available) {
        return 1;
    } else {
        return getData(ssl_client, c, 1);
    }
}

bool SSLDrv::getData(sslclient_context *ssl_client, uint8_t *data, uint8_t peek)
{
    int ret = 0;
    int flag = 0;

    if (_available) {
        /* we already has data to read */
        data[0] = c[0];

        //if (peek) {
        //} else {
        //    /* It's not peek and the data has been taken */
        //    _available = false;
        //}
        if (!peek) {
            /* It's not peek and the data has been taken */
            _available = false;
        }

        return true;
    }

    if (peek) {
        flag |= 1;
    }

    ret = get_ssl_receive(ssl_client, c, 1, flag);

    if (ret == 1) {
        data[0] = c[0];
        if (peek) {
            _available = true;
        } else {
            _available = false;
        }
        return true;
    }

    return false;
}

int SSLDrv::getDataBuf(sslclient_context *ssl_client, uint8_t *_data, uint16_t _dataLen)
{
    int ret;

    if (_available) {
        /* there is one byte cached */
        _data[0] = c[0];
        _available = false;
        _dataLen--;
        if (_dataLen > 0) {
            ret = get_ssl_receive(ssl_client, &_data[1], _dataLen, 0);
            if (ret > 0) {
                ret++;
                return ret;
            } else {
                return 1;
            }
        } else {
            return 1;
        }
    } else {
        ret = get_ssl_receive(ssl_client, _data, _dataLen, 0);
    }

    return ret;
}

void SSLDrv::stopClient(sslclient_context *ssl_client)
{
    stop_ssl_socket(ssl_client);
    _available = false;
}

bool SSLDrv::sendData(sslclient_context *ssl_client, const uint8_t *data, uint16_t len)
{
    int ret;

    if (ssl_client->socket < 0) {
        return false;
    }

    ret = send_ssl_data(ssl_client, data, len);

    if (ret == 0) {
        return false;
    }

    return true;
}

int SSLDrv::startClient(sslclient_context *ssl_client, uint32_t ipAddress, uint32_t port, unsigned char* rootCABuff, unsigned char* cli_cert, unsigned char* cli_key, unsigned char* pskIdent, unsigned char* psKey, char* SNI_hostname)
{
    int ret;

    ret = start_ssl_client(ssl_client, ipAddress, port, rootCABuff, cli_cert, cli_key, pskIdent, psKey, SNI_hostname);

    return ret;
}

int SSLDrv::getLastErrno(sslclient_context *ssl_client)
{
    return get_ssl_sock_errno(ssl_client);
}

int SSLDrv::setSockRecvTimeout(int sock, int timeout)
{
    return setSockRecvTimeout(sock, timeout);
}
