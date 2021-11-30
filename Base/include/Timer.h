#pragma once

#include <list>
#include <atomic>
#include <mutex>
#include <functional>

#include "Singleton.hpp"
#include "Thread.h"

namespace suho
{
	namespace time
	{
#define CHECK_INTERVAL		500			// 타이머 루프 밀리초
#define RECALC_INTERVAL		50			// 재계산 루프 횟수

		enum class TimerType : int
		{
			FLAT_CYCLE_TIMER = 0,
			NORMAL_CYCLE_TIMER,
			INSTANT_TIMER,
		};

		class Timer : public suho::pattern::singleton::Singleton<Timer>, public suho::thread::BaseThread
		{
		public:
			enum class FlatTime : int
			{
				// 등록 후 아래 정각에 따라 발동 ( 밀리초 )
				EVERY_1_SECOND = 1 * 1000,
				EVERY_5_SECOND = EVERY_1_SECOND * 5,
				EVERY_10_SECOND = EVERY_1_SECOND * 10,
				EVERY_15_SECOND = EVERY_1_SECOND * 15,

				EVERY_20_SECOND = EVERY_1_SECOND * 20,
				EVERY_30_SECOND = EVERY_1_SECOND * 30,

				EVERY_1_MINUTE = EVERY_1_SECOND * 60,
				EVERY_5_MINUTE = EVERY_1_MINUTE * 5,
				EVERY_10_MINUTE = EVERY_1_MINUTE * 10,
				EVERY_15_MINUTE = EVERY_1_MINUTE * 15,
				EVERY_20_MINUTE = EVERY_1_MINUTE * 20,
				EVERY_30_MINUTE = EVERY_1_MINUTE * 30,

				EVERY_1_HOUR = EVERY_1_MINUTE * 60,
				EVERY_2_HOUR = EVERY_1_HOUR * 2,
				EVERY_3_HOUR = EVERY_1_HOUR * 3,
				EVERY_4_HOUR = EVERY_1_HOUR * 4,
				EVERY_6_HOUR = EVERY_1_HOUR * 6,
				EVERY_8_HOUR = EVERY_1_HOUR * 8,
				EVERY_12_HOUR = EVERY_1_HOUR * 12,
				EVERY_24_HOUR = EVERY_1_HOUR * 24,
			};

			typedef std::function<void( void* arg )> _TimerCallBack;

		public:
			Timer();
			~Timer() override;

			void RegisterFlatTimer( const _TimerCallBack& func, const Timer::FlatTime flat_type, void* arg = nullptr );     // 정각 반복 타이머
			void RegisterNormalTimer( const _TimerCallBack& func, int interval_sec, void* arg = nullptr );                  // 반복 타이머
			void RegisterInstantTimer( const _TimerCallBack& func, int sec, void* arg = nullptr );                           // 1회용 타이머

			void RemoveTimer( const _TimerCallBack& func );                            // 타이머 함수 제거

		private:
			void Run( void* param ) override;

			int CalcRemainMSec( const FlatTime timer_type );               // 다음 정각까지 남은 시간 계산 (밀리초)

		private:
			struct TimerInfo
			{
				_TimerCallBack _callback;
				int            _interval_msec;
				int            _remain_msec;
				TimerType      _timerType;
				void* _arg;

				TimerInfo() : _callback( nullptr ), _interval_msec( 0 ), _remain_msec( 0 ), _timerType( TimerType::INSTANT_TIMER ), _arg( nullptr ) {}
				TimerInfo( const _TimerCallBack& _func, int _interval, int _waitcount, const TimerType _timetype, void* arg ) :
					_callback( _func ), _interval_msec( _interval ), _remain_msec( _waitcount ), _timerType( _timetype ), _arg( arg ) {}
				TimerInfo( const TimerInfo& other ) :
					_callback( other._callback ), _interval_msec( other._interval_msec ), _remain_msec( other._remain_msec ), _timerType( other._timerType ), _arg( other._arg ) {}

				void SetRemainTimeCount( int _count ) { _remain_msec = _count; }
			};

			typedef std::list<TimerInfo> TimerList;

			TimerList  _timerlist;
			std::mutex _mutex;
		};
	}   // end namespace suho
}   // end namespace time

