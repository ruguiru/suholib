#include "OverlappedSocket.h"

using namespace suho::winnet;
using namespace suho::winnet::sock;
using namespace suho::log;

void OverlappedSocket::AsyncAccept(SOCKET listen_sock, char * buf, int bufsize, LPOVERLAPPED overlapped)
{
    Init();
	
    WSABUF wsabuf;
    wsabuf.buf = buf;
    wsabuf.len = bufsize;

    DWORD recvbytes = 0;
    if (!AcceptEx(listen_sock, _socket, wsabuf.buf, 0,
        sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
        &recvbytes, overlapped))
    {
        if (GetLastError() != WSA_IO_PENDING)
        {
            IocpLog(level::INFO, "Error AcceptEx()");
        }
    }
}

bool OverlappedSocket::AsyncRecieve(char * buffer, int size, LPOVERLAPPED overlapped)
{
    if (_socket == INVALID_SOCKET)
        return false;

	if (!atomic_load(&_is_enable))
	{
		return false;
	}

    DWORD recvbytes = 0;
    DWORD flags = 0;

    WSABUF wsabuf;
    wsabuf.buf = buffer;
    wsabuf.len = size;

    int result = WSARecv(_socket, &wsabuf, 1, &recvbytes, &flags, overlapped, NULL);
    if (result == SOCKET_ERROR )
    {
		int errorcode = WSAGetLastError();
        if (errorcode != ERROR_IO_PENDING)
        {
            //IocpLog(level::ERR, "WSARecv() Not Pending Error code:%d", errorcode);		// LOG
            if (errorcode == WSAECONNRESET || errorcode == WSAENETRESET || errorcode == WSAENOTCONN || errorcode == WSAECONNABORTED)
            {
                // 辆丰 贸府
				return false;
            }
        }
    }

    return true;
}

int OverlappedSocket::AsyncSend(char * buffer, int size, LPOVERLAPPED overlapped)
{
    if (_socket == INVALID_SOCKET)
        return -1;

	if (!atomic_load(&_is_enable))
	{
		return -1;
	}

    DWORD sendbytes = 0;
    DWORD flags = 0;

    WSABUF wsabuf;
    wsabuf.buf = buffer;
    wsabuf.len = size;

    int result = WSASend(_socket, &wsabuf, 1, &sendbytes, flags, overlapped, NULL);
    if (result == SOCKET_ERROR)
    {
        int errorcode = WSAGetLastError();
        if (errorcode != ERROR_IO_PENDING)
        {
            //IocpLog(level::ERR, "WSASend() Not Pending Error code:%d", errorcode);		// LOG
            if (errorcode == WSAECONNRESET || errorcode == WSAENETRESET || errorcode == WSAENOTCONN || errorcode == WSAECONNABORTED)
            {
                // 辆丰 贸府
				return -1;
            }
        }
    }

    return sendbytes;
}

bool OverlappedSocket::Create()
{
    if (_socket != INVALID_SOCKET)
        return false;

    _socket = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (_socket == INVALID_SOCKET)
    {
        return false;
    }

    _is_blocking = false;

    return true;
}

void OverlappedSocket::Close()
{
	BasicSocket::Close();
}

bool OverlappedSocket::Reuse()
{
	if (!TransmitFile(GetSocket(), NULL, 0, 0, NULL, NULL,
		TF_DISCONNECT | TF_REUSE_SOCKET))
	{
		int errorcode = WSAGetLastError();
		if (errorcode != ERROR_IO_PENDING)
		{
			IocpLog(level::ERR, "TransmitFile() Not Pending Error code:%d", errorcode);
			return false;
		}
	}

	return true;
}

bool OverlappedSocket::SetUpdateAcceptContext(SOCKET listen_socket)
{
    int result = setsockopt(_socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
        reinterpret_cast<const char*>(&listen_socket), sizeof(listen_socket));
	if (result == SOCKET_ERROR)
	{
		IocpLog(level::FATAL, "SetOpt SO_UPDATE_ACCEPT_CONTEXT");
		return false;
	}

	return true;
}

