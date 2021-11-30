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
		UNREFERENCED(key)
		printf("On Insert TimeServer\n");	// LOG
	}

	void OnRemove(long key) override
	{
		UNREFERENCED(key)
		printf("On Remove TimeServer\n");	// LOG
	}

	std::mutex							_mutex;
};
