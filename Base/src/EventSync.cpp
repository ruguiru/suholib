#include "EventSync.h"

using namespace suho::thread;

void EventSync::Wait()
{
	std::unique_lock<std::mutex> lock( _mutex );
	_condition.wait( lock, [&] () { return _signal; } );
	lock.unlock();
}

void EventSync::Set()
{
	std::lock_guard<std::mutex> lock( _mutex );

	_signal = true;
	_condition.notify_one();
}

void EventSync::Reset()
{
	std::lock_guard<std::mutex> lock( _mutex );

	_signal = false;
}