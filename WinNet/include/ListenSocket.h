#pragma once

#include "OverlappedSocket.h"

namespace suho {
namespace winnet {
namespace iocp
{
    class ListenSocket : public suho::winnet::sock::OverlappedSocket
	{
	public:
		ListenSocket();
		~ListenSocket() override;

		virtual bool Init() override;
		virtual void SetOption() override;

		void SetConditionalAccept(bool onoff);

    private:
	};
}
}
}