#pragma once

#include <chrono>
#include <time.h>

#include "TimePoint.h"
#include "TimeSpan.h"

namespace suho
{
	namespace time
	{
		class StopWatch
		{
		public:
			StopWatch();
			~StopWatch();

			// ������ �ð� ���� (����ð�)
			const TimePoint& Start();
			// ����ð� ����
			TimeSpan Stop();

			static TimePoint Now();

			static TimeSpan Elapse( const std::chrono::system_clock::time_point& passed_time_point );
			static time_t Elapse( const time_t& passed_time );

		private:
			TimePoint                   _timestamp;
		};

	}   // end namespace time
}   // end namespace suho