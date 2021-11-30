#include "ConnectSocket.h"
#include "DNS.hpp"

#include <WS2tcpip.h>

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

bool ConnectSocket::AsyncConnect(const suho::winnet::SocketAddress& sockaddr, LPOVERLAPPED overlapped)
{	
	LPFN_CONNECTEX ConnectEx = NULL;
	if (!LoadConnectExFunction(ConnectEx))
		return false;

	if(!ConnectEx(_socket, sockaddr.GetSockAddr(), sockaddr.GetSize(), NULL, 0, NULL, overlapped))
	{
		if (GetLastError() != WSA_IO_PENDING)
		{
			IocpLog(level::LogLevel::INFO, "Error ConnectEx()");
			return false;
		}
	}

	return true;
}

bool ConnectSocket::SetUpdateConnectContext()
{
	int result = setsockopt(_socket, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0);
	if (result == SOCKET_ERROR)
	{
		IocpLog(level::LogLevel::FATAL, "SetOpt SO_UPDATE_CONNECT_CONTEXT");
		return false;
	}

	return true;
}

bool ConnectSocket::LoadConnectExFunction(LPFN_CONNECTEX& connectex)
{
	SOCKET sock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, NULL, WSA_FLAG_OVERLAPPED);
	if (sock == INVALID_SOCKET)
		return false;

	DWORD dwBytes = 0;
	GUID guid = WSAID_CONNECTEX;
	int result = WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guid, sizeof(guid),
		static_cast<LPVOID>(&connectex), sizeof(connectex),
		&dwBytes, NULL, NULL);
	if (result != 0)
	{
		IocpLog(level::LogLevel::FATAL, "Load ConnectEx Fail");
		closesocket(sock);
		return false;
	}	

	closesocket(sock);
	return true;
}
