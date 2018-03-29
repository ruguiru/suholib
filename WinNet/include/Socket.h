#pragma once

#include "SocketAddress.h"

namespace suho {
namespace winnet {
namespace sock
{
    class BasicSocket
    {
    public:
		BasicSocket() {}
		BasicSocket(SOCKET socket) { _socket = socket; }
        virtual ~BasicSocket() { Close();}

		bool Create(int af, int type, int proto);
        void Close();
        void ShutDown(int operation);

		// ¼­ºê Å¬·¡½Ì¿ë
		virtual bool Init() { return true; }			// Create ÈÄ È£ÃâµÊ
		virtual void SetOption() {}						// Init ÈÄ È£ÃâµÊ

        virtual bool Bind(const suho::winnet::SocketAddress& sockaddr);
        virtual bool Connect(const suho::winnet::SocketAddress& sockaddr);
		
		virtual int Send(const void* buffer, int size, int flags = 0);
		virtual int Recieve(void* buffer, int size, int flags = 0);
		virtual int SendTo(const void* buffer, int size, const SocketAddress& address, int flags = 0);
		virtual int RecieveFrom(void* buffer, int size, SocketAddress& address, int flags = 0);

		void SetSocket(SOCKET socket) { _socket = socket; }

		void SetSendBufferSize(int size);
		int GetSendBufferSize();
		void SetRecieveBufferSize(int size);
		int GetRecieveBufferSize();

		void SetSendTimeOut(int milisec);
		void SetRecieveTimeOut(int milisec);

        void SetLinger(bool onoff, unsigned short seconds);
		void SetReuseAddress(bool onoff);
        void SetKeepAlive(bool onoff);

        SocketAddress GetAddress();
        SocketAddress GetPeerAddress();

        SOCKET GetSocket() const { return _socket; }
        bool IsBlocking() const { return _is_blocking; }
		bool IsValidSocket() const { return _socket != INVALID_SOCKET; }

		DWORD LastError(const std::string & comment);

    protected:
        SOCKET                              _socket = INVALID_SOCKET;
        bool                                _is_blocking = true;
    };
}
}
}