#pragma once

#include <chrono>

namespace suho
{
	namespace time
	{
		typedef std::chrono::duration<long long, std::nano>             nanosec;
		typedef std::chrono::duration<long long, std::micro>            microsec;
		typedef std::chrono::duration<long long, std::milli>            millisec;
		typedef std::chrono::duration<long double>                      seconds;
		typedef std::chrono::duration<long double, std::ratio<60> >     minutes;
		typedef std::chrono::duration<long double, std::ratio<3600> >   hours;

		template<typename To, typename From>
		inline static To Caster( long long value )
		{
			From duration = From( value );
			return std::chrono::duration_cast<To>( duration );
		}

		class TimeSpan
		{
		public:
			TimeSpan() : _duration( seconds( 0 ) ) {}
			explicit TimeSpan( const long long& sec );
			TimeSpan( const seconds& sec );
			TimeSpan( const millisec& msec );
			TimeSpan( const microsec& mcsec );
			TimeSpan( const nanosec& nsec );
			TimeSpan( const TimeSpan& other );
			TimeSpan( TimeSpan&& other ) noexcept;
			TimeSpan& operator=( const TimeSpan& other );

			~TimeSpan();

			void Set( const long long& sec );
			void Set( const nanosec& nanosec );
			void Set( const microsec& microsec );
			void Set( const millisec& millisec );
			void Set( const seconds& sec );
			void Set( const minutes& minutes );
			void Set( const hours& hours );

			long long ToNanoSeconds() const;
			long long ToMicroSeconds() const;
			long long ToMiliSeconds() const;
			long double ToSeconds() const;
			long double ToMinutes() const;
			long double ToHours() const;

			seconds GetDuration() const { return _duration; }
			void Reset() { _duration = seconds( 0 ); }

		private:
			seconds _duration;
		};

	}   // end namespace time
}   // end namespace suho