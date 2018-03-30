#pragma once

#include <unordered_map>

#include "WinNet.h"
#include "NetUnit.h"
#include "atomic_shared_ptr.hpp"

namespace suho {
namespace winnet {
namespace iocp
{
	template<typename T>
	class ActiveNetUnitManager
	{
	public:
		typedef jss::atomic_shared_ptr<NetUnit>				ActiveUnit;

        ActiveNetUnitManager()
        {
            static_assert(std::is_base_of<NetUnit, T>::value, "ActiveNetUnitManager Construct() T not NetUnit or Child of NetUnit");
        }

		virtual ~ActiveNetUnitManager()
        {
            _active_connections.clear();
        }

		const ActiveUnit& Search(long key)
        {
            std::lock_guard<std::mutex> lock(_mutex);		// LOCK

            auto pos = _active_connections.find(key);
            if (pos == _active_connections.end())
                return nullptr;

            return pos->second;
        }

        void Insert(long key, const ActiveUnit& unit)
        {
			{
				std::lock_guard<std::mutex> lock(_mutex);		// LOCK
				_active_connections.insert(std::make_pair(key, unit));
			}

			OnInsert(key);
        }

        void Remove(long key)
        {
			{
				std::lock_guard<std::mutex> lock(_mutex);		// LOCK
				_active_connections.erase(key);
			}

			OnRemove(key);
        }

        virtual void BroadCast(void* buffer, int size)
        {
            std::lock_guard<std::mutex> lock(_mutex);		// LOCK

			for (auto& elem : _active_connections)
			{
				elem.second->SendRequest(buffer, size);
			}
        }

    private:
		virtual void OnInsert(long key) = 0;
		virtual void OnRemove(long key) = 0;

	protected:		
		std::unordered_map<long, ActiveUnit>				_active_connections;
        std::mutex											_mutex;
	};
}
}
}