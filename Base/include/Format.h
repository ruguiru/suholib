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
			// ��¥+�ð� ���� ���� �������̽�
			class FormatterInterface
			{
			public:
				virtual std::tuple<int, int, int, int, int, int> Parse( const std::string& dt ) = 0;
				virtual std::string ToString( int year, int month, int day, int hour, int minute, int second ) = 0;
			};

			// ���ǵ� ���� �̻��� ������ ���� �߻���Ű�� �뵵
			template<typename T>
			struct Formatter {};

			// ���� Ŭ���� ������
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