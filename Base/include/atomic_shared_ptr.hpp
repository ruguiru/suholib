#pragma once

#include <mutex>
#include <memory>

/// Ref
/// Just Software Solution http://www.stdthread.co.uk/doc/headers/experimental_atomic/atomic_shared_ptr.html
/// NDC Replay  http://ndcreplay.nexon.com/NDC2017/sessions/NDC2017_0093.html#c=NDC2017&t%5B%5D=%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%98%EB%B0%8D&p=2


namespace jss {
    using namespace std;

    template<typename T>
    struct atomic_shared_ptr
    {
    public:
        constexpr atomic_shared_ptr() noexcept = default;
        atomic_shared_ptr(shared_ptr<T> sptr) noexcept
        {
            m_lock.lock();
            m_ptr = sptr;
            m_lock.unlock();
        }

        std::shared_ptr<T> operator=(shared_ptr<T> sptr) noexcept
        {
            m_lock.lock();
            m_ptr = sptr;
            m_lock.unlock();
            return sptr;
        }

        atomic_shared_ptr(const atomic_shared_ptr& rhs)
        {
            store(rhs);
        }

        atomic_shared_ptr& operator=(const atomic_shared_ptr& rhs)
        {
			if (this != &rhs)
			{
				store(rhs);
			}
            
            return *this;
        }

        operator shared_ptr<T>() const noexcept
        {
            m_lock.lock();
            shared_ptr<T> t = m_ptr;
            m_lock.unlock();
            return t;
        }

        template<typename TargetType>
        inline bool operator==(shared_ptr<TargetType> const& rhs)
        {
            return load() == rhs;
        }

        T* operator->()
        {
            m_lock.lock();
            T* p = m_ptr.get();
            m_lock.unlock();

            return p;
        }

        T& operator*()
        {
            m_lock.lock();
            T* p = m_ptr.get();
            m_lock.unlock();
            return *p;
        }

        void store(shared_ptr<T> sptr, memory_order = memory_order_seq_cst) noexcept
        {
            m_lock.lock();
            m_ptr = sptr;
            m_lock.unlock();
        }

        shared_ptr<T> load(memory_order = memory_order_seq_cst) const noexcept
        {
            m_lock.lock();
            shared_ptr<T> t = m_ptr;
            m_lock.unlock();
            return t;
        }

        void reset()
        {
            m_lock.lock();
            m_ptr = nullptr;
            m_lock.unlock();
        }

        shared_ptr<T> exchange(shared_ptr<T> sptr, memory_order = memory_order_seq_cst) noexcept
        {
            m_lock.lock();
            shared_ptr<T> t = m_ptr;
            m_ptr = sptr;
            m_lock.unlock();
            return t;
        }

        bool compare_exchange_strong(shared_ptr<T>& expected_sptr, shared_ptr<T> new_sptr,
                                     memory_order order = memory_order_seq_st ) noexcept
        {
            bool success = false;

            m_lock.lock();
            shared_ptr<T> t = m_ptr;
            if (m_ptr.get() == expected_sptr.get())
            {
                m_ptr = new_sptr;
                success = true;
            }
            expected_sptr = m_ptr;
            m_lock.unlock();

            return success;
        }

        bool compare_exchange_weak(shared_ptr<T>& expected_sptr, shared_ptr<T> target_sptr,
                                   memory_order order = memory_order_seq_st) noexcept
        {
            return compare_exchange_strong(expected_sptr, target_sptr, memory_order);
        }

        bool is_lock_free() const noexcept { return false; }

    private:
        shared_ptr<T>                           m_ptr;
        mutable mutex                           m_lock;
    };
}