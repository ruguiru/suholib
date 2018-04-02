#include "TCPSocket.h"

using namespace suho::winnet;
using namespace suho::winnet::sock;

bool TCPSocket::Listen(int backlog)
{
	int result = listen(_socket, backlog);
	if (result != 0)
	{
		LastError("listen()");
		return false;
	}

	return true;
}

SOCKET TCPSocket::Accept(SocketAddress & address)
{
	if (_socket == INVALID_SOCKET)
	{
		return INVALID_SOCKET;
	}

	char temp[32] = {};
	struct sockaddr* sa = reinterpret_cast<struct sockaddr*>(temp);
	int len = sizeof(temp);

	SOCKET newsock = accept(_socket, sa, &len);
	if (newsock == INVALID_SOCKET)
	{
		LastError("accept()");
		return INVALID_SOCKET;
	}
	else
	{
		address = SocketAddress(sa);
	}

	return newsock;
}


void TCPSocket::SetTcpNodelay(bool onoff)
{
    int value = onoff ? 1 : 0;
    int result = setsockopt(_socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&value), sizeof(value));
    if (result == SOCKET_ERROR)
        LastError("SetOpt TCP_NODELAY");
}

bool TCPSocket::IsConnected()
{
    HANDLE eventhanle = WSACreateEvent();

    if (eventhanle == INVALID_HANDLE_VALUE || _socket == INVALID_SOCKET)
        return false;

    WSANETWORKEVENTS netEvent = {};
    ::WSAEventSelect(_socket, eventhanle, FD_READ | FD_CLOSE);
    ::WSAEnumNetworkEvents(_socket, eventhanle, &netEvent);

    bool result = !((netEvent.lNetworkEvents & FD_CLOSE) == FD_CLOSE);

    WSACloseEvent(eventhanle);

    return result;
}
