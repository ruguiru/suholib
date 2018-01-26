#include "StopWatch.h"

using namespace suho;
using namespace time;

StopWatch::StopWatch()
{
    Start();
}

StopWatch::~StopWatch()
{
}

const TimePoint& StopWatch::Start()
{
    _timestamp = Now();

    return _timestamp;
}

TimeSpan StopWatch::Stop()
{
    auto span = Now() - _timestamp;

    return span;
}

TimePoint StopWatch::Now()
{
    return TimePoint();
}

TimeSpan StopWatch::Elapse(const std::chrono::system_clock::time_point & passed_time_point)
{
    return  Now() - passed_time_point;
}

time_t StopWatch::Elapse(const time_t & passed_time)
{
    TimePoint t = Now() - passed_time;

    return  std::chrono::system_clock::to_time_t(t.GetTimePoint());
}
