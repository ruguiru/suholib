#include "ClientManager.h"
#include "LogContainer.h"
#include "MyPacket.h"
#include "Statistic.h"

using namespace suho::log;

void ClientManager::OnInsert(long key)
{
    //ClientLog(level::INFO, "[%d] Insert User Container", key);		// TEST
}

void ClientManager::OnRemove(long key)
{
    //ClientLog(level::INFO, "[%d] Remove User Container", key);		// TEST
}

void ClientManager::BroadCast(MyPacket& packet)
{
	Statistic::GetInstance()->AddSendCount(GetCount());
	
	ActiveNetUnitManager<Client>::BroadCast(packet.GetBuffer(), packet.GetPacketSize());
}