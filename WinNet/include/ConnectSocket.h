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

		// ip�� �����γ��� ������� ���� (address�� ip �Ǵ� �����γ���)
		bool Connect(const std::string& address, short port);

	};
}
}
}
