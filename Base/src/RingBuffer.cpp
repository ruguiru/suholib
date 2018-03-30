#include "RingBuffer.h"

using namespace suho::buffer;

void RingBuffer::Clear()
{
    _buffer.Clear();
    _head = 0;
    _tail = 0;
    _data_size = 0;
}

void RingBuffer::Resize(int capacity)
{
	_buffer.ReSize(capacity);
    Clear();
}

bool RingBuffer::Peek(char * buf, int size)
{
    if (buf == nullptr) return false;

    if (size > _data_size)
        return false;

    if (_head < _tail)
    {
        memcpy(buf, _buffer.Begin() + _head, size);
    }
    else
    {
        int space = _buffer.GetCapacity() - _head;

        if (space >= size)
        {
            memcpy(buf, _buffer.Begin() + _head, size);
        }
        else
        {
            memcpy(buf, _buffer.Begin() + _head, space);
            memcpy(buf + space, _buffer.Begin(), size - space);
        }
    }

    return true;
}

bool RingBuffer::Read(char * buf, int size)
{
    if (Peek(buf, size))
    {
        _head = (_head + size) % _buffer.GetCapacity();
        _data_size -= size;

        return true;
    }

    return false;
}

bool RingBuffer::Write(const char * data, int size)
{   
    if (data == nullptr) return false;    

    if (size > GetWritableSize())
        return false;

    if (_head <= _tail)
    {
        int space = _buffer.GetCapacity() - _tail;

        if (space >= size)
        {
            memcpy(_buffer.Begin() + _tail, data, size);
        }
        else
        {
            memcpy(_buffer.Begin() + _tail, data, space);
            memcpy(_buffer.Begin(), data + space, size - space);
        }
    }
    else
    {
        memcpy(_buffer.Begin() + _tail, data, size);
    }


    _tail = (_tail + size) % _buffer.GetCapacity();
    _data_size += size;

    return true;
}
