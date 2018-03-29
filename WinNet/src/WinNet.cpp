

#include "WinNet.h"
#include <string>


namespace suho {
namespace winnet
{
    void WinSockStartup()
    {      
        WORD version = MAKEWORD(2, 2);
        WSADATA wsa;

        WSAStartup(version, &wsa);
    }

    void WinsockCleanup()
    {
        WSACleanup();
    }
}
}

struct WinsockInitializer
{
    WinsockInitializer()
    {
        suho::winnet::WinSockStartup();	

		
    }

    ~WinsockInitializer()
    {
        suho::winnet::WinsockCleanup();
    }
};


const WinsockInitializer winsockinit;