#pragma once

#include <string>
#include <chrono>
#include <ratio>

#include "Noncopyable.h"
#include "DatetimeConfig.h"
#include "BasicFormat.h"
#include "CustomFormat.h"

namespace suho {
namespace datetime
{
    // 년, 시, 분, 초 타입 정의
    inline namespace timetype
    {
        struct TimeType {};

        // 윤년때문에 년, 월은 단위변환(duration) 계산 하지 않음, 타입으로만 사용
        struct YEAR : TimeType, suho::idioms::NonCopyable
        {
            YEAR(int v) : count(v) {}
            int Count() const { return count; }
        private:
            int     count = 0;
        };

        struct MONTH : TimeType, suho::idioms::NonCopyable
        {
            MONTH(int v) : count(v) {}
            int Count() const { return count; }
        private:
            int     count = 0;
        };

        struct DAY : TimeType, suho::idioms::NonCopyable
        {
            typedef std::chrono::duration<int, std::ratio<3600 * 24>> DurationType;
            DAY(int v) : duration(v) {}
            int Count() const { return duration.count(); }

            DurationType duration;
        };

        struct HOUR : TimeType, suho::idioms::NonCopyable
        {
            typedef std::chrono::hours DurationType;
            HOUR(int v) : duration(v) {}
            int Count() const { return duration.count(); }

            DurationType duration;
        };

        struct MIN : TimeType, suho::idioms::NonCopyable
        {
            typedef std::chrono::minutes DurationType;
            MIN(int v) : duration(v) {}
            int Count() const { return duration.count(); }

            DurationType duration;
        };

        // 단위변환의 기준은 초
        struct SEC : TimeType, suho::idioms::NonCopyable
        {
            typedef std::chrono::seconds DurationType;
            SEC(long long v) : duration(v) {}
            long long Count() const { return duration.count(); }

            DurationType duration;
        };
    }

    class DateTime
    {
    public:
        DateTime();
        DateTime(const DateTime& other);
        DateTime& operator=(const DateTime& other);
        ~DateTime();

        template<typename T> inline bool SetDate(const std::string& date, datetime::format::Formatter<T>& format);
        template<typename T> inline bool SetDateTime(const std::string& datetime, datetime::format::Formatter<T>& format);
        template<typename T> inline std::string ToString(datetime::format::Formatter<T>& format) const;

        bool SetDateTime(int year, int month, int day, int hour, int minute, int second);
        bool SetDateTime(long long datetime);                     // YYYYMMDDHHMISS 형식 (30001231235959)
        bool SetDate(int date);
        bool SetDate(int year, int month, int day);
        void SetNow();

        int GetYear() const { return _year; }
        int GetMonth() const { return _month; }
        int GetDay() const { return _day; }
        int GetHour() const { return _hour; }
        int GetMinute() const { return _min; }
        int GetSecond() const { return _sec; }

        long long GetDateTime() const { return static_cast<long long>(GetDate() * 1000000) + static_cast<long long>(GetTime()); }
        int	GetDate() const { return (_year) * 10000 + (_month) * 100 + _day; }
        int GetTime() const { return _hour * 10000 + _min * 100 + _sec; }

        int GetAge() const;
        long long GetTotalSeconds() const;
        int GetDayofweek() const { return GetDayofWeek(_year, _month, _day); }

        static int GetDayofWeek(int year, int month, int day);
        static int GetDaysFromFirstYear(int year, int month, int day);
        static bool IsLeapYear(int year);


        // 시간을 주, 일, 시, 분, 초 타입으로 가변인자로 더하고 빼는 기능, 원본에 적용 (-값 넣으면 시간 뒤로 돌린다)
        inline void Adjust() {}       // 재귀 중지용
        template<typename T, typename... ARGS>	inline void Adjust(const T& t, const ARGS&... rest);
        template<typename... ARGS>				inline void Adjust(const YEAR& value, const ARGS&... rest);
        template<typename... ARGS>				inline void Adjust(const MONTH& value, const ARGS&... rest);
        template<typename... ARGS>		        inline void Adjust(const DAY& value, const ARGS&... rest);
        template<typename... ARGS>		        inline void Adjust(const HOUR& value, const ARGS&... rest);
        template<typename... ARGS>		        inline void Adjust(const MIN& value, const ARGS&... rest);
        template<typename... ARGS>		        inline void Adjust(const SEC& value, const ARGS&... rest);

        // +,- 연산결과 리턴, 원본은 그대로
        friend DateTime& operator+(DateTime& dt, const YEAR& value);
        friend DateTime& operator+(DateTime& dt, const MONTH& value);
        friend DateTime& operator+(DateTime& dt, const DAY& value);
        friend DateTime& operator+(DateTime& dt, const HOUR& value);
        friend DateTime& operator+(DateTime& dt, const MIN& value);
        friend DateTime& operator+(DateTime& dt, const SEC& value);

