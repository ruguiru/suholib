#pragma once

#include <list>

#include "WinNet.h"
#include "NetUnit.h"
#include "atomic_shared_ptr.hpp"
#include "Thread.h"
#include "TimeSpan.h"
#include "TimePoint.h"

namespace suho {
namespace winnet {
namespace iocp
{
	using namespace suho::time;

	class AutoConnector : public suho::thread::BaseThread
	{
		class ConnectUnit
		{
		public:
			ConnectUnit(const jss::atomic_shared_ptr<NetUnit>& unit,
						const TimeSpan& ts, const SocketAddress& sa) : 
				_netunit(unit),_interval(ts), _destination(sa) {}

			ConnectUnit(const ConnectUnit& connunit) :
				_netunit(connunit._netunit), _interval(connunit._interval), 
				_destination(connunit._destination), _timepoint(connunit._timepoint) {}

			bool CheckConnection();
			void TryConnect();

		private:
			jss::atomic_shared_ptr<NetUnit>						_netunit;
			TimeSpan											_interval;
			SocketAddress										_destination;
			TimePoint											_timepoint;
		};

	public:
		AutoConnector() {}
		~AutoConnector() override {}

		void Register(const jss::atomic_shared_ptr<NetUnit>& netunit,
					   int interval_msec,
					   const std::string& address,
					   short port);

		void Destroy() { _connect_list.clear(); }

	private:
		void Run(void* param) override;

	private:
		std::list<ConnectUnit>									_connect_list;
	};
}
}
}

