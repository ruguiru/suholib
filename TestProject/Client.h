#pragma once

#include "NetUnit.h"
#include "MyPacket.h"

class Client : public suho::winnet::iocp::NetUnit
{
public:
	Client() {}
    Client(int id) : NetUnit(id)
	{

	}
    virtual ~Client() override {}

	virtual void OnInit() override;
	virtual void OnConnect() override;
	virtual void OnDisconnect() override;

	virtual DWORD HeaderParsing(void* packet, DWORD recvbytes) override;
	virtual void PacketProcessing(const void* packet, DWORD packetsize) override;

    int SendPacket(MyPacket& packet);


private:
	long							_key;
};

