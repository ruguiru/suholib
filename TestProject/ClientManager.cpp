#include "ClientManager.h"
#include "LogContainer.h"
#include "MyPacket.h"
#include "Statistic.h"

using namespace suho::log;

void ClientManager::OnInsert(long key)
{
	//std::lock_guard<std::mutex>		lock(_cm_mutex);
}

void ClientManager::OnRemove(long key)
{
	//std::lock_guard<std::mutex>		lock(_cm_mutex);
	//_active_connections.erase(key);
}

void ClientManager::BroadCast(MyPacket& packet)
{	
	//std::lock_guard<std::mutex>		lock(_cm_mutex);

	//for (auto& elem : _active_connections)
	//{
	//	elem.second->SendRequest(packet.GetBuffer(), packet.GetPacketSize());
	//}
	
	ActiveNetUnitManager<Client>::BroadCast(packet.GetBuffer(), packet.GetPacketSize());
	Statistic::GetInstance()->AddSendCount(GetCount());	
}