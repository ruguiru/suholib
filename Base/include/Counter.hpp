#pragma once

#include <atomic>

namespace suho {
namespace idioms
{
    // 객체를 카운트 하고자 하는 클래스에서 상속받아 사용
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

    
    class AtomicCounter
    {
    public:
        AtomicCounter(int initial_value) : _count(initial_value) {}
        ~AtomicCounter() {}

        int operator++() { return ++_count; }
        int operator++(int) { return _count++; }
        
        int operator--() { return --_count; }
        int operator--(int) { return _count--; }

        int Get() const { return _count.load(); }
        void Reset() { _count = 0; }

    private:
        std::atomic<int>                _count = 0;
    };
}   // end namespace idioms
}   // end namespace suho
