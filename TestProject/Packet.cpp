#include "Packet.h"

void AbstractPacket::Copy(const void * buffer, int size)
{
	Clear();
	_stream_buffer.Write(reinterpret_cast<const char*>(buffer), size);
}

bool AbstractPacket::Write(const void * data, int size)
{
	return _stream_buffer.Write(reinterpret_cast<const char*>(data), size);
}

bool AbstractPacket::Read(void * data, int size)
{
	return _stream_buffer.Read(reinterpret_cast<char*>(data), size);
}

#ifdef _DEBUG
void AbstractPacket::ShowMemory()
{
	for (int i = 0; i < _stream_buffer.GetSize(); i++)
		printf("%02x ", static_cast<unsigned char>(*(_stream_buffer.Begin() + i)));

	printf("\n");
}

void AbstractPacket::ShowCharacter()
{
	for (int i = 0; i < _stream_buffer.GetSize(); i++)
		printf("%c", static_cast<char>(*(_stream_buffer.Begin() + i)));

	printf("\n");
}

#endif // _DEBUG