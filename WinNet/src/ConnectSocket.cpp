#include "ConnectSocket.h"
#include "DNS.hpp"

using namespace suho::winnet::iocp;

bool ConnectSocket::Init()
{
	return Create();
}

void ConnectSocket::SetOption()
{
	SetReuseAddress(true);
	SetKeepAlive(true);

}

bool ConnectSocket::Connect(const std::string & address, short port)
{
	if (_socket == INVALID_SOCKET)
	{
		Init();
		SetOption();
	}

	IPAddress ip = DNS::GetIPAddressByName(address);
	SocketAddress sockaddr(ip, port);

	int result = connect(_socket, sockaddr.GetSockAddr(), sockaddr.GetSize());
	if (result == SOCKET_ERROR)
	{
		LastError("connect()");
		return false;
	}

	return true;
}
