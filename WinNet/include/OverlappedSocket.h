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

		// Transmit 으로 Reuse 하는 경우 사용
		bool Reuse();
		void Enable() { _is_enable = true; }
		void Disable() { _is_enable = false; }
		
        bool SetUpdateAcceptContext(SOCKET listen_socket);

	private:
		// Transmit 으로 Reuse 하는 경우 사용
		// Reuse의 경우 소켓을 닫지 않으므로 INVALID_SOCKET 의 검사를 할 수 없어 별도의 변수 필요
		atomic<bool>					_is_enable = true;
    };
}
}
}

