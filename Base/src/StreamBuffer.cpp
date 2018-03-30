#include "StreamBuffer.h"

using namespace suho::buffer;

StreamBuffer::StreamBuffer()
{
}

StreamBuffer::StreamBuffer(int capacity)
{
	_buffer.Create(capacity);
}

StreamBuffer::StreamBuffer(const StreamBuffer & other)
	:_read_pos(other._read_pos), _write_pos(other._write_pos)
{
	_buffer.Create(other.GetCapacity());
	memcpy(_buffer.Begin(), other._buffer.Begin(), other.GetSize());
	_buffer.SetSize(other.GetSize());
}

StreamBuffer::StreamBuffer(StreamBuffer && other) :
	_buffer(std::move(other._buffer)), _read_pos(other._read_pos), _write_pos(other._write_pos)
{
	other._read_pos = 0;
	other._write_pos = 0;
}

StreamBuffer::~StreamBuffer()
{
}

StreamBuffer & StreamBuffer::operator=(const StreamBuffer & other)
{
    if (this != &other)
    {
        _buffer = other._buffer;
        _read_pos = other._read_pos;
        _write_pos = other._write_pos;        
    }

    return *this;
}

StreamBuffer & StreamBuffer::operator=(StreamBuffer && other)
{
    if (this != &other)
    {
        _buffer = std::move(other._buffer);
        _read_pos = other._read_pos;
        _write_pos = other._write_pos;

        other._read_pos = 0;
        other._write_pos = 0;
    }

    return *this;
}

void StreamBuffer::Clear()
{
	_buffer.Clear();
	_read_pos = 0;
	_write_pos = 0;
}

void StreamBuffer::Resize(int capacity)
{
	_buffer.ReSize(capacity);

	Clear();
}

bool StreamBuffer::Peek(char * buf, int size)
{
	if (buf == nullptr) return false;

	if (size > GetReadableSize())
		return false;

	memcpy(buf, _buffer.Begin() + _read_pos, size);

	return true;
}

bool StreamBuffer::Read(char * buf, int size)
{
	if (Peek(buf, size))
	{
		_read_pos += size;			

		return true;
	}

	return false;
}

bool StreamBuffer::Write(const char * data, int size)
{
	if (data == nullptr) return false;	

	if (size > GetWritableSize())
		return false;

	memcpy(_buffer.Begin() + _write_pos, data, size);
	_buffer.AddSize(size);
	_write_pos += size;

	return true;
}

void StreamBuffer::CarriageReturn()
{
	int move_size = _write_pos - _read_pos;

	if (move_size > 0)
	{
		memmove_s(_buffer.Begin(), _buffer.GetCapacity(), _buffer.Begin() + _read_pos, move_size);
	}
	else if (move_size < 0)		// exception!
		return;
	
	_read_pos = 0;
	_write_pos = move_size;
	_buffer.SetSize(move_size);
}

void StreamBuffer::ShiftReadPos(int offset)
{
	_read_pos += offset;
}

void StreamBuffer::ShiftWritePos(int offset)
{
	_write_pos += offset;
	_buffer.AddSize(offset);
}
