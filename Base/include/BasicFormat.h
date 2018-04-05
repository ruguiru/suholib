#pragma once

#include "Format.h"
#include "Common.h"

namespace suho {
namespace datetime {
namespace format
{
    BUILDFORMAT(YYYYMMDDHHMISS,
		[](const std::string& date) -> decltype(std::tuple<int, int, int, int, int, int>())
		{
			int year = 0;
			int month = 0;
			int day = 0;
			int hour = 0;
			int minute = 0;
			int second = 0;

			if (date.size() < 14)
				return std::tie(year, month, day, hour, minute, second);

			year = static_cast<int>(atoi(date.substr(0, 4).c_str()));
			month = static_cast<int>(atoi(date.substr(4, 2).c_str()));
			day = static_cast<int>(atoi(date.substr(6, 2).c_str()));
			hour = static_cast<int>(atoi(date.substr(8, 2).c_str()));
			minute = static_cast<int>(atoi(date.substr(10, 2).c_str()));
			second = static_cast<int>(atoi(date.substr(12, 2).c_str()));

			return std::tie(year, month, day, hour, minute, second);
		}
		,
		[](int year, int month, int day, int hour, int minute, int second) -> decltype(std::string())
		{
			char date[32] = {};

			sprintf_s(date, "%04d%02d%02d%02d%02d%02d", year, month, day, hour, minute, second);

			return date;
		}
	)	// end BUILDFORMAT


	BUILDFORMAT(YYYY-MM-DD HH:MI:SS,
		[](const std::string& date) -> decltype(std::tuple<int, int, int, int, int, int>())
		{
			int year = 0;
			int month = 0;
			int day = 0;
			int hour = 0;
			int minute = 0;
			int second = 0;

			if (date.size() <= 19)
				return std::tie(year, month, day, hour, minute, second);

			year = static_cast<int>(atoi(date.substr(0, 4).c_str()));
			month = static_cast<int>(atoi(date.substr(5, 2).c_str()));
			day = static_cast<int>(atoi(date.substr(8, 2).c_str()));
			hour = static_cast<int>(atoi(date.substr(11, 2).c_str()));
			minute = static_cast<int>(atoi(date.substr(14, 2).c_str()));
			second = static_cast<int>(atoi(date.substr(17, 2).c_str()));

			return std::tie(year, month, day, hour, minute, second);
		}
		, 
		[](int year, int month, int day, int hour, int minute, int second) -> decltype(std::string())
		{
			char date[32] = {};

			sprintf_s(date, "%04d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second);

			return date;
		}
	)	// end BUILDFORMAT


	BUILDFORMAT(YYYYMMDD, 
		[](const std::string& date) -> decltype(std::tuple<int, int, int, int, int, int>())
		{
			int year = 0;
			int month = 0;
			int day = 0;
			int hour = 0;
			int minute = 0;
			int second = 0;

			if (date.size() < 8)
				return std::tie(year, month, day, hour, minute, second);

			year = atoi(date.substr(0, 4).c_str());
			month = atoi(date.substr(4, 2).c_str());
			day = atoi(date.substr(6, 2).c_str());

			return std::tie(year, month, day, hour, minute, second);
		}
		,
		[](int year, int month, int day, int hour, int minute, int second) -> decltype(std::string())
		{
			UNREFERENCED(hour);
			UNREFERENCED(minute);
			UNREFERENCED(second);

			char date[32] = {};

			sprintf_s(date, "%04d%02d%02d", year, month, day);

			return date;
		}
	)	// end BUILDFORMAT


	BUILDFORMAT(YYYY/MM/DD,
		[](const std::string& date) -> decltype(std::tuple<int,int,int,int,int,int>())
		{
			int year = 0;
			int month = 0;
			int day = 0;
			int hour = 0;
			int minute = 0;
			int second = 0;

			if (date.size() < 10)
				return std::tie(year, month, day, hour, minute, second);

			year = atoi(date.substr(0, 4).c_str());
			month = atoi(date.substr(5, 2).c_str());
			day = atoi(date.substr(8, 2).c_str());

			return std::tie(year, month, day, hour, minute, second);
		},
		[](int year, int month, int day, int hour, int minute, int second) -> decltype(std::string())
		{
			UNREFERENCED(hour);
			UNREFERENCED(minute);
			UNREFERENCED(second);

			char date[32] = {};

			sprintf_s(date, "%04d/%02d/%02d", year, month, day);

			return date;
		}
	)	// end BUILDFORMAT


	BUILDFORMAT(YYYY-MM-DD,
		[](const std::string& date) -> decltype(std::tuple<int, int, int, int, int, int>())
		{
			int year = 0;
			int month = 0;
			int day = 0;
			int hour = 0;
			int minute = 0;
			int second = 0;

			if (date.size() < 10)
				return std::tie(year, month, day, hour, minute, second);

			year = atoi(date.substr(0, 4).c_str());
			month = atoi(date.substr(5, 2).c_str());
			day = atoi(date.substr(8, 2).c_str());

			return std::tie(year, month, day, hour, minute, second);
		}
		,
		[](int year, int month, int day, int hour, int minute, int second) -> decltype(std::string())
		{
			UNREFERENCED(hour);
			UNREFERENCED(minute);
			UNREFERENCED(second);

			char date[32] = {};

			sprintf_s(date, "%04d-%02d-%02d", year, month, day);

			return date;
		}
	)	// end BUILDFORMAT
} 
}   // end namespace datetime 
}   // end namespace suho
