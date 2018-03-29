#pragma once

#include "EventSync.h"

#include <thread>
#include <atomic>

namespace suho {
namespace thread
{
    class BaseThread
    {
    public:
		BaseThread();
        virtual ~BaseThread();

		void Start(void* param = nullptr);
        void Terminate();
		void Join();        

		std::thread::id GetID() const { return _thread_id; }

    private:
		static void Invoker(void* thisptr, void* param);
        // 규칙! while( _is_started ) { .... } 로 구현하여 Terminate 호출로 루프 빠져나갈 수 있도록
        virtual void Run(void* param) = 0;

	protected:
		std::atomic<bool>		        _is_started = false;
		std::thread::id		            _thread_id;
    };

}   // end thread
}   // end namespace suho