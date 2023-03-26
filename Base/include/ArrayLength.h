#pragma once

template<typename T, int N>
constexpr int length( const T( &ary )[ N ] )
{
	return N;
}
