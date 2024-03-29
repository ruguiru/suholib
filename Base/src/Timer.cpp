#include "Common.h"
#include "Timer.h"
#include "StopWatch.h"

#include <algorithm>
#include <future>
#include <sys/timeb.h>


using namespace suho::time;


Timer::Timer()
{
}

Timer::~Timer()
{
	_timerlist.clear();
}

int Timer::CalcRemainMSec( const FlatTime flat_type )
{
	tm t;
	__time64_t long_time;

	_time64( &long_time );
	_localtime64_s( &t, &long_time );

	struct timeb timebuffer;
	ftime( &timebuffer );

	int cur_hour = t.tm_hour;
	int cur_min = t.tm_min;
	int cur_sec = t.tm_sec;
	int cur_msec = timebuffer.millitm;

	int remain_msec = 0;
	int remain_sec = 0;
	int remain_min = 0;
	int remain_hour = 0;

	const int flat_type_vale = static_cast<int>( flat_type );

	switch ( flat_type )
	{
	case FlatTime::EVERY_1_SECOND:
	case FlatTime::EVERY_5_SECOND:
	case FlatTime::EVERY_10_SECOND:
	case FlatTime::EVERY_15_SECOND:
	case FlatTime::EVERY_20_SECOND:
	case FlatTime::EVERY_30_SECOND:
		{
			remain_sec = ( flat_type_vale / 1000 - ( cur_sec % ( flat_type_vale / 1000 ) ) ) - 1;
			remain_msec = 1000 - cur_msec;
		}
		break;

	case FlatTime::EVERY_1_MINUTE:
	case FlatTime::EVERY_5_MINUTE:
	case FlatTime::EVERY_10_MINUTE:
	case FlatTime::EVERY_15_MINUTE:
	case FlatTime::EVERY_20_MINUTE:
	case FlatTime::EVERY_30_MINUTE:
		{
			remain_min = ( flat_type_vale / 60000 - ( cur_min % ( flat_type_vale / 60000 ) ) ) - 1;
			remain_sec = 60 - cur_sec - 1;
			remain_msec = 1000 - cur_msec;
		}
		break;

	case FlatTime::EVERY_1_HOUR:
	case FlatTime::EVERY_2_HOUR:
	case FlatTime::EVERY_3_HOUR:
	case FlatTime::EVERY_4_HOUR:
	case FlatTime::EVERY_6_HOUR:
	case FlatTime::EVERY_8_HOUR:
	case FlatTime::EVERY_12_HOUR:
	case FlatTime::EVERY_24_HOUR:
		{
			remain_hour = ( flat_type_vale / 3600000 - ( cur_hour % ( flat_type_vale / 3600000 ) ) ) - 1;
			remain_min = 60 - cur_min - 1;
			remain_sec = 60 - cur_sec - 1;
			remain_msec = 1000 - cur_msec;
		}
		break;
	}

	return ( remain_msec + remain_sec * 1000 + remain_min * 60000 + remain_hour * 3600000 );
}


void Timer::RegisterFlatTimer( const _TimerCallBack& func, const Timer::FlatTime flat_type, void* arg )
{
	std::lock_guard<std::mutex> lock( _mutex );

	const int remain_msec = CalcRemainMSec( flat_type );

	_timerlist.push_back( TimerInfo( func, static_cast<int>( flat_type ), remain_msec, TimerType::FLAT_CYCLE_TIMER, arg ) );
}

void Timer::RegisterNormalTimer( const _TimerCallBack& func, int interval_sec, void* arg )
{
	std::lock_guard<std::mutex> lock( _mutex );

	_timerlist.push_back( TimerInfo( func, interval_sec * 1000, 1, TimerType::NORMAL_CYCLE_TIMER, arg ) );
}

void Timer::RegisterInstantTimer( const _TimerCallBack& func, int sec, void* arg )
{
	std::lock_guard<std::mutex> lock( _mutex );

	_timerlist.push_back( TimerInfo( func, sec * 1000, sec * 1000, TimerType::INSTANT_TIMER, arg ) );
}

void Timer::RemoveTimer( const _TimerCallBack& func )
{
	std::lock_guard<std::mutex> lock( _mutex );

	auto iter = std::remove_if(
		_timerlist.begin(), _timerlist.end(),
		[&func] ( const TimerInfo& timerinfo )
		{
			return timerinfo._callback.target<void( * )( void* )>() == func.target<void( * )( void* )>();
		} );
}

void Timer::Run( void* param )
{
	UNREFERENCED( param )

		int timer_recalc_count = RECALC_INTERVAL;

	StopWatch watch;

	while ( _is_started )
	{
		int lapsed_msec = static_cast<int>( watch.Stop().ToMiliSeconds() );
		watch.Start();

		volatile bool recalc_flag = false;
		if ( --timer_recalc_count == 0 )
		{
			timer_recalc_count = RECALC_INTERVAL;
			recalc_flag = true;
		}

		{
			std::lock_guard<std::mutex> lock( _mutex );

			TimerList::iterator list_iter = _timerlist.begin();
			while ( list_iter != _timerlist.end() )
			{
				TimerInfo& funcinfo = *list_iter;

				funcinfo._remain_msec -= lapsed_msec;

				if ( funcinfo._remain_msec <= 0 )
				{
					auto f = std::async( std::launch::async, funcinfo._callback, funcinfo._arg );

					funcinfo.SetRemainTimeCount( funcinfo._interval_msec );

					if ( funcinfo._timerType == TimerType::INSTANT_TIMER )
					{
						_timerlist.erase( list_iter-- );
					}

					f.get();
				}

				// ���� ����
				if ( funcinfo._timerType <= TimerType::NORMAL_CYCLE_TIMER && recalc_flag == true )
				{
					int next_interval = CalcRemainMSec( static_cast<Timer::FlatTime>( funcinfo._interval_msec ) );

					funcinfo.SetRemainTimeCount( next_interval );
				}

				++list_iter;
			}
		}

		std::chrono::milliseconds dura( CHECK_INTERVAL );
		std::this_thread::sleep_for( dura );
	}
}