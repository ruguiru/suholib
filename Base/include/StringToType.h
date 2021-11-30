#pragma once

namespace suho
{
	namespace idioms
	{
		// 각각의 string 을 개별 타입으로 쓰기 위한 장치
		#define STR_1(S,I) (I < sizeof(S) ? S[I] : '\0')
		#define STR_2(S,I) STR_1(S,I), STR_1(S,I+1)
		#define STR_4(S,I) STR_2(S,I), STR_2(S,I+2)
		#define STR_8(S,I) STR_4(S,I), STR_4(S,I+4)
		#define STR_16(S,I) STR_8(S,I), STR_8(S,I+8)
		#define STR_32(S,I) STR_16(S,I), STR_16(S,I+16)
		#define STR_64(S,I) STR_32(S,I), STR_32(S,I+32)
		#define STR_128(S,I) STR_64(S,I), STR_64(S,I+64)
		#define STR(S) STR_128(S,0)

		template <char... Chars>
		struct MyString
		{
			static constexpr char value[] = { Chars..., '\0' };
		};
	}   // end namespace idioms
}   // end namespace suho
