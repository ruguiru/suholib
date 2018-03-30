#pragma once

#include "ActiveNetUnitManager.hpp"
#include "Client.h"
#include "Singleton.hpp"

class ClientManager : public suho::winnet::iocp::ActiveNetUnitManager<Client>,
					  public suho::pattern::singleton::Singleton<ClientManager>
{
public:
	ClientManager() {}
	~ClientManager() override { _active_connections.clear(); }

	void BroadCast(MyPacket& packet);
	//void Insert(long key, const ActiveUnit& unit)
	//{
	//	std::lock_guard<std::mutex> lock(_cm_mutex);		// LOCK
	//	_active_connections.insert(std::make_pair(key, unit));
	//}

	//void Remove(long key)
	//{
	//	std::lock_guard<std::mutex> lock(_cm_mutex);		// LOCK
	//	_active_connections.erase(key);
	//}

	void Destroy() { _active_connections.clear(); }
	int GetCount() const { return _active_connections.size(); }

private:
	void OnInsert(long key) override;
	void OnRemove(long key) override;

	std::mutex								_cm_mutex;
};