#include "Socket.h"

using namespace suho::winnet;
using namespace suho::winnet::sock;

bool BasicSocket::Create(int af, int type, int proto)
{
    if (_socket != INVALID_SOCKET)
        return false;

    _socket = socket(af, type, proto);
    if (_socket == INVALID_SOCKET)
    {
		LastError("socket()");
        return false;
    }

    return true;
}

void BasicSocket::Close()
{
    if (_socket != INVALID_SOCKET)
    {
        closesocket(_socket);
        _socket = INVALID_SOCKET;
    }
}

void BasicSocket::ShutDown(int operation)
{
    if (_socket != INVALID_SOCKET)
    {
        int result = shutdown(_socket, operation);
        if (result == SOCKET_ERROR)
            LastError("shutdown()");
    }
}

bool BasicSocket::Bind(const suho::winnet::SocketAddress & sockaddr)
{
    if (_socket == INVALID_SOCKET)
    {
        Init();
        SetOption();
    }

	int result = bind(_socket, sockaddr.GetSockAddr(), sockaddr.GetSize());
	if (result == SOCKET_ERROR)
	{
		LastError("bind()");
		return false;
	}

	return true;
}

bool BasicSocket::Connect(const suho::winnet::SocketAddress & sockaddr)
{
    if (_socket == INVALID_SOCKET)
    {
        Init();
        SetOption();
    }

	int result = connect(_socket, sockaddr.GetSockAddr(), sockaddr.GetSize());
	if (result == SOCKET_ERROR)
	{
		LastError("connect()");
		return false;
	}

	return true;
}

int BasicSocket::Send(const void* buffer, int size, int flags)
{
	int result = send(_socket, reinterpret_cast<const char*>(buffer), size, flags);
	if (result == SOCKET_ERROR)
		LastError("send()");

	return result;
}

int BasicSocket::Recieve(void * buffer, int size, int flags)
{
	int result = recv(_socket, reinterpret_cast<char*>(buffer), size, flags);
	if (result == SOCKET_ERROR)
		LastError("recv()");

	return result;
}

int BasicSocket::SendTo(const void * buffer, int size, const SocketAddress & address, int flags)
{
	int result = sendto(_socket, reinterpret_cast<const char*>(buffer), size, flags, address.GetSockAddr(), address.GetSize());
	if (result == SOCKET_ERROR)
		LastError("sendto()");

	return result;
}

int BasicSocket::RecieveFrom(void * buffer, int size, SocketAddress & address, int flags)
{
	char temp[32] = {};
	struct sockaddr* sa = reinterpret_cast<struct sockaddr*>(temp);
	int len = sizeof(temp);

	int result = recvfrom(_socket, reinterpret_cast<char*>(buffer), size, flags, sa, &len);
	if (result == SOCKET_ERROR)
		LastError("recvfrom()");

	address = SocketAddress(sa);

	return result;
}

void BasicSocket::SetSendBufferSize(int size)
{
	int result = setsockopt(_socket, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<const char*>(&size), sizeof(size));
	if (result == SOCKET_ERROR)
		LastError("SetOpt SO_SNDBUF");
}

int BasicSocket::GetSendBufferSize()
{
	int value = 0;
	int size = 0;
	int result = ::getsockopt(_socket, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<char*>(&value), &size);
	if (result == SOCKET_ERROR)
		LastError("GetOpt SO_SNDBUF");

	return value;
}

void BasicSocket::SetRecieveBufferSize(int size)
{
	int result = setsockopt(_socket, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const char*>(&size), sizeof(size));
	if (result == SOCKET_ERROR)
		LastError("SetOpt SO_RCVBUF");
}

int BasicSocket::GetRecieveBufferSize()
{
	int value = 0;
	int size = 0;
	int result = ::getsockopt(_socket, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char*>(&value), &size);
	if (result == SOCKET_ERROR)
		LastError("GetOpt SO_RCVBUF");

	return value;
}

void BasicSocket::SetSendTimeOut(int milisec)
{
    int result = setsockopt(_socket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&milisec), sizeof(milisec));
    if (result == SOCKET_ERROR)
        LastError("SetOpt SO_SNDTIMEO");
}

void BasicSocket::SetRecieveTimeOut(int milisec)
{
    int result = setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&milisec), sizeof(milisec));
    if (result == SOCKET_ERROR)
        LastError("SetOpt SO_RCVTIMEO");
}

void BasicSocket::SetLinger(bool onoff, unsigned short seconds)
{
    struct linger ling;
    ling.l_onoff = onoff ? 1 : 0;
    ling.l_linger = seconds;

    int result = setsockopt(_socket, SOL_SOCKET, SO_LINGER, reinterpret_cast<const char*>(&ling), sizeof(ling));
    if (result == SOCKET_ERROR)
        LastError("SetOpt SO_LINGER");
}

void BasicSocket::SetReuseAddress(bool onoff)
{
    int value = onoff ? 1 : 0;
    int result = setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&value), sizeof(value));
    if (result == SOCKET_ERROR)
        LastError("SetOpt SO_REUSEADDR");
}

void BasicSocket::SetKeepAlive(bool onoff)
{
    int value = onoff ? 1 : 0;
    int result = setsockopt(_socket, SOL_SOCKET, SO_KEEPALIVE, reinterpret_cast<const char*>(&value), sizeof(value));
    if (result == SOCKET_ERROR)
        LastError("SetOpt SO_KEEPALIVE");
}

SocketAddress BasicSocket::GetAddress()
{
    char buffer[32] = {};
    struct sockaddr* sockaddr = reinterpret_cast<struct sockaddr*>(buffer);
    int size = sizeof(buffer);

    int result = getsockname(_socket, sockaddr, &size);
    if (result == 0)
    {
        return SocketAddress(sockaddr);
    }

	LastError("getsockname()");
    return SocketAddress();
}

SocketAddress BasicSocket::GetPeerAddress()
{
    char buffer[32] = {};
    struct sockaddr* peeraddr = reinterpret_cast<struct sockaddr*>(buffer);
    int size = sizeof(buffer);

    int result = getpeername(_socket, peeraddr, &size);
    if (result == 0)
    {
        return SocketAddress(peeraddr);
    }

	LastError("getpeername()");
    return SocketAddress();
}

DWORD BasicSocket::LastError(const std::string & comment)
{
	DWORD error_code = WSAGetLastError();

	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, error_code,
		MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPTSTR)&lpMsgBuf, 0, NULL);

	char error_msg[1024] = {};
	sprintf_s(error_msg, "[%s] CODE:%d MSG:%s", comment.c_str(), error_code, reinterpret_cast<char*>(lpMsgBuf));

#ifdef _DEBUG
	printf(error_msg);
    printf("\n");
#endif

	LocalFree(lpMsgBuf);

    return error_code;
}

