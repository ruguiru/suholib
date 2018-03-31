#pragma once

#include "Packet.h"

class MyPacket : public AbstractPacket
{
	enum {	BUFFER_SIZE = 1024,		// ���� flexible �ϵ��� ����
			HEADER_SIZE = 8,	};

public:
	// header ���� ������ �������� ����
	struct Header
	{
		unsigned int datasize = 0;
		int protocol_id = 0;
	};

	MyPacket() : AbstractPacket(BUFFER_SIZE, HEADER_SIZE)
	{
		Init();
	}

	MyPacket(int protocol_id) : AbstractPacket(BUFFER_SIZE, HEADER_SIZE)
	{
		Init();
		SetProtocol(protocol_id);
	}

	MyPacket(const void* data, int size) : AbstractPacket(BUFFER_SIZE, HEADER_SIZE)
	{
		Set(data, size);
	}

	~MyPacket() override {}

	void Set(const void* data, int size);
	void Reset() { Init(); }
	void SetProtocol(int protocol_id) {	*_protocol_id = protocol_id; }	

	bool Write(const void* data, int size) override;

	const int GetProtocolID() const { return *_protocol_id; }
	static const unsigned int GetHeaderSize() { return HEADER_SIZE; }

private:
	void Init();

private:
	int*									_datasize = nullptr;
	int*									_protocol_id = nullptr;
};