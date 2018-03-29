
#include "test_common.h"
#include "TCPSocket.h"
#include "UDPSocket.h"

using namespace std;
using namespace suho::winnet;
using namespace suho::winnet::sock;


TCPSocket server_sock;


int main()
{
	SocketAddress server_addr(INADDR_ANY, 9000);

	server_sock.Bind(server_addr);
	server_sock.Listen();
	server_sock.SetLinger(true, 0);

	SocketAddress client_addr;
	SOCKET client = server_sock.Accept(client_addr);	
	if (client != INVALID_SOCKET)
	{
		TCPSocket client_sock(client);

		while (true)
		{
			char buffer[1024] = {};
			client_sock.Recieve(buffer, 1024);
			
			cout << buffer << "\n";

		}		
	}
}