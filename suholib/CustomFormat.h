#pragma once

#include "Format.h"

namespace suho {
namespace datetime
{
    namespace format
    {
        static std::tuple<int, int, int, int, int, int> f1_1(const std::string& date)
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
            month = atoi(date.substr(5, 2).c_str());
            day = atoi(date.substr(8, 2).c_str());

            return std::tie(year, month, day, hour, minute, second);
        };

        static std::string f1_2(int year, int month, int day, int hour, int minute, int seconde)
        {
            char date[32] = {};

            sprintf_s(date, "%04d-%02d-%02d", year, month, day);

            return date;
        };

        BUILDFORMAT(YYYY-MM-DD, f1_1, f1_2)
    }
}   // end namespace datetime 
}   // end namespace suho