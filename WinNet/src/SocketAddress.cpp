#include "SocketAddress.h"
#include "DNS.hpp"

using namespace suho::winnet;

void SocketAddress::Init()
{
    memset(&_sock_addr, 0, sizeof(_sock_addr));
    _sock_addr.sin_family = AF_INET;
}

void SocketAddress::Set(const IPAddress & ip, unsigned short port)
{
    Init();

    memcpy(&_sock_addr.sin_addr, ip.GetAddressPtr(), sizeof(_sock_addr.sin_addr));
    _sock_addr.sin_port = htons(port);
}

void SocketAddress::Set(const std::string & address, unsigned short port)
{
    Init();

	IPAddress ip = DNS::GetIPAddressByName(address);
	SocketAddress sockaddr(ip, port);

    memcpy(&_sock_addr.sin_addr, IPAddress(ip).GetAddressPtr(), sizeof(_sock_addr.sin_addr));
    _sock_addr.sin_port = htons(port);
}

void SocketAddress::Set(const unsigned long ip, unsigned short port)
{
	Init();

	_sock_addr.sin_addr.s_addr = htonl(ip);
	_sock_addr.sin_port = htons(port);
}

void SocketAddress::Set(const sockaddr * sockaddr)
{
	if (sockaddr == nullptr)
		return;

	Init();

	const sockaddr_in* addr_in = reinterpret_cast<const sockaddr_in*>(sockaddr);
	std::memcpy(&_sock_addr, addr_in, sizeof(_sock_addr));
}

std::string SocketAddress::ToString() const
{
    char buf[16] = {};
    _itoa_s(GetPort(), buf, 10);

    std::string str = GetIP().ToString();
    str += ":";
    str += buf;

    return str;
}