#pragma once

#include <string>
#include <tuple>

#include "StringToType.h"

namespace suho
{
	namespace datetime
	{
		namespace format
		{
			// 날짜+시간 형식 설정 인터페이스
			class FormatterInterface
			{
			public:
				virtual std::tuple<int, int, int, int, int, int> Parse( const std::string& dt ) = 0;
				virtual std::string ToString( int year, int month, int day, int hour, int minute, int second ) = 0;
			};

			// 정의된 포맷 미사용시 컴파일 에러 발생시키는 용도
			template<typename T>
			struct Formatter {};

			// 포맷 클래스 생성기
			#define BUILDFORMAT(x, func_parse, func_tostring)                                                              \
			    template<>                                                                                                 \
			    class Formatter<suho::idioms::MyString<STR(#x)>> : public FormatterInterface                               \
			    {                                                                                                          \
			    public:                                                                                                    \
			        inline std::tuple<int, int, int, int, int, int> Parse(const std::string& dt) override                  \
			        {                                                                                                      \
			            return func_parse(dt);                                                                             \
			        }                                                                                                      \
			        inline std::string ToString(int year, int month, int day, int hour, int minute, int seconde ) override \
			        {                                                                                                      \
			            return func_tostring(year, month, day, hour, minute, seconde);                                     \
			        }                                                                                                      \
			    };

		}   // end namespace format
	}   // end namespace format
}   // end namespace suho