#include "Buffer.h"

#include <utility>
#include <assert.h>


using namespace suho::buffer;


Buffer::Buffer( int capacity ) :
	_filled_size( 0 ), _begin( 0 )
{
	Create( capacity );
}

Buffer::Buffer( const Buffer& other ) :
	_filled_size( other._filled_size )
{
	Create( other._capacity );

	memcpy( _begin, other._begin, other._filled_size );
}

Buffer::Buffer( Buffer&& other ) noexcept :
	_begin( other._begin ), _capacity( other._capacity ), _filled_size( other._filled_size )
{
	other._begin = nullptr;
	other._capacity = 0;
	other._filled_size = 0;
}

Buffer::~Buffer()
{
	Free();
}

Buffer& Buffer::operator=( const Buffer& other )
{
	if ( this != &other )
	{
		Buffer temp( other );
		Swap( temp );
	}

	return *this;
}

Buffer& suho::buffer::Buffer::operator=( Buffer&& other ) noexcept
{
	if ( this != &other )
	{
		_begin = other._begin;
		_capacity = other._capacity;
		_filled_size = other._filled_size;

		other._begin = nullptr;
		other._capacity = 0;
		other._filled_size = 0;
	}

	return *this;
}

void Buffer::Create( int capacity )
{
	Free();
	Alloc( capacity );

	_capacity = capacity;
	memset( _begin, 0, capacity );
}

void Buffer::Assign( const char* buf, int size )
{
	if ( size < 1 ) return;

	ReSize( size, false );

	memcpy( _begin, buf, size );
	_filled_size = size;
}

void Buffer::Append( const char* buf, int size )
{
	if ( size < 1 ) return;

	ReSize( _filled_size + size, true );

	memcpy( _begin + _filled_size, buf, size );
	_filled_size += size;
}

void Buffer::ReSize( int newsize, bool is_keepdata )
{
	if ( newsize > _capacity )
	{
		char* begin_t = new char[ newsize ];
		memset( begin_t, 0, newsize );
		if ( is_keepdata )
		{
			memcpy( begin_t, _begin, _filled_size );
		}

		delete[] _begin;
		_begin = begin_t;
		_capacity = newsize;
	}
}

void Buffer::Clear()
{
	memset( _begin, 0, _filled_size );
	_filled_size = 0;
}

void Buffer::Alloc( int capacity )
{
	assert( capacity > 0 && "Buffer Minimum Size is 1" );

	_begin = new char[ capacity ];
}

void Buffer::Free()
{
	if ( _begin )
	{
		delete[] _begin;
		_begin = nullptr;
	}

	_capacity = 0;
	_filled_size = 0;
}

void Buffer::Swap( Buffer& other )
{
	std::swap( _capacity, other._capacity );
	std::swap( _filled_size, other._filled_size );
	std::swap( _begin, other._begin );
}