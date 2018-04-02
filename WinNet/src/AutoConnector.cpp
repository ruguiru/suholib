#include "AutoConnector.h"
#include "IocpServer.h"
#include "Common.h"
#include "atomic_weak_ptr.hpp"

using namespace suho::winnet;
using namespace suho::winnet::iocp;
using namespace suho::time;


#define LOOP_CYCLE_MSEC 500

void AutoConnector::Register(const jss::atomic_shared_ptr<NetUnit>& netunit, int interval_msec, const std::string & address, short port)
{
	_connect_list.push_back({
		netunit,
		TimeSpan(Caster<seconds, millisec>(interval_msec)),
		SocketAddress(address, port)
	});
}

void AutoConnector::Run(void * param)
{
	UNREFERENCED(param)

    IocpLog(level::INFO, "Start AutoConnect");

	while (_is_started)
	{
		for (auto& conn : _connect_list)
		{
			if (conn.CheckConnection())
			{
				continue;
			}			

			conn.TryConnect();
		}

		std::chrono::milliseconds dura(LOOP_CYCLE_MSEC);
		std::this_thread::sleep_for(dura);
	}
}

bool AutoConnector::ConnectUnit::CheckConnection()
{	
	if (_netunit->IsActive())
	{
		if (!_netunit->IsConnected())
		{
			IocpLog(level::INFO, "[%d] Active But NotConnect!", _netunit->GetIndex());
			_netunit->DisconnectRequest();
		}

		return true;
	}

	return false;
}

void AutoConnector::ConnectUnit::TryConnect()
{
	TimePoint tp_now;
	TimeSpan elased = tp_now - _timepoint;

	if (elased.ToMiliSeconds() >= _interval.ToMiliSeconds())
	{
		_timepoint = tp_now;

		_netunit->ConnectRequest(_destination);
	}
}

