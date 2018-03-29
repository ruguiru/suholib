#pragma once

#include <map>
#include <thread>
#include <mutex>


#include "Singleton.hpp"

namespace suho {
namespace thread
{
    class ThreadManager : public suho::pattern::singleton::Singleton<ThreadManager>
    {
    public:
		ThreadManager() {}
		~ThreadManager() {}

		template<typename Fun, typename... ARGS>
		std::thread::id Create(const Fun& func, const ARGS&... args)
		{
			std::lock_guard<std::mutex>		lock(_mutex);			

			auto th = std::make_unique<std::thread>(func, args...);
			std::thread::id thread_id = th->get_id();

			_threads.insert(std::make_pair(thread_id, std::move(th) ));

			return thread_id;
		}

		void Join(std::thread::id thread_id)
		{
			std::lock_guard<std::mutex>		lock(_mutex);

			auto pos = _threads.find(thread_id);
            auto& th = pos->second;
			if (th->joinable())
				th->join();
			
			_threads.erase(thread_id);
		}

		void JoinAll()
		{
			std::lock_guard<std::mutex>		lock(_mutex);

			if (_threads.empty())
				return;

			for (auto& item : _threads)
			{		
				auto& th = item.second;
				if (th->joinable())
					th->join();
			}

			_threads.clear();
		}

	private:
		std::map<std::thread::id, std::unique_ptr<std::thread>>				_threads;
		std::mutex											                _mutex;
    };
}   // end thread
}   // end namespace suho