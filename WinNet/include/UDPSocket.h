#pragma once

#include "Socket.h"

namespace suho {
namespace winnet {
namespace sock
{
	class UDPSocket : public BasicSocket
	{
	public:
		UDPSocket() {}
		UDPSocket(SOCKET socket) : BasicSocket(socket) {}
		virtual ~UDPSocket() override {}

		virtual bool Init() override;


	private:
	};

}
}
}