/*
 IPv6Address.h - Base class that provides IPv6Address
 Copyright (c) 2011 Adrian McEwen.  All right reserved.

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

#ifndef IPv6Address_h
#define IPv6Address_h

#include <stdint.h>
#include <Printable.h>

// A class to make it easier to handle and pass around IP addresses

class IPv6Address : public Printable {
    //private:
    public:
        char _ipv6Address[45];  // IP6ADDR_STRLEN_MAX    46
        union {
            uint8_t bytes[16];  
            uint32_t dword[4];
        } _address;

        // Access the raw byte array containing the address.  Because this returns a pointer
        // to the internal structure rather than a copy of the address this function should only
        // be used when you know that the usage of the returned uint8_t* will be transient and not
        // stored.
        uint8_t* raw_address() { return _address.bytes; }

    //public:
        // Constructors
        IPv6Address();
        //IPv6Address(uint8_t first_hextex, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
        IPv6Address(const uint32_t* address);
        IPv6Address(const uint8_t* address);       
        virtual ~IPv6Address() {}

        // Overloaded cast operator to allow IPAddress objects to be used where a pointer
        // to a four-byte uint8_t array is expected
        operator uint8_t*() { return _address.bytes; }
        operator uint32_t*() { return _address.dword; }
        bool operator==(const IPv6Address& addr) const { return (_address.dword[0] == addr._address.dword[0]) && (_address.dword[1] == addr._address.dword[1]) && (_address.dword[2] == addr._address.dword[2]) && (_address.dword[3] == addr._address.dword[3]); }
        bool operator==(const uint8_t* addr) const;
        
        // Overloaded index operator to allow getting and setting individual octets of the address
        uint8_t operator[](int index) const { return _address.bytes[index]; }
        uint8_t& operator[](int index) { return _address.bytes[index]; }

        // Overloaded copy operators to allow initialisation of IPv6Address objects from other types
        IPv6Address& operator=(const uint8_t* address);     
        IPv6Address& operator=(const uint32_t address);

        virtual size_t printTo(Print& p) const;

        friend class UDP;
        friend class Client;
        friend class Server;
    };

#endif
