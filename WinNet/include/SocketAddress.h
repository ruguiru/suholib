#pragma once

#include "IPAddress.h"

namespace suho {
namespace winnet
{
	class SocketAddress     // == endpoint
	{
	public:
        SocketAddress() { Init(); }
		~SocketAddress() { Clear(); }

        SocketAddress(const IPAddress& ip, unsigned short port) { Set(ip, port); }
        SocketAddress(const std::string& address, unsigned short port) { Set(address, port); }	// address에 도메인네임 IP 둘다 가능
		SocketAddress(const unsigned long ip, unsigned short port) { Set(ip, port); }
		SocketAddress(const sockaddr* sockaddr) { Set(sockaddr); }

        SocketAddress(const SocketAddress& other) { memcpy(&_sock_addr, &other._sock_addr, sizeof(_sock_addr)); }

        void Set(const IPAddress& ip, unsigned short port);
        void Set(const std::string& address, unsigned short port);	// address에 도메인네임 IP 둘다 가능
		void Set(const unsigned long ip, unsigned short port);
		void Set(const sockaddr* sockaddr);

		void Clear() { Init(); }

        const sockaddr* GetSockAddr() const { return reinterpret_cast<const sockaddr*>(&_sock_addr); }
        
        IPAddress GetIP() const { return IPAddress(_sock_addr.sin_addr); }
        unsigned short GetPort() const { return ntohs(_sock_addr.sin_port); }
        int GetSize() const { return sizeof(_sock_addr); }
        int GetFamily() const { return _sock_addr.sin_family; }

        std::string ToString() const;        

	private:
		void Init();

	private:
        sockaddr_in                                 _sock_addr;
	};
}	// end namespace suho
}	// end namespace winnet
