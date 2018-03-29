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
        // ��Ģ! while( _is_started ) { .... } �� �����Ͽ� Terminate ȣ��� ���� �������� �� �ֵ���
        virtual void Run(void* param) = 0;

	protected:
		std::atomic<bool>		        _is_started = false;
		std::thread::id		            _thread_id;
    };

}   // end thread
}   // end namespace suho