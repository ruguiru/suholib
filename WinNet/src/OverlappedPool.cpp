#include "OverlappedPool.h"

using namespace suho::winnet::iocp;

OverlappedEx * OverlappedPool::GetOverlapped(suho::winnet::iocp::OperatonType optype, void * owner)
{
    std::lock_guard<std::mutex> lock(_mutex);

    OverlappedEx* over_ex = _overlapped_pool.Borrow();
    over_ex->operation = optype;
    over_ex->owner = owner;
    return over_ex;
}

void OverlappedPool::ReturnObj(OverlappedEx * over_ex)
{
    std::lock_guard<std::mutex> lock(_mutex);

    over_ex->Clear();
    _overlapped_pool.Return(over_ex);
}

