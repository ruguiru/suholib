#include "UDPSocket.h"

using namespace suho::winnet;
using namespace suho::winnet::sock;

bool UDPSocket::Init()
{
	return Create(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
}
