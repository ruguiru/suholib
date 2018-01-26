#pragma once

#include <atomic>

namespace suho {
namespace idioms
{
    template<typename T>
    class InstanceCounter
    {
    public:
        InstanceCounter() { ++_count; }
        ~InstanceCounter() { --_count; }

        static int GetCount() { return _count; }

    private:
        static std::atomic<int>         _count;
    };

    template<typename T> std::atomic<int> Counter<T>::_count = 0;
}   // end namespace idioms
}   // end namespace suho
