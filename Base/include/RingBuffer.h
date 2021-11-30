#pragma once

#include <mutex>

#include "Buffer.h"

namespace suho
{
	namespace buffer
	{
		class RingBuffer
		{
		public:
			RingBuffer() {}
			RingBuffer( int capacity ) { _buffer.Create( capacity ); }
			~RingBuffer() {}

			void Create( int capacity ) { _buffer.Create( capacity ); }
			void Clear();
			void Resize( int capacity );

			bool Peek( char* buf, int size );			// 데이터만 읽음
			bool Read( char* buf, int size );			// 데이터 읽은 후 read_pos 이동
			bool Write( const char* data, int size );

			int GetCapacity() const { return _buffer.GetCapacity(); }
			int GetReadableSize() const { return _data_size; }
			int GetWritableSize() const { return _buffer.GetCapacity() - _data_size; }

		private:
			RingBuffer( const RingBuffer& other ) = delete;
			RingBuffer& operator=( const RingBuffer& other ) = delete;

			RingBuffer( RingBuffer&& other ) = delete;
			RingBuffer& operator=( RingBuffer&& other ) = delete;

		private:
			suho::buffer::Buffer _buffer;
			int                  _head = 0;          // read pos
			int                  _tail = 0;          // write pos
			int                  _data_size = 0;     // total writed data
		};
	}	// end namespace suho
}	// end namespace buffer