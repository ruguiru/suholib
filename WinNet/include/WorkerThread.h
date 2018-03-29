#pragma once

#include "WinNet.h"
#include "Thread.h"

namespace suho {
namespace winnet {
namespace iocp
{
    class WorkerThread : public suho::thread::BaseThread
    {
    public:
        WorkerThread();
        ~WorkerThread() override;
		
    private:
        void Run(void* param) override;
    };
}
}
}
