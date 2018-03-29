#pragma once

#include <mutex>

#include "Buffer.h"

namespace suho {
namespace buffer
{
	class StreamBuffer
	{
	public:
		StreamBuffer();
		StreamBuffer(int capacity);
		StreamBuffer(const StreamBuffer& other);
		StreamBuffer(StreamBuffer&& other);
		~StreamBuffer();

        StreamBuffer& operator=(const StreamBuffer& other);
        StreamBuffer& operator=(StreamBuffer&& other);

		void Create(int capacity) { _buffer.Create(capacity); }
		void Clear();
		void Resize(int capacity);

		bool Peek(char* buf, int size);						// �����͸� ����
		bool Read(char* buf, int size);						// ������ ���� �� read_pos �̵�		
		bool Write(const char* data, int size);				// ������ ���� �� write_pos �̵�
		// read, write �� ����ϸ� �ᱹ ������ ���� �����ϹǷ� �ѹ��� ��ó���� �����͸� �Ǿ����� �̵���Ű�� position�� �̵�
		void CarriageReturn();								

		// Write, Read ȣ����� ���۰� ���ϴ� ��� ��ó���� ����(ex WSABUF.buf = _buffer)
		void ShiftReadPos(int offset);
		void ShiftWritePos(int offset);

		char* GetReadPos() { return _buffer.Begin() + _read_pos; }
		char* GetWritePos() { return _buffer.Begin() + _write_pos; }
		char* Begin() { return _buffer.Begin(); }

        int GetCapacity() const { return _buffer.GetCapacity(); }
		int GetSize() const { return _buffer.GetSize(); }
        int GetReadableSize() const { return _write_pos - _read_pos; }
        int GetWritableSize() const { return _buffer.GetCapacity() - _write_pos; }

        std::mutex& GetMutex() { return _mutex; }

    private:
		suho::buffer::Buffer            _buffer;
		int								_read_pos = 0;
		int								_write_pos = 0;

        std::mutex				        _mutex;
    };
}	// end namespace suho
}	// end namespace buffer