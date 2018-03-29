#pragma once

#include <string>

#include "WinNet.h"

namespace suho {
namespace winnet
{
    class IPAddress
    {
    public:
        IPAddress() { std::memset(&_addr, 0, sizeof(_addr));  }
        ~IPAddress() { std::memset(&_addr, 0, sizeof(_addr)); }

        explicit IPAddress(const in_addr& addr) { Set(addr); }
        explicit IPAddress(unsigned long ip) { Set(ip); }
        explicit IPAddress(const std::string& ip) { Set(ip); }
        explicit IPAddress(const char* ip) { Set(ip); }

        IPAddress(const IPAddress& other) { std::memcpy(&_addr, &other._addr, sizeof(_addr)); }
        IPAddress& operator=(const IPAddress& other) { memcpy(&_addr, &other._addr, sizeof(_addr)); }

        void Set(const in_addr& addr) { std::memcpy(&_addr, &addr, sizeof(_addr)); }
        void Set(unsigned long ip) { _addr.s_addr = htonl(ip); }
        void Set(const std::string& ip) { _addr.s_addr = inet_addr(ip.c_str()); }
        void Set(const char* ip) { _addr.s_addr = inet_addr(ip); }

        const in_addr& GetAddress() const { return _addr; }
        const in_addr* GetAddressPtr() const { return &_addr; }
        int GetSize() const { return sizeof(_addr); }

        std::string GetOctetFirst();
        std::string GetOctetSecond();
        std::string GetOctetThird();
        std::string GetOctetLast();

        std::string ToString() const { return inet_ntoa(_addr); }
        unsigned long ToNumeric() const { return ntohl(_addr.s_addr); }
        
        bool IsLoopBack() const;
        bool IsPrivateIP() const;
        


        bool operator==(const IPAddress& other) const
        {
            return 0 == memcmp(&other._addr, &_addr, sizeof(_addr));
        }

        bool operator!=(const IPAddress& other) const
        {
            return !(*this == other);
        }

    private:  
        in_addr                 _addr;
    };
}
}