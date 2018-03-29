#pragma once

#include "TcpSocket.h"

#include <MSWSock.h>

namespace suho {
namespace winnet {
namespace sock
{
    class OverlappedSocket : public TCPSocket
    {
    public:
        OverlappedSocket() {}
        OverlappedSocket(SOCKET socket) : TCPSocket(socket) {}
        virtual ~OverlappedSocket() override {}

        void AsyncAccept(SOCKET listen_sock, char* buf, int bufsize, LPOVERLAPPED overlapped);
        bool AsyncRecieve(char * buffer, int size, LPOVERLAPPED overlapped);
        int AsyncSend(char * buffer, int size, LPOVERLAPPED overlapped);

        virtual bool Init() override
        {
            return Create();
        }

        virtual void SetOption() override
        {
        }

        bool Create();
        void Close();
        
        void SetUpdateAcceptContext(SOCKET listen_socket);

    };
}
}
}

