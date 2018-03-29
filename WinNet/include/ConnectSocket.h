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

		// ip와 도메인네임 관계없이 연결 (address가 ip 또는 도메인네임)
		bool Connect(const std::string& address, short port);

	};
}
}
}
