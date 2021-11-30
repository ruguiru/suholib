#include "TimeSpan.h"

using namespace suho::time;

TimeSpan::TimeSpan( const long long& sec ) :
	_duration( seconds( sec ) )
{
}

TimeSpan::TimeSpan( const seconds& sec ) :
	_duration( sec )
{
}

TimeSpan::TimeSpan( const millisec& msec )
{
	Set( msec );
}

TimeSpan::TimeSpan( const microsec& mcsec )
{
	Set( mcsec );
}

TimeSpan::TimeSpan( const nanosec& nsec )
{
	Set( nsec );
}

TimeSpan::TimeSpan( const TimeSpan& other ) :
	_duration( other._duration )
{
}

TimeSpan::TimeSpan( TimeSpan&& other ) noexcept :
	_duration( other._duration )
{
	other.Reset();
}

TimeSpan& TimeSpan::operator=( const TimeSpan& other )
{
	if ( this != &other )
		_duration = other._duration;

	return *this;
}

TimeSpan::~TimeSpan()
{
}

void TimeSpan::Set( const long long& sec )
{
	_duration = seconds( sec );
}

void TimeSpan::Set( const nanosec& nanosec )
{
	_duration = std::chrono::duration_cast<seconds>( nanosec );
}

void TimeSpan::Set( const microsec& microsec )
{
	_duration = std::chrono::duration_cast<seconds>( microsec );
}

void TimeSpan::Set( const millisec& millisec )
{
	_duration = std::chrono::duration_cast<seconds>( millisec );
}

void TimeSpan::Set( const seconds& sec )
{
	_duration = sec;
}

void TimeSpan::Set( const minutes& minutes )
{
	_duration = std::chrono::duration_cast<seconds>( minutes );
}

void TimeSpan::Set( const hours& hours )
{
	_duration = std::chrono::duration_cast<seconds>( hours );
}

long long TimeSpan::ToNanoSeconds() const
{
	return std::chrono::duration_cast<nanosec>( _duration ).count();
}

long long TimeSpan::ToMicroSeconds() const
{
	return std::chrono::duration_cast<microsec>( _duration ).count();
}

long long TimeSpan::ToMiliSeconds() const
{
	return std::chrono::duration_cast<millisec>( _duration ).count();
}

long double TimeSpan::ToSeconds() const
{
	return _duration.count();
}

long double TimeSpan::ToMinutes() const
{
	return std::chrono::duration_cast<minutes>( _duration ).count();
}

long double TimeSpan::ToHours() const
{
	return std::chrono::duration_cast<hours>( _duration ).count();
}