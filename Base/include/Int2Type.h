#pragma once

namespace suho
{
	namespace idioms
	{
		template<int N> struct Int2Type
		{
			enum { value = N };
		};
	}   // end namespace idioms
}   // end namespace suho