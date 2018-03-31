#include "MyPacket.h"

void MyPacket::Set(const void * data, int size)
{
	Init();
	_stream_buffer.ShiftWritePos(-_headersize);
	AbstractPacket::Write(data, size);
}

bool MyPacket::Write(const void * data, int size)
{
	bool success = AbstractPacket::Write(data, size);
	if (success)
		*_datasize += size;

	return success;
}

void MyPacket::Init()
{
	_stream_buffer.Clear();

	_stream_buffer.ShiftWritePos(_headersize);
	_stream_buffer.ShiftReadPos(_headersize);

	_datasize = reinterpret_cast<int*>(_stream_buffer.Begin());
	_protocol_id = reinterpret_cast<int*>(_stream_buffer.Begin() + sizeof(int));
}

