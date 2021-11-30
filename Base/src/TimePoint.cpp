#include "TimePoint.h"

using namespace suho::time;

TimePoint::TimePoint()
{
	SetNow();
}

TimePoint::TimePoint( const std::chrono::system_clock::time_point& tp ) :
	_timepoint( tp )
{
}

TimePoint::TimePoint( const time_t& t )
{
	_timepoint = std::chrono::system_clock::from_time_t( t );
}

TimePoint::TimePoint( const TimePoint& other ) :
	_timepoint( other._timepoint )
{

}

TimePoint& TimePoint::operator=( const TimePoint& other )
{
	if ( this != &other )
	{
		_timepoint = other._timepoint;
	}

	return *this;
}

TimePoint::~TimePoint()
{
}

void TimePoint::SetNow()
{
	_timepoint = std::chrono::system_clock::now();
}

void TimePoint::Set( const std::chrono::system_clock::time_point& tp )
{
	_timepoint = tp;
}

void TimePoint::Set( const time_t& t )
{
	_timepoint = std::chrono::system_clock::from_time_t( t );
}

void TimePoint::Add( const TimeSpan& timespan )
{
	TimeSpan ts( timespan );

	_timepoint += std::chrono::duration_cast<std::chrono::microseconds>( ts.GetDuration() );
}

void TimePoint::Add( const time_t& time )
{
	time_t t = std::chrono::system_clock::to_time_t( _timepoint );
	t += time;
	_timepoint = std::chrono::system_clock::from_time_t( t );
}

void TimePoint::Subtract( const TimeSpan& timespan )
{
	TimeSpan ts( timespan );

	_timepoint -= std::chrono::duration_cast<std::chrono::microseconds>( ts.GetDuration() );
}

void TimePoint::Subtract( const time_t& time )
{
	time_t t = std::chrono::system_clock::to_time_t( _timepoint );
	t -= time;
	_timepoint = std::chrono::system_clock::from_time_t( t );
}

TimePoint& suho::time::operator+( TimePoint& timepoint, const TimeSpan& timespan )
{
	timepoint.Add( timespan );
	return timepoint;
}

TimePoint& suho::time::operator+( TimePoint& timepoint, const time_t& time )
{
	timepoint.Add( time );
	return timepoint;
}

TimePoint& suho::time::operator-( TimePoint& timepoint, const TimeSpan& timespan )
{
	timepoint.Subtract( timespan );
	return timepoint;
}

TimePoint& suho::time::operator-( TimePoint& timepoint, const time_t& time )
{
	timepoint.Subtract( time );
	return timepoint;
}

suho::time::TimeSpan suho::time::operator-( const TimePoint& advanced, const TimePoint& lated )
{
	TimePoint tp1( advanced );
	TimePoint tp2( lated );

	return std::chrono::duration_cast<seconds>( tp1.GetTimePoint() - tp2.GetTimePoint() );
}
