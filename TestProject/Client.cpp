#include "Client.h"
#include "ClientManager.h"
#include "ConnectionManager.hpp"
#include "LogContainer.h"
#include "Statistic.h"

using namespace suho::winnet::iocp;

void Client::OnInit()
{
	_recv_buffersize = 1024*4;
	_recv_socket_buffersize = 1024 * 10;		// TEST
	_send_socket_buffersize = 1024 * 20;		// TEST
}

void Client::OnConnect()
{
	_own_socket->SetSendBufferSize(_send_socket_buffersize);
	_own_socket->SetRecieveBufferSize(_recv_buffersize);

	ClientManager::GetInstance()->Insert( _index,
		ConnectionManager::GetInstance()->Get(_index));
}

void Client::OnDisconnect()
{
	ClientManager::GetInstance()->Remove(_index);
}

DWORD Client::HeaderParsing(void* packet, DWORD recvbytes)
{
	DWORD headersize = MyPacket::GetHeaderSize();
	if (recvbytes < headersize )
		return 0;

	int datasize = *reinterpret_cast<int*>(packet);
	DWORD packetsize = headersize + datasize;

	return (recvbytes >= packetsize) ? packetsize: 0;
}

void Client::PacketProcessing(const void* packet, DWORD packetsize)
{
	MyPacket recvpacket(packet, packetsize);

    //ClientLog(level::INFO, "[%d] RecvPacket:%d", _index, recvpacket.GetProtocolID());		// LOG
	Statistic::GetInstance()->AddRecvCount();

	switch (recvpacket.GetProtocolID())	
	{
	case 1:
	{
		std::string data;
		int endsign = 0;
		recvpacket >> data >> endsign;

		//if (_index == 9999)		// TEST
		//	printf("[%d] Recv : %d\n", _index, endsign);
		{
			MyPacket sendPacket(1);
			sendPacket << "abcdefgh";
			SendPacket(sendPacket);
		}
		break;
	}

	case 2:
	{
		std::string data;
		int endsign = 0;
		recvpacket >> data >> endsign;

		//if (_index == 9999)		// TEST
		//	printf("[%d] Recv  : %d\n", _index, endsign);
		{
			MyPacket sendPacket(recvpacket.GetBuffer(), recvpacket.GetPacketSize());
			ClientManager::GetInstance()->BroadCast(recvpacket);
		}
		break;
	}
	
	default:
		break;
	}
}

int Client::SendPacket(MyPacket & packet)
{
	Statistic::GetInstance()->AddSendCount(1);
    //ClientLog(level::INFO, "[%d] SendPacket:%d", _index, packet.GetProtocolID());		// LOG
    return SendRequest(packet.GetBuffer(), packet.GetPacketSize());
}
