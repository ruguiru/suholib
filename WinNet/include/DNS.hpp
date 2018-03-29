#pragma once

#include <list>

#include "WinNet.h"
#include "IPAddress.h"

namespace suho {
namespace winnet
{
    class DNS
    {
    public:
        DNS() {}
        ~DNS() {}

        static std::string GetOfficialDomainName(const std::string& domain)
        {
            std::string officialname;

            unsigned long ip = inet_addr(domain.c_str());
            if (ip == INADDR_NONE)  // domain name
            {
                hostent* hosts = gethostbyname(domain.c_str());

                officialname = hosts->h_name;
            }

            return officialname;
        }

        static short GetAddressType(const std::string& domain)
        {
            short family = AF_UNSPEC;

            unsigned long ip = inet_addr(domain.c_str());
            if (ip == INADDR_NONE)  // domain name
            {
                hostent* hosts = gethostbyname(domain.c_str());

                family = hosts->h_addrtype;
            }

            return family;
        }

        static std::list<std::string> GetAliases(const std::string& domain)
        {
            std::list<std::string> aliases;

            unsigned long ip = inet_addr(domain.c_str());
            if (ip == INADDR_NONE)  // domain name
            {
                hostent* hosts = gethostbyname(domain.c_str());

                int i = 0;
                while (hosts->h_aliases[i])
                {
                    aliases.push_back(hosts->h_aliases[i++]);
                }
            }

            return aliases;
        }

        static std::list<suho::winnet::IPAddress> GetIPAddresses(const std::string& domain)
        {
            std::list<suho::winnet::IPAddress> ip_addresses;

            unsigned long ip = inet_addr(domain.c_str());
            if (ip == INADDR_NONE)  // domain name
            {
                hostent* hosts = gethostbyname(domain.c_str());

                int i = 0;
                while (hosts->h_addr_list[i])
                {
                    struct in_addr* addr = reinterpret_cast<in_addr*>(hosts->h_addr_list[i++]);
                    ip_addresses.push_back(IPAddress(*addr));
                }
            }

            return ip_addresses;
        }

        static suho::winnet::IPAddress GetIPAddressByName(const std::string& name_or_ip)
        {
            unsigned long ip = inet_addr(name_or_ip.c_str());
            if (ip == INADDR_NONE)  // domain name
            {
                hostent* hosts = gethostbyname(name_or_ip.c_str());
                if (hosts == nullptr)
                    return IPAddress();

                memcpy(&ip, hosts->h_addr, sizeof(unsigned long));

                return IPAddress(ip);
            }
            
            return IPAddress(name_or_ip);
        }

        static std::string GetLocalHostName()
        {
            char buffer[256];
            int result = gethostname(buffer, sizeof(buffer));
            if (result == 0)
                return std::string(buffer);

            return std::string();
        }

    private:

    };
}
}

