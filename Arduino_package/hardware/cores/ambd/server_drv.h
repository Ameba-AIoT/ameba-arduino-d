/*
  server_drv.h - Library for Arduino Wifi shield.
  Copyright (c) 2011-2014 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef SERVER_DRV_H
#define SERVER_DRV_H

#include <inttypes.h>
#include "wl_definitions.h"

#define DATA_LENTH 128
class ServerDrv {
public:
    int startClient(uint32_t ipAddress, uint16_t port, uint8_t portMode = TCP_MODE, tBlockingMode blockMode = NON_BLOCKING_MODE);
    int startServer(uint16_t port, uint8_t portMode = TCP_MODE, tBlockingMode blockMode = NON_BLOCKING_MODE);
    int startClientv6(uint32_t *ipv6Address, uint16_t port, uint8_t portMode = TCP_MODE);
    int startClientV6(const char *ipv6Address, uint16_t port, uint8_t portMode);
    int getAvailable(int sock);
    int availData(int sock);
    bool recvData(int sock, uint8_t *_data, uint16_t _dataLen);
    bool getData(int sock, uint8_t *data, uint8_t peek = 0);
    int getDataBuf(int sock, uint8_t *_data, uint16_t _dataLen);
    int getLastErrno(int sock);
    void stopSocket(int sock);
    bool sendData(int sock, const uint8_t *data, uint16_t len);
    bool sendtoData(int sock, const uint8_t *data, uint16_t len, uint32_t peer_ip, uint16_t peer_port);
    void getRemoteData(int sock, uint32_t *ip, uint16_t *port);
    int setSockRecvTimeout(int sock, int timeout);
    static int enableIPv6();
    static int getIPv6Status();
    void setIPv6UDPServer(void);

private:
    bool _available;
    uint32_t _peer_addr;
    uint16_t _peer_port;
};
#endif
