#include "test_common.h"
#include "Buffer.h"
#include "RingBuffer.h"
#include "StreamBuffer.h"

using namespace suho::buffer;

int main()
{

	RingBuffer rbuffer( 100 );

	Buffer buffer( 1024 );
	buffer.Assign( "abcd", 5 );

	Buffer buffer2 = std::move( buffer );

	buffer = buffer2;


	StreamBuffer sbuffer( 1024 );
	sbuffer.Write( "abcd", 5 );
	StreamBuffer sbuffer2( std::move( sbuffer ) );
	sbuffer = sbuffer2;


}