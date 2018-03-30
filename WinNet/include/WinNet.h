#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")	// acceptex

#include <WinSock2.h>
#include "LogHelper.h"

namespace suho {
namespace winnet
{
    void WinSockStartup();
    void WinsockCleanup();
}
}

extern "C" const struct WinsockInitializer winsockinit;

__pragma(comment(linker, "/export:_winsockinit"))

BUILD_LOGTYPE(IocpLog, "C:/Log", cycle::HOURLY)
#define IocpLog IocpLog::GetInstance()->Write