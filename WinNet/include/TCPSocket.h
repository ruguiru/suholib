#pragma once

#include "Socket.h"

namespace suho {
namespace winnet {
namespace sock
{
	class TCPSocket : public BasicSocket
	{
    public:
		TCPSocket() {}
		TCPSocket(SOCKET socket) : BasicSocket(socket) {}
		virtual ~TCPSocket() override {}

		virtual bool Init() override
		{
			return Create(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		}

        virtual void SetOption() override
        {
        }

		virtual bool Listen(int backlog = SOMAXCONN);
		virtual SOCKET Accept(SocketAddress& address);

        void SetTcpNodelay(bool onoff);
        bool IsConnected();

    private:
	};
}
}
}