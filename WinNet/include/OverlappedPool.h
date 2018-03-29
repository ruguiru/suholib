#pragma once

#include "NetUnit.h"
#include "ObjectPool.hpp"
#include "Singleton.hpp"

namespace suho {
namespace winnet {
namespace iocp
{
	class OverlappedPool : public suho::pattern::singleton::Singleton<OverlappedPool>
	{
	public:
		OverlappedPool()
		{
		}
		~OverlappedPool()
		{
		}

		void Create(int size)
		{
			_overlapped_pool.Create(size);
		}

        OverlappedEx* GetOverlapped(suho::winnet::iocp::OperatonType optype, void* owner);
        void ReturnObj(OverlappedEx* over_ex);

	private:
		suho::pool::ObjectPool<OverlappedEx>			_overlapped_pool;
		std::mutex										_mutex;
	};
}
}
}