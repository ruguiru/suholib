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

		bool Peek(char* buf, int size);						// 데이터만 읽음
		bool Read(char* buf, int size);						// 데이터 읽은 후 read_pos 이동		
		bool Write(const char* data, int size);				// 데이터 복사 후 write_pos 이동
		// read, write 만 계속하면 결국 버퍼의 끝에 도달하므로 한번씩 미처리한 데이터를 맨앞으로 이동시키며 position도 이동
		void CarriageReturn();								

		// Write, Read 호출없이 버퍼가 변하는 경우 후처리를 위해(ex WSABUF.buf = _buffer)
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