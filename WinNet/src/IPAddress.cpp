
#include "IPAddress.h"

using namespace suho::winnet;


std::string IPAddress::GetOctetFirst()
{
    char buf[8] = {};
    _itoa_s(_addr.S_un.S_un_b.s_b1, buf, 10);
    return buf;
}

std::string IPAddress::GetOctetSecond()
{
    char buf[8] = {};
    _itoa_s(_addr.S_un.S_un_b.s_b2, buf, 10);
    return buf;
}

std::string IPAddress::GetOctetThird()
{
    char buf[8] = {};
    _itoa_s(_addr.S_un.S_un_b.s_b3, buf, 10);
    return buf;
}

std::string IPAddress::GetOctetLast()
{
    char buf[8] = {};
    _itoa_s(_addr.S_un.S_un_b.s_b4, buf, 10);
    return buf;
}

bool IPAddress::IsLoopBack() const
{
    return (ntohl(_addr.s_addr) & 0xFF000000) == 0x7F000000;
}

bool IPAddress::IsPrivateIP() const
{
    unsigned long addr = ntohl(_addr.s_addr);
    return (addr & 0xFF000000) == 0x0A000000 ||     // 10.0.0.0 ~ 10.255.255.255
        (addr & 0xFFFF0000) == 0xC0A80000 ||        // 192.68.0.0 ~ 192.168.255.255
        (addr >= 0xAC100000 && addr <= 0xAC1FFFFF); // 172.16.0.0 ~ 172.31.255.255
}
