#pragma once

#include <type_traits>
#include <string>

#include "StreamBuffer.h"

class AbstractPacket
{
public:
	AbstractPacket( int buffersize, int headersize ) :
		_buffersize(buffersize), _headersize(headersize)
	{
		_stream_buffer.Create(_buffersize);
	}

	AbstractPacket(const AbstractPacket& other) :
		_headersize(other._headersize), _buffersize(other._buffersize),
		_stream_buffer(other._stream_buffer)
	{

	}

	AbstractPacket(AbstractPacket&& other) noexcept :
		_headersize(other._headersize), _buffersize(other._buffersize),
		_stream_buffer(std::move(other._stream_buffer))
	{
		other._headersize = 0;
		other._buffersize = 0;
	}

	virtual ~AbstractPacket()
	{		
	}

	const int GetHeaderSize() const { return _headersize; }
	int GetDataSize() const { return (_stream_buffer.GetSize() <= _headersize)? 0: _stream_buffer.GetSize() - _headersize; }
	int GetPacketSize() const { return _stream_buffer.GetSize(); }
	char* GetBuffer() { return _stream_buffer.Begin(); }
	char* GetDataBuffer() { return _stream_buffer.Begin() + _headersize; }

	// 메모리는 삭제하지 않음(메모리는 _buffer 소멸자에서)
	void Clear() { _stream_buffer.Clear(); }

	virtual void Copy(const void* buffer, int size);	// 시작위치에 카피
	virtual bool Write(const void* data, int size);		// writepos 에 카피
	virtual bool Read(void* data, int size);

	// 기본 타입 지원 (포인터나, 구조체, 배열 등은 하위 클래스에서 특수화)
	template<typename T> inline AbstractPacket& operator<<(const T& data);
	template<typename T> inline AbstractPacket& operator>>(T& data);
	// 문자열까지 지원	
	inline AbstractPacket& operator<<(const std::string& str);
	inline AbstractPacket& operator>>(std::string& str);
	inline AbstractPacket& operator<<(const char* sz);
	inline AbstractPacket& operator>>(char* sz);

#ifdef _DEBUG
	void ShowMemory();
	void ShowCharacter();
#endif // _DEBUG

protected:
	int							_headersize;
	int							_buffersize;
	suho::buffer::StreamBuffer	_stream_buffer;
};

template<typename T>
inline AbstractPacket & AbstractPacket::operator<<(const T & data)
{
	static_assert(!std::is_void<T>::value, "Packet::operator<< fail! Does Not Suppot Void type");
	static_assert(std::is_fundamental<T>::value, "Packet::operator<< fail! Only fundamental type");

	Write(&data, sizeof(T));

	return *this;
}

template<typename T>
inline AbstractPacket & AbstractPacket::operator>>(T & data)
{
	static_assert(!std::is_void<T>::value, "Packet::operator<< fail! Does Not Suppot Void type");
	static_assert(std::is_fundamental<T>::value, "Packet::operator<< fail! Only fundamental type");

	Read(&data, sizeof(T));

	return *this;
}
// 오버로딩이라 같은 위치에 있어야 인식
inline AbstractPacket & AbstractPacket::operator<<(const std::string & str)
{
	Write(str.c_str(), static_cast<int>(str.size()) + 1);

	return *this;
}

inline AbstractPacket & AbstractPacket::operator>>(std::string & str)
{
	int size = static_cast<int>(strlen(_stream_buffer.GetReadPos()) + 1);

	//Read(const_cast<char*>(str.c_str()), size);
	str = _stream_buffer.GetReadPos();
	_stream_buffer.ShiftReadPos(size);

	return *this;
}

inline AbstractPacket & AbstractPacket::operator<<(const char * sz)
{
	int size = static_cast<int>(strlen(sz)) + 1;

	Write(sz, size);

	return *this;
}

inline AbstractPacket & AbstractPacket::operator>>(char * sz)
{
	int size = static_cast<int>(strlen(_stream_buffer.GetReadPos())) + 1   ;

	Read(sz, size);

	return *this;
}