        friend DateTime& operator-(DateTime& dt, const YEAR& value);
        friend DateTime& operator-(DateTime& dt, const MONTH& value);
        friend DateTime& operator-(DateTime& dt, const DAY& value);
        friend DateTime& operator-(DateTime& dt, const HOUR& value);
        friend DateTime& operator-(DateTime& dt, const MIN& value);
        friend DateTime& operator-(DateTime& dt, const SEC& value);

        // datetime 간 차이를 초단위로 리턴
        friend long long operator-(const DateTime& advanced, const DateTime& lated);

    private:
        void Init();
        char Itoa64(const long long number, char* dest, int radix = 10);

        void AddYearImpl(const int y);
        void AddMonthImpl(const int m);
        void AddDaysImpl(const int d);
        void AddSecondsImpl(const long long s);

        void SubtractYearImpl(const int y);
        void SubtractMonthImpl(const int m);
        void SubtractSecondsImpl(const long long s);

    private:
        int _year = 0;
        int _month = 0;
        int _day = 0;
        int _hour = 0;
        int _min = 0;
        int _sec = 0;

        // 연산을 줄이기 위해 원년부터 최소년도 까지 미리 계산
        int             _precalc_intermid_days = 0;
        long long       _precalc_intermid_sec = 0;
    };


    // 함수 템플릿 정의부
    template<typename T>
    inline bool DateTime::SetDate(const std::string & date, datetime::format::Formatter<T>& format)
    {
        int year = 0;
        int month = 0;
        int day = 0;
        int hour = 0;
        int minute = 0;
        int second = 0;

        std::tie(year, month, day, hour, minute, second) = format.Parse(date);

        return SetDate(year, month, day);
    }

    template<typename T>
    inline bool DateTime::SetDateTime(const std::string & datetime, datetime::format::Formatter<T>& format)
    {
        int year = 0;
        int month = 0;
        int day = 0;
        int hour = 0;
        int minute = 0;
        int second = 0;

        std::tie(year, month, day, hour, minute, second) = format.Parse(datetime);

        return SetDateTime(year, month, day, hour, minute, second);
    }

    template<typename T>
    inline std::string DateTime::ToString(datetime::format::Formatter<T>& format) const
    {
        return format.ToString(_year, _month, _day, _hour, _min, _sec);
    }

    template<typename T, typename ...ARGS>
    inline void DateTime::Adjust(const T & t, const ARGS & ...rest)
    {
        if (std::is_base_of<TimeType, T>::value)   // 정의된 타입 아니면 건너뛴다
            Adjust(t, rest...);
        else
            Adjust(rest...);
    }

    template<typename ...ARGS>
    inline void DateTime::Adjust(const YEAR & value, const ARGS & ...rest)
    {
        int v = value.Count();
        (v >= 0) ? AddYearImpl(v) : SubtractYearImpl(-v);
        Adjust(rest...);
    }

    template<typename ...ARGS>
    inline void DateTime::Adjust(const MONTH & value, const ARGS & ...rest)
    {
        int v = value.Count();
        (v >= 0) ? AddMonthImpl(v) : SubtractMonthImpl(-v);
        Adjust(rest...);
    }

    template<typename ...ARGS>
    inline void DateTime::Adjust(const DAY & value, const ARGS & ...rest)
    {
        int v = value.Count();
        std::chrono::seconds s = std::chrono::duration_cast<SEC::DurationType>(value.duration);
        (v >= 0) ? AddDaysImpl(v) : SubtractSecondsImpl(-s.count());
        Adjust(rest...);
    }

    template<typename ...ARGS>
    inline void DateTime::Adjust(const HOUR & value, const ARGS & ...rest)
    {
        int v = value.Count();
        std::chrono::seconds s = std::chrono::duration_cast<SEC::DurationType>(value.duration);
        (v >= 0) ? AddSecondsImpl(s.count()) : SubtractSecondsImpl(-s.count());
        Adjust(rest...);
    }

    template<typename ...ARGS>
    inline void DateTime::Adjust(const MIN & value, const ARGS & ...rest)
    {
        int v = value.Count();
        std::chrono::seconds s = std::chrono::duration_cast<SEC::DurationType>(value.duration);
        (v >= 0) ? AddSecondsImpl(s.count()) : SubtractSecondsImpl(-s.count());
        Adjust(rest...);
    }

    template<typename ...ARGS>
    inline void DateTime::Adjust(const SEC & value, const ARGS & ...rest)
    {
        long long v = value.Count();
        std::chrono::seconds s = std::chrono::duration_cast<SEC::DurationType>(value.duration);
        (v >= 0) ? AddSecondsImpl(s.count()) : SubtractSecondsImpl(-s.count());
        Adjust(rest...);
    }
}   // end namespace datetime 

#define FORMAT(x) suho::datetime::format::Formatter<suho::idioms::MyString<STR(#x)>>()
}   // end namespace suho

#pragma warning ( disable:4239 )
