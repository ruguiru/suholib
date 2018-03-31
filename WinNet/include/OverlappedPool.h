#pragma once

#include "NetUnit.h"
#include "ObjectPool.hpp"
#include "Singleton.hpp"

namespace suho {
namespace winnet {
namespace iocp
{
	// recieve와 accept는 한소켓에 대한 요청과 결과의 overlapped가 보장되어 (사용자가 실수로 결과 받기전 추가로 호출하지 않는 이상)
	// 하나의 overlapped 변수로 돌려 쓸 수 있지만
	// send와 disconnect는 overlapped 하나로 쓰게되면 한소켓에 여러곳에서 스레드에서 접근하여
	// 동기화 문제가 발생 할 수 있으므로 overlapped를 요청시마다 새로운 변수로 전달해주어야 한다.
	// 따라서 매번 new, delete를 하기보다는 poolinig 방식을 사용한다.
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