#include "ListenSocket.h"

using namespace suho::winnet;
using namespace suho::winnet::iocp;

ListenSocket::ListenSocket()
{
}


ListenSocket::~ListenSocket()
{
}

bool ListenSocket::Init()
{
	return Create();
}

void ListenSocket::SetOption()
{
	SetLinger(true, 0);
	SetReuseAddress(true);
	SetKeepAlive(true);
}

void ListenSocket::SetConditionalAccept(bool onoff)
{
	int value = onoff ? 1 : 0;
	int result = setsockopt(_socket, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, reinterpret_cast<const char*>(&value), sizeof(value));
	if (result == SOCKET_ERROR)
		LastError("SetOpt SO_CONDITIONAL_ACCEPT");
}
