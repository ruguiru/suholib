#pragma once

#include "OverlappedSocket.h"

namespace suho {
namespace winnet {
namespace iocp
{
	class ConnectSocket : public suho::winnet::sock::OverlappedSocket
	{
	public:
		ConnectSocket() {}
		~ConnectSocket() override {}
		
		virtual bool Init() override;
		virtual void SetOption() override;

		bool AsyncConnect(const suho::winnet::SocketAddress& sockaddr, LPOVERLAPPED overlapped);	// non blocking

		bool SetUpdateConnectContext();

	private:
		bool LoadConnectExFunction(LPFN_CONNECTEX& connectex);

	private:

	};
}
}
}
