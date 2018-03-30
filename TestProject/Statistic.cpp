#include "Statistic.h"
#include "LogContainer.h"
#include "ClientManager.h"

constexpr int sec = 5;

Statistic::Statistic()
{
}

Statistic::~Statistic()
{
}

void Statistic::Run(void * param)
{
	while (_is_started)
	{
		Calculate();		

		this_thread::sleep_for(std::chrono::seconds(sec));
	}
}

void Statistic::Calculate()
{
	std::lock_guard<std::mutex> lock(_mutex);

	int cu = ClientManager::GetInstance()->GetCount();

	double avg_recv_per_sec = static_cast<double>(_recv_packet_count) / static_cast<double>(sec);
	double avg_send_per_sec = static_cast<double>(_send_packet_count) / static_cast<double>(sec);

	StatisticLog(level::INFO, "CU:%d Recv/sec:%f Send/sec:%f", 
		cu, avg_recv_per_sec, avg_send_per_sec);

	_recv_packet_count = 0;
	_send_packet_count = 0;
}
