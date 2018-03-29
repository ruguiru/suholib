#pragma once

#include <mutex>
#include <memory>

/// Ref
/// Just Software Solution http://www.stdthread.co.uk/doc/headers/experimental_atomic/atomic_weak_ptr.html
/// NDC Replay  http://ndcreplay.nexon.com/NDC2017/sessions/NDC2017_0093.html#c=NDC2017&t%5B%5D=%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%98%EB%B0%8D&p=2


namespace jss {
    using namespace std;

    template<typename T>
    struct atomic_weak_ptr
    {
    public:
        constexpr atomic_weak_ptr() noexcept = default;
        atomic_weak_ptr(weak_ptr<T> wptr) noexcept
        {
            m_lock.lock();
            m_ptr = wptr;
            m_lock.unlock();
        }

		atomic_weak_ptr(const shared_ptr<T>& sptr) noexcept
		{
			m_lock.lock();
			m_ptr = sptr;
			m_lock.unlock();
		}

        std::weak_ptr<T> operator=(weak_ptr<T> wptr) noexcept
        {
            m_lock.lock();
            m_ptr = wptr;
            m_lock.unlock();
            return wptr;
        }

		std::weak_ptr<T>& operator=(const shared_ptr<T>& sptr) noexcept
		{
			m_lock.lock();
			m_ptr = sptr;
			m_lock.unlock();
			return *this;
		}

        atomic_weak_ptr(const atomic_weak_ptr& rhs) = delete;
        atomic_weak_ptr& operator=(const atomic_weak_ptr& rhs) = delete;

        operator weak_ptr<T>() const noexcept
        {
            m_lock.lock();
            weak_ptr<T> t = m_ptr;
            m_lock.unlock();
            return t;
        }

        void store(weak_ptr<T> wptr, memory_order = memory_order_seq_cst) noexcept
        {
            m_lock.lock();
            m_ptr = wptr;
            m_lock.unlock();
        }

        weak_ptr<T> load(memory_order = memory_order_seq_cst) const noexcept
        {
            m_lock.lock();
            weak_ptr<T> t = m_ptr;
            m_lock.unlock();
            return t;
        }

        shared_ptr<T> lock() const noexcept
        {
            m_lock.lock();
            shared_ptr<T> sptr = m_ptr.lock();
            m_lock.unlock();
            return sptr;
        }

        void reset()
        {
            m_lock.lock();
            m_ptr.reset();
            m_lock.unlock();
        }

        weak_ptr<T> exchange(weak_ptr<T> wptr, memory_order = memory_order_seq_cst) noexcept
        {
            m_lock.lock();
            weak_ptr<T> t = m_ptr;
            m_ptr = wptr;
            m_lock.unlock();
            return t;
        }

        bool compare_exchange_strong(weak_ptr<T>& expected_wptr, weak_ptr<T> new_wptr,
            memory_order, memory_order) noexcept
        {
            bool success = false;
            lock_guard(m_lock);

            weak_ptr<T> t = m_ptr;
            shared_ptr<T> my_ptr = t.lock();
            if (!my_ptr) return false;
            
            shared_ptr<T> expected_sptr = expected_wptr.lock();
            if (!expected_sptr) return false;

            if (my_ptr.get() == expected_sptr.get())
            {
                success = true;
                m_ptr = new_wptr;                
            }

            expected_wptr = t;
            return success;
        }

        bool compare_exchange_weak(weak_ptr<T>& expected_wptr, weak_ptr<T> new_wptr,
            memory_order order = memory_order_seq_st) noexcept
        {
            return compare_exchange_strong(expected_wptr, new_wptr, memory_order);
        }

        bool is_lock_free() const noexcept { return false; }

    private:
        weak_ptr<T>                             m_ptr;
		mutable mutex                           m_lock;
    };
}