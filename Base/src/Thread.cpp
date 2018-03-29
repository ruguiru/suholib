#include "Thread.h"
#include "ThreadManager.hpp"

using namespace suho::thread;

BaseThread::BaseThread()
{
}


BaseThread::~BaseThread()
{
    Terminate();
}

void BaseThread::Start(void* param)
{
    if (_is_started)
    {
        return;
    }

	_is_started = true;
	_thread_id = ThreadManager::GetInstance()->Create(Invoker, this, param);
    //_thread_id = ThreadManager::GetInstance()->Create(Run, param);
}

void BaseThread::Terminate()
{
    _is_started = false;
}

void BaseThread::Join()
{
	ThreadManager::GetInstance()->Join(_thread_id);
}

void BaseThread::Invoker(void * thisptr, void * param)
{
	BaseThread* th = static_cast<BaseThread*>(thisptr);
    if (th)
    {
        th->Run(param);
    }
}

