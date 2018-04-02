#include "TimeServer.h"
#include "TimeServerManager.h"
#include "ConnectionManager.hpp"
#include "LogContainer.h"

using namespace suho::winnet::iocp;

void TimeServer::OnInit()
{
	_recv_buffersize = 1024 * 4;
	_recv_socket_buffersize = 1024 * 10;		// TEST
	_send_socket_buffersize = 1024 * 20;		// TEST
}

void TimeServer::OnConnect()
{
	_own_socket->SetSendBufferSize(_send_socket_buffersize);
	_own_socket->SetRecieveBufferSize(_recv_buffersize);
	
	TimeServerManager::GetInstance()->Insert(_index,
		ConnectionManager::GetInstance()->Get(_index));

	MyPacket sendPacket(1);
	sendPacket << "connect success";
	SendPacket(sendPacket);
}

void TimeServer::OnDisconnect()
{
	TimeServerManager::GetInstance()->Remove(_index);
}

DWORD TimeServer::HeaderParsing(void* packet, DWORD recvbytes)
{
	DWORD headersize = MyPacket::GetHeaderSize();
	if (recvbytes < headersize)
		return 0;

	int datasize = *reinterpret_cast<int*>(packet);
	DWORD packetsize = headersize + datasize;

	return (recvbytes >= packetsize) ? packetsize : 0;
}

void TimeServer::PacketProcessing(const void* packet, DWORD packetsize)
{
	MyPacket recvpacket(packet, packetsize);

	switch (recvpacket.GetProtocolID())
	{
	case 2:
	{
		std::string data;
		int endsign = 0;
		recvpacket >> data >> endsign;
		printf("[%d] recv from timeserver %s %d\n", _index, data.c_str(), endsign); // TEST


		MyPacket sendPacket(1);
		sendPacket << "send message";
		SendPacket(sendPacket);
		break;
	}

	default:
		break;
	}
}

int TimeServer::SendPacket(MyPacket & packet)
{
	return SendRequest(packet.GetBuffer(), packet.GetPacketSize());
}