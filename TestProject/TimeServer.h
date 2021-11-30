#pragma once

#include "NetUnit.h"
#include "MyPacket.h"

class TimeServer : public suho::winnet::iocp::NetUnit
{
public:
	TimeServer() {}
	TimeServer(int id) : NetUnit(id) {}
	~TimeServer() override {}

	virtual void OnInit() override;
	virtual void OnConnect() override;
	virtual void OnDisconnect() override;

	virtual DWORD HeaderParsing(void* packet, DWORD recvbytes) override;
	virtual void PacketProcessing(const void* packet, DWORD packetsize) override;

	int SendPacket(MyPacket& packet);

private:
};