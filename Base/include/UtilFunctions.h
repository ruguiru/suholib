#pragma once

#include <string>
#include <algorithm>

namespace suho {
namespace util
{
    inline static void ReplaceString(std::string &source, const std::string& from, const std::string& to)
    {
        size_t start_pos = 0;
        while ((start_pos = source.find(from, start_pos)) != std::string::npos)
        {
            source.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    }

    inline static void ToUpperString( const std::string& source, std::string& upperstring)
    {
        std::string upper(source);
        std::transform(upperstring.begin(), upperstring.end(), upperstring.begin(), ::toupper);
    }

    inline static void ToLowerString(const std::string& source, std::string& lowerstring)
    {
        std::string lower(source);
        std::transform(lowerstring.begin(), lowerstring.end(), lowerstring.begin(), ::tolower);
    }



}   // end namespace util
}   // end 
