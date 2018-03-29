#pragma once

#include "Thread.h"
#include "Singleton.hpp"

#include <atomic>
#include <mutex>

class Statistic : public suho::thread::BaseThread, public suho::pattern::singleton::Singleton<Statistic>
{
public:
	Statistic();
	~Statistic() override;

	void AddRecvCount() { ++_recv_packet_count; }
	void AddSendCount(int count) { _send_packet_count+=count; }

private:
	void Run(void* param) override;
	void Calculate();

private:
	std::atomic<int>				_current_user = 0;
	std::atomic<int>				_recv_packet_count = 0;
	std::atomic<int>				_send_packet_count = 0;

	std::mutex						_mutex;
};

