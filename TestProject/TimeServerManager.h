#pragma once

#include "ActiveNetUnitManager.hpp"
#include "TimeServer.h"
#include "Singleton.hpp"

class TimeServerManager : public suho::winnet::iocp::ActiveNetUnitManager<TimeServer>,
						  public suho::pattern::singleton::Singleton<TimeServerManager>
{
public:
	TimeServerManager() {}
	~TimeServerManager() override { _active_connections.clear(); }

private:
	void OnInsert(long key) override
	{
		printf("On Insert TimeServer\n");
	}

	void OnRemove(long key) override
	{
		printf("On Remove TimeServer\n");
	}

	std::mutex							_mutex;
};
