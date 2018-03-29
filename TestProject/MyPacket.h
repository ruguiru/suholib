#pragma once

#include "Packet.h"

class MyPacket : public AbstractPacket
{
	enum {	BUFFER_SIZE = 1024,		// 추후 flexible 하도록 수정
			HEADER_SIZE = 8,	};

public:
	// header 구조 실제로 쓰이지는 않음
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

	void Reset() { Init(); }
	void SetProtocol(int protocol_id) {	*_protocol_id = protocol_id; }	

	void Set(const void* data, int size)
	{		
		Init();
		_stream_buffer.ShiftWritePos(-_headersize);
		AbstractPacket::Write(data, size);
	}

	bool Write(const void* data, int size) override
	{		
		bool success = AbstractPacket::Write(data, size);
		if( success )
			*_datasize += size;

		return success;
	}

	const int GetProtocolID() const { return *_protocol_id; }
	static const unsigned int GetHeaderSize() { return HEADER_SIZE; }

private:
	void Init()
	{
		_stream_buffer.Clear();

		_stream_buffer.ShiftWritePos(_headersize);
		_stream_buffer.ShiftReadPos(_headersize);

		_datasize = reinterpret_cast<int*>(_stream_buffer.Begin());
		_protocol_id = reinterpret_cast<int*>(_stream_buffer.Begin() + sizeof(int));
	}

private:
	int*									_datasize = nullptr;
	int*									_protocol_id = nullptr;
};