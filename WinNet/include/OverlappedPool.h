#pragma once

#include "NetUnit.h"
#include "ObjectPool.hpp"
#include "Singleton.hpp"

namespace suho {
namespace winnet {
namespace iocp
{
	// recieve�� accept�� �Ѽ��Ͽ� ���� ��û�� ����� overlapped�� ����Ǿ� (����ڰ� �Ǽ��� ��� �ޱ��� �߰��� ȣ������ �ʴ� �̻�)
	// �ϳ��� overlapped ������ ���� �� �� ������
	// send�� disconnect�� overlapped �ϳ��� ���ԵǸ� �Ѽ��Ͽ� ���������� �����忡�� �����Ͽ�
	// ����ȭ ������ �߻� �� �� �����Ƿ� overlapped�� ��û�ø��� ���ο� ������ �������־�� �Ѵ�.
	// ���� �Ź� new, delete�� �ϱ⺸�ٴ� poolinig ����� ����Ѵ�.
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