#pragma once

#include <condition_variable>
#include <mutex>

namespace suho {
namespace thread
{
	class EventSync
	{
	public:
		EventSync(bool init_signal = false) : _signal(init_signal) {}
		~EventSync() {}

		void Wait();
		void Set();
		void Reset();

	private:
		std::condition_variable			_condition;
		std::mutex						_mutex;
		bool							_signal = false;
	};

}
}
