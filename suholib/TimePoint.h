#pragma once

#include <chrono>

#include "TimeSpan.h"

namespace suho {
namespace time
{
    class TimePoint
    {
    public:
        TimePoint();
        explicit TimePoint(const time_t& t);
        TimePoint(const std::chrono::system_clock::time_point& tp);        

        ~TimePoint();

        std::chrono::system_clock::time_point GetTimePoint() const { return _timepoint; }
        time_t GetTime() const { return std::chrono::system_clock::to_time_t(_timepoint); }

        void SetNow();
        void Set(const std::chrono::system_clock::time_point& tp);
        void Set(const time_t& t);

        void Add(const TimeSpan& timespan);
        void Add(const time_t& time);

        void Subtract(const TimeSpan& timespan);
        void Subtract(const time_t& time);

        friend TimePoint& operator+(TimePoint& timepoint, const TimeSpan& timespan);
        friend TimePoint& operator+(TimePoint& timepoint, const time_t& time);

        friend TimePoint& operator-(TimePoint& timepoint, const TimeSpan& timespan);
        friend TimePoint& operator-(TimePoint& timepoint, const time_t& time);

        friend TimeSpan operator-(const TimePoint& advanced, const TimePoint& lated);

    private:
        std::chrono::system_clock::time_point                   _timepoint;
    };

}   // end namespace time
}   // end namespace suho