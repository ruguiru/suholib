
#include "test_common.h"
#include "IPAddress.h"
#include "SocketAddress.h"
#include "DNS.hpp"

#include <iostream>
#include <string>

using namespace std;
using namespace suho::winnet;


int main()
{
    IPAddress ip("10.10.0.1");

    IPAddress ip2(0x7F000001);

    cout << ip.IsPrivateIP() << endl;

    SocketAddress sockaddr("127.0.0.1", 9000);

    cout << sockaddr.GetIP().ToString() << " " << sockaddr.GetPort() << "\n";

    cout << "\n\n\n\n";

    std::string domain("www.yahoo.com");

    std::string officialname = DNS::GetOfficialDomainName(domain);
    std::list<std::string> aliases = DNS::GetAliases(domain);
    std::list<IPAddress> addrlist = DNS::GetIPAddresses(domain);

    cout << officialname << "\n\n";

    for (auto& item : aliases)
    {
        cout << item << "\n";
    }

    cout << endl;

    for (auto& item : addrlist)
    {
        cout << item.ToString() << "\n";
    }

    cout << endl;


    cout << DNS::GetLocalHostName() << endl;
    DNS::GetIPAddresses(DNS::GetLocalHostName());
}