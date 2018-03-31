#include "ClientManager.h"
#include "LogContainer.h"
#include "MyPacket.h"
#include "Statistic.h"

using namespace suho::log;

void ClientManager::OnInsert(long key)
{
}

void ClientManager::OnRemove(long key)
{
}

void ClientManager::BroadCast(MyPacket& packet)
{		
	ActiveNetUnitManager<Client>::BroadCast(packet.GetBuffer(), packet.GetPacketSize());
	Statistic::GetInstance()->AddSendCount(GetCount());	
}